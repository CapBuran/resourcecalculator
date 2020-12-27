#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define StandartTestFileJson "Factorio.json"

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
  void PushButtonClickedTransportBeltsEditDialog();
  void PushButtonClickedImportFactorio();
private:
  void _setupTabs();
  void _createMenus();
  ResourceCalculator::ParamsCollection &_PC;
  ResourceCalculator::FullItemTree _tree;

  ProductionChainTabWidget *_PCTW;
};

#endif // MAINWINDOW_H
