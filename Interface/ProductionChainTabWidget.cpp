#include "ProductionChainTabWidget.h"
#include "ProductionChainWidget.h"
#include "ModulesSelectDialog.h"
#include "ProductionChainWidgetSummProductionItems.h"

ProductionChainTabWidget::ProductionChainTabWidget(ResourceCalculator::FullItemTree& tree, QWidget* parent):
  Tree_(tree), QTabWidget(parent)
{
  ProductionChainWidgetSummProductionItems *PCWSPI = new ProductionChainWidgetSummProductionItems(Tree_.GetPC(), this);
  addTab(PCWSPI, tr("Summ production items on all tabs") );
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
  ProductionChainWidget *PCW = new ProductionChainWidget(Tree_, ItemKey, this);
  addTab(PCW, QString::fromStdString(Tree_.GetPC().IC.GetItem(ItemKey)->GetName()));
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
  }
  Update();
}

void ProductionChainTabWidget::AddTabs(const std::list<ResourceCalculator::KEY_ITEM>& ToADD)
{
  for (auto it : ToADD) {
    AddTab(it);
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