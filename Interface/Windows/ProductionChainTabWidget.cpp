#include "ProductionChainTabWidget.h"
#include "ProductionChainWidget.h"
#include "ModulesSelectDialog.h"

ProductionChainTabWidget::ProductionChainTabWidget(const ResourceCalculator::ParamsCollection &PC, QWidget *parent):
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
  addTab(new ProductionChainWidget(_PC, ItemKey, this), QString::fromStdString(_PC.IC.GetItem(ItemKey)->GetName()));
}

void ProductionChainTabWidget::RemoveCurrentTab()
{
  int CI = currentIndex();
  if (CI >= 0) {
    QWidget *W = currentWidget();
    removeTab(CI);
  }
}