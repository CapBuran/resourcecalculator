#include "ItemsEditDialog.h"
#include "IconSelectedDialog.h"

#pragma region MODEL

ItemsEditModel::ItemsEditModel(ResourceCalculator::ParamsCollection &PC, QObject *parent) :
  QAbstractTableModel(parent), _PC(PC)
{
  using namespace ResourceCalculator;
  const std::map<KEY_ITEM, Item> &DATA = _PC.IC.GetData();
  _listOfItemsId.reserve(static_cast<int>(DATA.size()));
  for (auto &it : DATA) {
    _listOfItemsId.push_back(it);
  }
}

int ItemsEditModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return _listOfItemsId.size();
}

int ItemsEditModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return 3;
}

QVariant ItemsEditModel::data(const QModelIndex &index, int role) const
{
  using namespace ResourceCalculator;
  if (!index.isValid())
    return QVariant();
  if (index.row() >= _listOfItemsId.size() || index.row() < 0)
    return QVariant();
  if (role == Qt::DisplayRole) {
    const Item &R = _listOfItemsId.at(index.row()).second;
    switch (index.column()) {
    case 0:
      return QString::fromStdString(R.GetIconPath());
      break;
    case 1:
      return QString::fromUtf8(R.GetName().c_str());
      break;
    case 2:
      return QVariant(R.GetIsALiquidOrGas());
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
  if (index.column() == 1) {
    retval |= Qt::ItemIsEditable;
  }
  return retval;
}

bool ItemsEditModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if (index.isValid() && role == Qt::EditRole) {
    switch (index.column()) {
    case 0: {
      _listOfItemsId[index.row()].second.SetIconPath(value.toString().toStdString());
      break;
    }
    case 1: {
      std::string Name = value.toString().toStdString();
      if (Name.length() > 0) {
        _listOfItemsId[index.row()].second.SetName(Name);
      }
      break;
    }
    case 2:
      _listOfItemsId[index.row()].second.SetIsALiquidOrGas(value.toBool());
      break;
    default:
      return false;
      break;
    }
    _ItemsToAdd.erase(_listOfItemsId[index.row()].first);
    _ItemsToAdd.insert(_listOfItemsId[index.row()]);
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
    KEY_ITEM NewKey = _PC.IC.GetUniqueItemKey();
    QString Name(tr("New module") + QString(' ') + QString::number(static_cast<KEY_TO_Json>(NewKey)));
    std::pair<KEY_ITEM, Item > ToADD;
    ToADD.first = NewKey;
    ToADD.second.SetKey(NewKey);
    ToADD.second.SetName(Name.toStdString());
    _ItemsToAdd.insert(ToADD);
    _listOfItemsId.insert(position, ToADD);
  }
  endInsertRows();
  return true;
}

bool ItemsEditModel::removeRows(int position, int rows, const QModelIndex &index)
{
  Q_UNUSED(index);
  beginRemoveRows(QModelIndex(), position, position + rows - 1);
  for (int row = 0; row < rows; ++row) {
    _ItemsToDelete.insert(_listOfItemsId.at(position).first);
    _listOfItemsId.removeAt(position);
  }
  endRemoveRows();
  return true;
}

void ItemsEditModel::Commit()
{
  using namespace ResourceCalculator;
  for (auto it : _ItemsToAdd) {
    _ItemsToDelete.erase(it.first);
  }
  _PC.RC.Delete(_ItemsToDelete);
  _PC.IC.Delete(_ItemsToDelete);
  _PC.IC.Add(_ItemsToAdd);
  _ItemsToDelete.clear();
  _ItemsToAdd.clear();
  _listOfItemsId.clear();
  Select();
}

void ItemsEditModel::Select()
{
  using namespace ResourceCalculator;
  _ItemsToDelete.clear();
  _ItemsToAdd.clear();
  const std::map<KEY_ITEM, Item> &TypesModules = _PC.IC.GetData();
  _listOfItemsId.reserve(static_cast< int >(TypesModules.size()));
  for (auto & TypeFactory : TypesModules) {
    _listOfItemsId.push_back(TypeFactory);
  }
}

#pragma endregion MODEL

#pragma region DELEGATE

ItemEditDelegate::ItemEditDelegate(ResourceCalculator::ParamsCollection &PC, QObject *parent)
  : QStyledItemDelegate(parent), _PC(PC)
{
}

void ItemEditDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
  emit(editorEventDelegate(index));
  switch (index.column()) {
  case 0: {
    QString IcopPath = index.data().toString();
    const ResourceCalculator::Icon &icon = _PC.Icons.GetIcon(IcopPath.toStdString());
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
      IconSelectedDialog _IconSelectedDialog(_PC);
      if (_IconSelectedDialog.exec()) {
        const ResourceCalculator::Icon * Icon = _IconSelectedDialog.GetResult();
        if (Icon != nullptr) {
          model->setData(index, QString::fromStdString(Icon->GetIconPath()));
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

ItemsEditDialog::ItemsEditDialog(ResourceCalculator::ParamsCollection &PC, QWidget *parent)
  : QDialog(parent), _PC(PC), _Model(PC, parent)
{
  setMinimumSize(800, 600);

  QPushButton *okButton = new QPushButton(tr("OK"));
  QPushButton *cancelButton = new QPushButton(tr("Cancel"));
  QPushButton *addButton = new QPushButton(tr("ADD"));
  _removeButton = new QPushButton(tr("Remove"));

  ItemEditDelegate *Delegate = new ItemEditDelegate(PC, this);
  _tableView = new QTableView;
  _tableView->setModel(&_Model);
  _tableView->setItemDelegate(Delegate);
  _tableView->setSelectionMode(QTableView::SelectionMode::SingleSelection);
  _tableView->setSelectionBehavior(QTableView::SelectionBehavior::SelectRows);

  QHBoxLayout *buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(okButton);
  buttonLayout->addWidget(addButton);
  buttonLayout->addWidget(_removeButton);
  buttonLayout->addWidget(cancelButton);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(_tableView);
  mainLayout->addLayout(buttonLayout);
  setLayout(mainLayout);

  connect(okButton, SIGNAL(clicked()), SLOT(PushButtonOk()));
  connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);
  connect(addButton, &QAbstractButton::clicked, this, &ItemsEditDialog::add_item);
  connect(_removeButton, &QAbstractButton::clicked, this, &ItemsEditDialog::remove_item);
  connect(Delegate, &ItemEditDelegate::editorEventDelegate, this, &ItemsEditDialog::editorEventDelegate);

  setWindowTitle(tr("Item Edit"));
}

void ItemsEditDialog::remove_item()
{
  QModelIndexList RowsSelected = _tableView->selectionModel()->selectedRows();
  if (RowsSelected.size() > 0) {
    _Model.removeRow(RowsSelected[0].row());
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
  emit(&QDialog::accept);
  emit(close());
}