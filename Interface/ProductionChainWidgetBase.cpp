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
  setFont(QFont(font().family(), 10));
}

void ProductionChainHeaderView::paintSection(QPainter* painter, const QRect& rect, int logicalIndex) const
{
  QString data = model()->headerData(logicalIndex, orientation(), Qt::DisplayRole).toString();

  painter->save();
  painter->rotate(-90);
  QRect rect2(2 - rect.height(), 2 + rect.left(), rect.height() - 4, rect.width() - 4);

  painter->setFont(font());
  painter->drawText(rect2, Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, data);
  painter->restore();
}

int ProductionChainHeaderView::GetMaxHeight() const
{
  const int count = model()->columnCount();
  QFontMetrics fm(font());
  int MaxHeight = 0;
  for (int columb = 0; columb < count; columb++)
  {
    QString DisplayData = model()->headerData(columb, orientation()).toString();
    QRect rect = fm.boundingRect(QRect(0, 0, 150, 50), Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, DisplayData);
    auto width = rect.width();
    if (width > MaxHeight) MaxHeight = width;
  }

  return MaxHeight;
}

QSize ProductionChainHeaderView::sectionSizeFromContents(int logicalIndex) const
{
  QString DisplayData = model()->headerData(logicalIndex, orientation()).toString();
  QFontMetrics fm(font());
  return QSize(fm.height(), fm.horizontalAdvance(DisplayData));
}

ProductionChainDelegateBase::ProductionChainDelegateBase(const ResourceCalculator::ParamsCollection & PC, const HorizontalSizeHintsStorage& sizeHints, QObject * parent)
  : QStyledItemDelegate(parent)
  , _PC(PC)
  , _SizeHints(sizeHints)
{
}

QSize ProductionChainDelegateBase::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  QSize retValue = _SizeHints.GetRowsSizeMax(7);
  if (!retValue.isValid())
  {
    QStyleOptionViewItem optV4(option);
    initStyleOption(&optV4, index);
    QFontMetrics fm(optV4.fontMetrics);
    retValue = QSize(fm.horizontalAdvance(optV4.text) + fm.overlinePos(), fm.height());
    _SizeHints.SetRowsSize(7, retValue);
  }
  return retValue;
}

#pragma endregion DELEGATE

ProductionChainWidgetBase::ProductionChainWidgetBase(const ResourceCalculator::FullItemTree& tree,   QWidget* parent)
  : _PC(tree.GetPC())
  , QSplitter(parent)
{
}
