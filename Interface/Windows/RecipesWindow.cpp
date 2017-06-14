#include "RecipesWindow.h"

#include <QAction>
#include <QFileDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QWidget>

RecipesWindow::RecipesWindow(FactorioCalculator::RecipeCollection &RC):
  _RC(RC)
{
  QPushButton* ButtonRecipesOpen = new QPushButton("Recipes");
  QPushButton* ButtonItemOpen    = new QPushButton("Items");
  _RecipeWidget = new RecipeWidget(RC);

  QHBoxLayout *h = new QHBoxLayout();
  h->setMargin(5);
  h->setSpacing(15);
  h->addWidget(ButtonRecipesOpen);
  h->addWidget(ButtonItemOpen);

  QVBoxLayout *v = new QVBoxLayout();
  v->setMargin(5);
  v->setSpacing(15);
  v->addWidget(_RecipeWidget);
  v->addLayout(h);

  CentralWidget = new QWidget();
  CentralWidget->setLayout(v);

  setCentralWidget(CentralWidget);
  
  setWindowTitle(tr("Resurse calculator "));
}


//void RecipesWindow::openFile()
//{
//    QString fileName = QFileDialog::getOpenFileName(this);
//    if (!fileName.isEmpty()) {
//      _RecipeWidget->readFromFile(fileName);
//    }
//}
//
//void RecipesWindow::saveFile()
//{
//    //QString fileName = QFileDialog::getSaveFileName(this);
//    //if (!fileName.isEmpty())
//    //    addressWidget->writeToFile(fileName);
//}

void RecipesWindow::updateActions(const QItemSelection &selection)
{
    //QModelIndexList indexes = selection.indexes();

    //if (!indexes.isEmpty()) {
    //    removeAct->setEnabled(true);
    //    editAct->setEnabled(true);
    //} else {
    //    removeAct->setEnabled(false);
    //    editAct->setEnabled(false);
    //}
}
