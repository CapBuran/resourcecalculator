#include "IconSelectedDialog.h"
#include "ModulesEditDialog.h"

#pragma region MODEL

ModulesTypesEditModel::ModulesTypesEditModel( ResourceCalculator::ModuleCollection& MC, QObject *parent )
  : QAbstractTableModel( parent )
  , _MC(MC)
{
  Select();
}

int ModulesTypesEditModel::rowCount( const QModelIndex & parent ) const
{
  Q_UNUSED( parent );
  return _MC_EDIT.Size();
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
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  if (index.row() == 0)
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;

  return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

bool ModulesTypesEditModel::insertRows( int position, int rows, const QModelIndex & index )
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

bool ModulesTypesEditModel::removeRows( int position, int rows, const QModelIndex & index )
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
    _MC_EDIT.Delete({ _MC_EDIT(row) });
  }
  endRemoveRows();
  return true;  
}

bool ModulesTypesEditModel::setData( const QModelIndex & index, const QVariant & value, int role )
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

void ModulesTypesEditModel::Commit()
{
  _MC = _MC_EDIT;
  Select();
}

void ModulesTypesEditModel::Select()
{
  _MC_EDIT = _MC;
}

#pragma endregion MODEL

#pragma region DELEGATE

ModulesEditDelegate::ModulesEditDelegate(const ResourceCalculator::ModuleCollection& MC, const ResourceCalculator::IconCollection& icons, QObject *parent )
  : QStyledItemDelegate(parent)
  , _MC(MC)
  , _Icons(icons)
{
}

void ModulesEditDelegate::paint( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
  emit( editorEventDelegate( index ) );
  switch ( index.column() ) {
  case 0: {
    std::string IconPath = index.data().toString().toStdString();
    const ResourceCalculator::Icon &icon = _Icons.GetIcon( IconPath );
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
      IconSelectedDialog _IconSelectedDialog( _Icons );
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

ModulesEditDialog::ModulesEditDialog(ResourceCalculator::ModuleCollection& MC, const ResourceCalculator::IconCollection& icons, QWidget *parent ):
  _Model(MC, parent )
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
  _tableView->setItemDelegate( new ModulesEditDelegate(MC, icons) );
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
