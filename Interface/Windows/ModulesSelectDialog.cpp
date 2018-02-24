#include "ModulesSelectDialog.h"
#include "ModuleSelectDialog.h"

#pragma region MODEL

ModulesSelectModel::ModulesSelectModel(
  const ResourceCalculator::ParamsCollection &PC,
  int CountSlotsForModules,
  QObject *parent) :
  QAbstractTableModel(parent), _PC(PC)
{
  _Result.SetCountModules(CountSlotsForModules);
  for (int i = 0; i < CountSlotsForModules; i++) {
    _Result.SetModule(i, ResourceCalculator::KEY_MODULE::ID_CleanSlot);
  }
}

ModulesSelectModel::ModulesSelectModel(const ResourceCalculator::ParamsCollection & PC, const ResourceCalculator::FactoryModules &OldResult, QObject * parent) :
  QAbstractTableModel(parent), _PC(PC)
{
  _Result = OldResult;
}

int ModulesSelectModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return static_cast<int>(_Result.GetModules().size());
}

int ModulesSelectModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return 2;
}

QVariant ModulesSelectModel::data(const QModelIndex &index, int role) const
{
  using namespace ResourceCalculator;
  if (!index.isValid())
    return QVariant();
  if (index.row() >= _Result.GetModules().size())
    return QVariant();
  if (role == Qt::DisplayRole) {
    KEY_MODULE key = _Result.GetModules()[index.row()];
    const Module &module = _PC.MC.GetModule(key);
    switch (index.column()) {
    case 0:
      return QString(module.GetIconPath().c_str());
      break;
    case 1:
      if (module.GetKey() == KEY_MODULE::ID_CleanSlot) {
        return QString(tr("Empty slot"));
      } else {
        return QString(module.GetName().c_str());
      }
      break;
    default:
      return QVariant();
      break;
    }
  }
  return QVariant();
}

QVariant ModulesSelectModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole)
    return QVariant();
  if (orientation == Qt::Horizontal) {
    switch (section) {
    case 0:
      return tr("Icon");
    case 1:
      return tr("Module name");
    default:
      return QVariant();
    }
  }
  return QVariant();
}

Qt::ItemFlags ModulesSelectModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  return Qt::ItemIsEnabled;
}

bool ModulesSelectModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
  if (index.isValid() && role == Qt::EditRole) {
    switch (index.column()) {
    case 1:
    {
      ResourceCalculator::KEY_MODULE key = static_cast<ResourceCalculator::KEY_MODULE>(value.toInt());
      _Result.SetModule(index.row(), key);
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

const ResourceCalculator::FactoryModules & ModulesSelectModel::GetResult() const
{
  return _Result;
}

#pragma endregion MODEL

#pragma region DELEGATE

ModulesSelectDelegate::ModulesSelectDelegate(const ResourceCalculator::ParamsCollection &PC, QObject *parent)
  : QStyledItemDelegate(parent), _PC(PC)
{
}

void ModulesSelectDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
  switch (index.column()) {
  case 0:
  {
    std::string IconPath = index.data().toString().toStdString();
    const ResourceCalculator::Icon &icon = _PC.Icons.GetIcon(IconPath);
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
  default:
    QStyledItemDelegate::paint(painter, option, index);
    return;
    break;
  }
}

bool ModulesSelectDelegate::editorEvent(QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index)
{
  if (event->type() == QEvent::MouseButtonPress) {
    switch (index.column()) {
    case 1:
    {
      ModuleSelectDialog MSD(_PC, ResourceCalculator::KEY_MODULE::ID_CleanSlot);
      if (MSD.exec()) {
        model->setData(index, static_cast<int>(MSD.GetResult()));
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

#pragma endregion DELEGATE

ModulesSelectDialog::ModulesSelectDialog(
  const ResourceCalculator::ParamsCollection & PC,
  int CountSlotsForModules,
  QWidget * parent
)
  : QDialog(parent), _PC(PC), _Model(PC, CountSlotsForModules)
{
  setMinimumSize(400, 600);

  QPushButton *okButton = new QPushButton(tr("OK"));
  QPushButton *cancelButton = new QPushButton(tr("Cancel"));
  _tableView = new QTableView;
  _tableView->setSelectionMode(QTableView::SelectionMode::NoSelection);
  _tableView->setModel(&_Model);
  _tableView->setItemDelegate(new ModulesSelectDelegate(PC));

  QHBoxLayout *buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(okButton);
  buttonLayout->addWidget(cancelButton);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(_tableView);
  mainLayout->addLayout(buttonLayout);
  setLayout(mainLayout);

  connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);
  connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);

  setWindowTitle(tr("Select modules for factory"));
}

ModulesSelectDialog::ModulesSelectDialog(const ResourceCalculator::ParamsCollection & PC, const ResourceCalculator::FactoryModules & OldResult, QWidget * parent)
  : QDialog(parent), _PC(PC), _Model(PC, OldResult)
{
  setMinimumSize(400, 600);

  QPushButton *okButton = new QPushButton(tr("OK"));
  QPushButton *cancelButton = new QPushButton(tr("Cancel"));
  _tableView = new QTableView;
  _tableView->setSelectionMode(QTableView::SelectionMode::NoSelection);
  _tableView->setModel(&_Model);
  _tableView->setItemDelegate(new ModulesSelectDelegate(PC));
  _tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

  QHBoxLayout *buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(okButton);
  buttonLayout->addWidget(cancelButton);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(_tableView);
  mainLayout->addLayout(buttonLayout);
  setLayout(mainLayout);

  connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);
  connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);

  setWindowTitle(tr("Select modules for factory"));
}

const ResourceCalculator::FactoryModules &ModulesSelectDialog::GetResult() const
{
  return _Model.GetResult();
}
