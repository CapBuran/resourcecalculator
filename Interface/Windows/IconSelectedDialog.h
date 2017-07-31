#ifndef ICON_SELECTED_DIALOG_H
#define ICON_SELECTED_DIALOG_H

#include <QDialog>
#include <QtWidgets>

#include "../../ResourceCalculator/ParamsCollection.h"

int SetIconData(QIcon &Icon, QSize Size, int size_data, const char* data);

class IconSelectedDialog : public QDialog
{
  Q_OBJECT

public:
  IconSelectedDialog(const ResourceCalculator::ParamsCollection &PC, QWidget *parent = 0);
  const ResourceCalculator::Icon * GetResult() const;

private:

  QListWidget *_ListWidget;
  const ResourceCalculator::Icon * _Result;
  const ResourceCalculator::ParamsCollection &_PC;
};


#endif // ICON_SELECTED_DIALOG_H
