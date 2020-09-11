#pragma once

#include <QtWidgets>

#include <IconCollection.h>
#include <ItemCollection.h>
#include <RecipeCollection.h>

class ItemsEditModel : public QAbstractTableModel
{
  Q_OBJECT
public:
  ItemsEditModel(ResourceCalculator::ItemCollection& IC, QObject *parent = 0);
  int rowCount(const QModelIndex &parent) const override;
  int columnCount(const QModelIndex &parent) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
  bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
  bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
  void Commit();
  void Select();
private:
  ResourceCalculator::ItemCollection& _IC;
  ResourceCalculator::RecipeCollection _RC_EDIT;
  ResourceCalculator::ItemCollection _IC_EDIT;
};

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
  ItemsEditDialog(ResourceCalculator::ItemCollection& IC, const ResourceCalculator::IconCollection& Icons, QWidget *parent = 0);
private:
  QTableView *_tableView;
  QPushButton *_removeButton;
  ItemsEditModel _Model;
private Q_SLOTS:
  void add_item();
  void remove_item();
  void editorEventDelegate(const QModelIndex &index);
  void PushButtonOk();
};
