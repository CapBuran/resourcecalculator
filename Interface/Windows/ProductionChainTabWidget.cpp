#include "ProductionChainTabWidget.h"
#include "ProductionChainWidget.h"
#include "ModulesSelectDialog.h"
#include "ProductionChainWidgetSummProductionItems.h"

ProductionChainTabWidget::ProductionChainTabWidget(ResourceCalculator::ParamsCollection &PC, QWidget *parent):
  _PC(PC), QTabWidget(parent)
{
  ProductionChainWidgetSummProductionItems *PCWSPI = new ProductionChainWidgetSummProductionItems(PC, this);
  addTab(PCWSPI, tr("Summ production items") );

  connect(this, SIGNAL(currentChanged(int)), SLOT(OncurrentChanged(int)));
}

void ProductionChainTabWidget::Update()
{
  int ct = count();
  for (int index = 0; index < ct; index++) {
    QWidget *QW = widget(index);
    ProductionChainWidgetBase* PCB = dynamic_cast< ProductionChainWidgetBase* >(QW);
    if (PCB != nullptr) {
      PCB->UpdateModel();
    }
  }
}

void ProductionChainTabWidget::AddTab(ResourceCalculator::KEY_ITEM ItemKey)
{
  ResourceCalculator::ProductionChainModel *PCM = _PC.PCC.Add(ItemKey);
  ProductionChainWidget *PCW = new ProductionChainWidget(*PCM, this);
  addTab(PCW, QString::fromStdString(_PC.IC.GetItem(ItemKey)->GetName()));
  Update();
}

void ProductionChainTabWidget::RemoveCurrentTab()
{
  int CI = currentIndex();
  if (CI > 0) {
    QWidget *W = currentWidget();
    ProductionChainWidget *PCW = dynamic_cast<ProductionChainWidget*>(W);
    ResourceCalculator::ProductionChainModel &PCM = PCW->GetPCM();
    removeTab(CI);
    _PC.PCC.Remove(&PCM);
  }
  Update();
}

void ProductionChainTabWidget::AddTabs(const std::set<ResourceCalculator::ProductionChainModel*>& ToADD)
{
  for (auto it : ToADD) {
    ProductionChainWidget *PCW = new ProductionChainWidget(*it, this);
    ResourceCalculator::KEY_ITEM ItemKey = it->GetItemKey();
    addTab(PCW, QString::fromStdString(_PC.IC.GetItem(ItemKey)->GetName()));
  }
}

void ProductionChainTabWidget::OncurrentChanged(int index){
  if (index == 0) {
    QWidget *W = widget(index);
    ProductionChainWidgetSummProductionItems *PCWSPI = dynamic_cast<ProductionChainWidgetSummProductionItems*>(W);
    if (PCWSPI != nullptr) {
      PCWSPI->UpdateModel();
    }
  }
}