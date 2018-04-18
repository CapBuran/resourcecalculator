#ifndef ICON_ICONDISPATCHER_DIALOG_H
#define ICON_ICONDISPATCHER_DIALOG_H

#include <QtWidgets>

#include "../../ResourceCalculator/ParamsCollection.h"

int SetIconData(QIcon &Icon, QSize Size, int size_data, const char* data);

class IconDispatcherDialog: public QDialog
{
  Q_OBJECT

public:
  IconDispatcherDialog(const ResourceCalculator::ParamsCollection &PC, QWidget *parent = 0);
  const ResourceCalculator::Icon * GetResult() const;

private:

  QListWidget *_ListWidget;
  const ResourceCalculator::Icon * _Result;
  const ResourceCalculator::ParamsCollection &_PC;
};


#endif // ICON_ICONDISPATCHER_DIALOG_H
