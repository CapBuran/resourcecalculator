
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../../ResourceCalculator/ParamsCollection.h"
#include "RecipeWidget.h"

#include <QMainWindow>
#include <QItemSelection>
#include <QTabWidget>
#include <QPushButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(ResourceCalculator::ParamsCollection &PC);

private slots:
    void updateActions(const QItemSelection &selection);
    void openFile();
    void saveFile();
    void PushButtonClickedRecipesEditDialog();
    void PushButtonClickedItemsEditDialog();
    void PushButtonClickedFactorysEditDialog();

private:
    void createMenus();

    QWidget *CentralWidget;

    ResourceCalculator::ParamsCollection &_PC;

    RecipeWidget *_RecipeWidget;
    QPushButton  *_ButtonRecipesOpen;
    QPushButton  *_ButtonItemOpen;
    QPushButton  *_ButtonFactoryOpen;
    QPushButton  *_ButtonAddRecipeTab;
    QPushButton  *_ButtonDelRecipeTab;
    QPushButton  *_ButtonPropertyTableRecipeTab;
    QMenu *fileMenu;
    QMenu *toolMenu;
    QAction *openAct;
    QAction *saveAct;
    QAction *exitAct;
    QAction *addAct;
    QAction *editAct;
    QAction *removeAct;

};

#endif // MAINWINDOW_H
