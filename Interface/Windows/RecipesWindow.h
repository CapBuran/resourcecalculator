
#ifndef RECIPEWINDOW_H
#define RECIPEWINDOW_H

#include "../../ResourceCalculator/ParamsCollection.h"
#include "RecipeWidget.h"

#include <QMainWindow>
#include <QItemSelection>
#include <QTabWidget>

class RecipesWindow : public QMainWindow
{
    Q_OBJECT

public:
    RecipesWindow(ResourceCalculator::ParamsCollection &PC);

private slots:
    void updateActions(const QItemSelection &selection);

private:
    QWidget *CentralWidget;

    ResourceCalculator::ParamsCollection &_PC;

    RecipeWidget *_RecipeWidget;
    QMenu *fileMenu;
    QMenu *toolMenu;
    QAction *openAct;
    QAction *saveAct;
    QAction *exitAct;
    QAction *addAct;
    QAction *editAct;
    QAction *removeAct;
};

#endif // RECIPEWINDOW_H
