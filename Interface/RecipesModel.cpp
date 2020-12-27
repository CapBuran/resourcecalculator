#include <RecipesModel.h>

RecipesModel::RecipesModel(
  ResourceCalculator::RecipeCollection& RC,
  const ResourceCalculator::ItemCollection& IC,
  const ResourceCalculator::FactoryTypeCollection& FTC,
  QObject *parent)
  : QAbstractTableModel(parent)
  , _RC(RC)
  , _IC(IC)
  , _FTC(FTC)
{
  Select();
}

int RecipesModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return _RC_EDIT.Size();
}

int RecipesModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return 6;
}

QVariant RecipesModel::data(const QModelIndex &index, int role) const
{
  using namespace ResourceCalculator;
  if (!index.isValid())
    return QVariant();
  if (index.row() >= _RC_EDIT.Size() || index.row() < 0)
    return QVariant();
  if (role == Qt::DisplayRole) {
    const Recipe& R = _RC_EDIT[_RC_EDIT(index.row())];
    switch (index.column()) {
    case 0://Icon
      return QString::fromStdString(R.GetIconKey());
      break;
    case 1://Recipe name
      return QString::fromStdString(R.GetName());
      break;
    case 2://Recipe time
      return QString::number(R.GetTime());
      break;
    case 3://Result of recipe
    {
      return QVariant::fromValue<QT_CountsItem>(R.GetResult());
      break;
    }
    case 4://Ingredients of the recipe
    {
      return QVariant::fromValue<QT_CountsItem>(R.GetRequired());
      break;
    }
    case 5://Allowed factories
    {
      return QVariant(static_cast<int>(R.GetTypeFactory()));
      break;
    }
    default:
      return QVariant();
      break;
    }
  }
  return QVariant();
}

QVariant RecipesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole)
    return QVariant();
  if (orientation == Qt::Horizontal) {
    switch (section) {
    case 0:
      return tr("Icon");
    case 1:
      return tr("Recipe name");
    case 2:
      return tr("Recipe time");
    case 3:
      return tr("Result of recipe");
    case 4:
      return tr("Ingredients of the recipe");
    case 5:
      return tr("Allowed factories");
    default:
      return QVariant();
    }
  }
  return QVariant();
}

Qt::ItemFlags RecipesModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;
  return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool RecipesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if (index.isValid() && role == Qt::EditRole) {
    using namespace ResourceCalculator;
    Recipe& R = _RC_EDIT[_RC(index.row())];

    switch (index.column()) {
    case 0://Icon
    {
      R.SetIconKey(value.toString().toStdString());
      break;
    }
    case 1://Recipe name
    {
      QString Name = value.toString();
      if (Name.length() > 0) {
        R.SetName(Name.toStdString());
      }
      break;
    }
    case 2://Recipe time
    {
      double Value = value.toDouble();
      if (Value >= 0) {
        R.SetTime(Value);
      }
      break;
    }
    case 3://Result of recipe
      R.SetResult(value.value<QT_CountsItem>());
      break;
    case 4://Ingredients of the recipe
      R.SetRequired(value.value<QT_CountsItem>());
      break;
    case 5://Allowed factory
    {
      int Value = value.toInt();
      if (Value >= 0) {
        R.SetTypeFactory(static_cast< KEY_TYPE_FACTORY >(Value));
      }
      break;
    }
    default:
      return false;
      break;
    }
    emit(dataChanged(index, index));
    return true;
  }
  return false;
}

bool RecipesModel::insertRows(int position, int rows, const QModelIndex &index)
{
  Q_UNUSED(index);
  beginInsertRows(QModelIndex(), position, position + rows - 1);
  for (int row = 0; row < rows; ++row) {
    using namespace ResourceCalculator;
    const KEY_RECIPE NewKey = _RC_EDIT.NewKey();
    QString Name(tr("New recipe") + ' ' + QString::number(static_cast<KEY_TO_Json>(NewKey)));
    Recipe ToADD;
    ToADD.SetKey(NewKey);
    ToADD.SetName(Name.toStdString());
    ToADD.SetTypeFactory(KEY_TYPE_FACTORY::Unknown);
    _RC_EDIT.Add(ToADD);
  }
  endInsertRows();
  return true;
}

bool RecipesModel::removeRows(int position, int rows, const QModelIndex &index)
{
  beginResetModel();
  using namespace ResourceCalculator;
  std::set<KEY_RECIPE> ToDelete;
  for (int row = 0; row < rows; ++row) {
    ToDelete.insert(_RC_EDIT(position + row));
  }
  bool retval = _RC_EDIT.Delete(ToDelete);
  endResetModel();
  return retval;
}

void RecipesModel::Commit()
{
  _RC.CloneFrom(_RC_EDIT);
}

void RecipesModel::Select()
{
  _RC_EDIT.CloneFrom(_RC);
}

const ResourceCalculator::FactoryTypeCollection& RecipesModel::GetFactoryTypes() const
{
  return _FTC;
}

const ResourceCalculator::ItemCollection& RecipesModel::GetItems() const
{
  return _IC;
}
