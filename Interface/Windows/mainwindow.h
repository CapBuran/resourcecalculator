#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#include "../../ResourceCalculator/ParamsCollection.h"
#include "RecipeWidget.h"

#include "IconSelectedDialog.h"

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
    void PushButtonClickedDebug();

private:
    void createMenus();

    QWidget *CentralWidget;

    ResourceCalculator::ParamsCollection &_PC;

    //RecipeWidget *_RecipeWidget;
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


    QPushButton  *_ButtonDebug;

};

#endif // MAINWINDOW_H
