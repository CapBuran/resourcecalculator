#ifndef RECIPEMODEL_H
#define RECIPEMODEL_H

#include "../../ResourceCalculator/ParamsCollection.h"

#include <QAbstractTableModel>
#include <QList>
#include <QPair>

namespace ChainsCalcModel{

class RecipeModelNames : public QAbstractTableModel
{
    Q_OBJECT

public:
  RecipeModelNames(ResourceCalculator::ParamsCollection &PC, QObject *parent = 0);

  int rowCount(const QModelIndex &parent) const override;
  int columnCount(const QModelIndex &parent) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
  bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
  bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
  QList<QPair<QString, QString> > getList();

private:
  ResourceCalculator::ParamsCollection &_PC;
  QList<QPair<QString, QString> > listOfPairs;
};




}

#endif // RECIPEMODEL_H

