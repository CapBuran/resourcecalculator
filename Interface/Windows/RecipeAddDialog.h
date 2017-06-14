#ifndef RECIPE_ADD_DIALOG_H
#define RECIPE_ADD_DIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QTextEdit;
class QLineEdit;
QT_END_NAMESPACE

class RecipeAddDialog : public QDialog
{
    Q_OBJECT

public:
    RecipeAddDialog(QWidget *parent = 0);
    QLineEdit *nameText;
    QTextEdit *addressText;

private:
    QLabel *nameLabel;
    QLabel *addressLabel;
    QPushButton *okButton;
    QPushButton *cancelButton;
};

#endif // RECIPE_ADD_DIALOG_H
