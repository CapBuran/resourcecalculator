#include "RecipesEditDialog.h"
#include "ItemSelectedDialog.h"

#pragma region MODEL

RecipeListModel::RecipeListModel(ResourceCalculator::ParamsCollection &PC, QObject *parent):
  QAbstractTableModel(parent), _PC(PC)
{
  using namespace ResourceCalculator;
  const std::map<KEY_RECIPE, Recipe> &DATA = _PC.RC.GetData();
  _listOfRecipesId.reserve(static_cast<int>(DATA.size()));
  for (auto &it : DATA) {
    _listOfRecipesId.push_back(it.first);
  }
}

int RecipeListModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return _listOfRecipesId.size();
}

int RecipeListModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return 5;
}

QVariant RecipeListModel::data(const QModelIndex &index, int role) const
{
  using namespace ResourceCalculator;

  if (!index.isValid())
    return QVariant();

  if (index.row() >= _listOfRecipesId.size() || index.row() < 0)
    return QVariant();

  if (role == Qt::DisplayRole) {
    const KEY_RECIPE KeyRecipe = GetRecipeId( index.row() );

    Recipe *R = _PC.RC.GetRecipeForEdit( KeyRecipe );
    if (R == nullptr) {
      return QVariant();
    }

    switch (index.column()) {
    case 0:
      return QString::fromStdString( R->GetName() );
      break;
    case 1:
      return QString::number( R->GetTime() );
      break;
    case 2:
    case 3:
    case 4:
    default:
      return QVariant();
      break;
    }
  }

  return QVariant();
}

QVariant RecipeListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal) {
    switch (section) {
    case 0:
      return tr("Recipe Name");
    case 1:
      return tr("Recipe time");
    case 2:
      return tr("Result of recipe");
    case 3:
      return tr("Ingredients of the recipe");
    case 4:
      return tr("Allowed factories");
    default:
      return QVariant();
    }
  }
  return QVariant();
}
 
Qt::ItemFlags RecipeListModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool RecipeListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if (index.isValid() && role == Qt::EditRole) {
    using namespace ResourceCalculator;

    const KEY_RECIPE KeyRecipe = GetRecipeId( index.row( ) );

    Recipe *EditRecipe = _PC.RC.GetRecipeForEdit(KeyRecipe);
    if (EditRecipe == nullptr) {
      return false;
    }

    switch (index.column())
    {
    case 0: {
      QString Name = value.toString( );
      if ( Name.length() > 0 ) {
        EditRecipe->SetName( Name.toStdString( ) );
      }
      break;
    }
    case 1: {
      double Value = value.toDouble( );
      if ( Value >= 0 ) {
        EditRecipe->SetTime( Value );
      }
      break;
    }
    default:
      return false;
      break;
    }

    emit(dataChanged(index, index));

    return true;
  }

  return false;
}

bool RecipeListModel::insertRows(int position, int rows, const QModelIndex &index)
{
  Q_UNUSED(index);
  beginInsertRows(QModelIndex(), position, position + rows - 1);
  for (int row = 0; row < rows; ++row) {
    using namespace ResourceCalculator;
    KEY_RECIPE NewKey = _PC.RC.GetUniqueRecipeKey();
    QString Name(tr("New recipe") + ' ' + QString::number( static_cast<KEY_TO_Json>(NewKey) ));
    Recipe recipe;
    recipe.SetKey( NewKey );
    recipe.SetName( Name.toStdString() );
    _PC.RC.Add( recipe );
    _listOfRecipesId.insert( position, NewKey );
  }
  endInsertRows();
  return true;
}
  
bool RecipeListModel::removeRows(int position, int rows, const QModelIndex &index)
{
  Q_UNUSED(index);
  beginRemoveRows(QModelIndex(), position, position + rows - 1);
  for (int row = 0; row < rows; ++row) {
    _PC.DeleteRecipe(GetRecipeId(position));
    _listOfRecipesId.removeAt(position);
  }
  endRemoveRows();
  return true;
}

ResourceCalculator::KEY_RECIPE RecipeListModel::GetRecipeId(int Num) const
{
  return _listOfRecipesId[Num];
}

#pragma endregion MODEL

#pragma region DELEGATE

RecipesEditDelegate::RecipesEditDelegate(ResourceCalculator::ParamsCollection &PC, QObject *parent)
  : QStyledItemDelegate(parent), _PC(PC)
{
}

void RecipesEditDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
  QString ButtonCaption;
  switch (index.column()) {
  case 2: 
    ButtonCaption = tr("Result");
    break;
  case 3:
    ButtonCaption = tr( "Required" );
    break;
  case 4: 
    ButtonCaption = tr( "Factorys" ); 
    break;
  default:
    QStyledItemDelegate::paint(painter, option, index);
    return;
    break;
  }
  QStyleOptionButton button;
  button.rect = option.rect;
  button.text = ButtonCaption;
  button.state = QStyle::State_Enabled;
  QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter);
}

bool RecipesEditDelegate::editorEvent(QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index)
{
  if (event->type() == QEvent::MouseButtonPress){
    switch (index.column()) {
    case 2:
    case 3:
    {
      ItemSelectedDialogMode mode = index.column() == 2 ?
        ItemSelectedDialogMode::ForRecipeSelectItemsResult :
        ItemSelectedDialogMode::ForRecipeSelectItemsRequired;
      RecipeListModel &model_recipe = dynamic_cast<RecipeListModel &>(*model);
      const ResourceCalculator::KEY_RECIPE recipe_key = model_recipe.GetRecipeId(index.row());
      ItemSelectedDialog _ItemSelectedDialog(_PC, mode, recipe_key);
      if (_ItemSelectedDialog.exec()) {
        ResourceCalculator::Recipe *recipe = _PC.RC.GetRecipeForEdit(recipe_key);
        auto Result = _ItemSelectedDialog.GetResult();
        if (mode == ItemSelectedDialogMode::ForRecipeSelectItemsRequired) {
          recipe->SetRequired(Result);
        } else{
          recipe->SetResult(Result);
        }
      }
      return false;
      break;
    }
    case 4:
    default:
      bool ff = QStyledItemDelegate::editorEvent(event, model, option, index);
      return ff;
      break;
    }
  }
  return QStyledItemDelegate::editorEvent(event, model, option, index);
}

#pragma endregion DELEGATE

RecipesEditDialog::RecipesEditDialog(ResourceCalculator::ParamsCollection &PC, QWidget *parent)
  : QDialog(parent), _PC(PC)
{
  setMinimumSize(800, 600);
   
  QPushButton *okButton = new QPushButton( tr( "Ok" ) );
  QPushButton *addButton = new QPushButton( tr( "Add" ) );
  QPushButton *removelButton = new QPushButton( tr( "Remove" ) );
  QPushButton *cancelButton = new QPushButton( tr( "Cancel" ) );

  _tableView = new QTableView;
  _Model = new RecipeListModel( _PC, this );
  _tableView->setModel( _Model );
  _tableView->setItemDelegate( new RecipesEditDelegate( PC, this ) );
  _tableView->setSelectionMode( QTableView::SelectionMode::SingleSelection );
  _tableView->setSelectionBehavior( QTableView::SelectionBehavior::SelectRows );

  QHBoxLayout *buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget( okButton );
  buttonLayout->addWidget( addButton );
  buttonLayout->addWidget( removelButton );
  buttonLayout->addWidget( cancelButton );
  
  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget( _tableView );
  mainLayout->addLayout( buttonLayout );
  setLayout( mainLayout );

  connect( okButton, &QAbstractButton::clicked, this, &QDialog::accept );
  connect( cancelButton, &QAbstractButton::clicked, this, &QDialog::reject );
  connect( addButton, &QAbstractButton::clicked, this, &RecipesEditDialog::add_item );
  connect( removelButton, &QAbstractButton::clicked, this, &RecipesEditDialog::remove_item );

  setWindowTitle( tr( "Recipes Edit" ) );

}

void RecipesEditDialog::add_item()
{
  QModelIndexList Rows = _tableView->selectionModel()->selectedRows();
  if ( Rows.size() > 0 ) {
    _Model->insertRow( Rows[0].row() );
  } else {
    _Model->insertRow( 0 );
  }
}

void RecipesEditDialog::remove_item()
{
  QModelIndexList RowsSelected = _tableView->selectionModel()->selectedRows();
  if ( RowsSelected.size() > 0 ) {
    _Model->removeRow( RowsSelected[0].row() );
  }
}
