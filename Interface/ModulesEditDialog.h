#ifndef MODULES_EDIT_DIALOG_H
#define MODULES_EDIT_DIALOG_H

#include <QtWidgets>

#include "../../ResourceCalculator/ParamsCollection.h"

class ModulesTypesEditModel: public QAbstractTableModel {
  Q_OBJECT
public:
  ModulesTypesEditModel( ResourceCalculator::ModuleCollection& MC, QObject *parent = 0 );
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
  ResourceCalculator::ModuleCollection _MC_EDIT;
  ResourceCalculator::ModuleCollection& _MC;
};

class ModulesEditDelegate: public QStyledItemDelegate {
  Q_OBJECT
public:
  ModulesEditDelegate(const ResourceCalculator::ModuleCollection& MC, const ResourceCalculator::IconCollection& icons, QObject *parent = 0 );
  void paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const override;
  bool editorEvent( QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index ) override;
private:
  const ResourceCalculator::ModuleCollection& _MC;
  const ResourceCalculator::IconCollection& _Icons;
Q_SIGNALS:
  void editorEventDelegate( const QModelIndex & index ) const;
};

class ModulesEditDialog: public QDialog {
  Q_OBJECT
public:
  ModulesEditDialog(ResourceCalculator::ModuleCollection& MC, const ResourceCalculator::IconCollection& icons, QWidget *parent = 0 );
private:
  QTableView *_tableView;
  ModulesTypesEditModel _Model;
private Q_SLOTS:
  void PushButtonOk();
  void PushButtonAdd();
  void PushButtonRemove();  
};

#endif//MODULES_EDIT_DIALOG_H
