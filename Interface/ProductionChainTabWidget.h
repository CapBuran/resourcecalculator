#ifndef ProductionChainTabWidget_H
#define ProductionChainTabWidget_H

#include <ProductionChainModel.h>

#include <QtWidgets>

class ProductionChainTabWidget : public QTabWidget {
  Q_OBJECT
public:
  ProductionChainTabWidget(const ResourceCalculator::FullItemTree& tree, QWidget *parent = 0);
  void Update();
  void AddTab(ResourceCalculator::KEY_ITEM ItemKey);
  void AddTabs(const std::list<ResourceCalculator::KEY_ITEM>& ToADD);
  void RemoveCurrentTab();
private:
  const ResourceCalculator::FullItemTree& _tree;
private slots:
  void OncurrentChanged(int index);
};

#endif// ProductionChainTabWidget_H
