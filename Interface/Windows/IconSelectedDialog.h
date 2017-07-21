#ifndef ICON_SELECTED_DIALOG_H
#define ICON_SELECTED_DIALOG_H

#include <QDialog>
#include <QStyledItemDelegate>
#include <QAbstractTableModel>

#include "../../ResourceCalculator/ParamsCollection.h"

class IconSelectedDialog : public QDialog
{
  Q_OBJECT

public:
  IconSelectedDialog(ResourceCalculator::ParamsCollection &PC, QWidget *parent = 0);
  const ResourceCalculator::Icon * GetResult() const;

private:

  //QListWidgetItem _Icons;

  const ResourceCalculator::Icon * _Result;
  ResourceCalculator::ParamsCollection &_PC;
};

#endif // ICON_SELECTED_DIALOG_H
