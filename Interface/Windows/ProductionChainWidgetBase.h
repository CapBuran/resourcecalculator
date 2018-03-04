#ifndef ProductionChainWidgetBase_H
#define ProductionChainWidgetBase_H

#include "../../ResourceCalculator/ParamsCollection.h"

#include <QtWidgets>

enum class ProductionChainWidgetType
{
  Uncnown,
  ProductionChain,
  TreeItem,
  SummProductionItems
};

class ProductionChainHeaderView: public QHeaderView {
private:
  int _MaxHeight;
public:
  ProductionChainHeaderView( Qt::Orientation orientation, QWidget *parent = Q_NULLPTR );
  void paintSection( QPainter *painter, const QRect &rect, int logicalIndex ) const override;
  QSize sizeHint() const override;
  int GetMaxHeight();
  void SetMaxHeight( int MaxHeight );
  QSize sectionSizeFromContents( int logicalIndex ) const override;
};

class ProductionChainDelegateBase: public QStyledItemDelegate {
  Q_OBJECT
protected:
  const ResourceCalculator::ParamsCollection &_PC;
public:
  ProductionChainDelegateBase( const ResourceCalculator::ParamsCollection &PC, QObject *parent = 0 );
  virtual QSize sizeHint( const QStyleOptionViewItem &option, const QModelIndex &index ) const override;
};

class ProductionChainWidgetBase: public QSplitter {
  Q_OBJECT
public:
  virtual ProductionChainWidgetType GetType() const = 0;
  ProductionChainWidgetBase(const ResourceCalculator::ParamsCollection &PC, QWidget *parent = 0 );
  virtual void UpdateModel() = 0;
protected:
  const ResourceCalculator::ParamsCollection &_PC;
};

#endif// ProductionChainWidgetBase_H
