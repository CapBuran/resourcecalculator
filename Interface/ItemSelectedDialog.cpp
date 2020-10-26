#include "ItemSelectedDialog.h"

#pragma region MODEL


ItemSelectedModel::ItemSelectedModel(
  const ResourceCalculator::ItemCollection& IC,
  const std::set<ResourceCalculator::CountsItem>& oldValues,
  QObject* parent 
)
  : QAbstractTableModel(parent)
  , _IsOneItem(false)
  , _IC(IC)
{
  using namespace ResourceCalculator;
  TYPE_KEY CountItems = IC.Size();
  _Status.resize(CountItems);

  for (TYPE_KEY i = 0; i < CountItems; i++)
  {
    _Status[i].ItemId = _IC(i);
  }

  for (const auto& r: oldValues)
  {
    int rowId = _IC(r.ItemId);
    _Status[rowId] = r;
  }
}

ItemSelectedModel::ItemSelectedModel(
  const ResourceCalculator::ItemCollection& IC,
  QObject* parent
)
  : QAbstractTableModel(parent)
  , _IsOneItem(true)
  , _IC(IC)
{
  using namespace ResourceCalculator;
  TYPE_KEY CountItems = IC.Size();
  _Status.resize(CountItems);

  for (TYPE_KEY i = 0; i < CountItems; i++)
  {
    _Status[i].ItemId = _IC(i);
  }
}

int ItemSelectedModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return _IC.Size();
}

int ItemSelectedModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return _IsOneItem ? 2 : 3;
}

QVariant ItemSelectedModel::data(const QModelIndex &index, int role) const
{
  using namespace ResourceCalculator;

  if (!index.isValid())
    return QVariant();

  if (index.row() >= _IC.Size() || index.row() < 0)
    return QVariant();

  if (role == Qt::DisplayRole) {
    
    const Item& item = _IC[_IC(index.row())];
    if (!item) return QVariant();
    switch (index.column())
    {
    case 0:
      return QString(item.GetIconKey().c_str() );
      break;
    case 1:
      return QString(item.GetName().c_str() );
      break;
    case 2:
      return QString::number(_Status[index.row()].Count);
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

bool ItemSelectedModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
  if (index.isValid() && role == Qt::EditRole) {
    
    Q_ASSERT(index.column() == 2 && !_IsOneItem);

    switch (index.column())
    {
    case 2:
      _Status[index.row()].Count = value.toDouble();
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

std::set<ResourceCalculator::CountsItem> ItemSelectedModel::GetResult(const QModelIndexList& Rows) const
{
  std::set<ResourceCalculator::CountsItem> RetVal;

  for (auto& Row: Rows) {
    if (Row.row() < _Status.size())
    {
      ResourceCalculator::CountsItem InsertOne = _Status[Row.row()];
      RetVal.insert(InsertOne);
    }
  }
  return RetVal;
}

#pragma endregion MODEL

#pragma region DELEGATE

ItemSelectedDelegate::ItemSelectedDelegate(
  const ResourceCalculator::IconCollection& icons,
  ItemSelectedDialogMode Mode,
  QObject* parent
)
  : QStyledItemDelegate(parent)
  , _Mode(Mode)
  , _Icons(icons)
{
}

void ItemSelectedDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
  switch (index.column()) {
  case 0: {
    std::string IconPath = index.data().toString().toStdString();
    const ResourceCalculator::Icon &icon = _Icons.GetIcon( IconPath );
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

QSize ItemSelectedDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
  if (index.column() == 0)
  {
    return QSize(20, option.rect.height());
  }
  return QStyledItemDelegate::sizeHint(option, index);
}

#pragma endregion DELEGATE

ItemSelectedDialog::ItemSelectedDialog(
  const ResourceCalculator::ItemCollection& IC,
  const ResourceCalculator::IconCollection& icons,
  ItemSelectedDialogMode Mode,
  const std::set<ResourceCalculator::CountsItem>& select,
  QWidget *parent
)
  : QDialog( parent )
  , _Model(IC, select, parent)
{
  setMinimumSize(400, 600);
   
  QPushButton *okButton = new QPushButton(tr("OK"));
  QPushButton *cancelButton = new QPushButton(tr("Cancel"));
  _tableView = new QTableView;
  _tableView->setSelectionMode(
    Mode == ItemSelectedDialogMode::ForSelectOneItem ?
    QTableView::SelectionMode::SingleSelection:
    QTableView::SelectionMode::MultiSelection
  );
  _tableView->setSelectionBehavior(QTableView::SelectionBehavior::SelectRows);
  _tableView->setModel(&_Model);
  _tableView->setItemDelegate(new ItemSelectedDelegate(icons, Mode));
  _tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  //_tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
  //_tableView->setColumnWidth(0, 60);

  if (Mode != ItemSelectedDialogMode::ForSelectOneItem) {
    using namespace ResourceCalculator;
    for (const auto& it: select)
    {
      int row = IC(it.ItemId);
      if ( row >= 0 ) {
        _tableView->selectRow( row );
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

ItemSelectedDialog::ItemSelectedDialog(
  const ResourceCalculator::ItemCollection& IC,
  const ResourceCalculator::IconCollection& icons,
  QWidget * parent
)
  : QDialog(parent)
  , _Model(IC, parent)
{
  setMinimumSize(700, 600);

  QPushButton *okButton = new QPushButton(tr("OK"));
  QPushButton *cancelButton = new QPushButton(tr("Cancel"));
  _tableView = new QTableView(this);
  _tableView->setSelectionMode(QTableView::SelectionMode::SingleSelection);
  _tableView->setSelectionBehavior(QTableView::SelectionBehavior::SelectRows);
  //QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel();
  //proxyModel->setSourceModel(&_Model);
  //_tableView->setModel(proxyModel);
  _tableView->setModel(&_Model);
  _tableView->sortByColumn(1, Qt::AscendingOrder);
  _tableView->setItemDelegate(new ItemSelectedDelegate(icons, ItemSelectedDialogMode::ForSelectOneItem));
  _tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  //_tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
  //_tableView->setColumnWidth(0, 60);

  QHBoxLayout *buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(okButton);
  buttonLayout->addWidget(cancelButton);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(_tableView);
  mainLayout->addLayout(buttonLayout);
  setLayout(mainLayout);

  connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);
  connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);

  setWindowTitle(tr("Select an item"));
}

std::set<ResourceCalculator::CountsItem> ItemSelectedDialog::GetResult() const
{
  return _Model.GetResult(_tableView->selectionModel()->selectedRows());
}

