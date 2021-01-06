#include <FactoryTypesEditDialog.h>
#include <IconSelectedDialog.h>
#pragma region DELEGATE

FactoryTypesEditDelegate::FactoryTypesEditDelegate(const ResourceCalculator::IconCollection& icons, QObject* parent)
  : QStyledItemDelegate(parent)
  , _Icons(icons)
{
}

void FactoryTypesEditDelegate::paint( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex& index ) const
{
  emit( editorEventDelegate( index ) );
  switch ( index.column() ) {
  case 0:
  {
    QStyledItemDelegate::paint(painter, option, index);
    const ResourceCalculator::Icon &icon = _Icons.GetIcon(index.model()->data(index, Qt::DisplayRole).toString().toStdString());
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
      IconSelectedDialog _IconSelectedDialog(_Icons);
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

FactoryTypesEditDialog::FactoryTypesEditDialog(ResourceCalculator::FactoryTypeCollection &FTC, const ResourceCalculator::IconCollection& IC, QWidget *parent )
  : QDialog( parent )
  , _Model(FTC, parent)
  , _Delegate(IC, parent)
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
