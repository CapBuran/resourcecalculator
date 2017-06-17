#include "RecipesEditDialog.h"

#include <QtWidgets>

#pragma region MODEL

RecipeListModel::RecipeListModel(FactorioCalculator::RecipeCollection &RC, QObject *parent):
  QAbstractTableModel(parent), _RC(RC)
{
  using namespace FactorioCalculator;
  const std::map<KEY_RECIPE, Recipe> &DATA = _RC.GetData();
  _listOfRecipesId.reserve(static_cast<int>(DATA.size()));
  for (auto &it : DATA) {
    _listOfRecipesId.push_back(it.first);
  }
}

int RecipeListModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return _listOfRecipesId.size();
}

int RecipeListModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return 5;
}

QVariant RecipeListModel::data(const QModelIndex &index, int role) const
{
  using namespace FactorioCalculator;

  if (!index.isValid())
    return QVariant();

  if (index.row() >= _listOfRecipesId.size() || index.row() < 0)
    return QVariant();

  if (role == Qt::DisplayRole) {

    const FactorioCalculator::KEY_RECIPE KeyRecipe = GetRecipeId(index.row());

    Recipe *R = _RC.GetRecipeForEdit(KeyRecipe);
    if (R == nullptr) {
      return QVariant();
    }

    RecipeParams RP = R->GetRecipeParams();
    QString retval;

    switch (index.column())
    {
    case 0:
      retval = QString::fromUtf8(R->GetName().c_str());
      return retval;
      break;
    case 1:
      retval = QString::number(RP.Time);
      return retval;
      break;
    case 2:
      retval = "TODO: Интегриенты";
      return retval;
      break;
    case 3:
      retval = "TODO: Зависимость";
      return retval;
      break;
    case 4:
      retval = "TODO: Factorys allowes";
      return retval;
      break;
    default:
      return QVariant();
      break;
    }
  }

  return QVariant();
}

QVariant RecipeListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal) {
    switch (section) {
    case 0:
      return tr("Recipe Name");
    case 1:
      return tr("Recipe time");
    case 2:
      return tr("Recipe Зависимость");
    case 3:
      return tr("Recipe Интегреенты");
    case 4:
      return tr("Factorys allowes");
    default:
      return QVariant();
    }
  }
  return QVariant();
}
 
Qt::ItemFlags RecipeListModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool RecipeListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if (index.isValid() && role == Qt::EditRole) {
    int row = index.row();
    
    using namespace FactorioCalculator;

    const FactorioCalculator::KEY_RECIPE KeyRecipe = GetRecipeId(row);

    Recipe *R = _RC.GetRecipeForEdit(KeyRecipe);
    if (R == nullptr) {
      return false;
    }
    
    std::string Name = R->GetName();
    RecipeParams Params = R->GetRecipeParams();

    switch (index.column())
    {
    case 0:
      Name = value.toString().toStdString();
      break;
    case 1:
      Params.Time = value.toDouble();
      break;
    default:
      return false;
      break;
    }

    Recipe ToADD(Name, Params);
    _RC.Add(ToADD);

    _listOfRecipesId.replace(row, R->GetKey());
    
    emit(dataChanged(index, index));

    return true;
  }

  return false;
}

bool RecipeListModel::insertRows(int position, int rows, const QModelIndex &index)
{
  Q_UNUSED(index);
  beginInsertRows(QModelIndex(), position, position + rows - 1);
  for (int row = 0; row < rows; ++row) {
    using namespace FactorioCalculator;
    KEY_RECIPE NewKey = _RC.GetUniqueRecipeKey();
    std::string Name("Новый рецепт" + std::to_string(row));
    RecipeParams Params;
    Params.Key = NewKey;
    Recipe recipe(Name, Params);
    _listOfRecipesId.insert(position, NewKey);
  }
  endInsertRows();
  return true;
}
  
bool RecipeListModel::removeRows(int position, int rows, const QModelIndex &index)
{
  Q_UNUSED(index);
  beginRemoveRows(QModelIndex(), position, position + rows - 1);
  for (int row = 0; row < rows; ++row) {
    _RC.Delete(GetRecipeId(position));
    _listOfRecipesId.removeAt(position);
  }
  endRemoveRows();
  return true;
}

FactorioCalculator::KEY_RECIPE RecipeListModel::GetRecipeId(int Num) const
{
  return _listOfRecipesId[Num];
}

#pragma endregion MODEL

#pragma region DELEGATE

SpinBoxDelegate::SpinBoxDelegate(QObject *parent)
  : QStyledItemDelegate(parent)
{
}

QWidget *SpinBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
{
  QSpinBox *editor = new QSpinBox(parent);
  editor->setFrame(false);
  editor->setMinimum(0);
  editor->setMaximum(100);

  return editor;
}

void SpinBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  int value = index.model()->data(index, Qt::EditRole).toInt();

  QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
  spinBox->setValue(value);
}

void SpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
  QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
  spinBox->interpretText();
  int value = spinBox->value();

  model->setData(index, value, Qt::EditRole);
}

void SpinBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
  editor->setGeometry(option.rect);
}

#pragma endregion DELEGATE

RecipesEditDialog::RecipesEditDialog(FactorioCalculator::RecipeCollection &RC, QWidget *parent)
  : QDialog(parent), _RC(RC)
{

  setMinimumSize(800, 600);
   
  okButton = new QPushButton("OK");
  cancelButton = new QPushButton("Cancel");

  RecipeListModel *table = new RecipeListModel(_RC, this);
  QTableView *tableView = new QTableView;
  tableView->setModel(table);

  QHBoxLayout *buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(okButton);
  buttonLayout->addWidget(cancelButton);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(tableView);
  mainLayout->addLayout(buttonLayout);
  setLayout(mainLayout);

  connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);
  connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);

  setWindowTitle(tr("Recipes Edit"));




}
