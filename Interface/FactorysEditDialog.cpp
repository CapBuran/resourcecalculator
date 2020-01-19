#include "FactorysEditDialog.h"
#include "FactoryTypesEditDialog.h"
#include "IconSelectedDialog.h"

#pragma region MODELS

#pragma region FactoryTypesViewModel

FactoryTypesViewModel::FactoryTypesViewModel(const ResourceCalculator::ParamsCollection &PC, QObject *parent) :
  _PC(PC)
{
  Select();
}

int FactoryTypesViewModel::rowCount(const QModelIndex & parent) const
{
  Q_UNUSED(parent);
  return _listOfItemsId.size();
}

int FactoryTypesViewModel::columnCount(const QModelIndex & parent) const
{
  Q_UNUSED(parent);
  return 1;
}

QVariant FactoryTypesViewModel::data(const QModelIndex & index, int role) const
{
  using namespace ResourceCalculator;

  if (!index.isValid())
    return QVariant();

  if (index.row() >= _listOfItemsId.size() || index.row() < 0)
    return QVariant();

  if (role == Qt::DisplayRole) {
    switch (index.column()) {
    case 0:
      return QString::fromStdString(_listOfItemsId[index.row()].second.Name);
      break;
    default:
      return QVariant();
      break;
    }
  }
  return QVariant();
}

int FactoryTypesViewModel::GetRowFromFactoryType(ResourceCalculator::KEY_TYPE_FACTORY KFT) const
{
  int Counter = 0;
  for (auto &it : _listOfItemsId) {
    if (it.first == KFT) {
      return Counter;
    }
    Counter++;
  }
  return -1;
}

void FactoryTypesViewModel::Select()
{
  using namespace ResourceCalculator;
  auto & TypesFactorys = _PC.FC.GetTypesFactorys();
  _listOfItemsId.clear();
  _listOfItemsId.reserve(static_cast< int >(TypesFactorys.size()));
  for (auto & TypeFactory : TypesFactorys) {
    _listOfItemsId.push_back(TypeFactory);
  }
}

std::pair<ResourceCalculator::KEY_TYPE_FACTORY, ResourceCalculator::FactoryType > FactoryTypesViewModel::GetDataRow(int Row) const
{
  return _listOfItemsId[Row];
}

QVariant FactoryTypesViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal) {
    switch (section) {
    case 0:
      return tr("Factory type name");
    default:
      return QVariant();
    }
  }
  return QVariant();
}

Qt::ItemFlags FactoryTypesViewModel::flags(const QModelIndex & index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

#pragma endregion FactoryTypesViewModel

#pragma region FactorysEditDialogModel

FactorysEditDialogModel::FactorysEditDialogModel(ResourceCalculator::ParamsCollection &PC, QObject *parent) :
  QAbstractTableModel(parent), _PC(PC)
{
  Select();
}

int FactorysEditDialogModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return _listOfItemsId.size();
}

int FactorysEditDialogModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return 7;
}

QVariant FactorysEditDialogModel::data(const QModelIndex &index, int role) const
{
  using namespace ResourceCalculator;

  if (!index.isValid())
    return QVariant();

  if (index.row() >= _listOfItemsId.size() || index.row() < 0)
    return QVariant();

  const Factory &factory = _listOfItemsId[index.row()].second;

  if (role == Qt::DisplayRole && 0 <= index.column() && index.column() <= 6) {
    switch (index.column()) {
    case 0:
      return QVariant(tr("There should be an icon"));
      break;
    case 1:
    {
      return QVariant(QString::fromStdString(factory.GetName()));
      break;
    }
    case 2:
    {
      return QVariant(QString::fromStdString(_PC.FC.GetFactoryType(factory.GetKey()).Name));
      break;
    }
    case 3:
      return QVariant::fromValue(factory.GetSpeed());
      break;
    case 4:
      return QVariant::fromValue(factory.GetPower());
      break;
    case 5:
      return QVariant::fromValue(factory.GetCountSlotsForRecipes());
      break;
    case 6:
      return QVariant::fromValue(factory.GetCountSlotsForModules());
      break;
    default:
      return QVariant();
      break;
    }
  }
  return QVariant();
}

QVariant FactorysEditDialogModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal) {
    switch (section) {
    case 0:
      return tr("Icon");
    case 1:
      return tr("Factory name");
    case 2:
      return tr("Factory type name");
    case 3:
      return tr("Speed");
    case 4:
      return tr("Drilling power");
    case 5:
      return tr("Slots for recipe");
    case 6:
      return tr("Slots for modules");
    default:
      return QVariant();
    }
  }
  return QVariant();
}

bool FactorysEditDialogModel::insertRows(int position, int rows, const QModelIndex &index)
{
  Q_ASSERT(position + rows - 1 >= 0);
  Q_UNUSED(index);
  beginInsertRows(QModelIndex(), position, position + rows - 1);
  for (int row = 0; row < rows; ++row) {
    using namespace ResourceCalculator;
    KEY_FACTORY NewKey = _PC.FC.GetUniqueFactoryKey();
    QString Name(tr("New factory") + QString(' ') + QString::number(static_cast<KEY_TO_Json>(NewKey)));
    std::pair<KEY_FACTORY, Factory > ToADD;
    ToADD.first = NewKey;
    ToADD.second.SetName(Name.toStdString());
    ToADD.second.SetKey(NewKey);
    _FactorysKeyToAdd[NewKey] = ToADD.second;
    _listOfItemsId.insert(position, ToADD);
  }
  endInsertRows();
  return true;
}

bool FactorysEditDialogModel::removeRows(int position, int rows, const QModelIndex &index)
{
  Q_UNUSED(index);
  beginRemoveRows(QModelIndex(), position, position + rows - 1);
  for (int row = 0; row < rows; ++row) {
    _FactorysKeyToDelete.insert(_listOfItemsId.at(position).first);
    _listOfItemsId.removeAt(position);
  }
  endRemoveRows();
  return true;
}

Qt::ItemFlags FactorysEditDialogModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  Qt::ItemFlags RetValue = Qt::ItemIsSelectable | Qt::ItemIsEnabled;

  switch (index.column()) {
  case 1:
  case 2:
  case 3:
  case 4:
  case 5:
  case 6:
    RetValue |= Qt::ItemIsEditable;
    break;
  default:
    break;
  }

  return RetValue;
}

void FactorysEditDialogModel::Commit()
{
  _PC.FC.DeleteFactorys(_FactorysKeyToDelete);
  _PC.FC.AddFactorys(_FactorysKeyToAdd);
  Select();
}

void FactorysEditDialogModel::Select()
{
  using namespace ResourceCalculator;
  auto & Factorys = _PC.FC.GetFactorys();
  _listOfItemsId.clear();
  _listOfItemsId.reserve(static_cast< int >(Factorys.size()));
  for (auto & factory : Factorys) {
    _listOfItemsId.push_back(factory);
  }
}

ResourceCalculator::KEY_FACTORY FactorysEditDialogModel::GetDataRow(int Row) const
{
  return _listOfItemsId[Row].first;
}

const ResourceCalculator::Factory & FactorysEditDialogModel::GetCurrentFactory(int Row) const
{
  ResourceCalculator::KEY_FACTORY kf = GetDataRow(Row);
  const auto & f = _FactorysKeyToAdd.find(kf);
  if (f != _FactorysKeyToAdd.end()) {
    return f->second;
  }
  return _PC.FC.GetFactory(kf);
}

bool FactorysEditDialogModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
  if (index.isValid() && role == Qt::EditRole) {
    switch (index.column()) {
    case 0:
    {
      QString str = value.toString();
      std::string d = str.toStdString();
      if (str.size() > 0) {
        _listOfItemsId[index.row()].second.SetIconPath(str.toStdString());
      }
      break;
    }
    case 1:
    {
      QString str = value.toString();
      if (str.size() > 0) {
        _listOfItemsId[index.row()].second.SetName(str.toStdString());
      }
      break;
    }
    case 2:
    {
      int d = value.toInt();
      if (d >= 0) {
        _listOfItemsId[index.row()].second.SetType(
          static_cast< ResourceCalculator::KEY_TYPE_FACTORY >(d)
        );
      }
    }
    break;
    case 3:
    {
      double d = value.toDouble();
      if (d >= 0.0) {
        _listOfItemsId[index.row()].second.SetSpeed(d);
      }
    }
    break;
    case 4:
    {
      double d = value.toDouble();
      if (d >= 0.0) {
        _listOfItemsId[index.row()].second.SetPower(d);
      }
    }
    break;
    case 5:
    {
      int d = value.toInt();
      if (d >= 0.0) {
        _listOfItemsId[index.row()].second.SetCountSlotsForRecipes(d);
      }
    }
    break;
    case 6:
    {
      int d = value.toInt();
      if (d >= 0.0) {
        _listOfItemsId[index.row()].second.SetCountSlotsForModules(d);
      }
    }
    break;
    default:
      return false;
      break;
    }
    _FactorysKeyToAdd[_listOfItemsId[index.row()].first] = _listOfItemsId[index.row()].second;
    emit(dataChanged(index, index));
    return true;
  }
  return false;
}

#pragma endregion FactorysEditDialogModel

#pragma endregion MODELS

#pragma region DELEGATES

#pragma region FactorysEditDialogDelegate

FactorysEditDialogDelegate::FactorysEditDialogDelegate(
  ResourceCalculator::ParamsCollection &PC,
  const FactoryTypesViewModel &modelForComboBox,
  const FactorysEditDialogModel &Model,
  const QItemSelectionModel *ItemSelectionModel,
  QObject *parent)
  :
  QStyledItemDelegate(parent), _PC(PC), _Model(Model), _modelForComboBox(modelForComboBox)
{
  _ItemSelectionModel = ItemSelectionModel;
}

void FactorysEditDialogDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
  emit(ThePermissionToDeleteAnObjectHasBeenChanged(index, _ItemSelectionModel->selectedRows().size() > 0));
  switch (index.column()) {
  case 0:
  {
    using namespace ResourceCalculator;
    const Factory &factory = _Model.GetCurrentFactory(index.row());
    const ResourceCalculator::Icon &icon = _PC.Icons.GetIcon(factory.GetIconPath());
    if (icon.GetRawData().size() > 0) {
      QPixmap pixmap;
      bool dd = pixmap.loadFromData((uchar*)&icon.GetRawData()[0], (uint)icon.GetRawData().size());
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
  case 2:
  {
    const ResourceCalculator::Factory &factory = _Model.GetCurrentFactory(index.row());
    int RowInModel = _modelForComboBox.GetRowFromFactoryType(factory.GetType());
    QString Text = QString::fromStdString(_modelForComboBox.GetDataRow(RowInModel).second.Name);
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

bool FactorysEditDialogDelegate::editorEvent(QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index)
{
  if (event->type() == QEvent::MouseButtonPress) {
    switch (index.column()) {
    case 0:
    {
      IconSelectedDialog _IconSelectedDialog(_PC);
      if (_IconSelectedDialog.exec()) {
        const ResourceCalculator::Icon * Icon = _IconSelectedDialog.GetResult();
        if (Icon != nullptr) {
          model->setData(index, QString::fromStdString(Icon->GetIconPath()), Qt::EditRole);
        }
      }
      return false;
      break;
    }
    default:
      return QStyledItemDelegate::editorEvent(event, model, option, index);
      break;
    }
  }
  return QStyledItemDelegate::editorEvent(event, model, option, index);
}

QWidget * FactorysEditDialogDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem &option, const QModelIndex & index) const
{
  if (index.column() == 2) {
    const ResourceCalculator::Factory &factory = _Model.GetCurrentFactory(index.row());
    QComboBox * combobox = new QComboBox(parent);
    FactoryTypesViewModel *modelForComboBox = new FactoryTypesViewModel(_PC);
    int SetCurentRow = modelForComboBox->GetRowFromFactoryType(factory.GetType());
    combobox->setModel(modelForComboBox);
    combobox->setCurrentIndex(SetCurentRow);
    //combobox->setGeometry(option.rect);//TODO: No work =(
    //combobox->showPopup();
    return combobox;
  }
  return QStyledItemDelegate::createEditor(parent, option, index);
}

void FactorysEditDialogDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
  if (index.column() == 2) {
    QComboBox *comboBox = static_cast< QComboBox* >(editor);
  } else {
    QStyledItemDelegate::setEditorData(editor, index);
  }
}

void FactorysEditDialogDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{
  using namespace ResourceCalculator;
  if (index.column() == 2) {
    QComboBox *comboBox = static_cast< QComboBox* >(editor);
    int IndexCB = comboBox->currentIndex();
    const std::pair<KEY_TYPE_FACTORY, FactoryType > &FC = _modelForComboBox.GetDataRow(IndexCB);
    model->setData(index, static_cast<int>(FC.first), Qt::EditRole);
  } else {
    QStyledItemDelegate::setModelData(editor, model, index);
  }
}

void FactorysEditDialogDelegate::updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
  if (index.column() == 2) {
    editor->setGeometry(option.rect);
  } else {
    QStyledItemDelegate::updateEditorGeometry(editor, option, index);
  }
}

#pragma endregion FactorysEditDialogDelegate

#pragma endregion DELEGATES

FactorysEditDialog::FactorysEditDialog(ResourceCalculator::ParamsCollection &PC, QWidget *parent)
  : QDialog(parent), _PC(PC)
{
  setMinimumSize(1000, 600);

  QPushButton *okButton = new QPushButton(tr("OK"));
  QPushButton *addButton = new QPushButton(tr("Add"));
  _remoteButton = new QPushButton(tr("Remote"));
  QPushButton *cancelButton = new QPushButton(tr("Cancel"));
  QPushButton *typesFactorysButton = new QPushButton(tr("Factory types"));

  _Model = new FactorysEditDialogModel(_PC, this);
  _modelForComboBox = new FactoryTypesViewModel(PC, this);

  _tableView = new QTableView;
  _tableView->setSelectionMode(QTableView::SelectionMode::SingleSelection);
  _tableView->setSelectionBehavior(QTableView::SelectionBehavior::SelectRows);
  _tableView->setModel(_Model);
  _tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  _tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
  _tableView->setColumnWidth(0, 60);

  FactorysEditDialogDelegate *Delegate = new FactorysEditDialogDelegate(
    PC, *_modelForComboBox, *_Model, _tableView->selectionModel(), nullptr);

  _tableView->setItemDelegate(Delegate);

  QHBoxLayout *buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(okButton);
  buttonLayout->addWidget(typesFactorysButton);
  buttonLayout->addWidget(addButton);
  buttonLayout->addWidget(_remoteButton);
  buttonLayout->addWidget(cancelButton);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(_tableView);
  mainLayout->addLayout(buttonLayout);
  setLayout(mainLayout);

  connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);
  connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);
  connect(addButton, &QAbstractButton::clicked, this, &FactorysEditDialog::add_item);
  connect(typesFactorysButton, &QAbstractButton::clicked, this, &FactorysEditDialog::typesFactory);
  connect(_remoteButton, &QAbstractButton::clicked, this, &FactorysEditDialog::remove_item);
  connect(
    Delegate, &FactorysEditDialogDelegate::ThePermissionToDeleteAnObjectHasBeenChanged,
    this, &FactorysEditDialog::ThePermissionToDeleteAnObjectHasBeenChanged
  );
  setWindowTitle(tr("Editor factorys"));
}

void FactorysEditDialog::Commit()
{
  _Model->Commit();
}

void FactorysEditDialog::remove_item()
{
  QModelIndexList RowsSelected = _tableView->selectionModel()->selectedRows();
  if (RowsSelected.size() > 0) {
    _Model->removeRow(RowsSelected[0].row());
  }
}

void FactorysEditDialog::ThePermissionToDeleteAnObjectHasBeenChanged(const QModelIndex & index, bool IsAllowed)
{
  _remoteButton->setEnabled(IsAllowed);
}

void FactorysEditDialog::add_item()
{
  QModelIndexList Rows = _tableView->selectionModel()->selectedRows();
  if (Rows.size() > 0) {
    _Model->insertRow(Rows[0].row());
  } else {
    _Model->insertRow(0);
  }
}

void FactorysEditDialog::typesFactory()
{
  FactoryTypesEditDialog FTED(_PC);
  if (FTED.exec()) {
    FTED.Commit();
    _modelForComboBox->Select();
  }
}
