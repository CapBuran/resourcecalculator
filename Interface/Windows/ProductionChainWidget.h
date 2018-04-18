#ifndef ProductionChainWidget_H
#define ProductionChainWidget_H

#include <QtWidgets>

#include "ProductionChainWidgetBase.h"

class ProductionChainDelegate0: public ProductionChainDelegateBase {
  Q_OBJECT
private:
  const ResourceCalculator::ProductionChainModel &_PCM;
public:
  ProductionChainDelegate0( const ResourceCalculator::ProductionChainModel &PCM, QObject *parent = 0 );
  QSize sizeHint( const QStyleOptionViewItem &option, const QModelIndex &index ) const override;
  QWidget *createEditor( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const override;
  void setEditorData( QWidget *editor, const QModelIndex &index ) const override;
  void setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const override;
  void updateEditorGeometry( QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index ) const override;
  void paint( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const override;
  bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
};

class ProductionChainProxyModel0: public QSortFilterProxyModel {
  Q_OBJECT
public:
  ProductionChainProxyModel0( QObject *parent = 0 );
protected:
  bool filterAcceptsColumn( int source_column, const QModelIndex &source_parent ) const override;
  bool filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const override;
};

class ProductionChainProxyModel1: public QSortFilterProxyModel {
  Q_OBJECT
public:
  ProductionChainProxyModel1( QObject *parent = 0 );
protected:
  bool filterAcceptsColumn( int source_column, const QModelIndex &source_parent ) const override;
  bool filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const override;
};

class ProductionChainProxyModel2: public QSortFilterProxyModel {
  Q_OBJECT
public:
  ProductionChainProxyModel2( QObject *parent = 0 );
protected:
  bool filterAcceptsColumn( int source_column, const QModelIndex &source_parent ) const override;
  bool filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const override;
};

class ProductionChainProxyModel3: public QSortFilterProxyModel {
  Q_OBJECT
public:
  ProductionChainProxyModel3( QObject *parent = 0 );
protected:
  bool filterAcceptsColumn( int source_column, const QModelIndex &source_parent ) const override;
  bool filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const override;
};

class ProductionChainModel: public QAbstractTableModel {
  Q_OBJECT

public:
  ProductionChainModel(ResourceCalculator::ProductionChainModel &PCM, QObject *parent = 0 );

  int rowCount( const QModelIndex &parent ) const override;
  int columnCount( const QModelIndex &parent ) const override;
  QVariant data( const QModelIndex &index, int role ) const override;
  QVariant headerData( int section, Qt::Orientation orientation, int role ) const override;
  Qt::ItemFlags flags( const QModelIndex &index ) const override;

  void FitQuantity();

  bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole ) override;
  const ResourceCalculator::ProductionChainModel& GetPCM() const;

  void Update();

public slots:

  void ModelAllChanged();

Q_SIGNALS:
  void AllDataChanged();

private:

  ResourceCalculator::ProductionChainModel &_PCM;

};

class ProductionChainWidget: public ProductionChainWidgetBase {
  Q_OBJECT
public:

  ProductionChainModel* GetModel(){
    return &_Model;
  }

  ProductionChainWidget(ResourceCalculator::ProductionChainModel &PCM, QWidget *parent = 0 );
  ResourceCalculator::ProductionChainModel &GetPCM();
  ProductionChainWidgetType GetType() const override;
  void UpdateModel() override;
private:
  QTableView *tables[4];
  ProductionChainModel _Model;
  ResourceCalculator::ProductionChainModel &_PCM;
  void _Init();
signals:
  void selectionChanged(const QItemSelection &selected);
private slots:
  void OnResized(int logicalIndex, int oldSize, int newSize);
  void _PushButtonAutoFitQuantity();
};

#endif// ProductionChainWidget_H
