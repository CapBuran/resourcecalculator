#include <ProductionChainTreeModel.h>
#include <ProductionChainTree.h>

ProductionChainTreeItem::ProductionChainTreeItem(const ResourceCalculator::FullItemTree& tree, ResourceCalculator::KEY_ITEM key)
  : _m_parentItem(nullptr)
  , Tree(tree)
  , Key(ResourceCalculator::NoFoundKey)
  , Type(ResourceCalculator::TreeNodeType::RECIPE)
{
  _m_childItems.append(new ProductionChainTreeItem(Tree, key, this));
}

ProductionChainTreeItem::ProductionChainTreeItem(const ResourceCalculator::FullItemTree& tree, ResourceCalculator::KEY_ITEM key, ProductionChainTreeItem* parentItem)
  : _m_parentItem(parentItem)
  , Tree(tree)
  , Key(static_cast<ResourceCalculator::TYPE_KEY>(key))
  , Type(ResourceCalculator::TreeNodeType::ITEM)
{}

ProductionChainTreeItem::ProductionChainTreeItem(const ResourceCalculator::FullItemTree& tree, ResourceCalculator::KEY_RECIPE key, ProductionChainTreeItem* parentItem)
  : _m_parentItem(parentItem)
  , Tree(tree)
  , Key(static_cast<ResourceCalculator::TYPE_KEY>(key))
  , Type(ResourceCalculator::TreeNodeType::RECIPE)
{}

ProductionChainTreeItem::~ProductionChainTreeItem()
{ 
  qDeleteAll(_m_childItems);
}

ProductionChainTreeItem* ProductionChainTreeItem::parentItem()
{
  return const_cast<ProductionChainTreeItem*>(_m_parentItem);
}

ProductionChainTreeItem* ProductionChainTreeItem::child(int row)
{
  return _m_childItems.value(row);
}

int ProductionChainTreeItem::childCount()
{
  Init(_m_childItems);
  return _m_childItems.count();
}

int ProductionChainTreeItem::columnCount()
{
  return 2;
}

int ProductionChainTreeItem::childNumber()
{
  if (_m_parentItem)
    return _m_parentItem->_m_childItems.indexOf(const_cast<ProductionChainTreeItem*>(this));
  return 0;
}

QVariant ProductionChainTreeItem::data(int column)
{
  switch (Type)
  {
  case ResourceCalculator::TreeNodeType::ITEM:
    {
      const ResourceCalculator::Item& item = Tree.GetPC().IC[static_cast<ResourceCalculator::KEY_ITEM>(Key)];
      switch (column)
      {
      case 0:
        return QString::fromStdString(item.GetIconKey());
      case 1:
        return tr("Item: ") + QString::fromStdString(item.GetName());
      default:
        break;
      }
    }
  break;
  case ResourceCalculator::TreeNodeType::RECIPE:
    {
      const ResourceCalculator::Recipe& recipe = Tree.GetPC().RC[static_cast<ResourceCalculator::KEY_RECIPE>(Key)];
      switch (column)
      {
      case 0:
        return QString::fromStdString(recipe.GetIconKey());
      case 1:
        return tr("Recipe: ") + QString::fromStdString(recipe.GetName());
      default:
        break;
      }
    }
  }
  return QVariant();
}

void ProductionChainTreeItem::Init(QList<ProductionChainTreeItem*>& list)
{
  using namespace ResourceCalculator;

  if (_IsInit) return;
  _IsInit = true;

  switch (Type)
  {
  case TreeNodeType::ITEM:
  {
    const ItemNode& root = Tree[static_cast<ResourceCalculator::KEY_ITEM>(Key)];
    for (auto Children : root.Childrens)
    {
      list.append(new ProductionChainTreeItem(Tree, Children, this));
    }
  }
    break;
  case ResourceCalculator::TreeNodeType::RECIPE:
  {
    const RecipeNode& root = Tree[static_cast<ResourceCalculator::KEY_RECIPE>(Key)];
    if (root)
    {
      for (auto Children: root.Childrens)
      {
        list.append(new ProductionChainTreeItem(Tree, Children, this));
      }
    }
  }
    break;
  default:
    break;
  }
}

ProductionChainTreeModel::ProductionChainTreeModel(const ResourceCalculator::FullItemTree& tree, QObject* parent)
  : QAbstractItemModel(parent)
  , _Tree(tree)
  , _RootItem(nullptr)
{
}

ProductionChainTreeModel::~ProductionChainTreeModel()
{ 
  if(_RootItem) delete _RootItem;
}

int ProductionChainTreeModel::columnCount(const QModelIndex &parent) const
{
  if (_RootItem)
  {
    if (parent.isValid())
      return static_cast<ProductionChainTreeItem*>(parent.internalPointer())->columnCount();
    else
      return _RootItem->columnCount();
  }
  return 0;
}

void ProductionChainTreeModel::Reset()
{
  beginResetModel();
  endResetModel();
}

void ProductionChainTreeModel::SetItemID(ResourceCalculator::KEY_ITEM ItemID)
{
  beginResetModel();
  if (_RootItem) delete _RootItem;
  _RootItem = new ProductionChainTreeItem(_Tree, ItemID);
  endResetModel();
}

QVariant ProductionChainTreeModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid()) return QVariant();
  if (role != Qt::DisplayRole) return QVariant();
  ProductionChainTreeItem* item = static_cast<ProductionChainTreeItem*>(index.internalPointer());
  return item->data(index.column());
}

Qt::ItemFlags ProductionChainTreeModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return Qt::NoItemFlags;

  return QAbstractItemModel::flags(index);
}

QVariant ProductionChainTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    switch (section) {
    case 0:
      return tr("Icon");
    case 1:
      return tr("Name");
    default:
      break;
    }
  return QVariant();
}

QModelIndex ProductionChainTreeModel::index(int row, int column, const QModelIndex &parent) const
{
  if (!hasIndex(row, column, parent))
    return QModelIndex();

  ProductionChainTreeItem* parentItem = nullptr;

  if (parent.isValid())
    parentItem = static_cast<ProductionChainTreeItem*>(parent.internalPointer()); 
  else
    parentItem = _RootItem;

  ProductionChainTreeItem* childItem = parentItem->child(row);
  if (childItem)
    return createIndex(row, column, childItem);

  return QModelIndex();
}

QModelIndex ProductionChainTreeModel::parent(const QModelIndex& child) const
{
  if (!child.isValid())
    return QModelIndex();

  ProductionChainTreeItem* childItem = static_cast<ProductionChainTreeItem*>(child.internalPointer());
  ProductionChainTreeItem* parentItem = childItem->parentItem();

  if (parentItem == _RootItem)
    return QModelIndex();

  return createIndex(parentItem->childNumber(), 0, parentItem);
}

int ProductionChainTreeModel::rowCount(const QModelIndex &parent) const
{
  ProductionChainTreeItem* parentItem = nullptr;
  if (!parent.isValid())
    parentItem = _RootItem;
  else
    parentItem = static_cast<ProductionChainTreeItem*>(parent.internalPointer());
  return parentItem == nullptr ? 0 : parentItem->childCount();
}
