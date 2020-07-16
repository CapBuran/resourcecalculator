#include "ProductionChainWidgetSummProductionItems.h"

#pragma region MODEL

ProductionChainModelSummProductionItems::ProductionChainModelSummProductionItems(const ResourceCalculator::ParamsCollection &PC, QObject *parent )
  : QAbstractTableModel( parent ), _PC(PC)
{
}

int ProductionChainModelSummProductionItems::rowCount( const QModelIndex &parent ) const
{
  Q_UNUSED( parent );
  return static_cast<int>(_PC.IC.GetData().size());
}

int ProductionChainModelSummProductionItems::columnCount( const QModelIndex &parent ) const
{
  Q_UNUSED( parent );
  //return static_cast<int>(_PC.PCC.GetReadingPCMs().size() + 2);
  return 2;//TODO
}

QVariant ProductionChainModelSummProductionItems::data( const QModelIndex &index, int role ) const
{
  using namespace ResourceCalculator;

  if ( !index.isValid() )
    return QVariant();

  if ( role != Qt::DisplayRole )
    return QVariant();

  if (index.column() >= 1) {
    if (_DATA.size() > index.row()) {
      if (_DATA[index.row()].size() > index.column() - 1) {
        return QVariant(ToOut(_DATA[index.row()][index.column() - 1]));
      }
    }
  }

  if (index.column() == 0) {
    const Item *item = _PC.IC.GetItemByID(index.row());
    if (item != nullptr) {
      return QString::fromStdString(item->GetIconKey());
    }
  }
  
  return QVariant();
}

QVariant ProductionChainModelSummProductionItems::headerData( int section, Qt::Orientation orientation, int role ) const
{
  using namespace ResourceCalculator;

  if ( role != Qt::DisplayRole )
    return QVariant();

  if ( orientation == Qt::Horizontal) {
    return _HorizontalHeader[section];
  }

  if (orientation == Qt::Vertical) {
    return _VerticalHeader[section];
  }
  return QVariant();
}

Qt::ItemFlags ProductionChainModelSummProductionItems::flags( const QModelIndex &index ) const
{
  if ( !index.isValid() )
    return Qt::ItemIsEnabled;
  Qt::ItemFlags flags = QAbstractTableModel::flags( index );
  return flags;
}

void ProductionChainModelSummProductionItems::Update()
{
  using namespace ResourceCalculator;
  beginResetModel();
  const int CountItems = static_cast<int>(_PC.IC.GetData().size());
  //const int CountChain = static_cast<int>(_PC.PCC.GetReadingPCMs().size());
  const int CountChain = 0;

  _HorizontalHeader.clear();
  _VerticalHeader.clear();
  _DATA.clear();

  _DATA.resize(CountItems + 1);
  _VerticalHeader.resize(CountItems);
  for (int row = 0; row < CountItems; row++) {
    const Item * item = _PC.IC.GetItemByID(row);
    if (item != nullptr) {
      _VerticalHeader[row] = QString::fromStdString(item->GetName());
    } else {
      _VerticalHeader[row] = tr("Production chain is deleted!!!");
    }
    QVector<double> &RowData = _DATA[row];
    RowData.resize(CountChain + 1);
    for (int col = 0; col < CountChain; col++) {
      RowData[col] = 0.0;
    }
  }

  _HorizontalHeader.resize(CountChain + 2);
  _HorizontalHeader[0] = tr("Icon");
  _HorizontalHeader[CountChain+1] = tr("Summ by production chain");
  for (int col = 0; col < CountChain; col++) {
    //const ProductionChainModel* PCM = _PC.PCC.GetPCM(col);
    //const Item * item = _PC.IC.GetItem(PCM->GetItemKey());
    //if (item != nullptr) {
    //  _HorizontalHeader[col + 1] = QString::fromStdString(item->GetName());
    //} else {
    //  _HorizontalHeader[col + 1] = tr("Production chain is deleted!!!");
    //}
    //double SummByChain = 0.0;
    //const std::vector<KEY_ITEM>& ColsItems = PCM->GetColsItems();
    //for (size_t ItemIDFind = 0; ItemIDFind < ColsItems.size(); ItemIDFind++) {
    //  KEY_ITEM KeyFind = ColsItems[ItemIDFind];
    //  int row = _PC.IC.GetItemIdByKey(KeyFind);
    //  KEY_ITEM KeyFind2 = _PC.IC.GetItemByID(row)->GetKey();
    //  if (KeyFind == KeyFind2) {
    //    QVector<double> &RowData = _DATA[row];
    //    const std::vector<double> &SummSpeeds = PCM->GetSummSpeeds();
    //    _DATA[row][col] = SummSpeeds[ItemIDFind];
    //  }
    //}
  }
  for (int row = 0; row < CountItems; row++) {
    QVector<double> &RowData = _DATA[row];
    double Summ = 0.0;
    for (int col = 0; col < CountChain; col++) {
      Summ += RowData[col];
    }
    RowData[CountChain] = Summ;
  }

  endResetModel();
}

#pragma endregion MODEL

#pragma region DELEGATE

ProductionChainDelegateSummProductionItems::ProductionChainDelegateSummProductionItems(const ResourceCalculator::ParamsCollection &PC, QObject *parent)
  : QStyledItemDelegate(parent), _PC(PC)
{
}

void ProductionChainDelegateSummProductionItems::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
  switch (index.column()) {
  case 0:
  {
    std::string IconPath = index.data().toString().toStdString();
    const ResourceCalculator::Icon &icon = _PC.Icons.GetIcon(IconPath);
    if (icon.GetRawData().size() > 0) {
      QPixmap pixmap;
      pixmap.loadFromData((uchar*)&icon.GetRawData()[0], (uint)icon.GetRawData().size());
      const int MinCoord = std::min(option.rect.width(), option.rect.height());
      const int MaxCoord = std::max(option.rect.width(), option.rect.height());
      QRect rect;
      if (MaxCoord == option.rect.width()) {
        rect.setCoords(
          option.rect.left() + 2, option.rect.top(),
          option.rect.left() + 2 + MinCoord, option.rect.bottom());
      } else {
        rect.setCoords(
          option.rect.left() + 2, option.rect.top(),
          option.rect.right() + 2, option.rect.top() + MinCoord);
      }
      painter->drawPixmap(rect, pixmap);
    }
    break;
  }
  default:
    QStyledItemDelegate::paint(painter, option, index);
    return;
    break;
  }
}

QSize ProductionChainDelegateSummProductionItems::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const
{
  if (index.column() == 0)
  {
    return QSize(20, option.rect.height());
  }
  return QStyledItemDelegate::sizeHint(option, index);
}

#pragma endregion DELEGATE

ProductionChainWidgetSummProductionItems::ProductionChainWidgetSummProductionItems(const ResourceCalculator::ParamsCollection &PC, QWidget *parent ):
  ProductionChainWidgetBase(PC, parent), _Model(PC, parent), _ModelTree(PC)
{
  _Model.Update();

  setMinimumSize(700, 600);

  QTableView *tableView = new QTableView();
  tableView->setSelectionMode(QTableView::SelectionMode::SingleSelection);
  tableView->setSelectionBehavior(QTableView::SelectionBehavior::SelectRows);
  tableView->setModel(&_Model);
  tableView->sortByColumn(1, Qt::AscendingOrder);
  tableView->setItemDelegate(new ProductionChainDelegateSummProductionItems(PC));
  tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  tableView->setHorizontalHeader(new ProductionChainHeaderView(Qt::Orientation::Horizontal));
  tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

  tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
  tableView->setColumnWidth(0, 30);

  QTreeView *TreeView = new QTreeView();
  TreeView->setModel(&_ModelTree);
  TreeView->setSelectionMode(QTableView::SelectionMode::SingleSelection);
  TreeView->setSelectionBehavior(QTableView::SelectionBehavior::SelectRows);
  TreeView->setItemDelegate(new ProductionChainDelegateSummProductionItems(PC));
  TreeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

  QItemSelectionModel *sm = tableView->selectionModel();
  connect(sm, SIGNAL(selectionChanged(QItemSelection, QItemSelection)), this, SLOT(on_selectionChanged(QItemSelection, QItemSelection)));

  addWidget(tableView);
  addWidget(TreeView);

  setWindowTitle(tr("Sum of all items produced"));
}

ProductionChainWidgetType ProductionChainWidgetSummProductionItems::GetType() const
{
  return ProductionChainWidgetType::SummProductionItems;
}

void ProductionChainWidgetSummProductionItems::UpdateModel()
{
  _Model.Update();
}

void ProductionChainWidgetSummProductionItems::on_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
  if (selected.indexes().size() > 0) {
    int row = selected.indexes()[0].row();
    const ResourceCalculator::Item *item = _PC.IC.GetItemByID(row);
    if (item!= nullptr) {
      _ModelTree.SetItemID(item->GetKey());
    }
  }
}
