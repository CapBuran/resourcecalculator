#ifndef FACTORY_TYPES_EDIT_DIALOG_H
#define FACTORY_TYPES_EDIT_DIALOG_H

#include <QtWidgets>

#include "../../ResourceCalculator/ParamsCollection.h"

enum class FactoryTypesEditDialogMode {
  ForRecipeSelectItemsRequired,
  ForRecipeSelectItemsResult,
  ForSelectOneItem
};

class FactoryTypesEditModel: public QAbstractTableModel {
  Q_OBJECT
public:
  FactoryTypesEditModel( ResourceCalculator::ParamsCollection &PC,  QObject *parent = 0 );
  int rowCount( const QModelIndex &parent ) const override;
  int columnCount( const QModelIndex &parent ) const override;
  QVariant data( const QModelIndex &index, int role ) const override;
  QVariant headerData( int section, Qt::Orientation orientation, int role ) const override;
  Qt::ItemFlags flags( const QModelIndex &index ) const override;
  bool insertRows( int position, int rows, const QModelIndex &index = QModelIndex() ) override;
  bool removeRows( int position, int rows, const QModelIndex &index = QModelIndex() ) override;
  ResourceCalculator::FactoryType GetDataRow( int Row ) const;
  ResourceCalculator::KEY_TYPE_FACTORY GetDataRowType( int Row ) const;
  bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole ) override;
  void Commit();
  void Select();

private:
  ResourceCalculator::ParamsCollection &_PC;
  std::list<ResourceCalculator::KEY_TYPE_FACTORY> _FactoryTypesKeyToDelete;
  std::map<ResourceCalculator::KEY_TYPE_FACTORY, ResourceCalculator::FactoryType > _FactoryTypesKeyToAdd;
  QList<std::pair<ResourceCalculator::KEY_TYPE_FACTORY, ResourceCalculator::FactoryType > > _listOfItemsId;
};

class FactoryTypesEditDelegate: public QStyledItemDelegate {
  Q_OBJECT
  const FactoryTypesEditModel &_Model;
  ResourceCalculator::ParamsCollection &_PC;
public:
  FactoryTypesEditDelegate( ResourceCalculator::ParamsCollection &PC, const FactoryTypesEditModel &_Model, QObject *parent = 0 );
  void paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const override;
  bool editorEvent( QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index ) override;

Q_SIGNALS:
  void editorEventDelegate( const QModelIndex & index ) const;
};

class FactoryTypesEditDialog: public QDialog {
  Q_OBJECT

public:
  FactoryTypesEditDialog( ResourceCalculator::ParamsCollection & PC, QWidget * parent = 0 );
  void Commit();
private:

  ResourceCalculator::ParamsCollection &_PC;
  FactoryTypesEditModel *_Model;
  QPushButton *_remoteButton;
  QTableView *_tableView;

private Q_SLOTS:
  void add_item();
  void remove_item();
  void editorEventDelegate( const QModelIndex &index );

};

#endif // FACTORY_TYPES_EDIT_DIALOG_H
