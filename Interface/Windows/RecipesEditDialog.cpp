#include "RecipesEditDialog.h"

#include <QtWidgets>

RecipesEditDialog::RecipesEditDialog(FactorioCalculator::RecipeCollection &RC, QWidget *parent)
  : QDialog(parent), _RC(RC)
{
    //nameLabel = new QLabel("Name");
    //addressLabel = new QLabel("Address");
    okButton = new QPushButton("OK");
    cancelButton = new QPushButton("Cancel");

    table = new ChainsCalcModel::RecipeListModel(_RC, this);
    QTableView *tableView = new QTableView;
    tableView->setModel(table);

    //nameText = new QLineEdit;
    //addressText = new QTextEdit;

    QGridLayout *gLayout = new QGridLayout;
    gLayout->setColumnStretch(1, 1);

    //gLayout->addWidget(tableView, 0, 0);
    //gLayout->addWidget(tableView, 1, 0, Qt::AlignLeft|Qt::AlignTop);
    //gLayout->addWidget(tableView, 1, 0, Qt::AlignHorizontal_Mask | Qt::AlignVertical_Mask);
    gLayout->addWidget(tableView, 1, 0, Qt::AlignCenter);
        
    //gLayout->addWidget(nameLabel, 0, 0);
    //gLayout->addWidget(nameText, 0, 1);

    //gLayout->addWidget(addressLabel, 1, 0, Qt::AlignLeft|Qt::AlignTop);
    //gLayout->addWidget(addressText, 1, 1, Qt::AlignLeft);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    gLayout->addLayout(buttonLayout, 2, 1, Qt::AlignRight);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(gLayout);
    setLayout(mainLayout);

    connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);

    setWindowTitle(tr("Add a Contact"));




}
