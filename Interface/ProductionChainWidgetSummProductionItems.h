#ifndef ProductionChainWidgetSummProductionItems_H
#define ProductionChainWidgetSummProductionItems_H

#include "ProductionChainTreeModel.h"

#include <QtWidgets>

#include "ProductionChainWidgetBase.h"

class ProductionChainModelSummProductionItems: public QAbstractTableModel {
  Q_OBJECT

public:
  ProductionChainModelSummProductionItems(const ResourceCalculator::ParamsCollection &PC, QObject *parent = 0 );
  int rowCount( const QModelIndex &parent ) const override;
  int columnCount( const QModelIndex &parent ) const override;
  QVariant data( const QModelIndex &index, int role ) const override;
  QVariant headerData( int section, Qt::Orientation orientation, int role ) const override;
  Qt::ItemFlags flags( const QModelIndex &index ) const override;
  void Update();
private:
  QVector<QVector<double> > _DATA;
  QVector<QString> _HorizontalHeader, _VerticalHeader;
  const ResourceCalculator::ParamsCollection &_PC;
};

class ProductionChainDelegateSummProductionItems : public QStyledItemDelegate
{
  Q_OBJECT
    const ResourceCalculator::ParamsCollection &_PC;
public:
  ProductionChainDelegateSummProductionItems(const ResourceCalculator::ParamsCollection &PC, QObject *parent = 0);
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
  QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const override;
};

class ProductionChainWidgetSummProductionItems: public ProductionChainWidgetBase {
  Q_OBJECT
public:
  ProductionChainWidgetSummProductionItems(const ResourceCalculator::ParamsCollection &PC, QWidget *parent = 0 );
  ProductionChainWidgetType GetType() const override;
  void UpdateModel() override;
private:
  ProductionChainModelSummProductionItems _Model;
  ProductionChainTreeModel _ModelTree;
public slots:
  void on_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
};

#endif// ProductionChainWidgetSummProductionItems_H
