#ifndef ProductionChainTabWidget_H
#define ProductionChainTabWidget_H

#include "../../ResourceCalculator/ParamsCollection.h"

#include <QtWidgets>

class ProductionChainTabWidget : public QTabWidget {
  Q_OBJECT
private:
  ResourceCalculator::ParamsCollection &_PC;
public:
  ProductionChainTabWidget(ResourceCalculator::ParamsCollection &PC, QWidget *parent = 0);
  void Update();
  void AddTab(ResourceCalculator::KEY_ITEM ItemKey);
  void RemoveCurrentTab();
  void AddTabs(std::set<ResourceCalculator::ProductionChainModel*>& ToADD);
};

#endif// ProductionChainTabWidget_H
