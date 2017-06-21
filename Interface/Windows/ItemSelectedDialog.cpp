#include "ItemSelectedDialog.h"

#include <QtWidgets>

#pragma region MODEL

ItemSelectedModel::ItemSelectedModel(ResourceCalculator::ParamsCollection &PC, QObject *parent):
  QAbstractTableModel(parent), _PC(PC)
{
  using namespace ResourceCalculator;
  const std::map<KEY_ITEM, Item> &DATA = _PC.IC.GetData();
  _listOfItemsId.reserve(static_cast<int>(DATA.size()));
  for (auto &it : DATA) {
    _listOfItemsId.push_back(it.first);
  }
}

int ItemSelectedModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return _listOfItemsId.size();
}

int ItemSelectedModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return 1;
}

QVariant ItemSelectedModel::data(const QModelIndex &index, int role) const
{
  using namespace ResourceCalculator;

  if (!index.isValid())
    return QVariant();

  if (index.row() >= _listOfItemsId.size() || index.row() < 0)
    return QVariant();

  if (role == Qt::DisplayRole) {

    const ResourceCalculator::KEY_ITEM KeyItem = GetItemId(index.row());

    Item *R = _PC.IC.GetItemForEdit(KeyItem);
    if (R == nullptr) {
      return QVariant();
    }

    QString retval;

    switch (index.column())
    {
    case 0:
      retval = QString::fromUtf8(R->GetName().c_str());
      return retval;
      break;

    default:
      return QVariant();
      break;
    }
  }

  return QVariant();
}

QVariant ItemSelectedModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal) {
    switch (section) {
    case 0:
      return tr("Item Name");
    default:
      return QVariant();
    }
  }
  return QVariant();
}
 
Qt::ItemFlags ItemSelectedModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool ItemSelectedModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if (index.isValid() && role == Qt::EditRole) {
    int row = index.row();
    
    using namespace ResourceCalculator;

    const ResourceCalculator::KEY_ITEM KeyItem = GetItemId(row);

    Item *R = _PC.IC.GetItemForEdit(KeyItem);
    if (R == nullptr) {
      return false;
    }
    
    std::string Name = R->GetName();

    switch (index.column())
    {
    case 0:
      Name = value.toString().toStdString();
      break;
    case 1:
      break;
    default:
      return false;
      break;
    }

    Item ToADD(Name, R->GetKey());
    _PC.IC.ADD(ToADD);

    _listOfItemsId.replace(row, R->GetKey());
    
    emit(dataChanged(index, index));

    return true;
  }

  return false;
}

bool ItemSelectedModel::insertRows(int position, int rows, const QModelIndex &index)
{
  Q_UNUSED(index);
  beginInsertRows(QModelIndex(), position, position + rows - 1);
  for (int row = 0; row < rows; ++row) {
    using namespace ResourceCalculator;
    KEY_ITEM NewKey = _PC.IC.GetUniqueRecipeKey();
    std::string Name("Новый предмет" + std::to_string(row));
    Item ToADD(Name, NewKey);
    _PC.IC.ADD(ToADD);
    _listOfItemsId.insert(position, NewKey);
  }
  endInsertRows();
  return true;
}
  
bool ItemSelectedModel::removeRows(int position, int rows, const QModelIndex &index)
{
  Q_UNUSED(index);
  beginRemoveRows(QModelIndex(), position, position + rows - 1);
  for (int row = 0; row < rows; ++row) {
    _PC.IC.Delete(GetItemId(position));
    _listOfItemsId.removeAt(position);
  }
  endRemoveRows();
  return true;
}

ResourceCalculator::KEY_ITEM ItemSelectedModel::GetItemId(int Num) const
{
  return _listOfItemsId[Num];
}

#pragma endregion MODEL

#pragma region DELEGATE

ItemSelectedDelegate::ItemSelectedDelegate(QObject *parent)
  : QStyledItemDelegate(parent)
{
}

QWidget *ItemSelectedDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
{
  QSpinBox *editor = new QSpinBox(parent);
  editor->setFrame(false);
  editor->setMinimum(0);
  editor->setMaximum(100);

  return editor;
}

void ItemSelectedDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  int value = index.model()->data(index, Qt::EditRole).toInt();

  QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
  spinBox->setValue(value);
}

void ItemSelectedDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
  QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
  spinBox->interpretText();
  int value = spinBox->value();

  model->setData(index, value, Qt::EditRole);
}

void ItemSelectedDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
  editor->setGeometry(option.rect);
}

#pragma endregion DELEGATE

ItemSelectedDialog::ItemSelectedDialog(ResourceCalculator::ParamsCollection &PC, QWidget *parent)
  : QDialog(parent), _PC(PC)
{
  setMinimumSize(400, 600);
   
  okButton = new QPushButton("OK");
  cancelButton = new QPushButton("Cancel");

  _table = new ItemSelectedModel(_PC, this);
  QTableView *tableView = new QTableView;
  tableView->setModel(_table);

  QHBoxLayout *buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(okButton);
  buttonLayout->addWidget(cancelButton);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(tableView);
  mainLayout->addLayout(buttonLayout);
  setLayout(mainLayout);

  connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);
  connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);

  setWindowTitle(tr("Выберете предметы"));

}

QList<ResourceCalculator::KEY_ITEM> ItemSelectedDialog::GetResult()
{
  return QList<ResourceCalculator::KEY_ITEM>();
}
