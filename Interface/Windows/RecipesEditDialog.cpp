#include "RecipesEditDialog.h"

#include <QtWidgets>

RecipesEditDialog::RecipesEditDialog(FactorioCalculator::RecipeCollection &RC, QWidget *parent)
  : QDialog(parent), _RC(RC)
{

  setMinimumSize(800, 600);
   
  okButton = new QPushButton("OK");
  cancelButton = new QPushButton("Cancel");

  table = new ChainsCalcModel::RecipeListModel(_RC, this);
  QTableView *tableView = new QTableView;
  tableView->setModel(table);

  QHBoxLayout *buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(okButton);
  buttonLayout->addWidget(cancelButton);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(tableView);
  mainLayout->addLayout(buttonLayout);
  setLayout(mainLayout);

  connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);
  connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);

  setWindowTitle(tr("Recipes Edit"));




}
