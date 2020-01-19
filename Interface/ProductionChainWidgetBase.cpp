#include "ProductionChainWidgetBase.h"

#pragma region DELEGATE

QString ToOut(double Value)
{
  if (abs(Value) < 0.0001) {
    Value = 0.0;
  }
  return QString::number(Value, 'g', EpsilonToOut);
}

ProductionChainHeaderView::ProductionChainHeaderView(Qt::Orientation orientation, QWidget * parent) :
  QHeaderView(orientation, parent)
{
  _MaxHeight = 0;
  setFont(QFont(font().family(), 10));
}

void ProductionChainHeaderView::paintSection(QPainter * painter, const QRect & rect, int logicalIndex) const
{
  QString data = model()->headerData(logicalIndex, orientation()).toString();
  painter->rotate(-90);
  painter->setFont(font());
  painter->drawText(-rect.height() + 5, rect.left() + rect.width() / 2, data);
}

QSize ProductionChainHeaderView::sizeHint() const
{
  int count = model()->columnCount();
  int MaxWidth = _MaxHeight;
  int MaxHeight = 0;
  for (int columb = 0; columb < count; columb++) {
    QString DisplayData = model()->headerData(columb, orientation()).toString();
    QFontMetrics fm(font());
    int width = fm.width(DisplayData) + fm.overlinePos();
    if (width >  MaxWidth) {
      MaxWidth = width;
      MaxHeight = fm.height();
    }
  }
  return QSize(MaxHeight, MaxWidth);
}

int ProductionChainHeaderView::GetMaxHeight()
{
  if (_MaxHeight == 0) {
    _MaxHeight = sizeHint().height();
  }
  return _MaxHeight;
}

void ProductionChainHeaderView::SetMaxHeight(int MaxHeight)
{
  _MaxHeight = MaxHeight;
}

QSize ProductionChainHeaderView::sectionSizeFromContents(int logicalIndex) const
{
  QString DisplayData = model()->headerData(logicalIndex, orientation()).toString();
  QFontMetrics fm(font());
  return QSize(fm.height(), fm.width(DisplayData));
}

ProductionChainDelegateBase::ProductionChainDelegateBase(const ResourceCalculator::ParamsCollection & PC, QObject * parent) :
  QStyledItemDelegate(parent), _PC(PC)
{
}

QSize ProductionChainDelegateBase::sizeHint(
  const QStyleOptionViewItem &option,
  const QModelIndex &index) const
{
  QStyleOptionViewItem optV4(option);
  this->initStyleOption(&optV4, index);
  QFontMetrics fm(optV4.fontMetrics);
  return QSize(fm.width(optV4.text) + fm.overlinePos(), fm.height());
}

#pragma endregion DELEGATE

ProductionChainWidgetBase::ProductionChainWidgetBase(const ResourceCalculator::ParamsCollection &PC, QWidget *parent):
  _PC(PC), QSplitter(parent)
{
}
