#pragma once

#include <QtWidgets>

#include <IconCollection.h>

int SetIconData(QIcon &Icon, QSize Size, int size_data, const char* data);

class IconSelectedDialog : public QDialog
{
  Q_OBJECT

public:
  IconSelectedDialog(const ResourceCalculator::IconCollection &IC, QWidget *parent = 0);
  const ResourceCalculator::Icon * GetResult() const;

private:

  QListWidget *_ListWidget;
  const ResourceCalculator::Icon * _Result;
  const ResourceCalculator::IconCollection&_IC;
};
