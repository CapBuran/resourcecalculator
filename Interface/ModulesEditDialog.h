#ifndef MODULES_EDIT_DIALOG_H
#define MODULES_EDIT_DIALOG_H

#include <QtWidgets>

#include "../../ResourceCalculator/ParamsCollection.h"

class ModulesTypesEditModel: public QAbstractTableModel {
  Q_OBJECT
public:
  ModulesTypesEditModel( ResourceCalculator::ParamsCollection &PC, QObject *parent = 0 );
  int rowCount( const QModelIndex &parent ) const override;
  int columnCount( const QModelIndex &parent ) const override;
  QVariant data( const QModelIndex &index, int role ) const override;
  QVariant headerData( int section, Qt::Orientation orientation, int role ) const override;
  Qt::ItemFlags flags( const QModelIndex &index ) const override;
  bool insertRows( int position, int rows, const QModelIndex &index = QModelIndex() ) override;
  bool removeRows( int position, int rows, const QModelIndex &index = QModelIndex() ) override;
  ResourceCalculator::KEY_MODULE GetDataRowModuleType( int Row ) const;
  bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole ) override;
  void Commit();
  void Select();
  void SetKeyPathForItem( int Row, const std::string & KeyPath );
private:
  ResourceCalculator::ParamsCollection &_PC;
  std::set<ResourceCalculator::KEY_MODULE> _ModulesToDelete;
  std::set<ResourceCalculator::KEY_MODULE> _ModulesToAdd;
  QList<std::pair<ResourceCalculator::KEY_MODULE, ResourceCalculator::Module > > _listOfItemsId;
};

class ModulesEditDelegate: public QStyledItemDelegate {
  Q_OBJECT
public:
  ModulesEditDelegate( const ResourceCalculator::ParamsCollection &PC, QObject *parent = 0 );
  void paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const override;
  bool editorEvent( QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index ) override;
private:
  const ResourceCalculator::ParamsCollection &_PC;
Q_SIGNALS:
  void editorEventDelegate( const QModelIndex & index ) const;
};

class ModulesEditDialog: public QDialog {
  Q_OBJECT
public:
  ModulesEditDialog( ResourceCalculator::ParamsCollection &PC, QWidget *parent = 0 );
private:
  ResourceCalculator::ParamsCollection &_PC;
  QTableView *_tableView;
  ModulesTypesEditModel _Model;
private Q_SLOTS:
  void PushButtonOk();
  void PushButtonAdd();
  void PushButtonRemove();  
};

#endif//MODULES_EDIT_DIALOG_H
