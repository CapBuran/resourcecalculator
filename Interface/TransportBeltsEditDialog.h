#ifndef TRANSPORTBELTS_EDIT_DIALOG_H
#define TRANSPORTBELTS_EDIT_DIALOG_H

#include <QtWidgets>

#include "../../ResourceCalculator/ParamsCollection.h"

class TransportBeltsEditModel: public QAbstractTableModel {
  Q_OBJECT
public:
  TransportBeltsEditModel( ResourceCalculator::ParamsCollection &PC, QObject *parent = 0 );
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
  ResourceCalculator::ParamsCollection &_PC;
  std::set<ResourceCalculator::KEY_TRANSPORT_BELT> _TransportBeltsToDelete;
  std::set<ResourceCalculator::KEY_TRANSPORT_BELT> _TransportBeltsToAdd;
  QList<std::pair<ResourceCalculator::KEY_TRANSPORT_BELT, ResourceCalculator::TransportBelt > > _listOfItemsId;
};

class TransportBeltsEditDelegate : public QStyledItemDelegate {
  Q_OBJECT
public:
  TransportBeltsEditDelegate(const ResourceCalculator::ParamsCollection &PC, QObject *parent = 0);
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
  bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
private:
  const ResourceCalculator::ParamsCollection &_PC;
Q_SIGNALS:
  void editorEventDelegate(const QModelIndex & index) const;
};

class TransportBeltsEditDialog : public QDialog {
  Q_OBJECT
public:
  TransportBeltsEditDialog( ResourceCalculator::ParamsCollection &PC, QWidget *parent = 0 );
private:
  ResourceCalculator::ParamsCollection &_PC;
  QTableView *_tableView;
  TransportBeltsEditModel _Model;
private Q_SLOTS:
  void PushButtonOk();
  void PushButtonAdd();
  void PushButtonRemove();  
};

#endif//TRANSPORTBELTS_EDIT_DIALOG_H
