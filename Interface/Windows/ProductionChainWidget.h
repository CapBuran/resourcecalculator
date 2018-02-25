#ifndef ProductionChainWidget_H
#define ProductionChainWidget_H

#include "../../ResourceCalculator/ParamsCollection.h"

#include <QtWidgets>

class ProductionChainWidgetHeaderView: public QHeaderView {
private:
  int _MaxHeight;
public:
  ProductionChainWidgetHeaderView( Qt::Orientation orientation, QWidget *parent = Q_NULLPTR );
  void paintSection( QPainter *painter, const QRect &rect, int logicalIndex ) const override;
  QSize sizeHint() const override;
  int GetMaxHeight();
  void SetMaxHeight( int MaxHeight );
  virtual QSize sectionSizeFromContents( int logicalIndex ) const override;
};

class ProductionChainWidgetDelegateBase: public QStyledItemDelegate {
  Q_OBJECT
protected:
  const ResourceCalculator::ParamsCollection &_PC;
  const ResourceCalculator::ProductionChainModel &_PCM;
public:
  ProductionChainWidgetDelegateBase( const ResourceCalculator::ParamsCollection &PC, const ResourceCalculator::ProductionChainModel &PCM, QObject *parent = 0 );
  virtual QSize sizeHint( const QStyleOptionViewItem &option, const QModelIndex &index ) const override;
};


class ProductionChainWidgetDelegate0: public ProductionChainWidgetDelegateBase {
  Q_OBJECT
public:
  ProductionChainWidgetDelegate0( const ResourceCalculator::ParamsCollection &PC, const ResourceCalculator::ProductionChainModel &PCM, QObject *parent = 0 );
  QSize sizeHint( const QStyleOptionViewItem &option, const QModelIndex &index ) const override;
  QWidget *createEditor( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const override;
  void setEditorData( QWidget *editor, const QModelIndex &index ) const override;
  void setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const override;
  void updateEditorGeometry( QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index ) const override;
  void paint( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const override;
  bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
};


class ProductionChainWidgetProxyModel0: public QSortFilterProxyModel {
  Q_OBJECT
public:
  ProductionChainWidgetProxyModel0( QObject *parent = 0 );
protected:
  bool filterAcceptsColumn( int source_column, const QModelIndex &source_parent ) const override;
  bool filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const override;
};

class ProductionChainWidgetProxyModel1: public QSortFilterProxyModel {
  Q_OBJECT
public:
  ProductionChainWidgetProxyModel1( QObject *parent = 0 );
protected:
  bool filterAcceptsColumn( int source_column, const QModelIndex &source_parent ) const override;
  bool filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const override;
};

class ProductionChainWidgetProxyModel2: public QSortFilterProxyModel {
  Q_OBJECT
public:
  ProductionChainWidgetProxyModel2( QObject *parent = 0 );
protected:
  bool filterAcceptsColumn( int source_column, const QModelIndex &source_parent ) const override;
  bool filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const override;
};

class ProductionChainWidgetProxyModel3: public QSortFilterProxyModel {
  Q_OBJECT
public:
  ProductionChainWidgetProxyModel3( QObject *parent = 0 );
protected:
  bool filterAcceptsColumn( int source_column, const QModelIndex &source_parent ) const override;
  bool filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const override;
};

class ProductionChainWidgetModel: public QAbstractTableModel {
  Q_OBJECT

public:
  ProductionChainWidgetModel(ResourceCalculator::ProductionChainModel &PCM, QObject *parent = 0 );

  int rowCount( const QModelIndex &parent ) const override;
  int columnCount( const QModelIndex &parent ) const override;
  QVariant data( const QModelIndex &index, int role ) const override;
  QVariant headerData( int section, Qt::Orientation orientation, int role ) const override;
  Qt::ItemFlags flags( const QModelIndex &index ) const override;

  void FitQuantity();

  bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole ) override;
  const ResourceCalculator::ProductionChainModel& GetPCM() const;

public slots:

  void ModelAllChanged();

Q_SIGNALS:
  void AllDataChanged();

private:

  ResourceCalculator::ProductionChainModel &_PCM;

};

class ProductionChainWidget: public QSplitter {
  Q_OBJECT
public:
  ProductionChainWidget(ResourceCalculator::ProductionChainModel &PCM, QWidget *parent = 0 );
  ResourceCalculator::ProductionChainModel &GetPCM();
private:
  QTableView *tables[4];
  ProductionChainWidgetModel _Model;
  ResourceCalculator::ProductionChainModel &_PCM;
  const ResourceCalculator::ParamsCollection &_PC;
  void _Init();
signals:
  void selectionChanged(const QItemSelection &selected);
private slots:
  void OnResized(int logicalIndex, int oldSize, int newSize);
  void _PushButtonAutoFitQuantity();
};

#endif// ProductionChainWidget_H
