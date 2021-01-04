#pragma once

#include <QAbstractItemModel>

#include <ProductionChainTree.h>

class ProductionChainTreeItem;

class ProductionChainTreeItem : private QObject
{
  Q_OBJECT
public:
  ProductionChainTreeItem(const ResourceCalculator::FullItemTree& tree, ResourceCalculator::KEY_ITEM key);
  ProductionChainTreeItem(const ResourceCalculator::FullItemTree& tree, ResourceCalculator::KEY_ITEM key, ProductionChainTreeItem* parentItem);
  ProductionChainTreeItem(const ResourceCalculator::FullItemTree& tree, ResourceCalculator::KEY_RECIPE key, ProductionChainTreeItem* parentItem);
  ~ProductionChainTreeItem();

  ProductionChainTreeItem* parentItem();
  ProductionChainTreeItem* child(int row);
  int childCount();
  int columnCount();
  int childNumber();
  QVariant data(int column);
private:
  void Init(QList<ProductionChainTreeItem*>& list);
  const ProductionChainTreeItem* _m_parentItem;
  QList<ProductionChainTreeItem*> _m_childItems;
  const ResourceCalculator::FullItemTree& Tree;
  ResourceCalculator::TYPE_KEY Key;
  const ResourceCalculator::TreeNodeType Type;
  bool _IsInit = false;
};

class ProductionChainTreeModel : public QAbstractItemModel
{
  Q_OBJECT

public:
  ProductionChainTreeModel(const ResourceCalculator::FullItemTree& tree, QObject *parent = 0);
  ~ProductionChainTreeModel();

  QVariant data(const QModelIndex &index, int role) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
  QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex& child) const override;
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

  void Reset();
  void SetItemID(ResourceCalculator::KEY_ITEM ItemID);

private:
  const ResourceCalculator::FullItemTree& _Tree;
  ProductionChainTreeItem* _RootItem;
};

