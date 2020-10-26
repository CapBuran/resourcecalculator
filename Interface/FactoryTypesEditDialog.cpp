
#include "FactoryTypesEditDialog.h"
#include "IconSelectedDialog.h"

#pragma region MODEL

FactoryTypesEditModel::FactoryTypesEditModel(ResourceCalculator::FactoryCollection& FC, QObject *parent )
  : QAbstractTableModel( parent )
  , _FC(FC)
{
  Select();
}

int FactoryTypesEditModel::rowCount( const QModelIndex &parent ) const
{
  Q_UNUSED( parent );
  return _FC_Edit.Size();
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

  if ( index.row() >= _FC_Edit.Size() || index.row() < 0 )
    return QVariant();

  if ( role == Qt::DisplayRole ) {
    switch ( index.column() ) {
    case 0:
      return QString::fromStdString(_FC_Edit.GetTypes().GetFactoryType(_FC_Edit.GetTypes()(index.row())).IconPath );
      break;
    case 1:
      return QString::fromStdString(_FC_Edit.GetTypes().GetFactoryType(_FC_Edit.GetTypes()(index.row())).Name );
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
  for (int row = 0; row < rows; ++row) {
    using namespace ResourceCalculator;
    KEY_TYPE_FACTORY NewKey = _FC_Edit.GetTypes().NewKey();
    QString Name(tr("New type factory") + QString(' ') + QString::number(static_cast<KEY_TO_Json>(NewKey)));
    std::pair<KEY_TYPE_FACTORY, FactoryType > ToADD;
    ToADD.first = NewKey;
    ToADD.second.Name = Name.toStdString();
    _FC_Edit.GetTypes().AddFactorysTypes({ToADD});
  }
  endInsertRows();
  return true;
}

bool FactoryTypesEditModel::removeRows( int position, int rows, const QModelIndex &index )
{
  Q_UNUSED( index );
  beginRemoveRows( QModelIndex(), position, position + rows - 1 );
  using namespace ResourceCalculator;
  std::set<KEY_TYPE_FACTORY> FactoryTypesKey;
  for ( int row = 0; row < rows; ++row ) {
    FactoryTypesKey.insert(_FC_Edit.GetTypes()(row));
  }
  _FC_Edit.GetTypes().DeleteFactorysTypes(FactoryTypesKey);
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
  _FC = _FC_Edit;
  Select();
}

void FactoryTypesEditModel::Select()
{
  _FC_Edit = _FC;
}

bool FactoryTypesEditModel::setData( const QModelIndex & index, const QVariant & value, int role )
{
  if ( index.isValid() && role == Qt::EditRole ) {
    ResourceCalculator::FactoryType& ft = _FC_Edit.GetTypes().GetFactoryType(_FC_Edit.GetTypes()(index.row()));
    switch ( index.column() ) {
    case 0:
      ft.IconPath = value.toString().toStdString();
      break;
    case 1:
      ft.Name = value.toString().toStdString();
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

#pragma endregion MODEL

#pragma region DELEGATE

FactoryTypesEditDelegate::FactoryTypesEditDelegate( ResourceCalculator::FactoryCollection& FC, const ResourceCalculator::IconCollection& IC, const FactoryTypesEditModel &Model, QObject *parent )
  : QStyledItemDelegate( parent )
  , _Model( Model )
  , _IC(IC)
{
}

void FactoryTypesEditDelegate::paint( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
  emit( editorEventDelegate( index ) );
  switch ( index.column() ) {
  case 0:
  {
    QStyledItemDelegate::paint(painter, option, index);
    const ResourceCalculator::Icon &icon = _IC.GetIcon(_Model.data(index, Qt::DisplayRole).toString().toStdString());
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
      IconSelectedDialog _IconSelectedDialog(_IC);
      if ( _IconSelectedDialog.exec() ) {
        const ResourceCalculator::Icon * Icon = _IconSelectedDialog.GetResult();
        if ( Icon != nullptr ) {
          model->setData( index, QString::fromStdString( Icon->GetKeyPath() ), Qt::EditRole );
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

FactoryTypesEditDialog::FactoryTypesEditDialog(ResourceCalculator::FactoryCollection &FC, const ResourceCalculator::IconCollection& IC, QWidget *parent )
  : QDialog( parent )
  , _Model(FC, parent)
  , _Delegate(FC, IC, _Model, parent)
{
  setMinimumSize( 400, 600 );

  QPushButton *okButton = new QPushButton( tr( "OK" ) );
  QPushButton *addButton = new QPushButton( tr( "Add" ) );
  _remoteButton = new QPushButton( tr( "Remote" ) );
  QPushButton *cancelButton = new QPushButton( tr( "Cancel" ) );

  _tableView = new QTableView;
  _tableView->setSelectionMode( QTableView::SelectionMode::SingleSelection );
  _tableView->setSelectionBehavior( QTableView::SelectionBehavior::SelectRows );
  _tableView->setModel( &_Model );
  _tableView->setItemDelegate( &_Delegate );
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
  connect( &_Delegate, &FactoryTypesEditDelegate::editorEventDelegate, this, &FactoryTypesEditDialog::editorEventDelegate );

  setWindowTitle( tr( "Editor types factory items" ) );
}

void FactoryTypesEditDialog::Commit()
{
  _Model.Commit();
}

void FactoryTypesEditDialog::remove_item()
{
  QModelIndexList RowsSelected = _tableView->selectionModel()->selectedRows();
  if ( RowsSelected.size() > 0 ) {
    _Model.removeRow( RowsSelected[0].row() );
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
    _Model.insertRow( Rows[0].row() );
  } else {
    _Model.insertRow( 0 );
  }
}
