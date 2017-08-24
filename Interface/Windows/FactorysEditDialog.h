#ifndef FACTORYS_EDIT_DIALOG_H
#define FACTORYS_EDIT_DIALOG_H

#include "../../ResourceCalculator/ParamsCollection.h"

#include <QtWidgets>

class FactoryTypesViewModel: public QAbstractTableModel {
  Q_OBJECT
public:
  FactoryTypesViewModel( const ResourceCalculator::ParamsCollection &PC, QObject *parent = 0 );
  int rowCount( const QModelIndex &parent ) const override;
  int columnCount( const QModelIndex &parent ) const override;
  QVariant data( const QModelIndex &index, int role ) const override;
  QVariant headerData( int section, Qt::Orientation orientation, int role ) const override;
  Qt::ItemFlags flags( const QModelIndex &index ) const override;
  int GetRowFromFactoryType( ResourceCalculator::KEY_TYPE_FACTORY KFT ) const;
  void Select();
  std::pair<ResourceCalculator::KEY_TYPE_FACTORY, ResourceCalculator::FactoryType > GetDataRow( int Row ) const;
private:
  const ResourceCalculator::ParamsCollection &_PC;
  QList<std::pair<ResourceCalculator::KEY_TYPE_FACTORY, ResourceCalculator::FactoryType > > _listOfItemsId;
};

class FactorysEditDialogModel: public QAbstractTableModel {
  Q_OBJECT
public:
  FactorysEditDialogModel( ResourceCalculator::ParamsCollection &PC, QObject *parent = 0 );
  int rowCount( const QModelIndex &parent ) const override;
  int columnCount( const QModelIndex &parent ) const override;
  QVariant data( const QModelIndex &index, int role ) const override;
  QVariant headerData( int section, Qt::Orientation orientation, int role ) const override;
  Qt::ItemFlags flags( const QModelIndex &index ) const override;
  bool insertRows( int position, int rows, const QModelIndex &index = QModelIndex() ) override;
  bool removeRows( int position, int rows, const QModelIndex &index = QModelIndex() ) override;
  ResourceCalculator::KEY_FACTORY GetDataRow( int Row ) const;
  const ResourceCalculator::Factory & GetCurrentFactory( int Row ) const;
  bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole ) override;
  void Commit();
  void Select();

private:
  ResourceCalculator::ParamsCollection &_PC;
  std::list<ResourceCalculator::KEY_FACTORY> _FactorysKeyToDelete;
  std::map<ResourceCalculator::KEY_FACTORY, ResourceCalculator::Factory > _FactorysKeyToAdd;
  QList<std::pair<ResourceCalculator::KEY_FACTORY, ResourceCalculator::Factory > > _listOfItemsId;
};

class FactorysEditDialogDelegate: public QStyledItemDelegate {
  Q_OBJECT
  const ResourceCalculator::ParamsCollection &_PC; 
  const FactorysEditDialogModel &_Model;
  const FactoryTypesViewModel &_modelForComboBox;
  const QItemSelectionModel *_ItemSelectionModel;
  
public:
  FactorysEditDialogDelegate(
    ResourceCalculator::ParamsCollection &PC,
    const FactoryTypesViewModel &modelForComboBox,
    const FactorysEditDialogModel &_Model, 
    const QItemSelectionModel *ItemSelectionModel,
    QObject *parent = 0 );

  void paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const override;
  bool editorEvent( QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index ) override;

  QWidget *createEditor( QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &/* index */ ) const;
  void setEditorData( QWidget *editor, const QModelIndex &index ) const;
  void setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const;
  void updateEditorGeometry( QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */ ) const;

Q_SIGNALS:
  void ThePermissionToDeleteAnObjectHasBeenChanged( const QModelIndex & index, bool IsAllowed ) const;
};

class FactorysEditDialog: public QDialog {
  Q_OBJECT

public:
  FactorysEditDialog( ResourceCalculator::ParamsCollection & PC, QWidget * parent = 0 );
  void Commit();
private:

  ResourceCalculator::ParamsCollection &_PC;
  FactorysEditDialogModel *_Model;
  QPushButton *_remoteButton;
  QTableView *_tableView;
  FactoryTypesViewModel *_modelForComboBox;

private Q_SLOTS:
  void add_item();
  void remove_item();
  void ThePermissionToDeleteAnObjectHasBeenChanged( const QModelIndex &index, bool IsAllowed );

  void typesFactory();

};

#endif // FACTORYS_EDIT_DIALOG_H
