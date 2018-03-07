#include "mainwindow.h"
#include "RecipesEditDialog.h"
#include "ItemsEditDialog.h"
#include "FactoryTypesEditDialog.h"
#include "FactorysEditDialog.h"
#include "ItemSelectedDialog.h"
#include "ModulesEditDialog.h"

MainWindow::MainWindow(ResourceCalculator::ParamsCollection &PC):
  _PC(PC)
{

  QPushButton *_ButtonAddRecipeTab = new QPushButton(tr("Recipe tab add"));
  connect(_ButtonAddRecipeTab, SIGNAL(clicked()), SLOT( PushButtonClickedAddTab()));

  QPushButton *_ButtonDelRecipeTab = new QPushButton(tr("Recipe tab del"));
  connect(_ButtonDelRecipeTab, SIGNAL(clicked()), SLOT( PushButtonClickedRemoveTab()));

  QPushButton *_ButtonItemOpen = new QPushButton(tr("Items editor"));
  connect(_ButtonItemOpen, SIGNAL(clicked()), SLOT(PushButtonClickedItemsEditDialog()));

  QPushButton *_ButtonRecipesOpen = new QPushButton(tr("Recipes editor"));
  connect(_ButtonRecipesOpen, SIGNAL(clicked()), SLOT(PushButtonClickedRecipesEditDialog()));

  QPushButton *_ButtonFactoryOpen = new QPushButton(tr("Factorys editor"));
  connect(_ButtonFactoryOpen, SIGNAL(clicked()), SLOT(PushButtonClickedFactorysEditDialog()));

  QPushButton *_ButtonModulesEdit = new QPushButton( tr( "Modules editor" ) );
  connect( _ButtonModulesEdit, SIGNAL( clicked() ), SLOT( PushButtonClickedModulesEditDialog() ) );
  
  QHBoxLayout *h = new QHBoxLayout();
  h->addWidget( _ButtonAddRecipeTab );
  h->addWidget( _ButtonDelRecipeTab );
  h->addWidget( _ButtonRecipesOpen );
  h->addWidget( _ButtonItemOpen );
  h->addWidget( _ButtonFactoryOpen );
  h->addWidget( _ButtonModulesEdit );

  _PCTW = new ProductionChainTabWidget( _PC );
  QVBoxLayout *v = new QVBoxLayout();
  v->addWidget(_PCTW);
  v->addLayout( h );

  QWidget *CentralWidget = new QWidget();
  CentralWidget->setLayout(v);

  setCentralWidget( CentralWidget );
  
  _createMenus();

  setWindowTitle(tr("Resurse calculator"));

  _setupTabs();
}

void MainWindow::_setupTabs()
{
  const std::set<ResourceCalculator::ProductionChainModel*>& ToADDs = _PC.PCC.GetReadingPCMs();
  if (ToADDs.size() > 0) {
    _PCTW->AddTabs(ToADDs);
  } else {
    _PCTW->AddTab(ResourceCalculator::KEY_ITEM::ID_ITEM_science_pack_1);
    _PCTW->AddTab(ResourceCalculator::KEY_ITEM::ID_ITEM_Sherst);
    _PCTW->AddTab(ResourceCalculator::KEY_ITEM::ID_ITEM_science_pack_2);
  }
}

void MainWindow::_createMenus()
{
  QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

  QAction *openAct = new QAction(tr("&Open..."), this);
  fileMenu->addAction(openAct);
  connect(openAct, &QAction::triggered, this, &MainWindow::openFile);

  QAction *saveAsAct = new QAction(tr("&Save As..."), this);
  fileMenu->addAction(saveAsAct);
  connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveAsFile);

  QAction *saveAct = new QAction(tr("&Save..."), this);
  fileMenu->addAction(saveAct);
  connect(saveAct, &QAction::triggered, this, &MainWindow::saveFile);

  fileMenu->addSeparator();

  QAction *exitAct = new QAction(tr("E&xit"), this);
  fileMenu->addAction(exitAct);
  connect(exitAct, &QAction::triggered, this, &QWidget::close);

  QMenu *toolMenu = menuBar()->addMenu(tr("&Tools"));

  QAction *addAct = new QAction(tr("&Add Entry..."), this);
  toolMenu->addAction(addAct);
//  connect(addAct, &QAction::triggered, _RecipeWidget, &RecipeWidget::showAddEntryDialog);

  QAction *editAct = new QAction(tr("&Edit Entry..."), this);
  editAct->setEnabled(false);
  toolMenu->addAction(editAct);
//  connect(editAct, &QAction::triggered, _RecipeWidget, &RecipeWidget::editEntry);

  toolMenu->addSeparator();

  QAction *removeAct = new QAction(tr("&Remove Entry"), this);
  removeAct->setEnabled(false);
  toolMenu->addAction(removeAct);
 // connect(removeAct, &QAction::triggered, _RecipeWidget, &RecipeWidget::removeEntry);

//  connect(_RecipeWidget, &RecipeWidget::selectionChanged, this, &MainWindow::updateActions);
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
  _PCTW->Update();
}

void MainWindow::saveFile()
{
  QFile file(StandartTestFileJson);
  if (!file.open(QIODevice::WriteOnly)) {
    QMessageBox::warning(this, tr("Unable to open file"), file.errorString());
    return;
  }
  Json::Value jsonPr;
  _PC.WriteToJson(jsonPr);
  Json::StyledWriter styledWriter;
  file.write(styledWriter.write(jsonPr).c_str());
}

void MainWindow::saveAsFile()
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
  RecipesEditDialog Dialog(_PC);
  Dialog.exec();
  _PCTW->Update();
}

void MainWindow::PushButtonClickedItemsEditDialog()
{
  ItemsEditDialog Dialog(_PC);
  Dialog.exec();
  _PCTW->Update();
}

void MainWindow::PushButtonClickedAddTab()
{
  ItemSelectedDialog _ItemsSelectedDialog( _PC, ItemSelectedDialogMode::ForSelectOneItem );
  if ( _ItemsSelectedDialog.exec() ) {
    _PCTW->AddTab(_ItemsSelectedDialog.GetResultOne());
  }
}

void MainWindow::PushButtonClickedRemoveTab()
{
  _PCTW->RemoveCurrentTab();
}

void MainWindow::PushButtonClickedFactorysEditDialog()
{
  FactorysEditDialog Dialog( _PC );
  if (Dialog.exec() ) {
    Dialog.Commit();
    _PCTW->Update();
  }
}

void MainWindow::PushButtonClickedModulesEditDialog()
{
  ModulesEditDialog Dialog( _PC );
  Dialog.exec();
 _PCTW->Update();
}
