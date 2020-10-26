#include "ItemsEditDialog.h"
#include "IconSelectedDialog.h"

#pragma region MODEL

ItemsEditModel::ItemsEditModel(ResourceCalculator::ItemCollection& IC, QObject* parent)
  : QAbstractTableModel(parent)
  , _IC(IC)
  , _RC_EDIT(_IC.GetRecipes().GetFactoryTypes())
  , _IC_EDIT(_RC_EDIT)
{
  Select();
}

int ItemsEditModel::rowCount(const QModelIndex& parent) const
{
  Q_UNUSED(parent);
  return _IC_EDIT.Size();
}

int ItemsEditModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return 4;
}

QVariant ItemsEditModel::data(const QModelIndex &index, int role) const
{
  using namespace ResourceCalculator;
  if (!index.isValid())
    return QVariant();
  if (index.row() >= _IC_EDIT.Size() || index.row() < 0)
    return QVariant();
  if (role == Qt::DisplayRole) {
    const Item& item = _IC_EDIT[_IC_EDIT(index.row())];
    if (!item) return QVariant();
    switch (index.column()) {
    case 0:
      return QString::fromStdString(item.GetIconKey());
      break;
    case 1:
      return QString::fromUtf8(item.GetName().c_str());
      break;
    case 2:
      return QVariant(item.GetIsALiquidOrGas());
      break;
    case 3:
      return QVariant(item.GetMiningHardness());
      break;
    default:
      return QVariant();
      break;
    }
  }
  return QVariant();
}

QVariant ItemsEditModel::headerData(int section, Qt::Orientation orientation, int role) const
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
      return tr("Is a liquid or gas");
    case 3:
      return tr("Mining hardness");
    default:
      return QVariant();
    }
  }
  return QVariant();
}

Qt::ItemFlags ItemsEditModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;
  Qt::ItemFlags retval = QAbstractTableModel::flags(index);
  if (index.column() == 1 || index.column() == 3) {
    retval |= Qt::ItemIsEditable;
  }
  return retval;
}

bool ItemsEditModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  using namespace ResourceCalculator;
  if (index.isValid() && role == Qt::EditRole) {
    Item& item = _IC_EDIT[(_IC_EDIT(index.row()))];
    if (item) {
      return false;
    }
    switch (index.column()) {
    case 0: {
      item.SetIconKey(value.toString().toStdString());
      break;
    }
    case 1: {
      std::string Name = value.toString().toStdString();
      if (Name.length() > 0) {
        item.SetName(Name);
      }
      break;
    }
    case 2:
      item.SetIsALiquidOrGas(value.toBool());
      break;
    case 3:
      item.SetMiningHardness(value.toDouble());
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

bool ItemsEditModel::insertRows(int position, int rows, const QModelIndex &index)
{
  Q_UNUSED(index);
  beginInsertRows(QModelIndex(), position, position + rows - 1);
  for (int row = 0; row < rows; ++row) {
    using namespace ResourceCalculator;
    KEY_ITEM NewKey = _IC_EDIT.NewKey();
    QString Name(tr("New item") + QString(' ') + QString::number(static_cast<KEY_TO_Json>(NewKey)));
    std::pair<KEY_ITEM, Item > ToADD;
    ToADD.first = NewKey;
    ToADD.second.SetKey(NewKey);
    ToADD.second.SetName(Name.toStdString());
    _IC_EDIT.Add({ ToADD });
  }
  endInsertRows();
  return true;
}

bool ItemsEditModel::removeRows(int position, int rows, const QModelIndex &index)
{
  Q_UNUSED(index);
  beginResetModel();
  std::set<ResourceCalculator::KEY_ITEM> removes;
  for (int row = 0; row < rows; ++row)
  {
    removes.insert(_IC(position));
  }
  bool retval = _IC_EDIT.Delete(removes);
  endResetModel();
  return retval;
}

void ItemsEditModel::Commit()
{
  _IC = _IC_EDIT;
  _IC.GetRecipes() = _RC_EDIT;
}

void ItemsEditModel::Select()
{
  _IC_EDIT = _IC;
  _RC_EDIT = _IC.GetRecipes();
}

#pragma endregion MODEL

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
  _tableView->setSelectionMode(QTableView::SelectionMode::MultiSelection);
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