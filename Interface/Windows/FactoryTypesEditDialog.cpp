
#include "FactoryTypesEditDialog.h"
#include "IconSelectedDialog.h"

#pragma region MODEL

FactoryTypesEditModel::FactoryTypesEditModel(ResourceCalculator::ParamsCollection &PC, QObject *parent ):
  QAbstractTableModel( parent ), _PC( PC )
{
  Select();
}

int FactoryTypesEditModel::rowCount( const QModelIndex &parent ) const
{
  Q_UNUSED( parent );
  return _listOfItemsId.size();
}

int FactoryTypesEditModel::columnCount( const QModelIndex &parent ) const
{
  Q_UNUSED( parent );
  return 2;
}

QVariant FactoryTypesEditModel::data( const QModelIndex &index, int role ) const
{
  using namespace ResourceCalculator;

  if ( !index.isValid() )
    return QVariant();

  if ( index.row() >= _listOfItemsId.size() || index.row() < 0 )
    return QVariant();

  if ( role == Qt::DisplayRole ) {
    switch ( index.column() ) {
    case 0:
      return QVariant( tr( "There should be an icon" ) );
      break;
    case 1:
      return QString::fromStdString( _listOfItemsId[index.row()].second.Name );
      break;
    default:
      return QVariant();
      break;
    }
  }
  return QVariant();
}

QVariant FactoryTypesEditModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
  if ( role != Qt::DisplayRole )
    return QVariant();

  if ( orientation == Qt::Horizontal ) {
    switch ( section ) {
    case 0:
      return tr( "Icon" );
    case 1:
      return tr( "Factory type name" );
    default:
      return QVariant();
    }
  }
  return QVariant();
}

bool FactoryTypesEditModel::insertRows( int position, int rows, const QModelIndex &index )
{
  Q_UNUSED( index );
  beginInsertRows( QModelIndex(), position, position + rows - 1 );
  for ( int row = 0; row < rows; ++row ) {
    using namespace ResourceCalculator;
    KEY_TYPE_FACTORY NewKey = _PC.FC.GetUniqueFactoryTypeKey();
    QString Name( tr( "New type factory" ) + QString( ' ' ) + QString::number( static_cast<KEY_TO_Json>( NewKey ) ) );
    std::pair<KEY_TYPE_FACTORY, FactoryType > ToADD;
    ToADD.first = NewKey;
    ToADD.second.Name = Name.toStdString();
    _FactoryTypesKeyToAdd[NewKey] = ToADD.second;
    _listOfItemsId.insert( position, ToADD );
  }
  endInsertRows();
  return true;
}

bool FactoryTypesEditModel::removeRows( int position, int rows, const QModelIndex &index )
{
  Q_UNUSED( index );
  beginRemoveRows( QModelIndex(), position, position + rows - 1 );
  for ( int row = 0; row < rows; ++row ) {
    _FactoryTypesKeyToDelete.insert( _listOfItemsId.at( position ).first );
    _listOfItemsId.removeAt( position );
  }
  endRemoveRows();
  return true;
}

Qt::ItemFlags FactoryTypesEditModel::flags( const QModelIndex &index ) const
{
  if ( !index.isValid() )
    return Qt::ItemIsEnabled;

  return Qt::ItemIsSelectable | Qt::ItemIsEnabled | 
    ( index.column() == 1 ? Qt::ItemIsEditable : Qt::NoItemFlags );
}

void FactoryTypesEditModel::Commit()
{
  _PC.FC.DeleteFactorysTypes( _FactoryTypesKeyToDelete );
  _PC.FC.AddFactorysTypes( _FactoryTypesKeyToAdd );
  Select();
}

void FactoryTypesEditModel::Select()
{
  using namespace ResourceCalculator;
  auto & TypesFactorys = _PC.FC.GetTypesFactorys();
  _listOfItemsId.reserve( static_cast< int >( TypesFactorys.size() ) );
  for ( auto & TypeFactory : TypesFactorys ) {
    _listOfItemsId.push_back( TypeFactory );
  }
}

ResourceCalculator::FactoryType FactoryTypesEditModel::GetDataRow( int Row ) const
{
  return _listOfItemsId[Row].second;
}

ResourceCalculator::KEY_TYPE_FACTORY FactoryTypesEditModel::GetDataRowType( int Row ) const
{
  return _listOfItemsId[Row].first;
}

bool FactoryTypesEditModel::setData( const QModelIndex & index, const QVariant & value, int role )
{
  if ( index.isValid() && role == Qt::EditRole ) {
    switch ( index.column() ) {
    case 0:
      _listOfItemsId[index.row()].second.IconPath = value.toString().toStdString();
      break;
    case 1:
      _listOfItemsId[index.row()].second.Name = value.toString().toStdString();
      break;
    default:
      return false;
      break;
    }
    _FactoryTypesKeyToAdd[_listOfItemsId[index.row()].first] = _listOfItemsId[index.row()].second;
    emit( dataChanged( index, index ) );
    return true;
  }
  return false;
}

#pragma endregion MODEL

#pragma region DELEGATE

FactoryTypesEditDelegate::FactoryTypesEditDelegate( ResourceCalculator::ParamsCollection &PC, const FactoryTypesEditModel &Model, QObject *parent )
  : QStyledItemDelegate( parent ), _PC( PC ), _Model( Model )
{
}

void FactoryTypesEditDelegate::paint( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
  emit( editorEventDelegate( index ) );
  switch ( index.column() ) {
  case 0: {
    std::string IconPath = _Model.GetDataRow(index.row()).IconPath;
    const ResourceCalculator::Icon &icon = _PC.Icons.GetIcon( IconPath );
    if ( icon.GetRawData().size() > 0 ) {
      QPixmap pixmap;
      bool dd = pixmap.loadFromData( ( uchar* ) &icon.GetRawData()[0], ( uint ) icon.GetRawData().size() );
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

bool FactoryTypesEditDelegate::editorEvent( QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index )
{
  if ( event->type() == QEvent::MouseButtonPress ) {
    switch ( index.column() ) {
    case 0:
    {
      IconSelectedDialog _IconSelectedDialog( _PC );
      if ( _IconSelectedDialog.exec() ) {
        const ResourceCalculator::Icon * Icon = _IconSelectedDialog.GetResult();
        if ( Icon != nullptr ) {
          model->setData( index, QString::fromStdString( Icon->GetIconPath() ), Qt::EditRole );
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

FactoryTypesEditDialog::FactoryTypesEditDialog(ResourceCalculator::ParamsCollection &PC, QWidget *parent )
  : QDialog( parent ), _PC( PC )
{
  setMinimumSize( 400, 600 );

  QPushButton *okButton = new QPushButton( tr( "OK" ) );
  QPushButton *addButton = new QPushButton( tr( "Add" ) );
  _remoteButton = new QPushButton( tr( "Remote" ) );
  QPushButton *cancelButton = new QPushButton( tr( "Cancel" ) );
  _Model = new FactoryTypesEditModel( _PC, this );

  FactoryTypesEditDelegate *Delegate = new FactoryTypesEditDelegate( PC, *_Model, nullptr );

  _tableView = new QTableView;
  _tableView->setSelectionMode( QTableView::SelectionMode::SingleSelection );
  _tableView->setSelectionBehavior( QTableView::SelectionBehavior::SelectRows );
  _tableView->setModel( _Model );
  _tableView->setItemDelegate( Delegate );
  _tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  _tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
  _tableView->setColumnWidth(0, 60);

  QHBoxLayout *buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget( okButton );
  buttonLayout->addWidget( addButton );
  buttonLayout->addWidget( _remoteButton );
  buttonLayout->addWidget( cancelButton );

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget( _tableView );
  mainLayout->addLayout( buttonLayout );
  setLayout( mainLayout );

  connect( okButton, &QAbstractButton::clicked, this, &QDialog::accept );
  connect( cancelButton, &QAbstractButton::clicked, this, &QDialog::reject );
  connect( addButton, &QAbstractButton::clicked, this, &FactoryTypesEditDialog::add_item );
  connect( _remoteButton, &QAbstractButton::clicked, this, &FactoryTypesEditDialog::remove_item );
  connect( Delegate, &FactoryTypesEditDelegate::editorEventDelegate, this, &FactoryTypesEditDialog::editorEventDelegate );

  setWindowTitle( tr( "Editor types factory items" ) );
}

void FactoryTypesEditDialog::Commit()
{
  _Model->Commit();
}

void FactoryTypesEditDialog::remove_item()
{
  QModelIndexList RowsSelected = _tableView->selectionModel()->selectedRows();
  if ( RowsSelected.size() > 0 ) {
    _Model->removeRow( RowsSelected[0].row() );
  }
}

void FactoryTypesEditDialog::editorEventDelegate( const QModelIndex & index )
{
  QModelIndexList Rows = _tableView->selectionModel()->selectedRows();
  _remoteButton->setEnabled( Rows.size() > 0 );
}

void FactoryTypesEditDialog::add_item()
{
  QModelIndexList Rows = _tableView->selectionModel()->selectedRows();
  if ( Rows.size() > 0 ) {
    _Model->insertRow( Rows[0].row() );
  } else {
    _Model->insertRow( 0 );
  }
}
