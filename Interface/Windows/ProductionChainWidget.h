#ifndef ProductionChainWidget_H
#define ProductionChainWidget_H

#include "../../ResourceCalculator/ParamsCollection.h"

#include <QtWidgets>

class ProductionChainWidgetHeaderView: public QHeaderView {
public:
  ProductionChainWidgetHeaderView( Qt::Orientation orientation, QWidget *parent = Q_NULLPTR );
  void paintSection( QPainter *painter, const QRect &rect, int logicalIndex ) const override;
  QSize sizeHint() const override;
  virtual QSize sectionSizeFromContents( int logicalIndex ) const override;

};

class ProductionChainWidgetDelegate: public QStyledItemDelegate {
  Q_OBJECT
  const ResourceCalculator::ParamsCollection &_PC;
public:
  ProductionChainWidgetDelegate( const ResourceCalculator::ParamsCollection &PC, QObject *parent = 0 );
  QSize sizeHint( const QStyleOptionViewItem &option, const QModelIndex &index ) const override;
  void paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const override;
};


class ProductionChainWidgetModel: public QAbstractTableModel {
  Q_OBJECT

public:
  ProductionChainWidgetModel( const ResourceCalculator::ParamsCollection &PC, QObject *parent = 0 );

  int rowCount( const QModelIndex &parent ) const override;
  int columnCount( const QModelIndex &parent ) const override;
  QVariant data( const QModelIndex &index, int role ) const override;
  QVariant headerData( int section, Qt::Orientation orientation, int role ) const override;
  Qt::ItemFlags flags( const QModelIndex &index ) const override;
  //bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole ) override;

  //int SetItemKey( ResourceCalculator::KEY_ITEM );


private:

  ResourceCalculator::ProductionChainModel _PCM;

};




class ProductionChainWidget: public QTabWidget {
  Q_OBJECT

public:
  ProductionChainWidget( const ResourceCalculator::ParamsCollection &PC, QWidget *parent = 0 );

public slots:
  void showAddEntryDialog();
  void addEntry( QString name, QString address );
  void editEntry();
  void removeEntry();

public slots:
  void AddTab();
  

signals:
  void selectionChanged( const QItemSelection &selected );

private:
  void setupTabs();
  const ResourceCalculator::ParamsCollection &_PC;
};

#endif// ProductionChainWidget_H
