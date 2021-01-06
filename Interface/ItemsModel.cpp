#include <ItemsModel.h>

ItemsModelRead::ItemsModelRead(const ResourceCalculator::ItemCollection& IC, QObject* parent)
  : QAbstractTableModel(parent)
  , _IC_EDIT(_RC_EDIT)
{
  _IC_EDIT.CloneFrom(IC);
  _RC_EDIT.CloneFrom(IC.GetRecipes());
}

ItemsModel::ItemsModel(ResourceCalculator::ItemCollection& IC, QObject* parent)
  : ItemsModelRead(IC, parent)
  , _IC(IC)
  , _RC(IC.GetRecipes())
{
}

int ItemsModelRead::rowCount(const QModelIndex& parent) const
{
  Q_UNUSED(parent);
  return _IC_EDIT.Size();
}

int ItemsModelRead::columnCount(const QModelIndex& parent) const
{
  Q_UNUSED(parent);
  return 5;
}

QVariant ItemsModelRead::data(const QModelIndex& index, int role) const
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
    case 4:
      return QVariant(item.GetCount());
      break;
    default:
      return QVariant();
      break;
    }
  }
  return QVariant();
}

QVariant ItemsModelRead::headerData(int section, Qt::Orientation orientation, int role) const
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
    case 4:
      return tr("Count");
    default:
      return QVariant();
    }
  }
  return QVariant();
}

Qt::ItemFlags ItemsModelRead::flags(const QModelIndex& index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;
  return QAbstractTableModel::flags(index);
}

Qt::ItemFlags ItemsModel::flags(const QModelIndex& index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;
  Qt::ItemFlags retval = QAbstractTableModel::flags(index);
  if (index.column() == 1 || index.column() == 3) {
    retval |= Qt::ItemIsEditable;
  }
  return retval;
}

bool ItemsModel::setData(const QModelIndex& index, const QVariant& value, int role)
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
    case 4:
      item.SetCount(value.toDouble());
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

bool ItemsModel::insertRows(int position, int rows, const QModelIndex& index)
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

bool ItemsModel::removeRows(int position, int rows, const QModelIndex& index)
{
  Q_UNUSED(index);
  beginResetModel();
  using namespace ResourceCalculator;
  std::set<KEY_ITEM> ToDelete;
  for (int row = 0; row < rows; ++row) {
    ToDelete.insert(_IC_EDIT(position + row));
  }
  bool retval = _IC_EDIT.Delete(ToDelete);
  endResetModel();
  return retval;
}

void ItemsModel::Commit()
{
  _IC.CloneFrom(_IC_EDIT);
  _RC.CloneFrom(_RC_EDIT);
};

ResourceCalculator::CountsItem ItemsModelRead::GetCountItem(const QModelIndex& index) const
{
  return ResourceCalculator::CountsItem(_IC_EDIT(index.row()), index.siblingAtColumn(4).data(Qt::DisplayRole).toDouble());
}

int ItemsModelRead::GetIndex(const ResourceCalculator::CountsItem& ci) const
{
  return _IC_EDIT(ci.ItemId);
}
