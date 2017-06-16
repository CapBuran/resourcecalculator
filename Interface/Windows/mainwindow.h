
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../../FactorioCalculator/RecipeCollection.h"
#include "RecipeWidget.h"

#include <QMainWindow>
#include <QItemSelection>
#include <QTabWidget>
#include <QPushButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(FactorioCalculator::RecipeCollection &RC);

private slots:
    void updateActions(const QItemSelection &selection);
    void openFile();
    void saveFile();
    void PushButtonClicked();

private:
    void createMenus();

    QWidget *CentralWidget;

    FactorioCalculator::RecipeCollection &_RC;

    RecipeWidget *_RecipeWidget;
    QPushButton  *_ButtonRecipesOpen;
    QPushButton  *_ButtonItemOpen;
    QPushButton  *_ButtonFactoryOpen;
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
