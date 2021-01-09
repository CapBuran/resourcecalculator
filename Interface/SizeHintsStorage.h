#pragma once

#include <QTCore>

class HorizontalSizeHintsStorage
{
public:
  HorizontalSizeHintsStorage(int size);
  
  void SetHeaderSize(int columb, QSize size) const;
  void SetRowsSize(int columb, QSize size) const;

  QSize GetHeaderSizeMax(int columb) const;
  QSize GetRowsSizeMax(int columb) const;

private:
  mutable QVector<int> _MaxsWidth;
  mutable int _MaxsWidthOverflow;
  mutable int _MaxHeightHeader;
  mutable int _MaxHeightRows;
};
