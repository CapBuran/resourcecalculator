#include "ProductionChainTreeModel.h"
#include "../../ResourceCalculator/ProductionChainTree.h"


using IsPresentInTreeType = std::map<ResourceCalculator::KEY_ITEM, std::set<ResourceCalculator::KEY_RECIPE> >;

class ProductionChainTreeItem : public QObject
{

public:
  ProductionChainTreeItem(const ResourceCalculator::FullItemTree& tree, IsPresentInTreeType isPresent, ResourceCalculator::KEY_ITEM ItemKey, ProductionChainTreeItem *parentItem = 0);
  ProductionChainTreeItem(const ResourceCalculator::FullItemTree& tree, IsPresentInTreeType& isPresent, ResourceCalculator::KEY_RECIPE RecipeKey, ProductionChainTreeItem *parentItem = 0);
  ~ProductionChainTreeItem();

  ProductionChainTreeItem* child(int row);
  int childCount() const;
  int columnCount() const;
  QVariant data(int column) const;
  int row() const;
  ProductionChainTreeItem *parentItem();

private:
  const ResourceCalculator::FullItemTree& _tree;
  ProductionChainTreeItem* _m_parentItem;
  QList<ProductionChainTreeItem*> _m_childItems;
  const ResourceCalculator::TreeBase* _treeElementData;
};

ProductionChainTreeModel::ProductionChainTreeModel(const ResourceCalculator::FullItemTree& tree, QObject* parent)
  : QAbstractItemModel(parent)
  , _Tree(tree)
  , _rootItem(nullptr)
{
}

ProductionChainTreeModel::~ProductionChainTreeModel()
{
  if(_rootItem != nullptr) delete _rootItem;
}

int ProductionChainTreeModel::columnCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return static_cast<ProductionChainTreeItem*>(parent.internalPointer())->columnCount();
  else
    return _rootItem->columnCount();
}

void ProductionChainTreeModel::SetItemID(ResourceCalculator::KEY_ITEM ItemID)
{
  beginResetModel();
  if (_rootItem != nullptr) delete _rootItem;
  IsPresentInTreeType isPresent;
  _rootItem = new ProductionChainTreeItem(_Tree, isPresent, ItemID);
  endResetModel();
}

QVariant ProductionChainTreeModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  if (role != Qt::DisplayRole)
    return QVariant();
  
  ProductionChainTreeItem *item = static_cast<ProductionChainTreeItem*>(index.internalPointer());
  return item->data(index.column());
}

Qt::ItemFlags ProductionChainTreeModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return 0;

  return QAbstractItemModel::flags(index);
}

QVariant ProductionChainTreeModel::headerData(int section, Qt::Orientation orientation,
                                              int role) const
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

  ProductionChainTreeItem *parentItem = nullptr;

  if (!parent.isValid())
    parentItem = _rootItem;
  else
    parentItem = static_cast<ProductionChainTreeItem*>(parent.internalPointer());

  ProductionChainTreeItem *childItem = parentItem->child(row);
  if (childItem)
    return createIndex(row, column, childItem);
  else
    return QModelIndex();
}

QModelIndex ProductionChainTreeModel::parent(const QModelIndex &index) const
{
  if (!index.isValid())
    return QModelIndex();

  ProductionChainTreeItem *childItem = static_cast<ProductionChainTreeItem*>(index.internalPointer());
  ProductionChainTreeItem *parentItem = childItem->parentItem();

  if (parentItem == _rootItem)
    return QModelIndex();

  return createIndex(parentItem->row(), 0, parentItem);
}

int ProductionChainTreeModel::rowCount(const QModelIndex &parent) const
{
  ProductionChainTreeItem *parentItem = nullptr;
  if (!parent.isValid())
    parentItem = _rootItem;
  else
    parentItem = static_cast<ProductionChainTreeItem*>(parent.internalPointer());
  return parentItem == nullptr ? 0 : parentItem->childCount();
}

ProductionChainTreeItem::ProductionChainTreeItem(
  const ResourceCalculator::FullItemTree& tree,
  IsPresentInTreeType isPresent,
  ResourceCalculator::KEY_ITEM ItemKey,
  ProductionChainTreeItem* parentItem
)
  : _tree(tree)
  , _m_parentItem(parentItem)
  , _treeElementData(
    tree.FactoryItemTree(
      ItemKey,
      parentItem ?
        dynamic_cast<const ResourceCalculator::RecipeResultTree*>(parentItem->_treeElementData)->MyKey:
        ResourceCalculator::KEY_RECIPE::ID_RECIPE_NoFindRecipe
    )
  )
{
  using namespace ResourceCalculator;
  const ItemResultTree& root = tree.GetRootItemTree(ItemKey);
  _m_childItems.reserve(root.Childrens.size());

  if (isPresent.count(ItemKey) > 0)
  {
    for (KEY_RECIPE k : root.Childrens)
    {
      if (isPresent[ItemKey].count(k) == 0)
      {
        isPresent[ItemKey].insert(k);
        _m_childItems.append(new ProductionChainTreeItem(tree, isPresent, k, this));
      }
    }
  }
  else
  {
    for (KEY_RECIPE k : root.Childrens)
    {
      if (isPresent[ItemKey].count(k) > 0) continue;
      isPresent[ItemKey] = { k };
      _m_childItems.append(new ProductionChainTreeItem(tree, isPresent, k, this));
    }
  }

}

ProductionChainTreeItem::ProductionChainTreeItem(
  const ResourceCalculator::FullItemTree& tree,
  IsPresentInTreeType& isPresent,
  ResourceCalculator::KEY_RECIPE RecipeKey,
  ProductionChainTreeItem* parentItem
)
  : _tree(tree)
  , _m_parentItem(parentItem)
  , _treeElementData(
    tree.FactoryRecipeTree(
      RecipeKey,
      parentItem ?
        dynamic_cast<const ResourceCalculator::ItemResultTree*>(parentItem->_treeElementData)->MyKey :
        ResourceCalculator::KEY_ITEM::ID_ITEM_NoFind_Item
    )
  )
{
  using namespace ResourceCalculator;
  const RecipeResultTree& root = tree.GetRootRecipeTree(RecipeKey);
  _m_childItems.reserve(root.Childrens.size());
  for (KEY_ITEM k : root.Childrens)
    _m_childItems.append(new ProductionChainTreeItem(tree, isPresent, k, this));
}

ProductionChainTreeItem::~ProductionChainTreeItem()
{
  qDeleteAll(_m_childItems);
  delete _treeElementData;
}

ProductionChainTreeItem * ProductionChainTreeItem::child(int row)
{
  return _m_childItems.value(row);
}

int ProductionChainTreeItem::childCount() const
{
  return _m_childItems.count();
}

int ProductionChainTreeItem::columnCount() const
{
  return 2;
}

QVariant ProductionChainTreeItem::data(int column) const
{
  switch (_treeElementData->Type) {
  case ResourceCalculator::TreeBaseType::ITEM:
  {
    const ResourceCalculator::ItemResultTree& treeElement = dynamic_cast<const ResourceCalculator::ItemResultTree&>(*_treeElementData);
    const ResourceCalculator::Item *item = _tree.GetPC().IC.GetItem(treeElement.MyKey);
    if (item != nullptr) {
      switch (column) {
      case 0:
        return QString::fromStdString(item->GetIconKey());
        break;
      case 1:
        return tr("Item: ") + QString::fromStdString(item->GetName());
        break;
      default:
        break;
      }
    }
  }
    break;
  case ResourceCalculator::TreeBaseType::RECIPE:
  {
    const ResourceCalculator::RecipeResultTree& treeElement = dynamic_cast<const ResourceCalculator::RecipeResultTree&>(*_treeElementData);
    const ResourceCalculator::Recipe* recipe = _tree.GetPC().RC.GetRecipe(treeElement.MyKey);
    if (recipe != nullptr) {
      switch (column) {
      case 0:
        return QString::fromStdString(recipe->GetIconKey());
        break;
      case 1:
        return tr("Recpie: ") + QString::fromStdString(recipe->GetName());
        break;
      default:
        break;
      }
    }
  }
    break;
  default:
    break;
  }
  return QVariant();
}

int ProductionChainTreeItem::row() const
{
  if (_m_parentItem)
    return _m_parentItem->_m_childItems.indexOf(const_cast<ProductionChainTreeItem*>(this));

  return 0;
}

ProductionChainTreeItem * ProductionChainTreeItem::parentItem()
{
  return _m_parentItem;
}
