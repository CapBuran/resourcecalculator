#pragma once

#include <QtWidgets>

#include <FactoryCollection.h>
#include <IconCollection.h>

class FactoryTypesEditModel: public QAbstractTableModel {
  Q_OBJECT
public:
  FactoryTypesEditModel( ResourceCalculator::FactoryCollection& FC,  QObject *parent = 0 );
  int rowCount( const QModelIndex &parent ) const override;
  int columnCount( const QModelIndex &parent ) const override;
  QVariant data( const QModelIndex &index, int role ) const override;
  QVariant headerData( int section, Qt::Orientation orientation, int role ) const override;
  Qt::ItemFlags flags( const QModelIndex &index ) const override;
  bool insertRows( int position, int rows, const QModelIndex &index = QModelIndex() ) override;
  bool removeRows( int position, int rows, const QModelIndex &index = QModelIndex() ) override;
  bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole ) override;
  void Commit();
  void Select();

private:
  ResourceCalculator::FactoryCollection& _FC;
  ResourceCalculator::FactoryCollection _FC_Edit;
};

class FactoryTypesEditDelegate: public QStyledItemDelegate {
  Q_OBJECT
public:
  FactoryTypesEditDelegate(ResourceCalculator::FactoryCollection& FC, const ResourceCalculator::IconCollection& IC, const FactoryTypesEditModel &_Model, QObject *parent = 0 );
  void paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const override;
  bool editorEvent( QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index ) override;
private:
  const ResourceCalculator::IconCollection& _IC;
  const FactoryTypesEditModel& _Model;
Q_SIGNALS:
  void editorEventDelegate( const QModelIndex & index ) const;
};

class FactoryTypesEditDialog: public QDialog {
  Q_OBJECT

public:
  FactoryTypesEditDialog( ResourceCalculator::FactoryCollection& FC, const ResourceCalculator::IconCollection& IC, QWidget* parent = 0 );
  void Commit();
private:

  QPushButton* _remoteButton;
  QTableView* _tableView;

  FactoryTypesEditModel _Model;
  FactoryTypesEditDelegate _Delegate;

private Q_SLOTS:
  void add_item();
  void remove_item();
  void editorEventDelegate( const QModelIndex &index );
};
