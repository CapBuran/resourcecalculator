#include <SizeHintsStorage.h>

HorizontalSizeHintsStorage::HorizontalSizeHintsStorage(int size)
{
  _MaxsWidth.resize(size);
  for (int& i: _MaxsWidth)
  {
    i = -1;
  }
  _MaxHeightHeader = -1;
  _MaxHeightRows = -1;
  _MaxsWidthOverflow = -1;
}

void HorizontalSizeHintsStorage::SetHeaderSize(int columb, QSize size) const
{
  int& maxWidth = columb >= _MaxsWidth.size() ? _MaxsWidthOverflow : _MaxsWidth[columb];
  if (maxWidth < size.width()) maxWidth = size.width();
  if (_MaxHeightHeader < size.height()) _MaxHeightHeader = size.height();
}

void HorizontalSizeHintsStorage::SetRowsSize(int columb, QSize size) const
{
  int& maxWidth = columb >= _MaxsWidth.size() ? _MaxsWidthOverflow : _MaxsWidth[columb];
  if (maxWidth < size.width()) maxWidth = size.width();
  if (_MaxHeightRows < size.height()) _MaxHeightRows = size.height();
}

QSize HorizontalSizeHintsStorage::GetHeaderSizeMax(int columb) const
{
  return columb >= _MaxsWidth.size() ? QSize(_MaxsWidthOverflow, _MaxHeightHeader) : QSize(_MaxsWidth[columb], _MaxHeightHeader);
}

QSize HorizontalSizeHintsStorage::GetRowsSizeMax(int columb) const
{
  return columb >= _MaxsWidth.size() ? QSize(_MaxsWidthOverflow, _MaxHeightRows) : QSize(_MaxsWidth[columb], _MaxHeightRows);
}
