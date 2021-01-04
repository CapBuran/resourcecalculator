#include <FactorysModel.h>

FactorysModel::FactorysModel(ResourceCalculator::FactoryTypeCollection& FTC, QObject* parent)
  : QAbstractTableModel(parent)
  , _FTC(FTC)
  , _FTC_Edit(_FC_Edit)
{
  Select();
}

int FactorysModel::rowCount(const QModelIndex& parent) const
{
  Q_UNUSED(parent);
  return _FC_Edit.Size();
}

int FactorysModel::columnCount(const QModelIndex& parent) const
{
  Q_UNUSED(parent);
  return 7;
}

QVariant FactorysModel::data(const QModelIndex& index, int role) const
{
  using namespace ResourceCalculator;

  if (!index.isValid())
    return QVariant();

  if (index.row() >= _FC_Edit.Size() || index.row() < 0)
    return QVariant();

  const Factory& factory = _FC_Edit[_FC_Edit(index.row())];

  if (role == Qt::DisplayRole && 0 <= index.column() && index.column() <= 6) {
    switch (index.column()) {
    case 0:
      return QString::fromStdString(factory.GetIconKey());
      break;
    case 1:
      return QString::fromStdString(factory.GetName());
      break;
    case 2:
      return QVariant::fromValue<QT_KeysFactoryType>(factory.GetTypes());
      break;
    case 3:
      return QVariant::fromValue(factory.GetSpeed());
      break;
    case 4:
      return QVariant::fromValue(factory.GetPower());
      break;
    case 5:
      return QVariant::fromValue(factory.GetCountSlotsForRecipes());
      break;
    case 6:
      return QVariant::fromValue(factory.GetCountSlotsForModules());
      break;
    default:
      return QVariant();
      break;
    }
  }
  return QVariant();
}

QVariant FactorysModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal) {
    switch (section) {
    case 0:
      return tr("Icon");
    case 1:
      return tr("Factory name");
    case 2:
      return tr("Factory types");
    case 3:
      return tr("Speed");
    case 4:
      return tr("Drilling power");
    case 5:
      return tr("Slots for recipe");
    case 6:
      return tr("Slots for modules");
    default:
      return QVariant();
    }
  }
  return QVariant();
}

bool FactorysModel::insertRows(int position, int rows, const QModelIndex& index)
{
  Q_ASSERT(position + rows - 1 >= 0);
  Q_UNUSED(index);
  beginInsertRows(QModelIndex(), position, position + rows - 1);
  for (int row = 0; row < rows; ++row) {
    using namespace ResourceCalculator;
    KEY_FACTORY NewKey = _FC_Edit.NewKey();
    QString Name(tr("New factory") + QString(' ') + QString::number(static_cast<KEY_TO_Json>(NewKey)));
    Factory ToADD;
    ToADD.SetName(Name.toStdString());
    ToADD.SetKey(NewKey);
    _FC_Edit.Add(ToADD);
  }
  endInsertRows();
  return true;
}

bool FactorysModel::removeRows(int position, int rows, const QModelIndex& index)
{
  Q_UNUSED(index);
  beginRemoveRows(QModelIndex(), position, position + rows - 1);

  std::set<ResourceCalculator::KEY_FACTORY> factoryKeys;

  for (int row = 0; row < rows; ++row) {
    factoryKeys.insert(_FC_Edit(row + position));
  }

  _FC_Edit.Delete(factoryKeys);

  endRemoveRows();

  return true;
}

Qt::ItemFlags FactorysModel::flags(const QModelIndex& index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  Qt::ItemFlags RetValue = Qt::ItemIsSelectable | Qt::ItemIsEnabled;

  switch (index.column()) {
  case 1:
  case 2:
  case 3:
  case 4:
  case 5:
  case 6:
    RetValue |= Qt::ItemIsEditable;
    break;
  default:
    break;
  }
  return RetValue;
}

void FactorysModel::Commit()
{
  _FTC.CloneFrom(_FTC_Edit);
  _FTC.GetFactoryCollection().CloneFrom(_FC_Edit);
  Select();
}

void FactorysModel::Select()
{
  _FTC_Edit.CloneFrom(_FTC);
  _FC_Edit.CloneFrom(_FTC.GetFactoryCollection());
}

ResourceCalculator::FactoryTypeCollection& FactorysModel::GetTypesData()
{
  return _FTC_Edit;
}

const ResourceCalculator::FactoryTypeCollection& FactorysModel::GetTypesData() const
{
  return _FTC_Edit;
}

bool FactorysModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
  if (index.isValid() && role == Qt::EditRole) {
    ResourceCalculator::Factory& factory = _FC_Edit[_FC_Edit(index.row())];
    switch (index.column()) {
    case 0:
    {
      QString str = value.toString();
      std::string d = str.toStdString();
      if (str.size() > 0) {
        factory.SetIconKey(str.toStdString());
      }
      break;
    }
    case 1:
    {
      QString str = value.toString();
      if (str.size() > 0) {
        factory.SetName(str.toStdString());
      }
      break;
    }
    case 2:
      factory.SetTypes(value.value<QT_KeysFactoryType>());
    break;
    case 3:
    {
      double d = value.toDouble();
      if (d >= 0.0) {
        factory.SetSpeed(d);
      }
    }
    break;
    case 4:
    {
      double d = value.toDouble();
      if (d >= 0.0) {
        factory.SetPower(d);
      }
    }
    break;
    case 5:
    {
      int d = value.toInt();
      if (d >= 0.0) {
        factory.SetCountSlotsForRecipes(d);
      }
    }
    break;
    case 6:
    {
      int d = value.toInt();
      if (d >= 0.0) {
        factory.SetCountSlotsForModules(d);
      }
    }
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
