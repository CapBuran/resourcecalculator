#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define StandartTestFileJson "Factorio.json"

#include "../../ResourceCalculator/ParamsCollection.h"

#include <QtWidgets>

#include "ProductionChainTabWidget.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  MainWindow(ResourceCalculator::ParamsCollection &PC);
private slots:
  void openFile();
  void saveAsFile();
  void saveFile();
  void PushButtonClickedAddTab();
  void PushButtonClickedRemoveTab();
  void PushButtonClickedRecipesEditDialog();
  void PushButtonClickedItemsEditDialog();
  void PushButtonClickedFactorysEditDialog();
  void PushButtonClickedModulesEditDialog();
private:
  void _setupTabs();
  void _createMenus();
  ResourceCalculator::ParamsCollection &_PC;
  ProductionChainTabWidget *_PCTW;
};

#endif // MAINWINDOW_H
