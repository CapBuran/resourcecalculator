#pragma once

#include <QStyledItemDelegate>
#include <QDialog>
#include <QTableView>

#include <FactoryCollection.h>
#include <IconCollection.h>

#include <FactorysTypesModel.h>
#include <FactorysModel.h>

class FactorysEditDialogDelegate: public QStyledItemDelegate
{
  Q_OBJECT
public:
  FactorysEditDialogDelegate( const ResourceCalculator::IconCollection& IC, QObject *parent = 0 );
  bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) override;
  void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
  QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override;
private:
  const ResourceCalculator::IconCollection& _IC;
Q_SIGNALS:
  void ThePermissionToDeleteAnObjectHasBeenChanged( const QModelIndex & index, bool IsAllowed ) const;
};

class FactorysTypesSelectedDialog: public QDialog
{
  Q_OBJECT
public:
  FactorysTypesSelectedDialog(FactoryTypesModel& model, const ResourceCalculator::IconCollection& IC, bool isMultiSelect, QWidget* parent = 0);
  std::set<ResourceCalculator::KEY_TYPE_FACTORY> GetResult() const;
  void SetResult(std::set<ResourceCalculator::KEY_TYPE_FACTORY> result);
private:
  const bool _isMultiSelect;
  QTableView* _tableView;
  FactoryTypesModel& _Model;
  const ResourceCalculator::IconCollection& _IC;
};

class FactorysEditDialog: public QDialog
{
  Q_OBJECT

public:
  FactorysEditDialog( ResourceCalculator::FactoryTypeCollection& FTC, const ResourceCalculator::IconCollection& IC, QWidget* parent = 0 );
  void Commit();
private:
  QPushButton* _remoteButton;
  QTableView* _tableView;
  FactorysModel _Model;
  FactorysEditDialogDelegate _Delegate;
  const ResourceCalculator::IconCollection& _IC;
private Q_SLOTS:
  void add_item();
  void remove_item();
  void ThePermissionToDeleteAnObjectHasBeenChanged( const QModelIndex &index, bool IsAllowed );
  void typesFactory();
};
