#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../../ResourceCalculator/ParamsCollection.h"

#include <QtWidgets>

#include "ProductionChainWidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(ResourceCalculator::ParamsCollection &PC);

private slots:
    void updateActions(const QItemSelection &selection);
    void openFile();
    void saveFile();
    void PushButtonClickedAddTab();
    void PushButtonClickedRemoveTab();
    void PushButtonClickedRecipesEditDialog();
    void PushButtonClickedItemsEditDialog();
    void PushButtonClickedFactorysEditDialog();
    void PushButtonClickedDebug();

private:
    void createMenus();
    ResourceCalculator::ParamsCollection &_PC;
    QTabWidget *_PCW;
};

#endif // MAINWINDOW_H
