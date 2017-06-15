
#ifndef RECIPEWINDOW_H
#define RECIPEWINDOW_H

#include "../../FactorioCalculator/RecipeCollection.h"
#include "RecipeWidget.h"

#include <QMainWindow>
#include <QItemSelection>
#include <QTabWidget>

class RecipesWindow : public QMainWindow
{
    Q_OBJECT

public:
    RecipesWindow(FactorioCalculator::RecipeCollection &RC);

private slots:
    void updateActions(const QItemSelection &selection);

private:
    QWidget *CentralWidget;

    FactorioCalculator::RecipeCollection &_RC;

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
