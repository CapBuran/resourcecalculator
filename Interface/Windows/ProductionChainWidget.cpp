#include "ProductionChainWidget.h"
#include "ModulesSelectDialog.h"

Q_DECLARE_METATYPE(ResourceCalculator::FactoryModules);

#pragma region DELEGATE

ProductionChainDelegate0::ProductionChainDelegate0( const ResourceCalculator::ProductionChainModel &PCM, QObject *parent):
  _PCM(PCM), ProductionChainDelegateBase(PCM.GetPC(), parent)
{
}

QSize ProductionChainDelegate0::sizeHint( const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
  QSize RetValue = ProductionChainDelegateBase::sizeHint( option, index );
  if ( index.column() == 0 ) {
    RetValue.setWidth( RetValue.width() + 10 );
  }
  return RetValue;
}

QWidget *ProductionChainDelegate0::createEditor( QWidget *parent, const QStyleOptionViewItem &option,  const QModelIndex &index ) const
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
  if (index.column() == 1)
  {
    QComboBox *combobox = new QComboBox(parent);
    combobox->addItem(tr("Item received earlier"));
    KEY_RECIPE CurenRecipe = _PCM.GetRow(index.row()).GetRecipeCurrent();
    const Recipe *recipe = _PC.RC.GetRecipe(CurenRecipe);
    combobox->setCurrentIndex(0);
    if (recipe != nullptr) {
      combobox->addItem(QString::fromStdString(recipe->GetName()));
      combobox->setCurrentIndex(1);
    }
    if (CurenRecipe == KEY_RECIPE::ID_RECIPE_FindRecipeROOT) {
      combobox->setCurrentIndex(0);
    }
    combobox->setGeometry(option.rect);
    combobox->setFrame(true);
    return combobox;
  }
  return ProductionChainDelegateBase::createEditor( parent, option, index );
}

void ProductionChainDelegate0::setEditorData( QWidget *editor, const QModelIndex &index ) const
{
  bool IsOk = false;
  if ( index.column() == 0 || index.column() == 1) {
    QComboBox *combobox = dynamic_cast< QComboBox * >( editor );
    combobox->setCurrentText( index.model()->data( index, Qt::EditRole ).toString() );
    IsOk = true;
  }
  if ( !IsOk ) {
    ProductionChainDelegateBase::setEditorData( editor, index );
  }
}

void ProductionChainDelegate0::setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
{
  bool IsOk = false;
  if ( index.column() == 0 || index.column() == 1) {
    QComboBox *combobox = dynamic_cast< QComboBox * >( editor );
    int value = combobox->currentIndex();
    model->setData( index, value, Qt::EditRole );
    IsOk = true;
  }
  if ( !IsOk ) {
    ProductionChainDelegateBase::setModelData( editor, model, index );
  }
}

void ProductionChainDelegate0::updateEditorGeometry( QWidget *editor,
  const QStyleOptionViewItem &option, const QModelIndex &/* index */ ) const
{
  editor->setGeometry( option.rect );
}

void ProductionChainDelegate0::paint( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
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
  case 1: {
    KEY_RECIPE CurenRecipe = _PCM.GetRow(index.row()).GetRecipeCurrent();
    const Recipe *recipe = _PC.RC.GetRecipe(CurenRecipe);
    if (recipe != nullptr)
    {
      QString Text = QString::fromStdString(recipe->GetName());
      QStyleOptionComboBox comboBoxOption;
      comboBoxOption.rect = option.rect;
      comboBoxOption.currentText = Text;
      comboBoxOption.state = QStyle::State_Enabled;
      QApplication::style()->drawComplexControl(QStyle::CC_ComboBox, &comboBoxOption, painter, 0);
      QApplication::style()->drawControl(QStyle::CE_ComboBoxLabel, &comboBoxOption, painter, 0);
    } else {
      QStyledItemDelegate::paint(painter, option, index);
    }
    break;
  }
  default:
    QStyledItemDelegate::paint( painter, option, index );
    return;
    break;
  }
}

bool ProductionChainDelegate0::editorEvent(QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index)
{
  if (event->type() == QEvent::MouseButtonPress) {
    int column = index.column();
    if (column == 2 || column == 3) {
      ModulesSelectDialog MSD(_PC, _PCM.GetRow(index.row()).GetFM());
      if (MSD.exec()) {
        const ResourceCalculator::FactoryModules &Result = MSD.GetResult();
        QVariant data; data.setValue<ResourceCalculator::FactoryModules>(Result);
        model->setData(index, data);
      }
      return true;
    }
  }
  return ProductionChainDelegateBase::editorEvent(event, model, option, index);
}

#pragma endregion DELEGATE

#pragma region MODEL

ProductionChainModel::ProductionChainModel(ResourceCalculator::ProductionChainModel &PCM, QObject *parent )
  : QAbstractTableModel( parent ), _PCM(PCM)
{
}

int ProductionChainModel::rowCount( const QModelIndex &parent ) const
{
  Q_UNUSED( parent );
  return _PCM.CountRecipes() + 1;
}

int ProductionChainModel::columnCount( const QModelIndex &parent ) const
{
  Q_UNUSED( parent );
  return _PCM.CountItems() * 2 + 8;
}

QVariant ProductionChainModel::data( const QModelIndex &index, int role ) const
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
      return ToOut( ROW.GetSummProductivity(_PCM.GetPC()) );
    case 3:
      return ToOut( ROW.GetSummSpeed(_PCM.GetPC()) );
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

QVariant ProductionChainModel::headerData( int section, Qt::Orientation orientation, int role ) const
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

bool ProductionChainModel::setData( const QModelIndex &index, const QVariant &value, int role )
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
  if (index.column() == 1)
  {
    beginResetModel();
    if (value.toInt() == 0)
    {
      _PCM.SetRecipe(index.row(), KEY_RECIPE::ID_RECIPE_FindRecipeROOT);
    }
    endResetModel();
    emit(AllDataChanged());
    return true;
  }
  if (index.column() == 2 || index.column() == 3) {
    beginResetModel();
    const ResourceCalculator::FactoryModules &FM = value.value<ResourceCalculator::FactoryModules>();
    _PCM.SetModules(index.row(), FM);
    endResetModel();
    emit(AllDataChanged());
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

Qt::ItemFlags ProductionChainModel::flags( const QModelIndex &index ) const
{
  if ( !index.isValid() )
    return Qt::ItemIsEnabled;
  int CI = _PCM.CountItems();
  Qt::ItemFlags flags = QAbstractTableModel::flags( index );
  if ( index.column() == 0 || index.column() == 1 || index.column() == 2 || index.column() == 3 || index.column() == 7 ) {
    flags |= Qt::ItemIsEditable;
  }
  if ( index.column() > CI + 7 && index.row() < _PCM.CountRecipes() ) {
    flags |= Qt::ItemIsEditable;
  }
  return flags;
}

void ProductionChainModel::FitQuantity()
{
  _PCM.FitQuantity();
  _PCM.ReInit();

  emit( AllDataChanged() );
}

const ResourceCalculator::ProductionChainModel & ProductionChainModel::GetPCM() const
{
  return _PCM;
}

void ProductionChainModel::ModelAllChanged()
{
  beginResetModel();
  _PCM.Optimize();
  endResetModel();
}

void ProductionChainModel::Update()
{
  beginResetModel();
  _PCM.ReInit();
  endResetModel();
}

#pragma endregion MODEL

#pragma region PROXYMODEL

ProductionChainProxyModel0::ProductionChainProxyModel0( QObject *parent ):
  QSortFilterProxyModel( parent )
{
}

bool ProductionChainProxyModel0::filterAcceptsColumn( int source_column, const QModelIndex & sourceParent ) const
{
  return source_column < 8;
}

bool ProductionChainProxyModel0::filterAcceptsRow( int source_row, const QModelIndex & source_parent ) const
{
  return dynamic_cast< ProductionChainModel* >( sourceModel() )->GetPCM().CountRecipes() > source_row;
}


ProductionChainProxyModel1::ProductionChainProxyModel1( QObject *parent ):
  QSortFilterProxyModel( parent )
{
}

bool ProductionChainProxyModel1::filterAcceptsColumn( int source_column, const QModelIndex & sourceParent ) const
{
  int CI = dynamic_cast< ProductionChainModel* >( sourceModel() )->GetPCM().CountItems();
  return 7 < source_column && source_column <= CI + 7;
}

bool ProductionChainProxyModel1::filterAcceptsRow( int source_row, const QModelIndex & source_parent ) const
{
  return dynamic_cast< ProductionChainModel* >( sourceModel() )->GetPCM().CountRecipes() > source_row;
}


ProductionChainProxyModel2::ProductionChainProxyModel2( QObject *parent ):
  QSortFilterProxyModel( parent )
{
}

bool ProductionChainProxyModel2::filterAcceptsColumn( int source_column, const QModelIndex & sourceParent ) const
{
  int CI = dynamic_cast< ProductionChainModel* >( sourceModel() )->GetPCM().CountItems();
  return source_column > 7 + CI;
}

bool ProductionChainProxyModel2::filterAcceptsRow( int source_row, const QModelIndex & source_parent ) const
{
  return dynamic_cast< ProductionChainModel* >( sourceModel() )->GetPCM().CountRecipes() > source_row;
}


ProductionChainProxyModel3::ProductionChainProxyModel3( QObject *parent ):
  QSortFilterProxyModel( parent )
{
}

bool ProductionChainProxyModel3::filterAcceptsColumn( int source_column, const QModelIndex & sourceParent ) const
{
  int CI = dynamic_cast< ProductionChainModel* >( sourceModel() )->GetPCM().CountItems();
  return source_column > 7 + CI;
}

bool ProductionChainProxyModel3::filterAcceptsRow( int source_row, const QModelIndex & source_parent ) const
{
  return dynamic_cast< ProductionChainModel* >( sourceModel() )->GetPCM().CountRecipes() == source_row;
}

#pragma endregion PROXYMODEL

ProductionChainWidget::ProductionChainWidget(ResourceCalculator::ProductionChainModel &PCM, QWidget *parent )
  : ProductionChainWidgetBase(PCM.GetPC(), parent), _Model(PCM, parent), _PCM(PCM)
{
  _Init();
}

ResourceCalculator::ProductionChainModel & ProductionChainWidget::GetPCM()
{
  return _PCM;
}

ProductionChainWidgetType ProductionChainWidget::GetType() const
{
  return ProductionChainWidgetType::ProductionChain;
}

void ProductionChainWidget::UpdateModel()
{
  _Model.Update();
}

void ProductionChainWidget::_Init( )
{
  QSortFilterProxyModel *Proxys[4];

  Proxys[0] = new ProductionChainProxyModel0( this );
  Proxys[1] = new ProductionChainProxyModel1( this );
  Proxys[2] = new ProductionChainProxyModel2( this );
  Proxys[3] = new ProductionChainProxyModel3( this );

  int MaxHeight = 0;

  for ( int i = 0; i < 4; i++ ) {
    tables[i] = new QTableView( this );
    Proxys[i]->setSourceModel( &_Model );
    tables[i]->setModel( Proxys[i] );
    tables[i]->setSelectionBehavior( QAbstractItemView::SelectRows );
    //tables[i]->setEditTriggers( QAbstractItemView::NoEditTriggers );
    tables[i]->setSelectionMode( QAbstractItemView::SingleSelection );
    tables[i]->setHorizontalHeader( new ProductionChainHeaderView( Qt::Orientation::Horizontal, tables[i] ) );
    if ( i == 0 ) {
      tables[i]->setItemDelegate( new ProductionChainDelegate0( _Model.GetPCM(), tables[i] ) );
    } else {
      tables[i]->setItemDelegate( new ProductionChainDelegateBase( _PC, tables[i] ) );
      tables[i]->horizontalHeader()->setSectionResizeMode( QHeaderView::Fixed );
    }
    tables[i]->verticalHeader()->hide();
    tables[i]->setAlternatingRowColors( true );
    tables[i]->horizontalHeader()->setStretchLastSection( false );
    int Height = dynamic_cast< ProductionChainHeaderView* >( tables[i]->horizontalHeader() )->GetMaxHeight();
    if ( Height > MaxHeight ) {
      MaxHeight = Height;
    }
    dynamic_cast< ProductionChainHeaderView* >( tables[i]->horizontalHeader() )->SetMaxHeight( MaxHeight );
  }

  for ( int i = 0; i < 4; i++ ) {
    tables[i]->resizeRowsToContents();
    tables[i]->resizeColumnsToContents();
  }

  int VerticalSizeResult = 25;

  tables[0]->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  tables[0]->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

  tables[1]->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  tables[1]->setEditTriggers( QAbstractItemView::AllEditTriggers );
  tables[1]->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

  tables[2]->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

  tables[3]->setFixedHeight( VerticalSizeResult );
  tables[3]->horizontalHeader()->hide();
  tables[3]->verticalScrollBar()->hide();
  tables[3]->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  tables[3]->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
  tables[3]->horizontalHeader()->setStretchLastSection(false);

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

  QWidget *Widget2 = new QWidget(this);
  Widget2->setLayout( qGridLayout );

  addWidget(Widget1);
  addWidget(Widget2);

  connect( tables[1]->horizontalScrollBar(), SIGNAL( valueChanged( int ) ), tables[2]->horizontalScrollBar(), SLOT( setValue( int ) ) );
  connect( tables[1]->horizontalScrollBar(), SIGNAL( valueChanged( int ) ), tables[3]->horizontalScrollBar(), SLOT( setValue( int ) ) );
  connect( tables[2]->horizontalScrollBar(), SIGNAL( valueChanged( int ) ), tables[1]->horizontalScrollBar(), SLOT( setValue( int ) ) );
  connect( tables[2]->horizontalScrollBar(), SIGNAL( valueChanged( int ) ), tables[3]->horizontalScrollBar(), SLOT( setValue( int ) ) );
  connect( tables[3]->horizontalScrollBar(), SIGNAL( valueChanged( int ) ), tables[1]->horizontalScrollBar(), SLOT( setValue( int ) ) );
  connect( tables[3]->horizontalScrollBar(), SIGNAL( valueChanged( int ) ), tables[2]->horizontalScrollBar(), SLOT( setValue( int ) ) );

  connect( tables[2]->verticalScrollBar(), SIGNAL( valueChanged( int ) ), tables[0]->verticalScrollBar(), SLOT( setValue( int ) ) );
  connect( tables[2]->verticalScrollBar(), SIGNAL( valueChanged( int ) ), tables[1]->verticalScrollBar(), SLOT( setValue( int ) ) );

  connect(tables[2]->horizontalHeader(), SIGNAL(sectionResized(int, int, int)), SLOT(OnResized(int, int, int)));

  connect( &_Model, SIGNAL( AllDataChanged() ), &_Model, SLOT( ModelAllChanged( ) ) );
  
  connect(AutoFitQuantityButton, SIGNAL(clicked()), SLOT(_PushButtonAutoFitQuantity()));

}

void ProductionChainWidget::_PushButtonAutoFitQuantity()
{
  _Model.FitQuantity();
}

void ProductionChainWidget::OnResized(int logicalIndex, int oldSize, int newSize)
{
  tables[3]->setColumnWidth(logicalIndex, newSize);
}

