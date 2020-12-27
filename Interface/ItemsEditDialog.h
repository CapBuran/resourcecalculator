#pragma once

#include <QtWidgets>

#include <IconCollection.h>
#include <ItemCollection.h>
#include <RecipeCollection.h>
#include <ItemsModel.h>

class ItemEditDelegate : public QStyledItemDelegate
{
  Q_OBJECT

public:
  ItemEditDelegate( const ResourceCalculator::IconCollection& icons, QObject* parent = 0);
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
  bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
Q_SIGNALS:
  void editorEventDelegate(const QModelIndex & index) const;
private:
  const ResourceCalculator::IconCollection& _Icons;
};

class ItemsEditDialog: public QDialog
{
  Q_OBJECT
public:
  ItemsEditDialog(ResourceCalculator::ItemCollection& IC, ResourceCalculator::RecipeCollection& RC, const ResourceCalculator::IconCollection& Icons, QWidget *parent = 0);
private:
  QTableView *_tableView;
  QPushButton *_removeButton;
  ItemsModel _Model;
private Q_SLOTS:
  void add_item();
  void remove_item();
  void editorEventDelegate(const QModelIndex &index);
  void PushButtonOk();
};
