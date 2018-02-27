#include "ProductionChainTabWidget.h"
#include "ProductionChainWidget.h"
#include "ModulesSelectDialog.h"

ProductionChainTabWidget::ProductionChainTabWidget(ResourceCalculator::ParamsCollection &PC, QWidget *parent):
  _PC(PC), QTabWidget(parent)
{

}

void ProductionChainTabWidget::Update()
{
  //ProductionChainWidget* PCW = dynamic_cast< ProductionChainWidget* >( _PCW );
  //PCW->Update();
  //TODO: update all tabs
}

void ProductionChainTabWidget::AddTab(ResourceCalculator::KEY_ITEM ItemKey)
{
  ResourceCalculator::ProductionChainModel *PCM = _PC.PCC.Add(ItemKey);
  ProductionChainWidget *PCW = new ProductionChainWidget(*PCM, this);
  addTab(PCW, QString::fromStdString(_PC.IC.GetItem(ItemKey)->GetName()));
}

void ProductionChainTabWidget::RemoveCurrentTab()
{
  int CI = currentIndex();
  if (CI >= 0) {
    QWidget *W = currentWidget();
    ProductionChainWidget *PCW = dynamic_cast<ProductionChainWidget*>(W);
    ResourceCalculator::ProductionChainModel &PCM = PCW->GetPCM();
    removeTab(CI);
    _PC.PCC.Remove(&PCM);
  }
}

void ProductionChainTabWidget::AddTabs(std::set<ResourceCalculator::ProductionChainModel*>& ToADD)
{
  for (auto it : ToADD) {
    ProductionChainWidget *PCW = new ProductionChainWidget(*it, this);
    ResourceCalculator::KEY_ITEM ItemKey = it->GetItemKey();
    addTab(PCW, QString::fromStdString(_PC.IC.GetItem(ItemKey)->GetName()));
  }
}
