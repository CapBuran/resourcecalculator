#pragma once

#include <QAbstractItemModel>

#include <RecipeCollection.h>
#include <ItemCollection.h>

using QT_CountsItem = std::set<ResourceCalculator::CountsItem>;
Q_DECLARE_METATYPE(QT_CountsItem);

class RecipesModel : public QAbstractTableModel
{
  Q_OBJECT
public:
  RecipesModel(ResourceCalculator::RecipeCollection& RC, const ResourceCalculator::ItemCollection& IC, const ResourceCalculator::FactoryTypeCollection& FTC, QObject *parent = 0);
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
  const ResourceCalculator::FactoryTypeCollection& GetFactoryTypes() const;
  const ResourceCalculator::ItemCollection& GetItems() const;
private:
  const ResourceCalculator::FactoryTypeCollection& _FTC;
  const ResourceCalculator::ItemCollection& _IC;
  ResourceCalculator::RecipeCollection& _RC;
  ResourceCalculator::RecipeCollection _RC_EDIT;
};
