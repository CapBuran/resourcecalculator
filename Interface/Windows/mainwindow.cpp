#include "mainwindow.h"
#include "RecipesEditDialog.h"
#include "ItemsEditDialog.h"

#include <QAction>
#include <QFileDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QWidget>

MainWindow::MainWindow(ResourceCalculator::ParamsCollection &PC):
  _PC(PC)
{

  _ButtonPropertyTableRecipeTab = new QPushButton(tr("MainButtonRecipeTabProperty", "Mainwindow button Recipe Tab Property"));
  connect(_ButtonPropertyTableRecipeTab, SIGNAL(clicked()), SLOT(PushButtonClickedFactorysEditDialog()));

  _ButtonAddRecipeTab = new QPushButton(tr("MainButtonRecipeTabAdd", "Mainwindow button Recipe Tab Add"));
  connect(_ButtonAddRecipeTab, SIGNAL(clicked()), SLOT(PushButtonClickedItemsEditDialog()));

  _ButtonDelRecipeTab = new QPushButton(tr("MainButtonRecipeTabDel", "Mainwindow button Recipe Tab Del"));
  connect(_ButtonDelRecipeTab, SIGNAL(clicked()), SLOT(PushButtonClickedRecipesEditDialog()));

  _ButtonItemOpen = new QPushButton(tr("MainButtonItems", "Mainwindow button Items"));
  connect(_ButtonItemOpen, SIGNAL(clicked()), SLOT(PushButtonClickedItemsEditDialog()));

  _ButtonRecipesOpen = new QPushButton(tr("MainButtonRecipes", "Mainwindow button Recipes"));
  connect(_ButtonRecipesOpen, SIGNAL(clicked()), SLOT(PushButtonClickedRecipesEditDialog()));

  _ButtonFactoryOpen = new QPushButton(tr("MainButtonFactorys", "Mainwindow button Factorys"));
  connect(_ButtonFactoryOpen, SIGNAL(clicked()), SLOT(PushButtonClickedFactorysEditDialog()));

  _RecipeWidget       = new RecipeWidget(PC);

  QHBoxLayout *h = new QHBoxLayout();
  h->setMargin(5);
  h->setSpacing(5);
  h->addWidget(_ButtonPropertyTableRecipeTab); 
  h->addWidget(_ButtonAddRecipeTab);
  h->addWidget(_ButtonDelRecipeTab);
  h->addWidget(_ButtonRecipesOpen);
  h->addWidget(_ButtonItemOpen);
  h->addWidget(_ButtonFactoryOpen);

  QVBoxLayout *v = new QVBoxLayout();
  v->setMargin(5);
  v->setSpacing(5);
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
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
      QMessageBox::warning(this, tr("Unable to open file"), file.errorString());
      return;
    }
    QByteArray fileData = file.readAll();
    Json::Value jsonPrRestore;
    Json::Reader JsonReader;
    if (JsonReader.parse(fileData.constData(), jsonPrRestore)) {
      _PC.ReadFromJson(jsonPrRestore);
    } else {
      QMessageBox::warning(this, tr("Unable parse json file"), JsonReader.getFormattedErrorMessages().c_str());
      return;
    }
  }
}

void MainWindow::saveFile()
{
  QString fileName = QFileDialog::getSaveFileName(this);
  if (!fileName.isEmpty()) {
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
      QMessageBox::warning(this, tr("Unable to open file"), file.errorString());
      return;
    }
    Json::Value jsonPr;
    _PC.WriteToJson(jsonPr);
    Json::StyledWriter styledWriter;
    file.write(styledWriter.write(jsonPr).c_str());
  }
}

void MainWindow::PushButtonClickedRecipesEditDialog()
{
  RecipesEditDialog _RecipesEditDialog(_PC);
  if (_RecipesEditDialog.exec()) {
    //QString name = _RecipesEditDialog.nameText->text();
    //QString address = _RecipesEditDialog.addressText->toPlainText();
    //emit sendDetails(name, address);
    int d = 0;
    d += 5;
  }

}

void MainWindow::PushButtonClickedItemsEditDialog()
{
  ItemsEditDialog _ItemsEditDialog(_PC);
  if (_ItemsEditDialog.exec()) {
    //QString name = _RecipesEditDialog.nameText->text();
    //QString address = _RecipesEditDialog.addressText->toPlainText();
    //emit sendDetails(name, address);
  }

}

void MainWindow::PushButtonClickedFactorysEditDialog()
{
  //RecipesEditDialog _RecipesEditDialog(_PC);
  //if (_RecipesEditDialog.exec()) {
  //  //QString name = _RecipesEditDialog.nameText->text();
  //  //QString address = _RecipesEditDialog.addressText->toPlainText();
  //  //emit sendDetails(name, address);
  //}

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
