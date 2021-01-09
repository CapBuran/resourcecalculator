#pragma once

#include <SizeHintsStorage.h>

#include <ProductionChainTree.h>

#include <QtWidgets>

#define EpsilonToOut 4

QString ToOut(double Value);

enum class ProductionChainWidgetType
{
  Uncnown,
  ProductionChain,
  TreeItem,
  SummProductionItems
};

class ProductionChainHeaderView: public QHeaderView {
public:
  ProductionChainHeaderView( Qt::Orientation orientation, QWidget *parent = Q_NULLPTR );
  void paintSection( QPainter *painter, const QRect &rect, int logicalIndex ) const override;
  int GetMaxHeight() const;
  QSize sectionSizeFromContents( int logicalIndex ) const override;
};

class ProductionChainDelegateBase: public QStyledItemDelegate {
  Q_OBJECT
protected:
  const ResourceCalculator::ParamsCollection &_PC;
  const HorizontalSizeHintsStorage& _SizeHints;
public:
  ProductionChainDelegateBase( const ResourceCalculator::ParamsCollection &PC, const HorizontalSizeHintsStorage& sizeHints, QObject *parent = 0 );
  virtual QSize sizeHint( const QStyleOptionViewItem &option, const QModelIndex &index ) const override;
};

class ProductionChainWidgetBase: public QSplitter {
  Q_OBJECT
public:
  virtual ProductionChainWidgetType GetType() const = 0;
  ProductionChainWidgetBase(const ResourceCalculator::FullItemTree& tree, QWidget *parent = 0 );
  virtual void UpdateModel() = 0;
protected:
  const ResourceCalculator::ParamsCollection &_PC;
};
