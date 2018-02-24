#ifndef ProductionChainTabWidget_H
#define ProductionChainTabWidget_H

#include "../../ResourceCalculator/ParamsCollection.h"

#include <QtWidgets>

class ProductionChainTabWidget : public QTabWidget {
  Q_OBJECT
private:
  const ResourceCalculator::ParamsCollection &_PC;
public:
  ProductionChainTabWidget(const ResourceCalculator::ParamsCollection &PC, QWidget *parent = 0);
  void Update();
  void AddTab(ResourceCalculator::KEY_ITEM ItemKey);
  void RemoveCurrentTab();
};

#endif// ProductionChainTabWidget_H
