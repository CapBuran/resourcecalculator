#include "ItemSelectedDialog.h"

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

//ItemsModel(ResourceCalculator::ItemCollection& IC, ResourceCalculator::RecipeCollection& RC, QObject* parent = 0);

ItemSelectedDialog::ItemSelectedDialog(
  const ResourceCalculator::ItemCollection& IC,
  const ResourceCalculator::IconCollection& icons,
  ItemSelectedDialogMode Mode,
  const std::set<ResourceCalculator::CountsItem>& select,
  QWidget *parent
)
  : QDialog( parent )
  , _Model(IC)
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
      int row = _Model.GetIndex(it);
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
  , _Model(IC)
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
  std::set<ResourceCalculator::CountsItem> keys;
  const auto indexes_qt = _tableView->selectionModel()->selectedRows();
  for (auto index: indexes_qt)
    keys.insert(_Model.GetCountItem(index));
  return keys;
}

