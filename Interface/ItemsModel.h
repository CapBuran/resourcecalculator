#pragma once

#include <QAbstractItemModel>

#include <ItemCollection.h>
#include <RecipeCollection.h>

class ItemsModel: public QAbstractTableModel
{
public:
  ItemsModel(ResourceCalculator::ItemCollection& IC, QObject *parent = 0);
  int rowCount(const QModelIndex &parent) const override;
  int columnCount(const QModelIndex &parent) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
  bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
  bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
  void Commit();
  void Select();
  ResourceCalculator::CountsItem GetCountItem(const QModelIndex& index) const;
  int GetIndex(const ResourceCalculator::CountsItem& ci) const;
private:
  ResourceCalculator::RecipeCollection& _RC;
  ResourceCalculator::ItemCollection& _IC;
  ResourceCalculator::RecipeCollection _RC_EDIT;
  ResourceCalculator::ItemCollection _IC_EDIT;
};
