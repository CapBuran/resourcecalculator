#include "FactorysEditDialog.h"
#include "FactoryTypesEditDialog.h"
#include "IconSelectedDialog.h"
#include "Multilist.h"

using QT_KeysFactoryType = std::set<ResourceCalculator::KEY_TYPE_FACTORY>;
Q_DECLARE_METATYPE(QT_KeysFactoryType);

#pragma region MODELS

#pragma region FactoryTypesViewModel

FactoryTypesViewModel::FactoryTypesViewModel(const ResourceCalculator::FactoryTypeCollection& types, QObject* parent)
  : QAbstractTableModel(parent)
  , _types(types)
{
}

int FactoryTypesViewModel::rowCount(const QModelIndex & parent) const
{
  return _types.Size();
}

int FactoryTypesViewModel::columnCount(const QModelIndex & parent) const
{
  Q_UNUSED(parent);
  return 1;
}

QVariant FactoryTypesViewModel::data(const QModelIndex &index, int role) const
{
  using namespace ResourceCalculator;

  if (!index.isValid())
    return QVariant();

  if (index.row() >= _types.Size() || index.row() < 0)
    return QVariant();

  if (role == Qt::DisplayRole) {
    switch (index.column())
    {
    case 0:
      return QString::fromStdString(_types.GetFactoryType(_types.GetEnumKeyByKey(index.row())).Name);
      break;
    default:
      return QVariant();
      break;
    }
  }
  return QVariant();
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

std::set<ResourceCalculator::KEY_TYPE_FACTORY> FactoryTypesViewModel::GetFactoryTypes(QModelIndexList select) const
{
  std::set<ResourceCalculator::KEY_TYPE_FACTORY> RetVal;

  for (auto& row : select) {
    if (row.row() < _types.Size())
    {
      RetVal.insert(_types.GetEnumKeyByKey(row.row()));
    }
  }
  return RetVal;
}

QSet<int> FactoryTypesViewModel::GetSelectedRows(std::set<ResourceCalculator::KEY_TYPE_FACTORY> result) const
{
  QSet<int> retval;
  for(ResourceCalculator::KEY_TYPE_FACTORY key: result )
  {
    retval.insert(_types.GetKeyByEnumKey(key));
  }
  return retval;
}

#pragma endregion FactoryTypesViewModel

#pragma region FactorysEditDialogModel

FactorysEditDialogModel::FactorysEditDialogModel(ResourceCalculator::FactoryCollection& FC, QObject *parent) :
  QAbstractTableModel(parent), _FC(FC)
{
  Select();
}

int FactorysEditDialogModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return _FC_Edit.Size();
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

  if (index.row() >= _FC_Edit.Size() || index.row() < 0)
    return QVariant();

  const Factory &factory = _FC_Edit.GetFactory(_FC_Edit.GetEnumKeyByKey(index.row()));

  if (role == Qt::DisplayRole && 0 <= index.column() && index.column() <= 6) {
    switch (index.column()) {
    case 0:
      return QString::fromStdString(factory.GetIconKey());
      break;
    case 1:
      return QString::fromStdString(factory.GetName());
      break;
    case 2:
    {
      QVariant ret; ret.setValue<QT_KeysFactoryType>(factory.GetTypes());
      return ret;
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
      return tr("Factory types");
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
    KEY_FACTORY NewKey = _FC_Edit.GetUniqueEnumKey();
    QString Name(tr("New factory") + QString(' ') + QString::number(static_cast<KEY_TO_Json>(NewKey)));
    std::pair<KEY_FACTORY, Factory > ToADD;
    ToADD.first = NewKey;
    ToADD.second.SetName(Name.toStdString());
    ToADD.second.SetKey(NewKey);
    _FC_Edit.AddFactorys({ ToADD });
  }
  endInsertRows();
  return true;
}

bool FactorysEditDialogModel::removeRows(int position, int rows, const QModelIndex &index)
{
  Q_UNUSED(index);
  beginRemoveRows(QModelIndex(), position, position + rows - 1);

  std::set<ResourceCalculator::KEY_FACTORY> factoresKey;

  for (int row = 0; row < rows; ++row) {
    factoresKey.insert(_FC_Edit.GetEnumKeyByKey(row));
  }
  _FC_Edit.DeleteFactorys(factoresKey);

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
  _FC = _FC_Edit;
  Select();
}

void FactorysEditDialogModel::Select()
{
  _FC_Edit = _FC;
}

const ResourceCalculator::Factory& FactorysEditDialogModel::GetCurrentFactory(int row) const
{
  return _FC_Edit.GetFactory(_FC_Edit.GetEnumKeyByKey(row));
}

const ResourceCalculator::FactoryTypeCollection& FactorysEditDialogModel::GetTypesData() const
{
  return _FC_Edit.GetTypes();
}

std::set<ResourceCalculator::KEY_TYPE_FACTORY> FactorysEditDialogModel::GetFactoryTypes(QModelIndexList select) const
{
  return std::set<ResourceCalculator::KEY_TYPE_FACTORY>();
}

bool FactorysEditDialogModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
  if (index.isValid() && role == Qt::EditRole) {
    ResourceCalculator::Factory& factory = _FC_Edit.GetFactoryForEdit(_FC_Edit.GetEnumKeyByKey(index.row()));
    switch (index.column()) {
    case 0:
    {
      QString str = value.toString();
      std::string d = str.toStdString();
      if (str.size() > 0) {
        factory.SetIconKey(str.toStdString());
      }
      break;
    }
    case 1:
    {
      QString str = value.toString();
      if (str.size() > 0) {
        factory.SetName(str.toStdString());
      }
      break;
    }
    case 2:
    {
      const QT_KeysFactoryType& types = value.value<QT_KeysFactoryType>();
      factory.SetTypes(types);
    }
    break;
    case 3:
    {
      double d = value.toDouble();
      if (d >= 0.0) {
        factory.SetSpeed(d);
      }
    }
    break;
    case 4:
    {
      double d = value.toDouble();
      if (d >= 0.0) {
        factory.SetPower(d);
      }
    }
    break;
    case 5:
    {
      int d = value.toInt();
      if (d >= 0.0) {
        factory.SetCountSlotsForRecipes(d);
      }
    }
    break;
    case 6:
    {
      int d = value.toInt();
      if (d >= 0.0) {
        factory.SetCountSlotsForModules(d);
      }
    }
    break;
    default:
      return false;
      break;
    }
    emit(dataChanged(index, index));
    return true;
  }
  return false;
}

#pragma endregion FactorysEditDialogModel

#pragma endregion MODELS

#pragma region DELEGATES

#pragma region FactorysEditDialogDelegate

FactorysEditDialogDelegate::FactorysEditDialogDelegate(const ResourceCalculator::IconCollection& IC, QObject *parent)
  : QStyledItemDelegate(parent)
  , _IC(IC)
{
}

void FactorysEditDialogDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex& index) const
{
  //emit(ThePermissionToDeleteAnObjectHasBeenChanged(index, _ItemSelectionModel->selectedRows().size() > 0));
  switch (index.column()) {
  case 0:
  {
    using namespace ResourceCalculator;
    const ResourceCalculator::Icon &icon = _IC.GetIcon(index.data(Qt::DisplayRole).toString().toStdString());
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
          option.rect.left() + Sub1 + MinCoord, option.rect.bottom()
        );
      } else {
        rect.setCoords(
          option.rect.left(), option.rect.top() + Sub1,
          option.rect.right(), option.rect.top() + Sub1 + MinCoord
        );
      }
      painter->drawPixmap(rect, pixmap);
    }
    break;
  }
  case 2:
  {
    const QT_KeysFactoryType& types = index.data().value<QT_KeysFactoryType>();
    const FactorysEditDialogModel& model = dynamic_cast<const FactorysEditDialogModel&>(*index.model());
    const auto& FTC = model.GetTypesData();
    QString val = tr("Types not set!");
    if (types.empty()) val.clear();
    for (const auto& type: types)
    {
      if (!val.isEmpty()) val += tr(", ");
      val += QString::fromStdString(FTC.GetFactoryType(type).Name);
    }
    QStyleOptionComboBox comboBoxOption;
    comboBoxOption.rect = option.rect;
    comboBoxOption.currentText = val;
    comboBoxOption.state = QStyle::State_Enabled;
    QApplication::style()->drawComplexControl(QStyle::CC_ComboBox, &comboBoxOption, painter, 0);
    break;
  }
  default:
    QStyledItemDelegate::paint(painter, option, index);
    return;
    break;
  }
}

bool FactorysEditDialogDelegate::editorEvent(QEvent * event, QAbstractItemModel* model, const QStyleOptionViewItem & option, const QModelIndex& index)
{
  if (event->type() == QEvent::MouseButtonPress)
  {
    switch (index.column()) {
    case 0:
    {
      IconSelectedDialog _IconSelectedDialog(_IC);
      if (_IconSelectedDialog.exec()) {
        const ResourceCalculator::Icon* Icon = _IconSelectedDialog.GetResult();
        if (Icon != nullptr) {
          model->setData(index, QString::fromStdString(Icon->GetKeyPath()), Qt::EditRole);
        }
      }
      return false;
      break;
    }
    case 2:
    {
      const QT_KeysFactoryType& types = index.data().value<QT_KeysFactoryType>();
      FactorysEditDialogModel& modelFactory = dynamic_cast<FactorysEditDialogModel&>(*model);
      FactoryTypesViewModel modelFactoryTypesReadOnly(modelFactory.GetTypesData());
      FactorysTypesSelectedDialog dialog(modelFactoryTypesReadOnly, true, nullptr);
      if (dialog.exec()) {
        QVariant V; V.setValue<QT_KeysFactoryType>(dialog.GetResult());
        modelFactory.setData(index, V);
      }
    }
      break;
    default:
      return QStyledItemDelegate::editorEvent(event, model, option, index);
      break;
    }
  }
  return QStyledItemDelegate::editorEvent(event, model, option, index);
}

#pragma endregion FactorysEditDialogDelegate

#pragma endregion DELEGATES

FactorysEditDialog::FactorysEditDialog(
  ResourceCalculator::FactoryCollection& FC,
  ResourceCalculator::IconCollection& IC,
  QWidget *parent
)
  : QDialog(parent)
  , _FC(FC)
  , _IC(IC)
  , _Model(FC, parent)
  //, _modelForComboBox(_Model, parent)
  , _Delegate(IC, parent)
{
  setMinimumSize(1000, 600);

  QPushButton *okButton = new QPushButton(tr("OK"));
  QPushButton *addButton = new QPushButton(tr("Add"));
  _remoteButton = new QPushButton(tr("Remote"));
  QPushButton *cancelButton = new QPushButton(tr("Cancel"));
  QPushButton *typesFactorysButton = new QPushButton(tr("Factory types"));

  _tableView = new QTableView;
  _tableView->setSelectionMode(QTableView::SelectionMode::SingleSelection);
  _tableView->setSelectionBehavior(QTableView::SelectionBehavior::SelectRows);
  _tableView->setModel(&_Model);
  _tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  _tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
  _tableView->setColumnWidth(0, 60);

  _tableView->setItemDelegate(&_Delegate);

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
    &_Delegate, &FactorysEditDialogDelegate::ThePermissionToDeleteAnObjectHasBeenChanged,
    this, &FactorysEditDialog::ThePermissionToDeleteAnObjectHasBeenChanged
  );
  setWindowTitle(tr("Editor factorys"));
}

void FactorysEditDialog::Commit()
{
  _Model.Commit();
}

void FactorysEditDialog::remove_item()
{
  QModelIndexList RowsSelected = _tableView->selectionModel()->selectedRows();
  if (RowsSelected.size() > 0) {
    _Model.removeRow(RowsSelected[0].row());
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
    _Model.insertRow(Rows[0].row());
  } else {
    _Model.insertRow(0);
  }
}

void FactorysEditDialog::typesFactory()
{
  FactoryTypesEditDialog FTED(_FC, _IC, this);
  if (FTED.exec()) {
    FTED.Commit();
    //_modelForComboBox->Select();
  }
}

FactorysTypesSelectedDialog::FactorysTypesSelectedDialog(FactoryTypesViewModel& model, bool isMultiSelect, QWidget* parent)
  : QDialog(parent)
  , _Model(model)
  , _isMultiSelect(isMultiSelect)
{
  setMinimumSize(700, 600);

  QPushButton* okButton = new QPushButton(tr("OK"), this);
  QPushButton* cancelButton = new QPushButton(tr("Cancel"), this);
  _tableView = new QTableView(this);
  _tableView->setSelectionMode(_isMultiSelect ? QTableView::SelectionMode::MultiSelection : QTableView::SelectionMode::SingleSelection);
  _tableView->setSelectionBehavior(QTableView::SelectionBehavior::SelectRows);
  _tableView->setModel(&_Model);
  _tableView->sortByColumn(1, Qt::AscendingOrder);
//  _tableView->setItemDelegate(new ItemSelectedDelegate(PC, _Mode));
  _tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  //_tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
  //_tableView->setColumnWidth(0, 60);

  QHBoxLayout* buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(okButton);
  buttonLayout->addWidget(cancelButton);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(_tableView);
  mainLayout->addLayout(buttonLayout);
  setLayout(mainLayout);

  connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);
  connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);

  setWindowTitle(tr("Select an item"));
}

std::set<ResourceCalculator::KEY_TYPE_FACTORY> FactorysTypesSelectedDialog::GetResult() const
{
  return _Model.GetFactoryTypes(_tableView->selectionModel()->selectedRows());
}

void FactorysTypesSelectedDialog::SetResult(std::set<ResourceCalculator::KEY_TYPE_FACTORY> result)
{
  QSet<int> indexses = _Model.GetSelectedRows(result);
  for (int index: indexses)
  {
    _tableView->selectRow(index);
  }
}
