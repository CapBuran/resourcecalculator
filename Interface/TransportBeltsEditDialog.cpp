#include "IconSelectedDialog.h"
#include "TransportBeltsEditDialog.h"

#pragma region MODEL

TransportBeltsEditModel::TransportBeltsEditModel( ResourceCalculator::ParamsCollection &PC, QObject *parent ):
  QAbstractTableModel( parent ), _PC( PC )
{
  Select();
}

int TransportBeltsEditModel::rowCount( const QModelIndex & parent ) const
{
  Q_UNUSED( parent );
  return _listOfItemsId.size();
}

int TransportBeltsEditModel::columnCount( const QModelIndex & parent ) const
{
  return 5;
}

QVariant TransportBeltsEditModel::data( const QModelIndex & index, int role ) const
{
  using namespace ResourceCalculator;

  if ( !index.isValid() )
    return QVariant();

  if ( index.row() >= _listOfItemsId.size() || index.row() < 0 )
    return QVariant();

  if ( role == Qt::DisplayRole ) {
    switch ( index.column() ) {
    case 0:
      return QString::fromStdString( _listOfItemsId[index.row()].second.GetIconPath() );
      break;
    case 1:
      return QString::fromStdString( _listOfItemsId[index.row()].second.GetName() );
      break;
    case 2:
      return QVariant( _listOfItemsId[index.row()].second.GetSpeed() );
      break;
    case 3:
      return QVariant( _listOfItemsId[index.row()].second.GetMaxDensity() );
      break;
    case 4:
      return QVariant( _listOfItemsId[index.row()].second.GetMaxThroughput() );
      break;
    default:
      return QVariant();
      break;
    }
  }
  return QVariant();
}

QVariant TransportBeltsEditModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
  if ( role != Qt::DisplayRole )
    return QVariant();

  if ( orientation == Qt::Horizontal ) {
    switch ( section ) {
    case 0:
      return tr( "Icon" );
    case 1:
      return tr( "Transport belt name" );
    case 2:
      return tr( "Speed" );
    case 3:
      return tr( "Max. density" );
    case 4:
      return tr( "Max throughput" );
    default:
      return QVariant();
    }
  }
  return QVariant();
}

Qt::ItemFlags TransportBeltsEditModel::flags( const QModelIndex & index ) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  if (index.column() >= 4)
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;

  return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

bool TransportBeltsEditModel::insertRows( int position, int rows, const QModelIndex & index )
{
  Q_UNUSED( index );
  beginInsertRows( QModelIndex(), position, position + rows - 1 );
  for ( int row = 0; row < rows; ++row ) {
    using namespace ResourceCalculator;
    KEY_TRANSPORT_BELT NewKey = _PC.TC.GetUniqueItemKey();
    QString Name( tr( "New transport belt" ) + QString( ' ' ) + QString::number( static_cast<KEY_TO_Json>( NewKey ) ) );
    std::pair<KEY_TRANSPORT_BELT, TransportBelt > ToADD;
    ToADD.first = NewKey;
    ToADD.second.SetKey( NewKey );
    ToADD.second.SetName( Name.toStdString() );
    _TransportBeltsToAdd.insert( NewKey );
    _listOfItemsId.insert( position, ToADD );
  }
  endInsertRows();
  return true;
}

bool TransportBeltsEditModel::removeRows( int position, int rows, const QModelIndex & index )
{
  Q_UNUSED( index );
  beginRemoveRows( QModelIndex(), position, position + rows - 1 );
  for ( int row = 0; row < rows; ++row ) {
    _TransportBeltsToDelete.insert( _listOfItemsId.at( position ).first );
    _listOfItemsId.removeAt( position );
  }
  endRemoveRows();
  return true;  
}

bool TransportBeltsEditModel::setData( const QModelIndex & index, const QVariant & value, int role )
{
  if ( index.isValid() && role == Qt::EditRole ) {
    using namespace ResourceCalculator;
    const KEY_TRANSPORT_BELT KeyTransportBelt = _listOfItemsId[index.row()].first;
    _TransportBeltsToAdd.insert( KeyTransportBelt );
    bool IsOk = false;
    double val = 0.0;
    switch ( index.column() ) {
    case 0:
      _listOfItemsId[index.row()].second.SetIconPath( value.toString().toStdString() );
      break;
    case 1:
      if(value.toString().length() > 0)
        _listOfItemsId[index.row()].second.SetName( value.toString().toStdString() );
      break;
    case 2:
      val = value.toDouble(&IsOk);
      if (IsOk) {
        _listOfItemsId[index.row()].second.SetSpeed(val);
      }
      break;
    case 3:
      val = value.toDouble(&IsOk);
      if (IsOk) {
        _listOfItemsId[index.row()].second.SetMaxDensity(val);
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

void TransportBeltsEditModel::Commit()
{
  _PC.TC.Delete(_TransportBeltsToDelete);
  std::map<ResourceCalculator::KEY_TRANSPORT_BELT, ResourceCalculator::TransportBelt > TransportBeltsToAdd;
  for ( auto it : _TransportBeltsToAdd ) {
    for ( auto itm : _listOfItemsId ) {
      if ( itm.first == it ) {
        TransportBeltsToAdd[it] = itm.second;
      }
    }
  }
  _PC.TC.Add( TransportBeltsToAdd );
  _TransportBeltsToDelete.clear();
  _TransportBeltsToAdd.clear();
  _listOfItemsId.clear();
  Select();
}

void TransportBeltsEditModel::Select()
{
  using namespace ResourceCalculator;
  _TransportBeltsToDelete.clear();
  _TransportBeltsToAdd.clear();
  const std::map<KEY_TRANSPORT_BELT, TransportBelt> &TransportBelts = _PC.TC.GetData();
  _listOfItemsId.reserve( static_cast< int >(TransportBelts.size() ) );
  for ( auto & TransportBelt : TransportBelts) {
    _listOfItemsId.push_back(TransportBelt);
  }
}

#pragma endregion MODEL

#pragma region DELEGATE

TransportBeltsEditDelegate::TransportBeltsEditDelegate( const ResourceCalculator::ParamsCollection &PC, QObject *parent )
  : QStyledItemDelegate( parent ), _PC( PC )
{
}

void TransportBeltsEditDelegate::paint( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
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

bool TransportBeltsEditDelegate::editorEvent( QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index )
{
  if ( event->type() == QEvent::MouseButtonPress ) {
    switch ( index.column() ) {
    case 0:
    {
      IconSelectedDialog _IconSelectedDialog( _PC );
      if ( _IconSelectedDialog.exec() ) {
        const ResourceCalculator::Icon * Icon = _IconSelectedDialog.GetResult();
        if ( Icon != nullptr ) {
          model->setData( index, QString::fromStdString( Icon->GetKeyPath() ) );
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

TransportBeltsEditDialog::TransportBeltsEditDialog( ResourceCalculator::ParamsCollection &PC, QWidget *parent ):
  _PC(PC), _Model( PC, parent )
{

  setMinimumSize( 1200, 600 );

  QPushButton *okButton = new QPushButton( tr( "OK" ) );
  QPushButton *AddButton = new QPushButton( tr( "Add" ) );
  QPushButton *RemoveButton = new QPushButton( tr( "Remove" ) );
  QPushButton *cancelButton = new QPushButton( tr( "Cancel" ) );

  _tableView = new QTableView;
  _tableView->setSelectionMode(QTableView::SelectionMode::SingleSelection);
  _tableView->setSelectionBehavior( QTableView::SelectionBehavior::SelectRows );
  _tableView->setModel( &_Model );
  _tableView->setItemDelegate( new TransportBeltsEditDelegate( PC ) );
  _tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  _tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
  _tableView->setColumnWidth(0, 60);

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

  setWindowTitle( tr( "Edit transport belt" ) );
}

void TransportBeltsEditDialog::PushButtonAdd()
{
  QModelIndexList Rows = _tableView->selectionModel()->selectedRows();
  if ( Rows.size() > 0 ) {
    _Model.insertRow( Rows[0].row() );
  } else {
    _Model.insertRow( 0 );
  }
}

void TransportBeltsEditDialog::PushButtonOk()
{
  _Model.Commit();
  emit( &QDialog::accept );
  emit( close() );
}

void TransportBeltsEditDialog::PushButtonRemove()
{
  QModelIndexList RowsSelected = _tableView->selectionModel()->selectedRows();
  if ( RowsSelected.size() > 0 ) {
    _Model.removeRow( RowsSelected[0].row() );
  }
}
