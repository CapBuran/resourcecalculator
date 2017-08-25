#include "ProductionChainWidget.h"

#pragma region DELEGATE

ProductionChainWidgetHeaderView::ProductionChainWidgetHeaderView( Qt::Orientation orientation, QWidget * parent ):
QHeaderView( orientation, parent )
{
  _MaxHeight = 0;
  setFont( QFont( font().family(), 10 ) );
}

void ProductionChainWidgetHeaderView::paintSection( QPainter * painter, const QRect & rect, int logicalIndex ) const
{
  QString data = model()->headerData( logicalIndex, orientation() ).toString();
  painter->rotate( -90 );
  painter->setFont( font() );
  painter->drawText( -rect.height() + 5, rect.left() + ( rect.width() ) / 2, data );
}

QSize ProductionChainWidgetHeaderView::sizeHint() const
{
  int count = model()->columnCount();
  int MaxWidth = _MaxHeight;
  int MaxHeight = 0;
  for ( int columb = 0; columb < count; columb++ ) {
    QString DisplayData = model()->headerData( columb, orientation() ).toString();
    QFontMetrics fm( font() );
    int width = fm.width( DisplayData ) + fm.overlinePos();
    if ( width >  MaxWidth ) {
      MaxWidth = width;
      MaxHeight = fm.height();
    }
  }
  return QSize( MaxHeight, MaxWidth );
}

int ProductionChainWidgetHeaderView::GetMaxHeight()
{
  if ( _MaxHeight == 0 ) {
    _MaxHeight = sizeHint().height();
  }
  return _MaxHeight;
}

void ProductionChainWidgetHeaderView::SetMaxHeight( int MaxHeight )
{
  _MaxHeight = MaxHeight;
}

QSize ProductionChainWidgetHeaderView::sectionSizeFromContents( int logicalIndex ) const
{
  QString DisplayData = model()->headerData( logicalIndex, orientation() ).toString();
  QFontMetrics fm( font() );
  int MaxWidth = fm.width( DisplayData );
  int MaxHeight = fm.height();
  return QSize( MaxHeight, MaxWidth );
}



ProductionChainWidgetDelegateBase::ProductionChainWidgetDelegateBase( const ResourceCalculator::ParamsCollection & PC, QObject * parent ):
  QStyledItemDelegate( parent ), _PC( PC )
{
}

QSize ProductionChainWidgetDelegateBase::sizeHint(
  const QStyleOptionViewItem &option,
  const QModelIndex &index ) const
{
  QStyleOptionViewItem optV4( option );
  this->initStyleOption( &optV4, index );
  QFontMetrics fm( optV4.fontMetrics );
  return QSize( fm.width( optV4.text ) + fm.overlinePos(), fm.height() );
}

//void ProductionChainWidgetDelegateBase::paint( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
//{
//  //TODO:
//  QStyledItemDelegate::paint( painter, option, index );
//}

#pragma endregion DELEGATE

#pragma region MODEL

ProductionChainWidgetModel::ProductionChainWidgetModel( const ResourceCalculator::ParamsCollection &PC, QObject *parent )
  : QAbstractTableModel( parent ), _PCM( PC )
{
}

int ProductionChainWidgetModel::rowCount( const QModelIndex &parent ) const
{
  Q_UNUSED( parent );
  return _PCM.CountRecipes() + 1;
}

int ProductionChainWidgetModel::columnCount( const QModelIndex &parent ) const
{
  Q_UNUSED( parent );
  return _PCM.CountItems() * 2 + 8;
}

QVariant ProductionChainWidgetModel::data( const QModelIndex &index, int role ) const
{

  using namespace ResourceCalculator;

  if ( !index.isValid() )
    return QVariant();

  if ( role != Qt::DisplayRole )
    return QVariant();


  if ( index.row() > _PCM.CountRecipes() || index.row() < 0 )
    return QVariant();

  int CI = _PCM.CountItems();
 
  if ( index.row() == _PCM.CountRecipes() && index.column() <= 7 + CI )
    return QVariant();

  if ( index.column() <= 7 && index.row() < _PCM.CountRecipes()  ) {
    const ProductionChainDataRow& ROW = _PCM.GetRow( index.row() );
    switch ( index.column() ) {
    case 0:
      return QString::fromStdString( ROW.GetCurrentFactoryName() );
    case 1:
      return QString::fromStdString( ROW.GetCurrentRecipeName() );
    case 2:
      return QString::number( ROW.GetSummProductivity() );
    case 3:
      return QString::number( ROW.GetSummSpeed() );
    case 4:
      return QString::number( ROW.GetSpeedFactory() );
    case 5:
      return QString::number( ROW.GetSecPerOneRecipe() );
    case 6:
      return QString::number( ROW.GetRealTimeProductionOfOneItemPerSec() );
    case 7:
      return QString::number( ROW.GetCountFactorys() );
    default:
      break;
    }
  }

  if ( index.column() > 7 && index.row() < _PCM.CountRecipes() ) {
    const ProductionChainDataRow& ROW = _PCM.GetRow( index.row() );
    if ( 7 < index.column() && index.column() <= 7 + CI ) {
      return QString::number( ROW.GetCountItems()[index.column() - 8] );
    }
    if ( 7 < index.column() + CI && index.column() <= 7 + CI + CI ) {
      return QString::number( ROW.GetItemsPerSec()[index.column() - 8 - CI] );
    }
  }

  if ( index.column() > 7 && index.row() < _PCM.CountRecipes() ) {
    const ProductionChainDataRow& ROW = _PCM.GetRow( index.row() );
    if ( 7 < index.column() && index.column() <= 7 + CI ) {
      return QString::number( ROW.GetCountItems()[index.column() - 8] );
    }
    if ( 7 < index.column() + CI && index.column() <= 7 + CI + CI ) {
      return QString::number( ROW.GetItemsPerSec()[index.column() - 8 - CI] );
    }
  }

  if ( index.row() == _PCM.CountRecipes() ) {
//    return QString::number( index.column() + 8 );
    return QString::number( _PCM.GetSummSpeeds()[index.column() - 8 - CI] );
  }
  
  return QVariant();
}

QVariant ProductionChainWidgetModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
  if ( role != Qt::DisplayRole )
    return QVariant();

  if ( orientation == Qt::Horizontal ) {
    switch ( section ) {
    case 0:
      return tr( "Factory" );
    case 1:
      return tr( "Recipe" );
    case 2:
      return tr( "Modules productivity" );
    case 3:
      return tr( "Modules speed" );
    case 4:
      return tr( "Speed factory" );
    case 5:
      return tr( "Time for recipe" );
    case 6:
      return tr( "Real time for recipe" );
    case 7:
      return tr( "Count factorys" );
    default:
      {
        int Col = section - 8;
        if ( Col >= _PCM.CountItems() ) {
          Col -= _PCM.CountItems();
        }
        return QString::fromStdString( _PCM.GetItemName( Col ));
      }
    }
  }
  return QVariant();
}

//
//bool ProductionChainWidgetModel::setData( const QModelIndex &index, const QVariant &value, int role )
//{
//  //if ( index.isValid() && role == Qt::EditRole ) {
//  //  int row = index.row();
//
//  //  QPair<QString, QString> p = listOfPairs.value( row );
//
//  //  if ( index.column() == 0 )
//  //    p.first = value.toString();
//  //  else if ( index.column() == 1 )
//  //    p.second = value.toString();
//  //  else
//  //    return false;
//
//  //  listOfPairs.replace( row, p );
//  //  emit( dataChanged( index, index ) );
//
//  //  return true;
//  //}
//
//  return false;
//}
//
//int ProductionChainWidgetModel::SetItemKey( ResourceCalculator::KEY_ITEM )
//{
//  return 0;
//}

Qt::ItemFlags ProductionChainWidgetModel::flags( const QModelIndex &index ) const
{
  if ( !index.isValid() )
    return Qt::ItemIsEnabled;

  //return QAbstractTableModel::flags( index ) | Qt::ItemIsEditable;
  return QAbstractTableModel::flags( index );
}

bool ProductionChainWidgetModel::SetItemKey( ResourceCalculator::KEY_ITEM ItemKey )
{
  return _PCM.SetItemKey( ItemKey );
}

const ResourceCalculator::ProductionChainModel & ProductionChainWidgetModel::GetPCM()
{
  return _PCM;
}

#pragma endregion MODEL

#pragma region PROXYMODEL

ProductionChainWidgetProxyModel0::ProductionChainWidgetProxyModel0( QObject *parent ):
  QSortFilterProxyModel( parent )
{
}

bool ProductionChainWidgetProxyModel0::filterAcceptsColumn( int source_column, const QModelIndex & sourceParent ) const
{
  return source_column < 8;
}

bool ProductionChainWidgetProxyModel0::filterAcceptsRow( int source_row, const QModelIndex & source_parent ) const
{
  return dynamic_cast< ProductionChainWidgetModel* >( sourceModel() )->GetPCM().CountRecipes() > source_row;
}


ProductionChainWidgetProxyModel1::ProductionChainWidgetProxyModel1( QObject *parent ):
  QSortFilterProxyModel( parent )
{
}

bool ProductionChainWidgetProxyModel1::filterAcceptsColumn( int source_column, const QModelIndex & sourceParent ) const
{
  int CI = dynamic_cast< ProductionChainWidgetModel* >( sourceModel() )->GetPCM().CountItems();
  return 7 < source_column && source_column <= CI + 7;
}

bool ProductionChainWidgetProxyModel1::filterAcceptsRow( int source_row, const QModelIndex & source_parent ) const
{
  return dynamic_cast< ProductionChainWidgetModel* >( sourceModel() )->GetPCM().CountRecipes() > source_row;
}


ProductionChainWidgetProxyModel2::ProductionChainWidgetProxyModel2( QObject *parent ):
  QSortFilterProxyModel( parent )
{
}

bool ProductionChainWidgetProxyModel2::filterAcceptsColumn( int source_column, const QModelIndex & sourceParent ) const
{
  int CI = dynamic_cast< ProductionChainWidgetModel* >( sourceModel() )->GetPCM().CountItems();
  return source_column > 7 + CI;
}

bool ProductionChainWidgetProxyModel2::filterAcceptsRow( int source_row, const QModelIndex & source_parent ) const
{
  return dynamic_cast< ProductionChainWidgetModel* >( sourceModel() )->GetPCM().CountRecipes() > source_row;
}


ProductionChainWidgetProxyModel3::ProductionChainWidgetProxyModel3( QObject *parent ):
  QSortFilterProxyModel( parent )
{
}

bool ProductionChainWidgetProxyModel3::filterAcceptsColumn( int source_column, const QModelIndex & sourceParent ) const
{
  int CI = dynamic_cast< ProductionChainWidgetModel* >( sourceModel() )->GetPCM().CountItems();
  return source_column > 7 + CI;
}

bool ProductionChainWidgetProxyModel3::filterAcceptsRow( int source_row, const QModelIndex & source_parent ) const
{
  return dynamic_cast< ProductionChainWidgetModel* >( sourceModel() )->GetPCM().CountRecipes() == source_row;
}

#pragma endregion PROXYMODEL

ProductionChainWidget::ProductionChainWidget( const ResourceCalculator::ParamsCollection &PC, QWidget *parent )
  : _PC(PC), QTabWidget( parent )
{
  //table = new ProductionChainWidgetModel( this );
  //connect( newAddressTab, &NewAddressTab::sendDetails, this, &ProductionChainWidget::addEntry );

  //addTab( newAddressTab, "Address Book" );

  setupTabs();
}

void ProductionChainWidget::showAddEntryDialog()
{
  //AddDialog aDialog;

  //if ( aDialog.exec() ) {
  //  QString name = aDialog.nameText->text();
  //  QString address = aDialog.addressText->toPlainText();

  //  addEntry( name, address );
  //}
}

void ProductionChainWidget::AddTab( ResourceCalculator::KEY_ITEM ItemKey )
{
  QTableView *tables[4];
  QSortFilterProxyModel *Proxys[4];

  ProductionChainWidgetModel *Model = new ProductionChainWidgetModel( _PC );
  Model->SetItemKey( ItemKey );

  QSplitter *splitter = new QSplitter( this );

  Proxys[0] = new ProductionChainWidgetProxyModel0( this );
  Proxys[1] = new ProductionChainWidgetProxyModel1( this );
  Proxys[2] = new ProductionChainWidgetProxyModel2( this );
  Proxys[3] = new ProductionChainWidgetProxyModel3( this );

  int MaxHeight = 0;

  for ( int i = 0; i < 4; i++ ) {
    tables[i] = new QTableView();
    Proxys[i]->setSourceModel( Model );
    tables[i]->setModel( Proxys[i] );
    tables[i]->setSelectionBehavior( QAbstractItemView::SelectRows );
    //tables[i]->setEditTriggers( QAbstractItemView::NoEditTriggers );
    tables[i]->setSelectionMode( QAbstractItemView::SingleSelection );
    tables[i]->setHorizontalHeader( new ProductionChainWidgetHeaderView( Qt::Orientation::Horizontal ) );
    tables[i]->setItemDelegate( new ProductionChainWidgetDelegateBase( _PC ) );
    tables[i]->verticalHeader()->hide();
    tables[i]->setAlternatingRowColors( true );
    //tables[i]->horizontalHeader()->setStretchLastSection( true );
    tables[i]->horizontalHeader()->setStretchLastSection( false );
    int Height = dynamic_cast< ProductionChainWidgetHeaderView* >( tables[i]->horizontalHeader() )->GetMaxHeight();
    if ( Height > MaxHeight ) {
      MaxHeight = Height;
    }
  }

  for ( int i = 0; i < 4; i++ ) {
    dynamic_cast< ProductionChainWidgetHeaderView* >( tables[i]->horizontalHeader() )->SetMaxHeight( MaxHeight );
    tables[i]->resizeRowsToContents();
    tables[i]->resizeColumnsToContents();
  }

  splitter->addWidget( tables[0] );

  int VerticalSizeResult = 25;

  tables[3]->setFixedHeight( VerticalSizeResult );
  tables[3]->verticalHeader()->hide();
  tables[3]->horizontalHeader()->hide();
  tables[3]->verticalScrollBar()->hide();
  tables[3]->verticalScrollBar()->setDisabled( true );
  tables[1]->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
  tables[2]->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
  tables[3]->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
  tables[3]->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

  QWidget *WidgetLabel = new QWidget();
  WidgetLabel->setFixedHeight( VerticalSizeResult );

  QVBoxLayout *VBoxLayout = new QVBoxLayout();
  VBoxLayout->setMargin( 0 );
  VBoxLayout->setSpacing( 0 );
  VBoxLayout->addWidget( tables[0] );
  VBoxLayout->addWidget( WidgetLabel );
  QWidget *Widget1 = new QWidget();
  Widget1->setLayout( VBoxLayout );

  QWidget *label2 = new QLabel( tr( "Summ results:  " ) );
  label2->setFixedHeight( VerticalSizeResult );

  QGridLayout *qGridLayout = new QGridLayout;
  qGridLayout->setMargin( 0 );
  qGridLayout->setSpacing( 0 );
  qGridLayout->addWidget( tables[1], 0, 0 );
  qGridLayout->addWidget( tables[2], 0, 1 );
  qGridLayout->addWidget( tables[3], 1, 1 );
  qGridLayout->addWidget( label2, 1, 0, Qt::AlignCenter | Qt::AlignRight );
  qGridLayout->setRowMinimumHeight( 1, VerticalSizeResult );
  qGridLayout->setRowStretch( 1, VerticalSizeResult );

  QWidget *Widget2 = new QWidget();
  Widget2->setLayout( qGridLayout );

  splitter->addWidget( Widget1 );
  splitter->addWidget( Widget2 );

  connect( tables[1]->horizontalScrollBar(), SIGNAL( valueChanged( int ) ), tables[2]->horizontalScrollBar(), SLOT( setValue( int ) ) );
  connect( tables[1]->horizontalScrollBar(), SIGNAL( valueChanged( int ) ), tables[3]->horizontalScrollBar(), SLOT( setValue( int ) ) );
  connect( tables[2]->horizontalScrollBar(), SIGNAL( valueChanged( int ) ), tables[1]->horizontalScrollBar(), SLOT( setValue( int ) ) );
  connect( tables[2]->horizontalScrollBar(), SIGNAL( valueChanged( int ) ), tables[3]->horizontalScrollBar(), SLOT( setValue( int ) ) );
  connect( tables[3]->horizontalScrollBar(), SIGNAL( valueChanged( int ) ), tables[1]->horizontalScrollBar(), SLOT( setValue( int ) ) );
  connect( tables[3]->horizontalScrollBar(), SIGNAL( valueChanged( int ) ), tables[2]->horizontalScrollBar(), SLOT( setValue( int ) ) );

  connect( tables[0]->verticalScrollBar(), SIGNAL( valueChanged( int ) ), tables[1]->verticalScrollBar(), SLOT( setValue( int ) ) );
  connect( tables[0]->verticalScrollBar(), SIGNAL( valueChanged( int ) ), tables[2]->verticalScrollBar(), SLOT( setValue( int ) ) );
  connect( tables[1]->verticalScrollBar(), SIGNAL( valueChanged( int ) ), tables[0]->verticalScrollBar(), SLOT( setValue( int ) ) );
  connect( tables[1]->verticalScrollBar(), SIGNAL( valueChanged( int ) ), tables[2]->verticalScrollBar(), SLOT( setValue( int ) ) );
  connect( tables[2]->verticalScrollBar(), SIGNAL( valueChanged( int ) ), tables[0]->verticalScrollBar(), SLOT( setValue( int ) ) );
  connect( tables[2]->verticalScrollBar(), SIGNAL( valueChanged( int ) ), tables[1]->verticalScrollBar(), SLOT( setValue( int ) ) );

  addTab( splitter, QString::fromStdString( _PC.IC.GetItem( ItemKey )->GetName() ) );
}

void ProductionChainWidget::addEntry( QString name, QString address )
{
  //QList<QPair<QString, QString> >list = table->getList();
  //QPair<QString, QString> pair( name, address );

  //if ( !list.contains( pair ) ) {
  //  table->insertRows( 0, 1, QModelIndex() );
  //  QModelIndex index = table->index( 0, 0, QModelIndex() );
  //  table->setData( index, name, Qt::EditRole );
  //  index = table->index( 0, 1, QModelIndex() );
  //  table->setData( index, address, Qt::EditRole );
  //} else {
  //  QMessageBox::information( this, tr( "Duplicate Name" ),
  //    tr( "The name \"%1\" already exists." ).arg( name ) );
  //}
}

void ProductionChainWidget::editEntry()
{
  //QTableView *temp = static_cast<QTableView*>( currentWidget() );
  //QSortFilterProxyModel *proxy = static_cast<QSortFilterProxyModel*>( temp->model() );
  //QItemSelectionModel *selectionModel = temp->selectionModel();

  //QModelIndexList indexes = selectionModel->selectedRows();
  //QString name;
  //QString address;
  //int row = -1;

  //foreach( QModelIndex index, indexes )
  //{
  //  row = proxy->mapToSource( index ).row();
  //  QModelIndex nameIndex = table->index( row, 0, QModelIndex() );
  //  QVariant varName = table->data( nameIndex, Qt::DisplayRole );
  //  name = varName.toString();

    //QModelIndex addressIndex = table->index( row, 1, QModelIndex() );
    //QVariant varAddr = table->data( addressIndex, Qt::DisplayRole );
    //address = varAddr.toString();
  //}

  //AddDialog aDialog;
  //aDialog.setWindowTitle( tr( "Edit a Contact" ) );

  //aDialog.nameText->setReadOnly( true );
  //aDialog.nameText->setText( name );
  //aDialog.addressText->setText( address );

  //if ( aDialog.exec() ) {
  //  QString newAddress = aDialog.addressText->toPlainText();
  //  if ( newAddress != address ) {
  //    QModelIndex index = table->index( row, 1, QModelIndex() );
  //    table->setData( index, newAddress, Qt::EditRole );
  //  }
  //}
}

void ProductionChainWidget::removeEntry()
{
  //QTableView *temp = static_cast<QTableView*>( currentWidget() );
  //QSortFilterProxyModel *proxy = static_cast<QSortFilterProxyModel*>( temp->model() );
  //QItemSelectionModel *selectionModel = temp->selectionModel();

  //QModelIndexList indexes = selectionModel->selectedRows();

  //foreach( QModelIndex index, indexes )
  //{
  //  int row = proxy->mapToSource( index ).row();
  //  table->removeRows( row, 1, QModelIndex() );
  //}

  //if ( table->rowCount( QModelIndex() ) == 0 ) {
  //  insertTab( 0, newAddressTab, "Address Book" );
  //}
}

void ProductionChainWidget::setupTabs()
{
  AddTab( ResourceCalculator::KEY_ITEM::ID_ITEM_Paket1 );
  AddTab( ResourceCalculator::KEY_ITEM::ID_ITEM_Sherst );
}

