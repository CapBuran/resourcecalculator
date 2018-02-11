#include "IconSelectedDialog.h"
#include "ModulesEditDialog.h"

#pragma region MODEL

ModulesTypesEditModel::ModulesTypesEditModel( ResourceCalculator::ParamsCollection &PC, QObject *parent ):
  QAbstractTableModel( parent ), _PC( PC )
{
  Select();
}

int ModulesTypesEditModel::rowCount( const QModelIndex & parent ) const
{
  Q_UNUSED( parent );
  return _listOfItemsId.size();
}

int ModulesTypesEditModel::columnCount( const QModelIndex & parent ) const
{
  return 6;
}

QVariant ModulesTypesEditModel::data( const QModelIndex & index, int role ) const
{
  using namespace ResourceCalculator;

  if ( !index.isValid() )
    return QVariant();

  if ( index.row() >= _listOfItemsId.size() || index.row() < 0 )
    return QVariant();

  if ( role == Qt::DisplayRole ) {
    switch ( index.column() ) {
    case 0:
      //return QVariant( tr( "There should be an icon" ) );
      return QString::fromStdString( _listOfItemsId[index.row()].second.GetIconPath() );
      break;
    case 1:
      return QString::fromStdString( _listOfItemsId[index.row()].second.GetName() );
      break;
    case 2:
      return QVariant( _listOfItemsId[index.row()].second.GetCoefficientPollution() );
      break;
    case 3:
      return QVariant( _listOfItemsId[index.row()].second.GetCoefficientEffectiveness() );
      break;
    case 4:
      return QVariant( _listOfItemsId[index.row()].second.GetCoefficientSpeed() );
      break;
    case 5:
      return QVariant( _listOfItemsId[index.row()].second.GetCoefficientProductivity() );
      break;
    default:
      return QVariant();
      break;
    }
  }
  return QVariant();
}

QVariant ModulesTypesEditModel::headerData( int section, Qt::Orientation orientation, int role ) const
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

Qt::ItemFlags ModulesTypesEditModel::flags( const QModelIndex & index ) const
{
  if ( !index.isValid() )
    return Qt::ItemIsEnabled;

  return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

bool ModulesTypesEditModel::insertRows( int position, int rows, const QModelIndex & index )
{
  Q_UNUSED( index );
  beginInsertRows( QModelIndex(), position, position + rows - 1 );
  for ( int row = 0; row < rows; ++row ) {
    using namespace ResourceCalculator;
    KEY_MODULE NewKey = _PC.MC.GetUniqueModuleKey();
    QString Name( tr( "New module" ) + QString( ' ' ) + QString::number( static_cast<KEY_TO_Json>( NewKey ) ) );
    std::pair<KEY_MODULE, Module > ToADD;
    ToADD.first = NewKey;
    ToADD.second.SetKey( NewKey );
    ToADD.second.SetName( Name.toStdString() );
    _ModulesToAdd.insert( NewKey );
    _listOfItemsId.insert( position, ToADD );
  }
  endInsertRows();
  return true;
}

bool ModulesTypesEditModel::removeRows( int position, int rows, const QModelIndex & index )
{
  Q_UNUSED( index );
  beginRemoveRows( QModelIndex(), position, position + rows - 1 );
  for ( int row = 0; row < rows; ++row ) {
    _ModulesToDelete.insert( _listOfItemsId.at( position ).first );
    _listOfItemsId.removeAt( position );
  }
  endRemoveRows();
  return true;  
}

ResourceCalculator::KEY_MODULE ModulesTypesEditModel::GetDataRowModuleType( int Row ) const
{
  return _listOfItemsId[Row].second.GetKey();
}

bool ModulesTypesEditModel::setData( const QModelIndex & index, const QVariant & value, int role )
{
  if ( index.isValid() && role == Qt::EditRole ) {
    using namespace ResourceCalculator;
    const KEY_MODULE KeyModule = _listOfItemsId[index.row()].first;
    _ModulesToAdd.insert( KeyModule );
    bool IsOk = false;
    switch ( index.column() ) {
    case 0:
      _listOfItemsId[index.row()].second.SetIconPath( value.toString().toStdString() );
      break;
    case 1:
      if(value.toString().length() > 0)
        _listOfItemsId[index.row()].second.SetName( value.toString().toStdString() );
      break;
    case 2:
      if ( value.toDouble( &IsOk ) > 0.0 )
        if ( IsOk )
          _listOfItemsId[index.row()].second.SetCoefficientPollution( value.toDouble() );
      break;
    case 3:
      if ( value.toDouble( &IsOk ) > 0.0 )
        if ( IsOk )
          _listOfItemsId[index.row()].second.SetCoefficientEffectiveness( value.toDouble() );
      break;
    case 4:
      if ( value.toDouble( &IsOk ) > 0.0 )
        if ( IsOk )
          _listOfItemsId[index.row()].second.SetCoefficientSpeed( value.toDouble() );
      break;
    case 5:
      if ( value.toDouble( &IsOk ) > 0.0 )
        if ( IsOk )
          _listOfItemsId[index.row()].second.SetCoefficientProductivity( value.toDouble() );
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

void ModulesTypesEditModel::Commit()
{
  _PC.MC.Delete( _ModulesToDelete );
  std::map<ResourceCalculator::KEY_MODULE, ResourceCalculator::Module > ModulesToAdd;
  for ( auto it : _ModulesToAdd ) {
    for ( auto itm : _listOfItemsId ) {
      if ( itm.first == it ) {
        ModulesToAdd[it] = itm.second;
      }
    }
  }
  _PC.MC.Add( ModulesToAdd );
  _ModulesToDelete.clear();
  _ModulesToAdd.clear();
  _listOfItemsId.clear();
  Select();
}

void ModulesTypesEditModel::Select()
{
  using namespace ResourceCalculator;
  _ModulesToDelete.clear();
  _ModulesToAdd.clear();
  const std::map<KEY_MODULE, Module> &TypesModules = _PC.MC.GetModules();
  _listOfItemsId.reserve( static_cast< int >( TypesModules.size() ) );
  for ( auto & TypeFactory : TypesModules ) {
    _listOfItemsId.push_back( TypeFactory );
  }
}

void ModulesTypesEditModel::SetKeyPathForItem( int Row, const std::string & KeyPath )
{
  _ModulesToAdd.insert( _listOfItemsId[Row].first );
  _listOfItemsId[Row].second.SetIconPath( KeyPath );
}

#pragma endregion MODEL

#pragma region DELEGATE

ModulesEditDelegate::ModulesEditDelegate( const ResourceCalculator::ParamsCollection &PC, QObject *parent )
  : QStyledItemDelegate( parent ), _PC( PC )
{
}

void ModulesEditDelegate::paint( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
  emit( editorEventDelegate( index ) );
  switch ( index.column() ) {
  case 0: {
    std::string IconPath = index.data().toString().toStdString();
    const ResourceCalculator::Icon &icon = _PC.Icons.GetIcon( IconPath );
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
    QStyledItemDelegate::paint( painter, option, index );
    return;
    break;
  }
}

bool ModulesEditDelegate::editorEvent( QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index )
{
  if ( event->type() == QEvent::MouseButtonPress ) {
    switch ( index.column() ) {
    case 0:
    {
      IconSelectedDialog _IconSelectedDialog( _PC );
      if ( _IconSelectedDialog.exec() ) {
        const ResourceCalculator::Icon * Icon = _IconSelectedDialog.GetResult();
        if ( Icon != nullptr ) {
          model->setData( index, QString::fromStdString( Icon->GetIconPath() ) );
        }
      }
      return false;
      break;
    }
    default:
      return QStyledItemDelegate::editorEvent( event, model, option, index );
      break;
    }
  }
  return QStyledItemDelegate::editorEvent( event, model, option, index );
}

#pragma endregion DELEGATE

ModulesEditDialog::ModulesEditDialog( ResourceCalculator::ParamsCollection &PC, QWidget *parent ):
  _PC(PC), _Model( PC, parent )
{

  setMinimumSize( 400, 600 );

  QPushButton *okButton = new QPushButton( tr( "OK" ) );
  QPushButton *AddButton = new QPushButton( tr( "Add" ) );
  QPushButton *RemoveButton = new QPushButton( tr( "Remove" ) );
  QPushButton *cancelButton = new QPushButton( tr( "Cancel" ) );

  _tableView = new QTableView;
  _tableView->setSelectionMode(QTableView::SelectionMode::SingleSelection);
  _tableView->setSelectionBehavior( QTableView::SelectionBehavior::SelectRows );
  _tableView->setModel( &_Model );
  _tableView->setItemDelegate( new ModulesEditDelegate( PC ) );

  QHBoxLayout *buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget( okButton );
  buttonLayout->addWidget( AddButton );
  buttonLayout->addWidget( RemoveButton );
  buttonLayout->addWidget( cancelButton );

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget( _tableView );
  mainLayout->addLayout( buttonLayout );
  setLayout( mainLayout );

  connect( okButton, SIGNAL( clicked() ), SLOT( PushButtonOk() ) );
  connect( AddButton, SIGNAL( clicked() ), SLOT( PushButtonAdd() ) );
  connect( RemoveButton, SIGNAL( clicked() ), SLOT( PushButtonRemove() ) );
  connect( cancelButton, &QAbstractButton::clicked, this, &QDialog::reject );

  setWindowTitle( tr( "Edit modules" ) );
}

void ModulesEditDialog::PushButtonAdd()
{
  QModelIndexList Rows = _tableView->selectionModel()->selectedRows();
  if ( Rows.size() > 0 ) {
    _Model.insertRow( Rows[0].row() );
  } else {
    _Model.insertRow( 0 );
  }
}

void ModulesEditDialog::PushButtonOk()
{
  _Model.Commit();
  emit( &QDialog::accept );
  emit( close() );
}

void ModulesEditDialog::PushButtonRemove()
{
  QModelIndexList RowsSelected = _tableView->selectionModel()->selectedRows();
  if ( RowsSelected.size() > 0 ) {
    _Model.removeRow( RowsSelected[0].row() );
  }
}
