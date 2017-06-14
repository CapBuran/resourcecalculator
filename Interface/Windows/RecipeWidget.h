#ifndef RECIPEWIDGET_H
#define RECIPEWIDGET_H

#include "RecipeModel.h"

#include <QItemSelection>
#include <QTabWidget>

QT_BEGIN_NAMESPACE
class QSortFilterProxyModel;
class QItemSelectionModel;
QT_END_NAMESPACE


class RecipeWidget : public QTabWidget
{
  Q_OBJECT

public:
  RecipeWidget(FactorioCalculator::RecipeCollection &RC, QWidget *parent = 0);
  void readFromFile(const QString &fileName);
  void writeToFile(const QString &fileName);

public slots:
  void showAddEntryDialog();
  void addEntry(QString name, QString address);
  void editEntry();
  void removeEntry();

signals:
  void selectionChanged(const QItemSelection &selected);

private:
  void setupTabs();

  FactorioCalculator::RecipeCollection &_RC;

  ChainsCalcModel::RecipeModelNames *table;

  //NewAddressTab *newAddressTab;
  QSortFilterProxyModel *proxyModel;
};

#endif // RECIPEWIDGET_H
