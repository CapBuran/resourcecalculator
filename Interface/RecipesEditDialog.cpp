#include "RecipesEditDialog.h"
#include "ItemSelectedDialog.h"
#include "IconSelectedDialog.h"
#include "FactorysEditDialog.h"

#pragma region MODEL

using QT_CountsItem = std::set<ResourceCalculator::CountsItem>;
Q_DECLARE_METATYPE(QT_CountsItem);
using QT_KeysFactoryType = std::set<ResourceCalculator::KEY_TYPE_FACTORY>;
Q_DECLARE_METATYPE(QT_KeysFactoryType);

RecipeListModel::RecipeListModel(
  ResourceCalculator::RecipeCollection& RC,
  const ResourceCalculator::ItemCollection& IC,
  QObject *parent)
  : QAbstractTableModel(parent)
  , _RC(RC)
  , _IC(IC)
  , _RC_EDIT(RC.GetFactoryTypes())
{
  Select();
}

int RecipeListModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return _RC_EDIT.Size();
}

int RecipeListModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return 6;
}

QVariant RecipeListModel::data(const QModelIndex &index, int role) const
{
  using namespace ResourceCalculator;
  if (!index.isValid())
    return QVariant();
  if (index.row() >= _RC_EDIT.Size() || index.row() < 0)
    return QVariant();
  if (role == Qt::DisplayRole) {
    const Recipe& R = *_RC_EDIT.GetRecipe(_RC_EDIT.GetEnumKeyByKey(index.row()));
    switch (index.column()) {
    case 0://Icon
      return QString::fromStdString(R.GetIconKey());
      break;
    case 1://Recipe name
      return QString::fromStdString(R.GetName());
      break;
    case 2://Recipe time
      return QString::number(R.GetTime());
      break;
    case 3://Result of recipe
    {
      const QT_CountsItem &CI = R.GetResult();
      QVariant ret; ret.setValue<QT_CountsItem>(CI);
      return ret;
      break;
    }
    case 4://Ingredients of the recipe
    {
      const QT_CountsItem &CI = R.GetRequired();
      QVariant ret; ret.setValue<QT_CountsItem>(CI);
      return ret;
      break;
    }
    case 5://Allowed factories
    {
      return QVariant(static_cast<int>(R.GetTypeFactory()));
      break;
    }
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
      return tr("Icon");
    case 1:
      return tr("Recipe name");
    case 2:
      return tr("Recipe time");
    case 3:
      return tr("Result of recipe");
    case 4:
      return tr("Ingredients of the recipe");
    case 5:
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
    using namespace ResourceCalculator;
    Recipe& R = *_RC_EDIT.GetRecipeForEdit(_RC.GetEnumKeyByKey(index.row()));

    switch (index.column()) {
    case 0://Icon
    {
      R.SetIconKey(value.toString().toStdString());
      break;
    }
    case 1://Recipe name
    {
      QString Name = value.toString();
      if (Name.length() > 0) {
        R.SetName(Name.toStdString());
      }
      break;
    }
    case 2://Recipe time
    {
      double Value = value.toDouble();
      if (Value >= 0) {
        R.SetTime(Value);
      }
      break;
    }
    case 3://Result of recipe
    {
      const QT_CountsItem &CI = value.value<QT_CountsItem>();
      R.SetResult(CI);
      break;
    }
    case 4://Ingredients of the recipe
    {
      const QT_CountsItem &CI = value.value<QT_CountsItem>();
      R.SetRequired(CI);
      break;
    }
    case 5://Allowed factory
    {
      int Value = value.toInt();
      if (Value >= 0) {
        R.SetTypeFactory(static_cast< KEY_TYPE_FACTORY >(Value));
      }
      break;
    }
    default:
      return false;
      break;
    }
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
    const KEY_RECIPE NewKey = _RC_EDIT.GetUniqueEnumKey();
    QString Name(tr("New recipe") + ' ' + QString::number(static_cast<KEY_TO_Json>(NewKey)));
    Recipe ToADD;
    ToADD.SetKey(NewKey);
    ToADD.SetName(Name.toStdString());
    ToADD.SetTypeFactory(KEY_TYPE_FACTORY::Unknown);
    _RC_EDIT.Add(ToADD);
  }
  endInsertRows();
  return true;
}

bool RecipeListModel::removeRows(int position, int rows, const QModelIndex &index)
{
  Q_UNUSED(index);
  beginRemoveRows(QModelIndex(), position, position + rows - 1);
  using namespace ResourceCalculator;
  std::set<KEY_RECIPE> ToDelete;
  for (int row = 0; row < rows; ++row) {
    ToDelete.insert(_RC.GetEnumKeyByKey(position + row));
  }
  _RC_EDIT.Delete(ToDelete);
  endRemoveRows();
  return true;
}

void RecipeListModel::Commit()
{
  _RC = _RC_EDIT;
}

void RecipeListModel::Select()
{
  _RC_EDIT = _RC;
}

const ResourceCalculator::FactoryTypeCollection& RecipeListModel::GetFactoryTypes() const
{
  return _RC_EDIT.GetFactoryTypes();
}

const ResourceCalculator::ItemCollection& RecipeListModel::GetItems() const
{
  return _IC;
}

#pragma endregion MODEL

#pragma region DELEGATE

RecipesEditDelegate::RecipesEditDelegate(const ResourceCalculator::IconCollection& icons, QObject *parent)
  : QStyledItemDelegate(parent)
  , _Icons(icons)
{
}

void RecipesEditDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  switch (index.column()) {
  case 0:
  {
    QString IcopPath = index.data().toString();
    const ResourceCalculator::Icon &icon = _Icons.GetIcon(IcopPath.toStdString());
    if (icon.GetRawData().size() > 0) {
      QPixmap pixmap;
      pixmap.loadFromData((uchar*)&icon.GetRawData()[0], (uint)icon.GetRawData().size());
      const int MinCoord = std::min(option.rect.width(), option.rect.height());
      const int MaxCoord = std::max(option.rect.width(), option.rect.height());
      const int Sub1 = (MaxCoord - MinCoord) / 2;
      QRect rect;
      if (MaxCoord == option.rect.width()) {
        rect.setCoords(
          option.rect.left() + Sub1, option.rect.top(),
          option.rect.left() + Sub1 + MinCoord, option.rect.bottom());
      } else {
        rect.setCoords(
          option.rect.left(), option.rect.top() + Sub1,
          option.rect.right(), option.rect.top() + Sub1 + MinCoord);
      }
      painter->drawPixmap(rect, pixmap);
    }
    break;
  }
  case 3:
  case 4:
  {
    QStyleOptionButton button;
    button.rect = option.rect;
    button.text = index.column() == 3 ? tr("Result") : tr("Ingredients");
    button.state = QStyle::State_Enabled;
    QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter);
    break;
  }
  case 5:
  {
    ResourceCalculator::KEY_TYPE_FACTORY keyFactory = static_cast<ResourceCalculator::KEY_TYPE_FACTORY>(index.data().toInt());
    const RecipeListModel& model = dynamic_cast<const RecipeListModel&>(*index.model());

    QStyleOptionButton button;
    button.text = QString::fromStdString(model.GetFactoryTypes().GetFactoryType(keyFactory).Name);
    button.rect = option.rect;
    button.state = QStyle::State_Enabled;
    QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter, 0);
    break;
  }
  default:
    QStyledItemDelegate::paint(painter, option, index);
    return;
    break;
  }
}

bool RecipesEditDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index)
{
  if (event->type() == QEvent::MouseButtonPress) {
    switch (index.column()) {
    case 0:
    {
      IconSelectedDialog _IconSelectedDialog(_Icons);
      if (_IconSelectedDialog.exec()) {
        const ResourceCalculator::Icon * Icon = _IconSelectedDialog.GetResult();
        if (Icon != nullptr) {
          model->setData(index, QString::fromStdString(Icon->GetKeyPath()));
        }
      }
      return false;
      break;
    }
    case 3:
    {
      const RecipeListModel& modelRecipes = dynamic_cast<const RecipeListModel&>(*index.model());
      ItemSelectedDialog _ItemSelectedDialog(modelRecipes.GetItems(), _Icons, ItemSelectedDialogMode::ForRecipeSelectItemsResult, model->data(index).value<QT_CountsItem>(), nullptr);
      if (_ItemSelectedDialog.exec()) {
        QVariant V; V.setValue<QT_CountsItem>(_ItemSelectedDialog.GetResult());
        model->setData(index, V);
      }
      return false;
      break;
    }
    case 4:
    {
      const RecipeListModel& modelRecipes = dynamic_cast<const RecipeListModel&>(*index.model());

      ItemSelectedDialog _ItemSelectedDialog(modelRecipes.GetItems(), _Icons, ItemSelectedDialogMode::ForRecipeSelectItemsRequired, model->data(index).value<QT_CountsItem>(), nullptr);
      if (_ItemSelectedDialog.exec()) {
        QVariant V; V.setValue<QT_CountsItem>(_ItemSelectedDialog.GetResult());
        model->setData(index, V);
      }
      return false;
      break;
    }
    case 5:
    {
      const RecipeListModel& modelRecipes = dynamic_cast<const RecipeListModel&>(*index.model());
      FactoryTypesViewModel model2(modelRecipes.GetFactoryTypes(), nullptr);
      FactorysTypesSelectedDialog factorysTypesSelectedDialog(model2, false, nullptr);
      QVariant V = model->data(index);
      factorysTypesSelectedDialog.SetResult({static_cast<ResourceCalculator::KEY_TYPE_FACTORY>(V.value<int>())});
      if (factorysTypesSelectedDialog.exec())
      {
        std::set<ResourceCalculator::KEY_TYPE_FACTORY> result = factorysTypesSelectedDialog.GetResult();
        if (!result.empty())
          model->setData(index, QVariant::fromValue<int>(static_cast<int>(*result.begin())));
      }
      return false;
      break;
    }
    default:
      return QStyledItemDelegate::editorEvent(event, model, option, index);
      break;
    }
  }
  return QStyledItemDelegate::editorEvent(event, model, option, index);
}

#pragma endregion DELEGATE

RecipesEditDialog::RecipesEditDialog(ResourceCalculator::RecipeCollection& RC, const ResourceCalculator::ItemCollection& IC, const ResourceCalculator::IconCollection& icons, QWidget *parent)
  : QDialog(parent)
  , _Model(RC, IC, parent)
{
  setMinimumSize(1000, 600);

  QPushButton *okButton = new QPushButton(tr("Ok"));
  QPushButton *addButton = new QPushButton(tr("Add"));
  _removeButton = new QPushButton(tr("Remove"));
  QPushButton *cancelButton = new QPushButton(tr("Cancel"));

  _tableView = new QTableView();
  _tableView->setModel(&_Model);
  _tableView->setItemDelegate(new RecipesEditDelegate(icons, _tableView));
  _tableView->setSelectionMode(QTableView::SelectionMode::MultiSelection);
  _tableView->setSelectionBehavior(QTableView::SelectionBehavior::SelectRows);
  _tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  _tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
  _tableView->setColumnWidth(0, 60);

  QHBoxLayout *buttonLayout = new QHBoxLayout();
  buttonLayout->addWidget(okButton);
  buttonLayout->addWidget(addButton);
  buttonLayout->addWidget(_removeButton);
  buttonLayout->addWidget(cancelButton);

  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addWidget(_tableView);
  mainLayout->addLayout(buttonLayout);
  setLayout(mainLayout);

  connect(okButton, SIGNAL(clicked()), SLOT(PushButtonOk()));
  //connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);
  connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);
  connect(addButton, &QAbstractButton::clicked, this, &RecipesEditDialog::add_item);
  connect(_removeButton, &QAbstractButton::clicked, this, &RecipesEditDialog::remove_item);
  //connect(Delegate, &RecipesEditDelegate::editorEventDelegate, this, &RecipesEditDelegate::editorEventDelegate);

  setWindowTitle(tr("Recipes Edit"));

}

void RecipesEditDialog::add_item()
{
  QModelIndexList Rows = _tableView->selectionModel()->selectedRows();
  if (Rows.size() > 0) {
    _Model.insertRow(Rows[0].row());
  } else {
    _Model.insertRow(0);
  }
}

void RecipesEditDialog::remove_item()
{
  QModelIndexList RowsSelected = _tableView->selectionModel()->selectedRows();
  for (const auto& it: RowsSelected)
  {
    _Model.removeRow(it.row());
  }
}

void RecipesEditDialog::editorEventDelegate(const QModelIndex & index)
{
  QModelIndexList Rows = _tableView->selectionModel()->selectedRows();
  _removeButton->setEnabled(Rows.size() > 0);
}

void RecipesEditDialog::PushButtonOk()
{
  _Model.Commit();
  emit(&QDialog::accept);
  emit(close());
}
