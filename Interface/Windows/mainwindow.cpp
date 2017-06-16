#include "mainwindow.h"
#include "RecipesEditDialog.h"

#include <QAction>
#include <QFileDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QWidget>

MainWindow::MainWindow(FactorioCalculator::RecipeCollection &RC):
  _RC(RC)
{
  _ButtonRecipesOpen = new QPushButton("Recipes");
  connect(_ButtonRecipesOpen, SIGNAL(clicked()), SLOT(PushButtonClicked())) ;
  
  _ButtonItemOpen    = new QPushButton("Items");
  connect(_ButtonItemOpen, SIGNAL(clicked()), SLOT(PushButtonClicked()));

  _ButtonFactoryOpen = new QPushButton("Factorys");
  connect(_ButtonFactoryOpen, SIGNAL(clicked()), SLOT(PushButtonClicked()));

  _RecipeWidget      = new RecipeWidget(RC);

  QHBoxLayout *h = new QHBoxLayout();
  h->setMargin(5);
  h->setSpacing(15);
  h->addWidget(_ButtonRecipesOpen);
  h->addWidget(_ButtonItemOpen);
  h->addWidget(_ButtonFactoryOpen);

  QVBoxLayout *v = new QVBoxLayout();
  v->setMargin(5);
  v->setSpacing(15);
  v->addWidget(_RecipeWidget);
  v->addLayout(h);

  CentralWidget = new QWidget();
  CentralWidget->setLayout(v);

  setCentralWidget(CentralWidget);
  
  createMenus();

  setWindowTitle(tr("Resurse calculator "));

}
void MainWindow::createMenus()
{
  fileMenu = menuBar()->addMenu(tr("&File"));

  openAct = new QAction(tr("&Open..."), this);
  fileMenu->addAction(openAct);
  connect(openAct, &QAction::triggered, this, &MainWindow::openFile);

  saveAct = new QAction(tr("&Save As..."), this);
  fileMenu->addAction(saveAct);
  connect(saveAct, &QAction::triggered, this, &MainWindow::saveFile);

  fileMenu->addSeparator();

  exitAct = new QAction(tr("E&xit"), this);
  fileMenu->addAction(exitAct);
  connect(exitAct, &QAction::triggered, this, &QWidget::close);

  toolMenu = menuBar()->addMenu(tr("&Tools"));

  addAct = new QAction(tr("&Add Entry..."), this);
  toolMenu->addAction(addAct);
  connect(addAct, &QAction::triggered, _RecipeWidget, &RecipeWidget::showAddEntryDialog);

  editAct = new QAction(tr("&Edit Entry..."), this);
  editAct->setEnabled(false);
  toolMenu->addAction(editAct);
  connect(editAct, &QAction::triggered, _RecipeWidget, &RecipeWidget::editEntry);

  toolMenu->addSeparator();

  removeAct = new QAction(tr("&Remove Entry"), this);
  removeAct->setEnabled(false);
  toolMenu->addAction(removeAct);
  connect(removeAct, &QAction::triggered, _RecipeWidget, &RecipeWidget::removeEntry);

  connect(_RecipeWidget, &RecipeWidget::selectionChanged, this, &MainWindow::updateActions);
}

void MainWindow::openFile()
{
  QString fileName = QFileDialog::getOpenFileName(this);
  if (!fileName.isEmpty()) {
    _RecipeWidget->readFromFile(fileName);
  }
}

void MainWindow::saveFile()
{
    //QString fileName = QFileDialog::getSaveFileName(this);
    //if (!fileName.isEmpty())
    //    addressWidget->writeToFile(fileName);
}

void MainWindow::PushButtonClicked()
{
  RecipesEditDialog _RecipesEditDialog(_RC);
  if (_RecipesEditDialog.exec()) {
    //QString name = _RecipesEditDialog.nameText->text();
    //QString address = _RecipesEditDialog.addressText->toPlainText();
    //emit sendDetails(name, address);
    int d = 0;
    d += 5;
  }

}

void MainWindow::updateActions(const QItemSelection &selection)
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
