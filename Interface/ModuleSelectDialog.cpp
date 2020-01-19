#include "ModuleSelectDialog.h"

#pragma region MODEL

ModuleSelectModel::ModuleSelectModel(
  const ResourceCalculator::ParamsCollection &PC,
  ResourceCalculator::KEY_MODULE OldResult,
  QObject *parent) :
  QAbstractTableModel(parent), _Result(OldResult)
{
  using namespace ResourceCalculator;
  const std::map<KEY_MODULE, Module> &Modules = PC.MC.GetModules();
  _listOfItemsId.reserve(static_cast<int>(Modules.size()));
  for (auto &it : Modules) {
    _listOfItemsId.push_back(it.second);
  }
}

int ModuleSelectModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return _listOfItemsId.size();
}

int ModuleSelectModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return 2;
}

QVariant ModuleSelectModel::data(const QModelIndex &index, int role) const
{
  using namespace ResourceCalculator;
  if (!index.isValid())
    return QVariant();
  if (index.row() >= _listOfItemsId.size())
    return QVariant();
  if (role == Qt::DisplayRole) {
    const Module &module = _listOfItemsId[index.row()];
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

bool ModuleSelectModel::setData(const QModelIndex &index, const QVariant & value, int role)
{
  if (!(index.isValid() && role == Qt::EditRole)) return false;
  _Result = _listOfItemsId[index.row()].GetKey();
  return true;
}

QVariant ModuleSelectModel::headerData(int section, Qt::Orientation orientation, int role) const
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

Qt::ItemFlags ModuleSelectModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

ResourceCalculator::KEY_MODULE ModuleSelectModel::GetResult() const
{
  return _Result;
}

#pragma endregion MODEL

#pragma region DELEGATE

ModuleSelectDelegate::ModuleSelectDelegate(const ResourceCalculator::ParamsCollection &PC, QObject *parent)
  : QStyledItemDelegate(parent), _PC(PC)
{
}

void ModuleSelectDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
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

bool ModuleSelectDelegate::editorEvent(QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index)
{
  if (event->type() == QEvent::MouseButtonPress) {
    model->setData(index, QVariant());
    return true;
  }
  return QStyledItemDelegate::editorEvent(event, model, option, index);
}

#pragma endregion DELEGATE

ModuleSelectDialog::ModuleSelectDialog(const ResourceCalculator::ParamsCollection & PC, ResourceCalculator::KEY_MODULE OldResult, QWidget * parent)
  : QDialog(parent), _Model(PC, OldResult)
{
  setMinimumSize(600, 600);

  QPushButton *okButton = new QPushButton(tr("OK"));
  QPushButton *cancelButton = new QPushButton(tr("Cancel"));
  _tableView = new QTableView;
  
  _tableView->setSelectionMode(QTableView::SelectionMode::SingleSelection);
  _tableView->setSelectionBehavior(QTableView::SelectionBehavior::SelectRows);
  _tableView->setModel(&_Model);
  _tableView->setItemDelegate(new ModuleSelectDelegate(PC));
  _tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  _tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
  _tableView->setColumnWidth(0, 60);

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

ResourceCalculator::KEY_MODULE ModuleSelectDialog::GetResult() const
{
  return _Model.GetResult();
}
