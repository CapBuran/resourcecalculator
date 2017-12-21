#include "ProductionChainWidget.h"

#pragma region DELEGATE

#define MINSizeWidthColumb 65
#define EpsilonToOut 4

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
  MaxWidth  = qMax( MaxWidth, MINSizeWidthColumb );
  MaxHeight = qMax( MaxHeight, MINSizeWidthColumb );
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
  int MaxWidth = qMax( fm.width( DisplayData ), MINSizeWidthColumb );
  int MaxHeight = qMax(fm.height(), MINSizeWidthColumb );
  return QSize( MaxHeight, MaxWidth );
}



ProductionChainWidgetDelegateBase::ProductionChainWidgetDelegateBase( const ResourceCalculator::ParamsCollection & PC, ResourceCalculator::ProductionChainModel &PCM, QObject * parent ):
  QStyledItemDelegate( parent ), _PC( PC ), _PCM( PCM )
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


ProductionChainWidgetDelegate0::ProductionChainWidgetDelegate0( const ResourceCalculator::ParamsCollection &PC, ResourceCalculator::ProductionChainModel &PCM, QObject *parent):
  ProductionChainWidgetDelegateBase( PC, PCM,  parent )
{
}

QSize ProductionChainWidgetDelegate0::sizeHint( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
  QSize RetValue = ProductionChainWidgetDelegateBase::sizeHint( option, index );
  if ( index.column() == 0 ) {
    RetValue.setWidth( RetValue.width() + 10 );
  }
  return RetValue;
}

QWidget *ProductionChainWidgetDelegate0::createEditor( QWidget *parent, const QStyleOptionViewItem &option,  const QModelIndex &index ) const
{
  using namespace ResourceCalculator;
  if ( index.column() == 0 ) {
    const std::vector <KEY_FACTORY> &Factorys = _PCM.GetRow( index.row() ).GetFactorys();
    QComboBox *combobox = new QComboBox( parent );
    KEY_FACTORY CurenFactory = _PCM.GetRow( index.row() ).GetFactoryCurrent();
    int Pos = -1;
    int Counter = 0;
    for ( KEY_FACTORY FI : Factorys ) {
      QString str = QString::fromStdString( _PC.FC.GetFactory( FI ).GetName() );
      combobox->addItem( str );
      if ( CurenFactory == FI ) {
        Pos = Counter;
      }
      Counter++;
    }
    combobox->setGeometry( option.rect );
    combobox->setCurrentIndex( Pos );
    combobox->setFrame( true );
    return combobox;
  }
  return ProductionChainWidgetDelegateBase::createEditor( parent, option, index );
}

void ProductionChainWidgetDelegate0::setEditorData( QWidget *editor, const QModelIndex &index ) const
{
  bool IsOk = false;
  if ( index.column() == 0 ) {
    QComboBox *combobox = dynamic_cast< QComboBox * >( editor );
    combobox->setCurrentText( index.model()->data( index, Qt::EditRole ).toString() );
    IsOk = true;
  }
  if ( !IsOk ) {
    ProductionChainWidgetDelegateBase::setEditorData( editor, index );
  }

  //int value = index.model()->data( index, Qt::EditRole ).toInt();
  //QSpinBox *spinBox = static_cast<QSpinBox*>( editor );
  //spinBox->setValue( value );
}

void ProductionChainWidgetDelegate0::setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
{
  bool IsOk = false;
  if ( index.column() == 0 ) {
    QComboBox *combobox = dynamic_cast< QComboBox * >( editor );
    int value = combobox->currentIndex();
    model->setData( index, value, Qt::EditRole );
    IsOk = true;
  }
  if ( !IsOk ) {
    ProductionChainWidgetDelegateBase::setModelData( editor, model, index );
  }
}

void ProductionChainWidgetDelegate0::updateEditorGeometry( QWidget *editor,
  const QStyleOptionViewItem &option, const QModelIndex &/* index */ ) const
{
  editor->setGeometry( option.rect );
}

void ProductionChainWidgetDelegate0::paint( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
  using namespace ResourceCalculator;
  switch ( index.column() ) {
  case 0: {
    KEY_FACTORY CurenFactory = _PCM.GetRow( index.row() ).GetFactoryCurrent();
    const Factory &factory = _PC.FC.GetFactory( CurenFactory );
    QString Text = QString::fromStdString( factory.GetName() );
    QStyleOptionComboBox comboBoxOption;
    comboBoxOption.rect = option.rect;
    comboBoxOption.currentText = Text;
    comboBoxOption.state = QStyle::State_Enabled;
    QApplication::style()->drawComplexControl( QStyle::CC_ComboBox, &comboBoxOption, painter, 0 );
    QApplication::style()->drawControl( QStyle::CE_ComboBoxLabel, &comboBoxOption, painter, 0 );
    break;
  }
  default:
    QStyledItemDelegate::paint( painter, option, index );
    return;
    break;
  }
}

#pragma endregion DELEGATE

#pragma region MODEL

QString ToOut( double Value )
{
  if ( abs( Value ) < 0.0001 ) {
    Value = 0.0;
  }
  return QString::number( Value, 'g', EpsilonToOut );
}

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
      return ToOut( ROW.GetSummProductivity() );
    case 3:
      return ToOut( ROW.GetSummSpeed() );
    case 4:
      return ToOut( ROW.GetSpeedFactory() );
    case 5:
      return ToOut( ROW.GetSecPerOneRecipe() );
    case 6:
      return ToOut( ROW.GetRealTimeProductionOfOneItemPerSec() );
    case 7:
      return ToOut( ROW.GetCountFactorys() );
    default:
      break;
    }
  }

  if ( index.column() > 7 && index.row() < _PCM.CountRecipes() ) {
    const ProductionChainDataRow& ROW = _PCM.GetRow( index.row() );
    if ( 7 < index.column() && index.column() <= 7 + CI ) {
      return ToOut( ROW.GetCountItems()[index.column() - 8] );
    }
    if ( 7 < index.column() + CI && index.column() <= 7 + CI + CI ) {
      return ToOut( ROW.GetItemsPerSec()[index.column() - 8 - CI] );
    }
  }

  if ( index.column() > 7 && index.row() < _PCM.CountRecipes() ) {
    const ProductionChainDataRow& ROW = _PCM.GetRow( index.row() );
    if ( 7 < index.column() && index.column() <= 7 + CI ) {
      return ToOut( ROW.GetCountItems()[index.column() - 8] );
    }
    if ( 7 < index.column() + CI && index.column() <= 7 + CI + CI ) {
      return ToOut( ROW.GetItemsPerSec()[index.column() - 8 - CI] );
    }
  }

  if ( index.row() == _PCM.CountRecipes() ) {
    return ToOut( _PCM.GetSummSpeeds()[index.column() - 8 - CI] );
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

bool ProductionChainWidgetModel::setData( const QModelIndex &index, const QVariant &value, int role )
{
  using namespace ResourceCalculator;
  if ( !( index.isValid() && role == Qt::EditRole ) )return false;
  int CI = _PCM.CountItems();
  if ( index.column() == 0 ) {
    beginResetModel();
    const ProductionChainDataRow& ROW = _PCM.GetRow( index.row() );
    KEY_FACTORY KeyFactory = ROW.GetFactoryIdFromIndex( value.toInt() );
    _PCM.SetFactory( index.row(), KeyFactory );
    endResetModel();
    emit( AllDataChanged() );
    return true;
  }
  if ( index.column() == 7 ) {
    beginResetModel();
    _PCM.SetCountFactores( index.row(), value.toDouble() );
    endResetModel();
    emit( AllDataChanged() );
    return true;
  }
  if ( index.column() > 7 && index.row() < _PCM.CountRecipes() ) {
    beginResetModel();
    ProductionChainDataRow& ROW = _PCM.GetRowEdit( index.row() );
    ROW.FindCountFactorysForItemsCount( index.column() - 8 - CI, value.toDouble() );
    _PCM.Optimize();
    endResetModel();
    emit( AllDataChanged() );
    return true;
  }
  return false;
}

Qt::ItemFlags ProductionChainWidgetModel::flags( const QModelIndex &index ) const
{
  if ( !index.isValid() )
    return Qt::ItemIsEnabled;
  int CI = _PCM.CountItems();
  Qt::ItemFlags flags = QAbstractTableModel::flags( index );
  if ( index.column() == 0 || index.column() == 2 || index.column() == 3 || index.column() == 7 ) {
    flags |= Qt::ItemIsEditable;
  }
  if ( index.column() > CI + 7 && index.row() < _PCM.CountRecipes() ) {
    flags |= Qt::ItemIsEditable;
  }
  return flags;
}

void ProductionChainWidgetModel::FitQuantity()
{
  _PCM.FitQuantity();
  emit( AllDataChanged() );
}

bool ProductionChainWidgetModel::SetItemKey( ResourceCalculator::KEY_ITEM ItemKey )
{
  return _PCM.SetItemKey( ItemKey );
}

ResourceCalculator::ProductionChainModel & ProductionChainWidgetModel::GetPCM()
{
  return _PCM;
}

void ProductionChainWidgetModel::ModelAllChanged()
{
  beginResetModel();
  _PCM.Optimize();
  endResetModel();
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
  QSortFilterProxyModel *Proxys[4];

  ProductionChainWidgetModel *Model = new ProductionChainWidgetModel( _PC, this );
  Model->SetItemKey( ItemKey );

  QSplitter *splitter = new QSplitter( this );

  Proxys[0] = new ProductionChainWidgetProxyModel0( this );
  Proxys[1] = new ProductionChainWidgetProxyModel1( this );
  Proxys[2] = new ProductionChainWidgetProxyModel2( this );
  Proxys[3] = new ProductionChainWidgetProxyModel3( this );

  int MaxHeight = 0;

  for ( int i = 0; i < 4; i++ ) {
    tables[i] = new QTableView( this );
    Proxys[i]->setSourceModel( Model );
    tables[i]->setModel( Proxys[i] );
    tables[i]->setSelectionBehavior( QAbstractItemView::SelectRows );
    //tables[i]->setEditTriggers( QAbstractItemView::NoEditTriggers );
    tables[i]->setSelectionMode( QAbstractItemView::SingleSelection );
    tables[i]->setHorizontalHeader( new ProductionChainWidgetHeaderView( Qt::Orientation::Horizontal, tables[i] ) );
    if ( i == 0 ) {
      tables[i]->setItemDelegate( new ProductionChainWidgetDelegate0( _PC, Model->GetPCM(), tables[i] ) );
    } else {
      tables[i]->setItemDelegate( new ProductionChainWidgetDelegateBase( _PC, Model->GetPCM(), tables[i] ) );
      tables[i]->horizontalHeader()->setSectionResizeMode( QHeaderView::Fixed );
    }
    tables[i]->verticalHeader()->hide();
    tables[i]->setAlternatingRowColors( true );
    tables[i]->horizontalHeader()->setStretchLastSection( false );
    int Height = dynamic_cast< ProductionChainWidgetHeaderView* >( tables[i]->horizontalHeader() )->GetMaxHeight();
    if ( Height > MaxHeight ) {
      MaxHeight = Height;
    }
    dynamic_cast< ProductionChainWidgetHeaderView* >( tables[i]->horizontalHeader() )->SetMaxHeight( MaxHeight );
  }

  for ( int i = 0; i < 4; i++ ) {
    tables[i]->resizeRowsToContents();
    tables[i]->resizeColumnsToContents();
  }

  splitter->addWidget( tables[0] );

  int VerticalSizeResult = 25;

  tables[0]->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

  tables[1]->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  tables[1]->setEditTriggers( QAbstractItemView::AllEditTriggers );

  tables[3]->setFixedHeight( VerticalSizeResult );
  tables[3]->horizontalHeader()->hide();
  tables[3]->verticalScrollBar()->hide();
  tables[3]->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );

  QWidget *WidgetLabel = new QWidget( this );
  WidgetLabel->setFixedHeight( VerticalSizeResult );

  QVBoxLayout *VBoxLayout = new QVBoxLayout( this );
  VBoxLayout->setMargin( 0 );
  VBoxLayout->setSpacing( 0 );
  VBoxLayout->addWidget( tables[0] );
  VBoxLayout->addWidget( WidgetLabel );
  QWidget *Widget1 = new QWidget( this );
  Widget1->setLayout( VBoxLayout );

  QPushButton *AutoFitQuantityButton = new QPushButton( tr( "Auto-fit quantity items" ) );
  connect( AutoFitQuantityButton, SIGNAL( clicked() ), SLOT( PushButtonAutoFitQuantity() ) );

  QWidget *label2 = new QLabel( tr( "Summ results:  " ), this );
  label2->setFixedHeight( VerticalSizeResult );
  
  QHBoxLayout *LabelsLayout = new QHBoxLayout;
  LabelsLayout->addWidget( AutoFitQuantityButton );
  LabelsLayout->addStretch( 1 );
  LabelsLayout->addWidget( label2 );

  QGridLayout *qGridLayout = new QGridLayout( this);
  qGridLayout->setMargin( 0 );
  qGridLayout->setSpacing( 0 );
  qGridLayout->addWidget( tables[1], 0, 0 );
  qGridLayout->addWidget( tables[2], 0, 1 );
  qGridLayout->addWidget( tables[3], 1, 1 );
  qGridLayout->addLayout( LabelsLayout, 1, 0 );
  qGridLayout->setRowMinimumHeight( 1, VerticalSizeResult );
  qGridLayout->setRowStretch( 1, VerticalSizeResult );

  QWidget *Widget2 = new QWidget( splitter );
  Widget2->setLayout( qGridLayout );

  splitter->addWidget( Widget1 );
  splitter->addWidget( Widget2 );

  connect( tables[1]->horizontalScrollBar(), SIGNAL( valueChanged( int ) ), tables[2]->horizontalScrollBar(), SLOT( setValue( int ) ) );
  connect( tables[1]->horizontalScrollBar(), SIGNAL( valueChanged( int ) ), tables[3]->horizontalScrollBar(), SLOT( setValue( int ) ) );
  connect( tables[2]->horizontalScrollBar(), SIGNAL( valueChanged( int ) ), tables[1]->horizontalScrollBar(), SLOT( setValue( int ) ) );
  connect( tables[2]->horizontalScrollBar(), SIGNAL( valueChanged( int ) ), tables[3]->horizontalScrollBar(), SLOT( setValue( int ) ) );
  connect( tables[3]->horizontalScrollBar(), SIGNAL( valueChanged( int ) ), tables[1]->horizontalScrollBar(), SLOT( setValue( int ) ) );
  connect( tables[3]->horizontalScrollBar(), SIGNAL( valueChanged( int ) ), tables[2]->horizontalScrollBar(), SLOT( setValue( int ) ) );

  connect( tables[2]->verticalScrollBar(), SIGNAL( valueChanged( int ) ), tables[0]->verticalScrollBar(), SLOT( setValue( int ) ) );
  connect( tables[2]->verticalScrollBar(), SIGNAL( valueChanged( int ) ), tables[1]->verticalScrollBar(), SLOT( setValue( int ) ) );

  connect( Model, SIGNAL( AllDataChanged() ), Model, SLOT( ModelAllChanged( ) ) );
  
  addTab( splitter, QString::fromStdString( _PC.IC.GetItem( ItemKey )->GetName() ) );

  _Tabs[splitter] = Model;
}

void ProductionChainWidget::Update()
{
  for ( auto & it :_Tabs ) {
    //it.second->AllDataChanged();
    it.second->ModelAllChanged();
  }
}

void ProductionChainWidget::PushButtonAutoFitQuantity()
{
  QWidget *CurrentWidget = currentWidget();
  std::map<QWidget*, ProductionChainWidgetModel*>::iterator F = _Tabs.find( CurrentWidget );
  if ( F == _Tabs.end() ) {
    Q_ASSERT( "No find QWidget!!!" );
    return;
  }
  ProductionChainWidgetModel *PCWM = F->second;
  PCWM->FitQuantity();
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
  int CI = currentIndex();
  if ( CI >= 0 ) {
    QWidget *W = currentWidget();
    auto F = _Tabs.find( W );
    Q_ASSERT( F != _Tabs.end() );
    _Tabs.erase( W );
    removeTab( CI );
  }
}

void ProductionChainWidget::setupTabs()
{
  AddTab( ResourceCalculator::KEY_ITEM::ID_ITEM_Paket1 );
  AddTab( ResourceCalculator::KEY_ITEM::ID_ITEM_Sherst );
}
