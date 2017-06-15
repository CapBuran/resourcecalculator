#ifndef RECIPE_EDIT_DIALOG_H
#define RECIPE_EDIT_DIALOG_H

#include <QDialog>

#include "../../FactorioCalculator/RecipeCollection.h"
#include "RecipeModel.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QTextEdit;
class QLineEdit;
QT_END_NAMESPACE

class RecipesEditDialog : public QDialog
{
  Q_OBJECT

public:
  RecipesEditDialog(FactorioCalculator::RecipeCollection &RC, QWidget *parent = 0);
  //QLineEdit *nameText;
  //QTextEdit *addressText;

private:

  FactorioCalculator::RecipeCollection &_RC;

  ChainsCalcModel::RecipeListModel *table;

  //QLabel *nameLabel;
  //QLabel *addressLabel;
  QPushButton *okButton;
  QPushButton *cancelButton;
};

#endif // RECIPE_EDIT_DIALOG_H
