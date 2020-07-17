#ifndef ProductionChainTreeModel_H
#define ProductionChainTreeModel_H

#include <ProductionChainTree.h>

#include <QtWidgets>
class ProductionChainTreeItem;

class ProductionChainTreeModel : public QAbstractItemModel
{
public:
  ProductionChainTreeModel(const ResourceCalculator::FullItemTree& tree, QObject *parent = 0);
  ~ProductionChainTreeModel();

  QVariant data(const QModelIndex &index, int role) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const override;
  QModelIndex index(int row, int column,
                    const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &index) const override;
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;

  void SetItemID(ResourceCalculator::KEY_ITEM ItemID);

private:
  const ResourceCalculator::FullItemTree& _Tree;
  ProductionChainTreeItem *_rootItem;
};

#endif //ProductionChainTreeModel_H
