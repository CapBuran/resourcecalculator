#pragma once

#include <QAbstractItemModel>

#include <RecipeCollection.h>
#include <ItemCollection.h>

using QT_CountsItem = std::set<ResourceCalculator::CountsItem>;
Q_DECLARE_METATYPE(QT_CountsItem);

class RecipesModelRead: public QAbstractTableModel
{
  Q_OBJECT
public:
  RecipesModelRead(const ResourceCalculator::ItemCollection& IC, const ResourceCalculator::FactoryTypeCollection& FTC, QObject *parent = 0);
  int rowCount(const QModelIndex &parent) const override;
  int columnCount(const QModelIndex &parent) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  const ResourceCalculator::FactoryTypeCollection& GetFactoryTypes() const;
  const ResourceCalculator::ItemCollection& GetItems() const;
protected:
  ResourceCalculator::ItemCollection _IC_EDIT;
  ResourceCalculator::RecipeCollection _RC_EDIT;
  const ResourceCalculator::FactoryTypeCollection& _FTC;
};

class RecipesModel: public RecipesModelRead
{
  Q_OBJECT
public:
  RecipesModel(ResourceCalculator::ItemCollection& IC, const ResourceCalculator::FactoryTypeCollection& FTC, QObject* parent = 0);
  Qt::ItemFlags flags(const QModelIndex& index) const override;
  bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
  bool insertRows(int position, int rows, const QModelIndex& index = QModelIndex()) override;
  bool removeRows(int position, int rows, const QModelIndex& index = QModelIndex()) override;
  void Commit();
private:
  ResourceCalculator::ItemCollection& _IC;
};
