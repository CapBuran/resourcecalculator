#include "RecipesEditDialog.h"
#include "ItemSelectedDialog.h"
#include "IconSelectedDialog.h"

#pragma region MODEL

typedef std::set<ResourceCalculator::CountsItem> QT_CountsItem;    // typedef for your type
Q_DECLARE_METATYPE(QT_CountsItem);       // makes your type available to QMetaType system

RecipeListModel::RecipeListModel(ResourceCalculator::ParamsCollection &PC, QObject *parent) :
  QAbstractTableModel(parent), _PC(PC)
{
  Select();
}

int RecipeListModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return _listOfRecipesId.size();
}

int RecipeListModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return 6;
}

QVariant RecipeListModel::data(const QModelIndex &index, int role) const
{
  using namespace ResourceCalculator;
  if (!index.isValid())
    return QVariant();
  if (index.row() >= _listOfRecipesId.size() || index.row() < 0)
    return QVariant();
  if (role == Qt::DisplayRole) {
    const Recipe &R = _listOfRecipesId[index.row()].second;
    switch (index.column()) {
    case 0://Icon
      return QString::fromStdString(R.GetIconPath());
      break;
    case 1://Recipe name
      return QString::fromStdString(R.GetName());
      break;
    case 2://Recipe time
      return QString::number(R.GetTime());
      break;
    case 3://Result of recipe
    {
      const QT_CountsItem &CI = R.GetResult();
      QVariant ret; ret.setValue<QT_CountsItem>(CI);
      return ret;
      break;
    }
    case 4://Ingredients of the recipe
    {
      const QT_CountsItem &CI = R.GetRequired();
      QVariant ret; ret.setValue<QT_CountsItem>(CI);
      return ret;
      break;
    }
    case 5://Allowed factories
    {
      return QVariant(static_cast<int>(R.GetTypeFactory()));
      break;
    }
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
      return tr("Icon");
    case 1:
      return tr("Recipe name");
    case 2:
      return tr("Recipe time");
    case 3:
      return tr("Result of recipe");
    case 4:
      return tr("Ingredients of the recipe");
    case 5:
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
    Recipe &R = _listOfRecipesId[index.row()].second;

    switch (index.column()) {
    case 0://Icon
    {
      R.SetIconPath(value.toString().toStdString());
      break;
    }
    case 1://Recipe name
    {
      QString Name = value.toString();
      if (Name.length() > 0) {
        R.SetName(Name.toStdString());
      }
      break;
    }
    case 2://Recipe time
    {
      double Value = value.toDouble();
      if (Value >= 0) {
        R.SetTime(Value);
      }
      break;
    }
    case 3://Result of recipe
    {
      const QT_CountsItem &CI = value.value<QT_CountsItem>();
      R.SetResult(CI);
      break;
    }
    case 4://Ingredients of the recipe
    {
      const QT_CountsItem &CI = value.value<QT_CountsItem>();
      R.SetRequired(CI);
      break;
    }
    case 5://Allowed factories
    {
      int Value = value.toInt();
      if (Value >= 0) {
        R.SetTypeFactory(static_cast< KEY_TYPE_FACTORY >(Value));
      }
      break;
    }
    default:
      return false;
      break;
    }
    _RecipesToAdd[R.GetKey()] = R;
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
    const KEY_RECIPE NewKey = _PC.RC.GetUniqueRecipeKey();
    QString Name(tr("New recipe") + ' ' + QString::number(static_cast<KEY_TO_Json>(NewKey)));
    std::pair<KEY_RECIPE, Recipe> ToADD;
    ToADD.first = NewKey;
    ToADD.second.SetKey(NewKey);
    ToADD.second.SetName(Name.toStdString());
    ToADD.second.SetTypeFactory(KEY_TYPE_FACTORY::Unknown);
    _listOfRecipesId.insert(position, ToADD);
    _RecipesToAdd.insert(ToADD);
  }
  endInsertRows();
  return true;
}

bool RecipeListModel::removeRows(int position, int rows, const QModelIndex &index)
{
  Q_UNUSED(index);
  beginRemoveRows(QModelIndex(), position, position + rows - 1);
  for (int row = 0; row < rows; ++row) {
    _RecipesToDelete.insert(_listOfRecipesId[row].first);
    _listOfRecipesId.removeAt(position);
  }
  endRemoveRows();
  return true;
}

ResourceCalculator::KEY_RECIPE RecipeListModel::GetRecipeId(int Num) const
{
  return _listOfRecipesId[Num].first;
}

void RecipeListModel::Commit()
{
  using namespace ResourceCalculator;
  for (auto it : _RecipesToAdd) {
    _RecipesToDelete.erase(it.first);
  }
  _PC.RC.Delete(_RecipesToDelete);
  for (auto it : _RecipesToAdd) {
    _PC.RC.Add(it.second);
  }
  _RecipesToDelete.clear();
  _RecipesToAdd.clear();
  _listOfRecipesId.clear();
  Select();
}

void RecipeListModel::Select()
{
  using namespace ResourceCalculator;
  _RecipesToDelete.clear();
  _RecipesToAdd.clear();
  const std::map<KEY_RECIPE, Recipe> &Recipes = _PC.RC.GetData();
  _listOfRecipesId.reserve(static_cast< int >(Recipes.size()));
  for (auto & recipe : Recipes) {
    _listOfRecipesId.push_back(recipe);
  }
}

#pragma endregion MODEL

#pragma region DELEGATE

RecipesEditDelegate::RecipesEditDelegate(ResourceCalculator::ParamsCollection &PC, QObject *parent)
  : QStyledItemDelegate(parent), _PC(PC)
{
}

void RecipesEditDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
  switch (index.column()) {
  case 0:
  {
    QString IcopPath = index.data().toString();
    const ResourceCalculator::Icon &icon = _PC.Icons.GetIcon(IcopPath.toStdString());
    if (icon.GetRawData().size() > 0) {
      QPixmap pixmap;
      pixmap.loadFromData((uchar*)&icon.GetRawData()[0], (uint)icon.GetRawData().size());
      const int MinCoord = std::min(option.rect.width(), option.rect.height());
      const int MaxCoord = std::max(option.rect.width(), option.rect.height());
      const int Sub1 = (MaxCoord - MinCoord) / 2;
      QRect rect;
      if (MaxCoord == option.rect.width()) {
        rect.setCoords(
          option.rect.left() + Sub1, option.rect.top(),
          option.rect.left() + Sub1 + MinCoord, option.rect.bottom());
      } else {
        rect.setCoords(
          option.rect.left(), option.rect.top() + Sub1,
          option.rect.right(), option.rect.top() + Sub1 + MinCoord);
      }
      painter->drawPixmap(rect, pixmap);
    }
    break;
  }
  case 3:
  case 4:
  {
    QStyleOptionButton button;
    button.rect = option.rect;
    button.text = index.column() == 3 ? tr("Result") : tr("Ingredients");
    button.state = QStyle::State_Enabled;
    QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter);
    break;
  }
  case 5: {
    using namespace ResourceCalculator;
    int key = index.data().toInt();
    KEY_TYPE_FACTORY KFT = static_cast<KEY_TYPE_FACTORY>(key);
    const std::map<KEY_TYPE_FACTORY, FactoryType> &MAP = _PC.FC.GetTypesFactorys();
    const auto &FT = MAP.find(KFT);
    Q_ASSERT(FT != MAP.end());
    QString Text = QString::fromStdString(FT->second.Name);
    QStyleOptionComboBox comboBoxOption;
    comboBoxOption.rect = option.rect;
    comboBoxOption.currentText = Text;
    comboBoxOption.state = QStyle::State_Enabled;
    QApplication::style()->drawComplexControl(QStyle::CC_ComboBox, &comboBoxOption, painter, 0);
    QApplication::style()->drawControl(QStyle::CE_ComboBoxLabel, &comboBoxOption, painter, 0);
    break;
  }
  default:
    QStyledItemDelegate::paint(painter, option, index);
    return;
    break;
  }
}

QWidget *RecipesEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  using namespace ResourceCalculator;
  if (index.column() == 5) {
    const std::map<KEY_TYPE_FACTORY, FactoryType> &MAP = _PC.FC.GetTypesFactorys();
    QComboBox *combobox = new QComboBox(parent);
    KEY_TYPE_FACTORY KFT = static_cast<KEY_TYPE_FACTORY>(index.data().toInt());
    int Pos = -1;
    int Counter = 0;
    for (auto &FI : MAP) {
      combobox->addItem(QString::fromStdString(FI.second.Name));
      if (FI.first == KFT) {
        Pos = Counter;
      }
      Counter++;
    }
    combobox->setGeometry(option.rect);
    combobox->setCurrentIndex(Pos);
    combobox->setFrame(true);
    return combobox;
  }
  return QStyledItemDelegate::createEditor(parent, option, index);
}

void RecipesEditDelegate::updateEditorGeometry(QWidget *editor,
                                               const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
  editor->setGeometry(option.rect);
}

void RecipesEditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  bool IsOk = false;
  if (index.column() == 5) {
    QComboBox *combobox = dynamic_cast< QComboBox * >(editor);
    combobox->setCurrentText(index.model()->data(index, Qt::EditRole).toString());
    IsOk = true;
  }
  if (!IsOk) {
    QStyledItemDelegate::setEditorData(editor, index);
  }
}

bool RecipesEditDelegate::editorEvent(QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index)
{
  if (event->type() == QEvent::MouseButtonPress) {
    switch (index.column()) {
    case 0:
    {
      IconSelectedDialog _IconSelectedDialog(_PC);
      if (_IconSelectedDialog.exec()) {
        const ResourceCalculator::Icon * Icon = _IconSelectedDialog.GetResult();
        if (Icon != nullptr) {
          model->setData(index, QString::fromStdString(Icon->GetIconPath()));
        }
      }
      return false;
      break;
    }
    case 3:
    case 4:
    {
      ItemSelectedDialogMode mode = index.column() == 3 ?
        ItemSelectedDialogMode::ForRecipeSelectItemsResult :
        ItemSelectedDialogMode::ForRecipeSelectItemsRequired;
      const QT_CountsItem &CI = model->data(index).value<QT_CountsItem>();
      ItemSelectedDialog _ItemSelectedDialog(_PC, mode, CI);
      if (_ItemSelectedDialog.exec()) {
        const std::set<ResourceCalculator::CountsItem> &Result = _ItemSelectedDialog.GetResult();
        QVariant V; V.setValue<QT_CountsItem>(Result);
        model->setData(index, V);
      }
      return false;
      break;
    }
    default:
      bool ff = QStyledItemDelegate::editorEvent(event, model, option, index);
      return ff;
      break;
    }
  }
  return QStyledItemDelegate::editorEvent(event, model, option, index);
}

void RecipesEditDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{
  bool IsOk = false;
  if (index.column() == 5) {
    QComboBox *combobox = dynamic_cast< QComboBox * >(editor);
    int value = combobox->currentIndex();
    model->setData(index, value, Qt::EditRole);
    IsOk = true;
  }
  if (!IsOk) {
    QStyledItemDelegate::setModelData(editor, model, index);
  }
}

#pragma endregion DELEGATE

RecipesEditDialog::RecipesEditDialog(ResourceCalculator::ParamsCollection &PC, QWidget *parent)
  : QDialog(parent), _PC(PC), _Model(PC, parent)
{
  setMinimumSize(800, 600);

  QPushButton *okButton = new QPushButton(tr("Ok"));
  QPushButton *addButton = new QPushButton(tr("Add"));
  _removeButton = new QPushButton(tr("Remove"));
  QPushButton *cancelButton = new QPushButton(tr("Cancel"));

  _tableView = new QTableView();
  _tableView->setModel(&_Model);
  _tableView->setItemDelegate(new RecipesEditDelegate(PC));
  _tableView->setSelectionMode(QTableView::SelectionMode::SingleSelection);
  _tableView->setSelectionBehavior(QTableView::SelectionBehavior::SelectRows);
  _tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

  QHBoxLayout *buttonLayout = new QHBoxLayout();
  buttonLayout->addWidget(okButton);
  buttonLayout->addWidget(addButton);
  buttonLayout->addWidget(_removeButton);
  buttonLayout->addWidget(cancelButton);

  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addWidget(_tableView);
  mainLayout->addLayout(buttonLayout);
  setLayout(mainLayout);

  connect(okButton, SIGNAL(clicked()), SLOT(PushButtonOk()));
  //connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);
  connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);
  connect(addButton, &QAbstractButton::clicked, this, &RecipesEditDialog::add_item);
  connect(_removeButton, &QAbstractButton::clicked, this, &RecipesEditDialog::remove_item);
  //connect(Delegate, &RecipesEditDelegate::editorEventDelegate, this, &RecipesEditDelegate::editorEventDelegate);

  setWindowTitle(tr("Recipes Edit"));

}

void RecipesEditDialog::add_item()
{
  QModelIndexList Rows = _tableView->selectionModel()->selectedRows();
  if (Rows.size() > 0) {
    _Model.insertRow(Rows[0].row());
  } else {
    _Model.insertRow(0);
  }
}

void RecipesEditDialog::remove_item()
{
  QModelIndexList RowsSelected = _tableView->selectionModel()->selectedRows();
  if (RowsSelected.size() > 0) {
    _Model.removeRow(RowsSelected[0].row());
  }
}

void RecipesEditDialog::editorEventDelegate(const QModelIndex & index)
{
  QModelIndexList Rows = _tableView->selectionModel()->selectedRows();
  _removeButton->setEnabled(Rows.size() > 0);
}

void RecipesEditDialog::PushButtonOk()
{
  _Model.Commit();
  emit(&QDialog::accept);
  emit(close());
}
