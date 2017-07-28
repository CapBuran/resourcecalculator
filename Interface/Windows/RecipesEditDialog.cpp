#include "RecipesEditDialog.h"
#include "ItemSelectedDialog.h"

#pragma region MODEL

RecipeListModel::RecipeListModel(ResourceCalculator::ParamsCollection &PC, QObject *parent):
  QAbstractTableModel(parent), _PC(PC)
{
  using namespace ResourceCalculator;
  const std::map<KEY_RECIPE, Recipe> &DATA = _PC.RC.GetData();
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
  using namespace ResourceCalculator;

  if (!index.isValid())
    return QVariant();

  if (index.row() >= _listOfRecipesId.size() || index.row() < 0)
    return QVariant();

  if (role == Qt::DisplayRole) {

    const ResourceCalculator::KEY_RECIPE KeyRecipe = GetRecipeId(index.row());

    Recipe *R = _PC.RC.GetRecipeForEdit(KeyRecipe);
    if (R == nullptr) {
      return QVariant();
    }

    //RecipeParams RP = R->GetRecipeParams();
    QString retval;

    switch (index.column())
    {
    case 0:
      retval = QString::fromUtf8(R->GetName().c_str());
      return retval;
      break;
    case 1:
      retval = QString::number(R->GetTime());
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
      return tr("Result of recipe");
    case 3:
      return tr("Ingredients of the recipe");
    case 4:
      return tr("Allowed factories");
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
    
    using namespace ResourceCalculator;

    const ResourceCalculator::KEY_RECIPE KeyRecipe = GetRecipeId(row);

    Recipe *EditRecipe = _PC.RC.GetRecipeForEdit(KeyRecipe);
    if (EditRecipe == nullptr) {
      return false;
    }

    switch (index.column())
    {
    case 0:
      EditRecipe->SetName(value.toString().toStdString());
      break;
    case 1:
      EditRecipe->SetTime(value.toDouble());
      break;
    default:
      return false;
      break;
    }

    _PC.RC.Add(*EditRecipe);

    _listOfRecipesId.replace(row, EditRecipe->GetKey());
    
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
    using namespace ResourceCalculator;
    KEY_RECIPE NewKey = _PC.RC.GetUniqueRecipeKey();
    std::string Name("Новый рецепт" + std::to_string(row));
    Recipe recipe;
    recipe.SetKey(NewKey);
    recipe.SetName(Name);
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
    _PC.RC.Delete(GetRecipeId(position));
    _listOfRecipesId.removeAt(position);
  }
  endRemoveRows();
  return true;
}

ResourceCalculator::KEY_RECIPE RecipeListModel::GetRecipeId(int Num) const
{
  return _listOfRecipesId[Num];
}

#pragma endregion MODEL

#pragma region DELEGATE

RecipesEditDelegate::RecipesEditDelegate(ResourceCalculator::ParamsCollection &PC, QObject *parent)
  : QStyledItemDelegate(parent), _PC(PC)
{
}

void RecipesEditDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
  QString ButtonCaption;
  switch (index.column()) {
  case 2: 
    ButtonCaption = "Результаты";
    break;
  case 3:
    ButtonCaption = "Интегреенты";
    break;
  case 4: 
    ButtonCaption = "Фабрики";
    break;
  default:
    QStyledItemDelegate::paint(painter, option, index);
    return;
    break;
  }
  QStyleOptionButton button;
  button.rect = option.rect;
  button.text = ButtonCaption;
  button.state = QStyle::State_Enabled;
  QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter);
}

bool RecipesEditDelegate::editorEvent(QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index)
{
  if (event->type() == QEvent::MouseButtonPress){
    switch (index.column()) {
    case 2:
    case 3:
    {
      ItemSelectedDialogMode mode = index.column() == 2 ?
        ItemSelectedDialogMode::ForRecipeSelectItemsResult :
        ItemSelectedDialogMode::ForRecipeSelectItemsRequired;
      RecipeListModel &model_recipe = dynamic_cast<RecipeListModel &>(*model);
      const ResourceCalculator::KEY_RECIPE recipe_key = model_recipe.GetRecipeId(index.row());
      ItemSelectedDialog _ItemSelectedDialog(_PC, mode, recipe_key);
      if (_ItemSelectedDialog.exec()) {
        ResourceCalculator::Recipe *recipe = _PC.RC.GetRecipeForEdit(recipe_key);
        auto Result = _ItemSelectedDialog.GetResult();
        if (mode == ItemSelectedDialogMode::ForRecipeSelectItemsRequired) {
          recipe->SetRequired(Result);
        } else{
          recipe->SetResult(Result);
        }
      }
      return false;
      break;
    }
    case 4:
    default:
      bool ff = QStyledItemDelegate::editorEvent(event, model, option, index);
      return ff;
      break;
    }
  }
  return QStyledItemDelegate::editorEvent(event, model, option, index);
}

#pragma endregion DELEGATE

RecipesEditDialog::RecipesEditDialog(ResourceCalculator::ParamsCollection &PC, QWidget *parent)
  : QDialog(parent), _PC(PC)
{

  setMinimumSize(800, 600);
   
  okButton = new QPushButton("OK");
  cancelButton = new QPushButton("Cancel");

  QTableView *tableView = new QTableView;
  tableView->setModel(new RecipeListModel(_PC, this));
  tableView->setItemDelegate(new RecipesEditDelegate(PC, this));

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
