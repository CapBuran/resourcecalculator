#include <FactorysTypesModel.h>

FactoryTypesModelRead::FactoryTypesModelRead(const ResourceCalculator::FactoryTypeCollection& FTC, QObject* parent)
  : QAbstractTableModel(parent)
  , _FTC_Edit(FTC)
{
  _FTC_Edit.CloneFrom(FTC);
}

FactoryTypesModel::FactoryTypesModel(ResourceCalculator::FactoryTypeCollection& FTC, QObject* parent)
  : FactoryTypesModelRead(FTC, parent)
  , _FTC(FTC)
{
}

int FactoryTypesModelRead::rowCount(const QModelIndex& parent) const
{
  Q_UNUSED(parent);
  return _FTC_Edit.Size();
}

int FactoryTypesModelRead::columnCount(const QModelIndex& parent) const
{
  Q_UNUSED(parent);
  return 2;
}

QVariant FactoryTypesModelRead::data(const QModelIndex& index, int role) const
{
  using namespace ResourceCalculator;

  if (!index.isValid())
    return QVariant();

  if (index.row() >= _FTC_Edit.Size() || index.row() < 0)
    return QVariant();

  if (role == Qt::DisplayRole) {
    switch (index.column()) {
    case 0:
      return QString::fromStdString(_FTC_Edit[_FTC_Edit(index.row())].GetIconKey());
      break;
    case 1:
      return QString::fromStdString(_FTC_Edit[_FTC_Edit(index.row())].GetName());
      break;
    default:
      return QVariant();
      break;
    }
  }
  return QVariant();
}

QVariant FactoryTypesModelRead::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal) {
    switch (section) {
    case 0:
      return tr("Icon");
    case 1:
      return tr("Factory type name");
    default:
      return QVariant();
    }
  }
  return QVariant();
}

bool FactoryTypesModel::insertRows(int position, int rows, const QModelIndex& index)
{
  Q_UNUSED(index);
  beginInsertRows(QModelIndex(), position, position + rows - 1);
  for (int row = 0; row < rows; ++row) {
    using namespace ResourceCalculator;
    KEY_TYPE_FACTORY NewKey = _FTC_Edit.NewKey();
    QString Name(tr("New type factory") + QString(' ') + QString::number(static_cast<KEY_TO_Json>(NewKey)));
    FactoryType ToADD;
    ToADD.SetKey(NewKey);
    ToADD.SetName(Name.toStdString());
    _FTC_Edit.Add(ToADD);
  }
  endInsertRows();
  return true;
}

bool FactoryTypesModel::removeRows(int position, int rows, const QModelIndex& index)
{
  Q_UNUSED(index);
  beginResetModel();
  using namespace ResourceCalculator;
  std::set<KEY_TYPE_FACTORY> FactoryTypesKey;
  for (int row = 0; row < rows; ++row) {
    FactoryTypesKey.insert(_FTC_Edit(position + row));
  }
  _FTC_Edit.Delete(FactoryTypesKey);
  endResetModel();
  return true;
}

Qt::ItemFlags FactoryTypesModelRead::flags(const QModelIndex& index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

Qt::ItemFlags FactoryTypesModel::flags(const QModelIndex& index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  return Qt::ItemIsSelectable | Qt::ItemIsEnabled |
    (index.column() == 1 ? Qt::ItemIsEditable : Qt::NoItemFlags);
}


void FactoryTypesModel::Commit()
{
  _FTC.CloneFrom(_FTC_Edit);
}

bool FactoryTypesModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
  if (index.isValid() && role == Qt::EditRole) {
    ResourceCalculator::FactoryType& ft = _FTC_Edit[_FTC_Edit(index.row())];
    switch (index.column()) {
    case 0:
      ft.SetIconKey(value.toString().toStdString());
      break;
    case 1:
      ft.SetName(value.toString().toStdString());
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
