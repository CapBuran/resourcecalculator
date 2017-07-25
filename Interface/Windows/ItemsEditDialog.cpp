#include <QtWidgets>

#include <QFile>
#include <QBytearray>


#include "ItemsEditDialog.h"
#include "IconSelectedDialog.h"


#pragma region MODEL

ItemsEditModel::ItemsEditModel(ResourceCalculator::ParamsCollection &PC, QObject *parent):
  QAbstractTableModel(parent), _PC(PC)
{
  using namespace ResourceCalculator;
  const std::map<KEY_ITEM, Item> &DATA = _PC.IC.GetData();
  _listOfItemsId.reserve(static_cast<int>(DATA.size()));
  for (auto &it : DATA) {
    _listOfItemsId.push_back(it.first);
  }
}

int ItemsEditModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return _listOfItemsId.size();
}

int ItemsEditModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return 2;
}

QVariant ItemsEditModel::data(const QModelIndex &index, int role) const
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

QVariant ItemsEditModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal) {
    switch (section) {
    case 0:
      return tr("Item Name");
    case 1:
      return tr("Icon");
    default:
      return QVariant();
    }
  }
  return QVariant();
}
 
Qt::ItemFlags ItemsEditModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool ItemsEditModel::setData(const QModelIndex &index, const QVariant &value, int role)
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


    //Item ToADD(Name, R->GetKey());
    Item ToADD;
    ToADD.SetName(Name); ToADD.SetKey(R->GetKey());

    _PC.IC.ADD(ToADD);

    _listOfItemsId.replace(row, R->GetKey());
    
    emit(dataChanged(index, index));

    return true;
  }

  return false;
}

bool ItemsEditModel::insertRows(int position, int rows, const QModelIndex &index)
{
  Q_UNUSED(index);
  beginInsertRows(QModelIndex(), position, position + rows - 1);
  for (int row = 0; row < rows; ++row) {
    using namespace ResourceCalculator;
    KEY_ITEM NewKey = _PC.IC.GetUniqueRecipeKey();
    std::string Name("Новый предмет" + std::to_string(row));
    //Item ToADD(Name, NewKey);
    Item ToADD;
    ToADD.SetName(Name); ToADD.SetKey(NewKey);
    _PC.IC.ADD(ToADD);
    _listOfItemsId.insert(position, NewKey);
  }
  endInsertRows();
  return true;
}
  
bool ItemsEditModel::removeRows(int position, int rows, const QModelIndex &index)
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

ResourceCalculator::KEY_ITEM ItemsEditModel::GetItemId(int Num) const
{
  return _listOfItemsId[Num];
}

#pragma endregion MODEL

#pragma region DELEGATE

ItemEditDelegate::ItemEditDelegate(ResourceCalculator::ParamsCollection &PC, const ItemsEditModel &Model, QObject *parent)
  : QStyledItemDelegate(parent), _PC(PC), _Model(Model)
{
}

void ItemEditDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
  switch (index.column()) {
  case 1: {
    const std::map<std::string, ResourceCalculator::Icon>& ff = _PC.Icons.GetAllIcon();

    if (ff.size() > 0) {
    
      const ResourceCalculator::Icon & icon = ff.begin()->second;

      QPixmap pixmap;
      pixmap.loadFromData((uchar*)&icon.GetRawData()[0], (uint)icon.GetRawData().size());
      const int MinCoord = std::min(option.rect.width(), option.rect.height());
      const int MaxCoord = std::max(option.rect.width(), option.rect.height());
      const int Sub1 = (MaxCoord - MinCoord) / 2;
      QRect rect;
      if (MaxCoord == option.rect.width()) {
        rect.setCoords(
          option.rect.left() + Sub1,            option.rect.top(),
          option.rect.left() + Sub1 + MinCoord, option.rect.bottom());
      } else {
        rect.setCoords(
          option.rect.left(),  option.rect.top() + Sub1,
          option.rect.right(), option.rect.top() + Sub1 + MinCoord);
      }
      painter->drawPixmap(rect, pixmap);
    }

    
  
    //QApplication::style()->drawControl(QStyle::SP_DirIcon, &button, painter);


    //QString ButtonCaption;
    //ButtonCaption = "Иконка";
    //QStyleOptionButton button;
    //button.rect = option.rect;
    //button.text = ButtonCaption;
    //..painter->drawImage()
    //button.state = QStyle::State_Enabled;
    //QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter);



    break;
  }
  default:
    QStyledItemDelegate::paint(painter, option, index);
    return;
    break;
  }
}

bool ItemEditDelegate::editorEvent(QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index)
{
  if (event->type() == QEvent::MouseButtonPress) {
    switch (index.column()) {
    case 1:
    {
      IconSelectedDialog _ItemSelectedDialog(_PC);
      if (_ItemSelectedDialog.exec()) {
        const ResourceCalculator::Icon * Icon = _ItemSelectedDialog.GetResult();
        if (Icon != nullptr) {
          ResourceCalculator::Item *item = _PC.IC.GetItemForEdit(dynamic_cast<ItemsEditModel *>(model)->GetItemId(index.row()));
          item->SetIconPath(Icon->ShortName);
        }
      }
      return false;
      break;
    }
    default:
      bool ff = QStyledItemDelegate::editorEvent(event, model, option, index);;
      return ff;
      break;
    }
  }
  return QStyledItemDelegate::editorEvent(event, model, option, index);
}


#pragma endregion DELEGATE

ItemsEditDialog::ItemsEditDialog(ResourceCalculator::ParamsCollection &PC, QWidget *parent)
  : QDialog(parent), _PC(PC)
{
  setMinimumSize(800, 600);
  QPushButton *okButton     = new QPushButton("OK");;
  QPushButton *cancelButton = new QPushButton("Cancel");

  ItemsEditModel *table = new ItemsEditModel(_PC, this);
  QTableView *tableView = new QTableView;
  tableView->setModel(table);
  tableView->setItemDelegate(new ItemEditDelegate(PC, *table, this));

  QHBoxLayout *buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(okButton);
  buttonLayout->addWidget(cancelButton);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(tableView);
  mainLayout->addLayout(buttonLayout);
  setLayout(mainLayout);

  connect(okButton,     &QAbstractButton::clicked, this, &QDialog::accept);
  connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);

  setWindowTitle(tr("Item Edit"));

}

