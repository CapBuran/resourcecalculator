#pragma once

#include <FactoryCollection.h>
#include <IconCollection.h>

#include <QtWidgets>

class FactorysEditDialogModel: public QAbstractTableModel
{
  Q_OBJECT
public:
  FactorysEditDialogModel( ResourceCalculator::FactoryCollection& FC, QObject *parent = 0 );
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
  const ResourceCalculator::Factory& GetCurrentFactory(int row) const;
  const ResourceCalculator::FactoryTypeCollection& GetTypesData() const;
  std::set<ResourceCalculator::KEY_TYPE_FACTORY> GetFactoryTypes(QModelIndexList select) const;
private:
  ResourceCalculator::FactoryCollection& _FC;
  ResourceCalculator::FactoryCollection _FC_Edit;
};

class FactoryTypesViewModel: public QAbstractTableModel
{
  Q_OBJECT
public:
  FactoryTypesViewModel( const ResourceCalculator::FactoryTypeCollection& types, QObject* parent = 0);
  int rowCount(const QModelIndex& parent) const override;
  int columnCount(const QModelIndex& parent) const override;
  QVariant data(const QModelIndex& index, int role) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
  Qt::ItemFlags flags(const QModelIndex& index) const override;
  std::set<ResourceCalculator::KEY_TYPE_FACTORY> GetFactoryTypes(QModelIndexList select) const;
  QSet<int> GetSelectedRows(std::set<ResourceCalculator::KEY_TYPE_FACTORY> result) const;
private:
  const ResourceCalculator::FactoryTypeCollection& _types;
};

class FactorysEditDialogDelegate: public QStyledItemDelegate
{
  Q_OBJECT
public:
  FactorysEditDialogDelegate( const ResourceCalculator::IconCollection& IC, QObject *parent = 0 );
  bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) override;
  void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
private:
  const ResourceCalculator::IconCollection& _IC;
Q_SIGNALS:
  void ThePermissionToDeleteAnObjectHasBeenChanged( const QModelIndex & index, bool IsAllowed ) const;
};

class FactorysTypesSelectedDialog: public QDialog
{
  Q_OBJECT
public:
  FactorysTypesSelectedDialog(FactoryTypesViewModel& model, bool isMultiSelect, QWidget* parent = 0);
  std::set<ResourceCalculator::KEY_TYPE_FACTORY> GetResult() const;
  void SetResult(std::set<ResourceCalculator::KEY_TYPE_FACTORY> result);
private:
  const bool _isMultiSelect;
  QTableView* _tableView;
  FactoryTypesViewModel& _Model;
};

class FactorysEditDialog: public QDialog
{
  Q_OBJECT

public:
  FactorysEditDialog( ResourceCalculator::FactoryCollection& FC, ResourceCalculator::IconCollection& IC, QWidget* parent = 0 );
  void Commit();
private:
  QPushButton* _remoteButton;
  QTableView* _tableView;
  ResourceCalculator::FactoryCollection& _FC;
  const ResourceCalculator::IconCollection& _IC;
  FactorysEditDialogModel _Model;
  FactorysEditDialogDelegate _Delegate;
private Q_SLOTS:
  void add_item();
  void remove_item();
  void ThePermissionToDeleteAnObjectHasBeenChanged( const QModelIndex &index, bool IsAllowed );
  void typesFactory();
};
