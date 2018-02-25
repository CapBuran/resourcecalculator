#include "ItemSelectedDialog.h"

#pragma region MODEL

ItemSelectedModel::ItemSelectedModel(
  const ResourceCalculator::ParamsCollection &PC,
  ItemSelectedDialogMode Mode,
  ResourceCalculator::KEY_RECIPE recipe_key,
  QObject *parent):
  QAbstractTableModel(parent), _PC(PC), _Mode(Mode)
{
  using namespace ResourceCalculator;

  std::set<CountsItem> _ResultOld;

  if (
    recipe_key != ResourceCalculator::KEY_RECIPE::ID_RECIPE_PreviouslyProduced &&
    recipe_key != ResourceCalculator::KEY_RECIPE::ID_RECIPE_NoFindRecipe &&
    recipe_key != ResourceCalculator::KEY_RECIPE::ID_RECIPE_FindRecipeROOT
  ){
    const Recipe * recipe = PC.RC.GetRecipe(recipe_key);
    if (recipe != nullptr) {
      if (Mode == ItemSelectedDialogMode::ForRecipeSelectItemsResult) {
        _ResultOld = recipe->GetResult();
      }
      if (Mode == ItemSelectedDialogMode::ForRecipeSelectItemsRequired) {
        _ResultOld = recipe->GetRequired();
      }
    }
  }

  const std::map<KEY_ITEM, Item> &DATA = _PC.IC.GetData();
  _listOfItemsId.reserve(static_cast<int>(DATA.size()));
  for (auto &it : DATA) {
    CountsItem CI(it.first);
    auto f = _ResultOld.find(it.first);
    if (f != _ResultOld.end()){
      CI.Count = f->Count;
    }
    _listOfItemsId.push_back(CI);
  }
}

ItemSelectedModel::ItemSelectedModel(const ResourceCalculator::ParamsCollection & PC, ItemSelectedDialogMode Mode, const std::set<ResourceCalculator::CountsItem>& Result, QObject * parent)
  : QAbstractTableModel(parent), _PC(PC), _Mode(Mode)
{
  using namespace ResourceCalculator;
  const std::map<KEY_ITEM, Item> &DATA = _PC.IC.GetData();
  _listOfItemsId.reserve(static_cast<int>(DATA.size()));
  for (auto &it : DATA) {
    CountsItem CI(it.first);
    auto f = Result.find(it.first);
    if (f != Result.end()) {
      CI.Count = f->Count;
    }
    _listOfItemsId.push_back(CI);
  }
}

int ItemSelectedModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return _listOfItemsId.size();
}

int ItemSelectedModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return _Mode == ItemSelectedDialogMode::ForSelectOneItem ? 2 : 3;
}

QVariant ItemSelectedModel::data(const QModelIndex &index, int role) const
{
  using namespace ResourceCalculator;

  if (!index.isValid())
    return QVariant();

  if (index.row() >= _listOfItemsId.size() || index.row() < 0)
    return QVariant();

  if (role == Qt::DisplayRole) {
    const Item *R = _PC.IC.GetItem( _listOfItemsId[index.row()].ItemId );
    if (R == nullptr) {
      return QVariant();
    }
    switch (index.column())
    {
    case 0:
      return QString( R->GetIconPath().c_str() );
      break;
    case 1:
      return QString( R->GetName().c_str() );
      break;
    case 2:
      return QString::number( _listOfItemsId[index.row()].Count );
      break;
    default:
      return QVariant();
      break;
    }
  }
  return QVariant();
}

QVariant ItemSelectedModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal) {
    switch (section) {
    case 0:
      return tr("Icon");
    case 1:
      return tr("Item Name");
    case 2:
      return tr("Number of items");
    default:
      return QVariant();
    }
  }
  return QVariant();
}
 
Qt::ItemFlags ItemSelectedModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  return Qt::ItemIsSelectable | Qt::ItemIsEnabled | (index.column() == 2 ? Qt::ItemIsEditable : Qt::NoItemFlags);
}

int ItemSelectedModel::GetItemRow(ResourceCalculator::KEY_ITEM ItemKey)
{
  for (int i = 0; i < _listOfItemsId.size(); i++){
    if (ItemKey == _listOfItemsId[i].ItemId){
      return i;
    }
  }
  return -1;
}

bool ItemSelectedModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
  if (index.isValid() && role == Qt::EditRole) {
    
    Q_ASSERT(
      index.column() == 2 && 
      (
        _Mode == ItemSelectedDialogMode::ForRecipeSelectItemsRequired ||
        _Mode == ItemSelectedDialogMode::ForRecipeSelectItemsResult
      )
    );

    switch (index.column())
    {
    case 2:
      _listOfItemsId[index.row()].Count = value.toDouble();
      break;
    default:
      return false;
      break;
    }
    emit(dataChanged(index, index));
    return true;
  }
  return false;
}

const ResourceCalculator::CountsItem & ItemSelectedModel::GetItemData(int Num) const
{
  return _listOfItemsId[Num];
}

#pragma endregion MODEL

#pragma region DELEGATE

ItemSelectedDelegate::ItemSelectedDelegate(const ResourceCalculator::ParamsCollection &PC, ItemSelectedDialogMode Mode, QObject *parent)
  : QStyledItemDelegate(parent), _Mode(Mode), _PC(PC)
{
}

void ItemSelectedDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
  switch (index.column()) {
  case 0: {
    std::string IconPath = index.data().toString().toStdString();
    const ResourceCalculator::Icon &icon = _PC.Icons.GetIcon( IconPath );
    if ( icon.GetRawData().size() > 0 ) {
      QPixmap pixmap;
      pixmap.loadFromData( ( uchar* ) &icon.GetRawData()[0], ( uint ) icon.GetRawData().size() );
      const int MinCoord = std::min( option.rect.width(), option.rect.height() );
      const int MaxCoord = std::max( option.rect.width(), option.rect.height() );
      const int Sub1 = ( MaxCoord - MinCoord ) / 2;
      QRect rect;
      if ( MaxCoord == option.rect.width() ) {
        rect.setCoords(
          option.rect.left() + Sub1, option.rect.top(),
          option.rect.left() + Sub1 + MinCoord, option.rect.bottom() );
      } else {
        rect.setCoords(
          option.rect.left(), option.rect.top() + Sub1,
          option.rect.right(), option.rect.top() + Sub1 + MinCoord );
      }
      painter->drawPixmap( rect, pixmap );
    }
   break;
  }
  default:
    QStyledItemDelegate::paint(painter, option, index);
    return;
    break;
  }
}

#pragma endregion DELEGATE

ItemSelectedDialog::ItemSelectedDialog(
  const ResourceCalculator::ParamsCollection &PC,
  ItemSelectedDialogMode Mode,
  ResourceCalculator::KEY_RECIPE recipe_key,
  QWidget *parent)
  : QDialog( parent ), _PC( PC ), _Mode( Mode ), _Model( PC, _Mode, recipe_key )
{
  setMinimumSize(400, 600);
   
  QPushButton *okButton = new QPushButton(tr("OK"));
  QPushButton *cancelButton = new QPushButton(tr("Cancel"));
  _tableView = new QTableView;
  _tableView->setSelectionMode(
    _Mode == ItemSelectedDialogMode::ForSelectOneItem ?
    QTableView::SelectionMode::SingleSelection:
    QTableView::SelectionMode::MultiSelection
  );
  _tableView->setSelectionBehavior(QTableView::SelectionBehavior::SelectRows);
  _tableView->setModel(&_Model);
  _tableView->setItemDelegate(new ItemSelectedDelegate(PC, _Mode));

  if (_Mode != ItemSelectedDialogMode::ForSelectOneItem) {
    using namespace ResourceCalculator;
    const Recipe *recipe = PC.RC.GetRecipe(recipe_key);
    if ( recipe != nullptr ) {
      auto _ResultOld = _Mode == ItemSelectedDialogMode::ForRecipeSelectItemsRequired ?
        recipe->GetRequired() : recipe->GetResult();
      for ( auto it : _ResultOld ) {
        int row = _Model.GetItemRow( it.ItemId );
        if ( row >= 0 ) {
          _tableView->selectRow( row );
        }
      }
    }
  }

  QHBoxLayout *buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(okButton);
  buttonLayout->addWidget(cancelButton);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(_tableView);
  mainLayout->addLayout(buttonLayout);
  setLayout(mainLayout);

  connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);
  connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);

  if (Mode == ItemSelectedDialogMode::ForSelectOneItem) {
    setWindowTitle(tr("Select an item"));
  }else{
    setWindowTitle(tr("Select items"));
  }
  
}

ItemSelectedDialog::ItemSelectedDialog(const ResourceCalculator::ParamsCollection & PC, ItemSelectedDialogMode Mode, const std::set<ResourceCalculator::CountsItem> &Result, QWidget * parent)
  : QDialog(parent), _PC(PC), _Mode(Mode), _Model(PC, _Mode, Result)
{
  setMinimumSize(700, 600);

  QPushButton *okButton = new QPushButton(tr("OK"));
  QPushButton *cancelButton = new QPushButton(tr("Cancel"));
  _tableView = new QTableView;
  _tableView->setSelectionMode(
    _Mode == ItemSelectedDialogMode::ForSelectOneItem ?
    QTableView::SelectionMode::SingleSelection :
    QTableView::SelectionMode::MultiSelection
  );
  _tableView->setSelectionBehavior(QTableView::SelectionBehavior::SelectRows);
  _tableView->setModel(&_Model);
  _tableView->setItemDelegate(new ItemSelectedDelegate(PC, _Mode));
  _tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

  if (_Mode != ItemSelectedDialogMode::ForSelectOneItem) {
    using namespace ResourceCalculator;
    for (auto it : Result) {
      int row = _Model.GetItemRow(it.ItemId);
      if (row >= 0) {
        _tableView->selectRow(row);
      }
    }
  }

  QHBoxLayout *buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(okButton);
  buttonLayout->addWidget(cancelButton);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(_tableView);
  mainLayout->addLayout(buttonLayout);
  setLayout(mainLayout);

  connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);
  connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);

  if (Mode == ItemSelectedDialogMode::ForSelectOneItem) {
    setWindowTitle(tr("Select an item"));
  } else {
    setWindowTitle(tr("Select items"));
  }

}

ResourceCalculator::KEY_ITEM ItemSelectedDialog::GetResultOne() const
{
  ResourceCalculator::KEY_ITEM RetVal = ResourceCalculator::KEY_ITEM::ID_ITEM_NoFind_Item;
  QModelIndexList Rows = _tableView->selectionModel()->selectedRows();
  if (Rows.size() > 0){
    RetVal = _Model.GetItemData( Rows[0].row() ).ItemId;
  }
  return RetVal;
}

std::set<ResourceCalculator::CountsItem> ItemSelectedDialog::GetResult() const
{
  std::set<ResourceCalculator::CountsItem> RetVal;
  QModelIndexList Rows = _tableView->selectionModel()->selectedRows();
  for (auto & Row : Rows){
    ResourceCalculator::CountsItem InsertOne = _Model.GetItemData(Row.row());
    RetVal.insert(InsertOne);
  }
  return RetVal;
}
