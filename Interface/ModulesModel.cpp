#include <ModulesModel.h>

ModulesModelRead::ModulesModelRead(const ResourceCalculator::ModuleCollection& MC, QObject* parent)
  : QAbstractTableModel(parent)
{
  _MC_EDIT = MC;
}

ModulesModel::ModulesModel(ResourceCalculator::ModuleCollection& MC, QObject* parent)
  : ModulesModelRead(MC, parent)
  , _MC(MC)
{
}

int ModulesModelRead::rowCount( const QModelIndex & parent ) const
{
  Q_UNUSED( parent );
  return _MC_EDIT.Size();
}

int ModulesModelRead::columnCount( const QModelIndex & parent ) const
{
  return 6;
}

QVariant ModulesModelRead::data( const QModelIndex & index, int role ) const
{
  using namespace ResourceCalculator;

  if ( !index.isValid() )
    return QVariant();

  if ( index.row() >= _MC_EDIT.Size() || index.row() < 0 )
    return QVariant();

  if ( role == Qt::DisplayRole ) {
    const Module& module = _MC_EDIT[_MC_EDIT(index.row())];
    switch ( index.column() ) {
    case 0:
      return QString::fromStdString(module.GetIconKey() );
      break;
    case 1:
      return QString::fromStdString(module.GetName() );
      break;
    case 2:
      return QVariant(module.GetCoefficientPollution() );
      break;
    case 3:
      return QVariant(module.GetCoefficientConsumption() );
      break;
    case 4:
      return QVariant(module.GetCoefficientSpeed() );
      break;
    case 5:
      return QVariant(module.GetCoefficientProductivity() );
      break;
    default:
      return QVariant();
      break;
    }
  }
  return QVariant();
}

QVariant ModulesModelRead::headerData( int section, Qt::Orientation orientation, int role ) const
{
  if ( role != Qt::DisplayRole )
    return QVariant();

  if ( orientation == Qt::Horizontal ) {
    switch ( section ) {
    case 0:
      return tr( "Icon" );
    case 1:
      return tr( "Module name" );
    case 2:
      return tr( "Coefficient pollution" );
    case 3:
      return tr( "Coefficient effectiveness" );
    case 4:
      return tr( "Coefficient speed" );
    case 5:
      return tr( "Coefficient productivity" );
    default:
      return QVariant();
    }
  }
  return QVariant();
}

Qt::ItemFlags ModulesModelRead::flags(const QModelIndex& index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

Qt::ItemFlags ModulesModel::flags(const QModelIndex& index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  if (index.row() == 0)
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;

  return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

bool ModulesModel::insertRows( int position, int rows, const QModelIndex & index )
{
  Q_UNUSED( index );
  beginInsertRows( QModelIndex(), position, position + rows - 1 );
  for ( int row = 0; row < rows; ++row ) {
    using namespace ResourceCalculator;
    KEY_MODULE NewKey = _MC.NewKey();
    QString Name( tr( "New module" ) + QString( ' ' ) + QString::number( static_cast<KEY_TO_Json>( NewKey ) ) );
    std::pair<KEY_MODULE, Module > ToADD;
    ToADD.first = NewKey;
    ToADD.second.SetKey( NewKey );
    ToADD.second.SetName( Name.toStdString() );
    _MC_EDIT.Add({ToADD});
  }
  endInsertRows();
  return true;
}

bool ModulesModel::removeRows( int position, int rows, const QModelIndex & index )
{
  Q_UNUSED(index);
  beginResetModel();
  using namespace ResourceCalculator;
  std::set<KEY_MODULE> ToDelete;
  for (int row = 0; row < rows; ++row) {
    ToDelete.insert(_MC_EDIT(position + row));
  }
  bool retval = _MC_EDIT.Delete(ToDelete);
  endResetModel();
  return retval;

  Q_UNUSED( index );
  beginRemoveRows( QModelIndex(), position, position + rows - 1 );
  for (int row = 0; row < rows; ++row)
  {
    _MC_EDIT.Delete({ _MC_EDIT(position + row) });
  }
  endRemoveRows();
  return true;  
}

bool ModulesModel::setData( const QModelIndex & index, const QVariant & value, int role )
{
  if ( index.isValid() && role == Qt::EditRole ) {
    using namespace ResourceCalculator;
    Module& module = _MC_EDIT[_MC_EDIT(index.row())];

    bool IsOk = false;
    double val = 0.0;
    switch ( index.column() ) {
    case 0:
      module.SetIconKey( value.toString().toStdString() );
      break;
    case 1:
      if(value.toString().length() > 0)
        module.SetName( value.toString().toStdString() );
      break;
    case 2:
      val = value.toDouble(&IsOk);
      if (IsOk) {
        module.SetCoefficientPollution(val);
      }
      break;
    case 3:
      val = value.toDouble(&IsOk);
      if (IsOk) {
        module.SetCoefficientConsumption(val);
      }
      break;
    case 4:
      val = value.toDouble(&IsOk);
      if (IsOk) {
        module.SetCoefficientSpeed(val);
      }
      break;
    case 5:
      val = value.toDouble(&IsOk);
      if (IsOk) {
        module.SetCoefficientProductivity(val);
      }
      break;
    default:
      return false;
      break;
    }
    emit( dataChanged( index, index ) );
    return true;
  }
  return false;
}

void ModulesModel::Commit()
{
  _MC = _MC_EDIT;
}
