#ifndef ProductionChainTabWidget_H
#define ProductionChainTabWidget_H

#include <ProductionChainModel.h>
#include "ProductionChainWidgetSummProductionItems.h"

#include <QtWidgets>

class ProductionChainTabWidget : public QTabWidget {
  Q_OBJECT
public:
  ProductionChainTabWidget(ResourceCalculator::FullItemTree& tree, QWidget *parent = 0);
  void Update();
  void AddTab(ResourceCalculator::KEY_ITEM ItemKey);
  void AddTabs(const std::list<ResourceCalculator::KEY_ITEM>& ToADD);
  void RemoveCurrentTab();
  void RemoveAllTabs();
  void UpdateModel();
  void Init();
private:
  const ResourceCalculator::FullItemTree& _tree;
  ProductionChainWidgetSummProductionItems* _PCWSPI;
private slots:
  void OncurrentChanged(int index);
};

#endif// ProductionChainTabWidget_H
