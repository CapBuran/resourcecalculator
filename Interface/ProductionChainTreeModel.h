#ifndef ProductionChainTreeModel_H
#define ProductionChainTreeModel_H

#include "../../ResourceCalculator/ParamsCollection.h"

#include <QtWidgets>
class ProductionChainTreeItem;

class ProductionChainTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    ProductionChainTreeModel(const ResourceCalculator::ParamsCollection &PC, QObject *parent = 0);
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
  const ResourceCalculator::ParamsCollection &_PC;

  ProductionChainTreeItem *rootItem;
};

#endif //ProductionChainTreeModel_H
