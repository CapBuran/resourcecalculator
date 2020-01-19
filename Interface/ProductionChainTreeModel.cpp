#include "ProductionChainTreeModel.h"
#include "../../ResourceCalculator/ProductionChainTree.h"

enum class ProductionChainTreeItemType
{
  ItemItemType,
  RecipeItemType
};

class ProductionChainTreeItem
{
public:
  ProductionChainTreeItem(const ResourceCalculator::ParamsCollection &PC, ResourceCalculator::KEY_ITEM ItemKey, int FiberTree = 10, ProductionChainTreeItem *parentItem = 0);
  ProductionChainTreeItem(const ResourceCalculator::ParamsCollection &PC, ResourceCalculator::KEY_RECIPE RecipeKey, int FiberTree = 10, ProductionChainTreeItem *parentItem = 0);
  ~ProductionChainTreeItem();

  ProductionChainTreeItem *child(int row);
  int childCount() const;
  int columnCount() const;
  QVariant data(int column) const;
  int row() const;
  ProductionChainTreeItem *parentItem();

private:
  QList<ProductionChainTreeItem*> m_childItems;
  const ProductionChainTreeItemType _ItemType;
  const ResourceCalculator::KEY_ITEM _ItemKey;
  const ResourceCalculator::KEY_RECIPE _RecipeKey;
  const ResourceCalculator::ParamsCollection &_PC;
  ProductionChainTreeItem *m_parentItem;
};

ProductionChainTreeModel::ProductionChainTreeModel(const ResourceCalculator::ParamsCollection &PC, QObject *parent ) :
  _PC(PC), QAbstractItemModel(parent), rootItem(nullptr)
{
}

ProductionChainTreeModel::~ProductionChainTreeModel()
{
  if(rootItem != nullptr) delete rootItem;
}

int ProductionChainTreeModel::columnCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return static_cast<ProductionChainTreeItem*>(parent.internalPointer())->columnCount();
  else
    return rootItem->columnCount();
}

void ProductionChainTreeModel::SetItemID(ResourceCalculator::KEY_ITEM ItemID)
{
  beginResetModel();
  if (rootItem != nullptr) delete rootItem;
  rootItem = new ProductionChainTreeItem(_PC, ItemID);
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
    parentItem = rootItem;
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

  if (parentItem == rootItem)
    return QModelIndex();

  return createIndex(parentItem->row(), 0, parentItem);
}

int ProductionChainTreeModel::rowCount(const QModelIndex &parent) const
{
  ProductionChainTreeItem *parentItem = nullptr;
  if (!parent.isValid())
    parentItem = rootItem;
  else
    parentItem = static_cast<ProductionChainTreeItem*>(parent.internalPointer());
  return parentItem == nullptr ? 0 : parentItem->childCount();
}

ProductionChainTreeItem::ProductionChainTreeItem(
  const ResourceCalculator::ParamsCollection & PC,
  ResourceCalculator::KEY_ITEM ItemKey,
  int FiberTree, 
  ProductionChainTreeItem * parentItem
):
  _PC(PC),
  _ItemKey(ItemKey),
  _RecipeKey(ResourceCalculator::KEY_RECIPE::ID_RECIPE_NoFindRecipe),
  _ItemType(ProductionChainTreeItemType::ItemItemType),
  m_parentItem(parentItem)
{
  if (FiberTree <= 0) {
    return;
  }
  using namespace ResourceCalculator;
  ItemResultTree IRT(PC, ItemKey);
  size_t Count = IRT.GetCountChildrens();
  for (size_t i = 0; i < Count; i++) {
    KEY_RECIPE RecipeKey = IRT.GetChildren(i);
    ProductionChainTreeItem *PCTI = new ProductionChainTreeItem(PC, RecipeKey, FiberTree - 1, this);
    m_childItems.append(PCTI);
  }
}

ProductionChainTreeItem::ProductionChainTreeItem(
  const ResourceCalculator::ParamsCollection & PC,
  ResourceCalculator::KEY_RECIPE RecipeKey,
  int FiberTree,
  ProductionChainTreeItem * parentItem
):
  _PC(PC),
  _ItemKey(ResourceCalculator::KEY_ITEM::ID_ITEM_NoFind_Item),
  _RecipeKey(RecipeKey),
  _ItemType(ProductionChainTreeItemType::RecipeItemType),
  m_parentItem(parentItem)
{
  if (FiberTree <= 0) {
    return;
  }
  using namespace ResourceCalculator;
  const Recipe* recipe = PC.RC.GetRecipe(RecipeKey);
  if (recipe != nullptr) {
    const std::set<CountsItem> &ResultItems = recipe->GetRequired();
    for (auto & it : ResultItems) {
      ProductionChainTreeItem *PCTI = new ProductionChainTreeItem(PC, it.ItemId, FiberTree - 1, this);
      m_childItems.append(PCTI);
    }
  }
}

ProductionChainTreeItem::~ProductionChainTreeItem()
{
  qDeleteAll(m_childItems);
}

ProductionChainTreeItem * ProductionChainTreeItem::child(int row)
{
  return m_childItems.value(row);
}

int ProductionChainTreeItem::childCount() const
{
  return m_childItems.count();
}

int ProductionChainTreeItem::columnCount() const
{
  return 2;
}

QVariant ProductionChainTreeItem::data(int column) const
{
  switch (_ItemType) {
  case ProductionChainTreeItemType::ItemItemType:
  {
    const ResourceCalculator::Item *item = _PC.IC.GetItem(_ItemKey);
    if (item != nullptr) {
      switch (column) {
      case 0:
        return QString::fromStdString(item->GetIconPath());
        break;
      case 1:
        return QString::fromStdString(item->GetName());
        break;
      default:
        break;
      }
    }
  }
    break;
  case ProductionChainTreeItemType::RecipeItemType:
  {
    const ResourceCalculator::Recipe *recipe = _PC.RC.GetRecipe(_RecipeKey);
    if (recipe != nullptr) {
      switch (column) {
      case 0:
        return QString::fromStdString(recipe->GetIconPath());
        break;
      case 1:
        return QString::fromStdString(recipe->GetName());
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
  if (m_parentItem)
    return m_parentItem->m_childItems.indexOf(const_cast<ProductionChainTreeItem*>(this));

  return 0;
}

ProductionChainTreeItem * ProductionChainTreeItem::parentItem()
{
  return m_parentItem;
}
