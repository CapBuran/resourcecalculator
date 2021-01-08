#include "ItemsEditDialog.h"
#include "IconSelectedDialog.h"

#pragma region DELEGATE

ItemEditDelegate::ItemEditDelegate(const ResourceCalculator::IconCollection& icons, QObject *parent)
  : QStyledItemDelegate(parent)
  , _Icons(icons)
{
}

void ItemEditDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
  emit(editorEventDelegate(index));
  switch (index.column()) {
  case 0: {
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
      }
      else {
        rect.setCoords(
          option.rect.left(), option.rect.top() + Sub1,
          option.rect.right(), option.rect.top() + Sub1 + MinCoord);
      }
      painter->drawPixmap(rect, pixmap);
    }
    break;
  }
  case 2: {
    bool IsALiquidOrGasForItem = index.data().toBool();
    QStyleOptionButton checkbox;
    checkbox.rect = option.rect;
    checkbox.text = IsALiquidOrGasForItem ? tr("Yes") : tr("No");
    checkbox.state = QStyle::State_Enabled;
    checkbox.state |= IsALiquidOrGasForItem ? QStyle::State_On : QStyle::State_Off;
    QApplication::style()->drawControl(QStyle::CE_CheckBox, &checkbox, painter);
    break;
  }
  default:
    QStyledItemDelegate::paint(painter, option, index);
    return;
    break;
  }
}

bool ItemEditDelegate::editorEvent(QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index)
{
  if (event->type() == QEvent::MouseButtonPress) {
    switch (index.column()) {
    case 0: {
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
    case 2:
      model->setData(index, !model->data(index).toBool());
      return false;
      break;
    default:
      return QStyledItemDelegate::editorEvent(event, model, option, index);
      break;
    }
  }
  return QStyledItemDelegate::editorEvent(event, model, option, index);
}

#pragma endregion DELEGATE

ItemsEditDialog::ItemsEditDialog(ResourceCalculator::ItemCollection& IC, const ResourceCalculator::IconCollection& icons, QWidget *parent)
  : QDialog(parent)
  , _Model(IC, parent)
{
  setMinimumSize(800, 600);

  QPushButton *okButton = new QPushButton(tr("OK"));
  QPushButton *cancelButton = new QPushButton(tr("Cancel"));
  QPushButton *addButton = new QPushButton(tr("ADD"));
  _removeButton = new QPushButton(tr("Remove"));

  ItemEditDelegate *Delegate = new ItemEditDelegate(icons, this);
  _tableView = new QTableView;
  //QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel();
  //proxyModel->setSourceModel(&_Model);
  //_tableView->setModel(proxyModel);
  _tableView->setModel(&_Model);
  _tableView->sortByColumn(1, Qt::AscendingOrder);
  _tableView->setItemDelegate(Delegate);
  _tableView->setSelectionMode(QTableView::SelectionMode::SingleSelection);
  _tableView->setSelectionBehavior(QTableView::SelectionBehavior::SelectRows);
  _tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  _tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
  _tableView->setColumnWidth(0, 60);

  QHBoxLayout *buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(okButton);
  buttonLayout->addWidget(addButton);
  buttonLayout->addWidget(_removeButton);
  buttonLayout->addWidget(cancelButton);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(_tableView);
  mainLayout->addLayout(buttonLayout);
  setLayout(mainLayout);

  //QObject::connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
  QObject::connect(okButton, SIGNAL(clicked()), this, SLOT(PushButtonOk()));
  QObject::connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

  //connect(okButton, SIGNAL(clicked()), SLOT(PushButtonOk()));
  //connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);
  connect(addButton, &QAbstractButton::clicked, this, &ItemsEditDialog::add_item);
  connect(_removeButton, &QAbstractButton::clicked, this, &ItemsEditDialog::remove_item);
  connect(Delegate, &ItemEditDelegate::editorEventDelegate, this, &ItemsEditDialog::editorEventDelegate);

  setWindowTitle(tr("Item Edit"));
}

void ItemsEditDialog::remove_item()
{
  QModelIndexList RowsSelected = _tableView->selectionModel()->selectedRows();
  for (const auto& it : RowsSelected)
  {
    _Model.removeRow(it.row());
  }
}

void ItemsEditDialog::editorEventDelegate(const QModelIndex & index)
{
  QModelIndexList Rows = _tableView->selectionModel()->selectedRows();
  _removeButton->setEnabled(Rows.size() > 0);
}

void ItemsEditDialog::add_item()
{
  QModelIndexList Rows = _tableView->selectionModel()->selectedRows();
  if (Rows.size() > 0) {
    _Model.insertRow(Rows[0].row());
  }
  else {
    _Model.insertRow(0);
  }
}

void ItemsEditDialog::PushButtonOk()
{
  _Model.Commit();
  //setResult(QDialog::Accepted);
  //close();

  emit(&QDialog::accept);
  emit(close());
}