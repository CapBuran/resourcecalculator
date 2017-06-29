#ifndef RECIPEMODEL_H
#define RECIPEMODEL_H

#include "../../ResourceCalculator/ParamsCollection.h"

#include <QAbstractTableModel>
#include <QList>
#include <QPair>

class ProductionChainModelLeft : public QAbstractTableModel
{
  Q_OBJECT
public:
  ProductionChainModelLeft(ResourceCalculator::ParamsCollection &PC, QObject *parent = 0);
  int rowCount(const QModelIndex &parent) const override;
  int columnCount(const QModelIndex &parent) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
  bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
  bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
private:
  ResourceCalculator::ParamsCollection &_PC;
  //QList<ResourceCalculator::KEY_RECIPE> _listOfRecipesId;
};

enum class ProductionChainModelRightDisplayMode
{
  DependenceOfRecipes,
  ProductionPerSecond
};

class ProductionChainModelRight : public QAbstractTableModel
{
  Q_OBJECT
public:
  ProductionChainModelRight(ResourceCalculator::ParamsCollection &PC, ProductionChainModelRightDisplayMode, QObject *parent = 0);
  int rowCount(const QModelIndex &parent) const override;
  int columnCount(const QModelIndex &parent) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
  bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
  bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
  ResourceCalculator::KEY_RECIPE GetRecipeId(int Num) const;
private:
  ResourceCalculator::ParamsCollection &_PC;
  //QList<ResourceCalculator::KEY_RECIPE> _listOfRecipesId;
};


#endif //RECIPEMODEL_H