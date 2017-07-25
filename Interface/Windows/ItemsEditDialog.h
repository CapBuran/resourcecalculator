#ifndef ITEM_EDIT_DIALOG_H
#define ITEM_EDIT_DIALOG_H

#include <QDialog>
#include <QStyledItemDelegate>
#include <QAbstractTableModel>

#include "../../ResourceCalculator/ParamsCollection.h"

class ItemsEditModel : public QAbstractTableModel
{
  Q_OBJECT
public:
  ItemsEditModel(ResourceCalculator::ParamsCollection &PC, QObject *parent = 0);
  int rowCount(const QModelIndex &parent) const override;
  int columnCount(const QModelIndex &parent) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
  bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
  bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
  ResourceCalculator::KEY_ITEM GetItemId(int Num) const;
private:
  ResourceCalculator::ParamsCollection &_PC;
  QList<ResourceCalculator::KEY_ITEM> _listOfItemsId;
};

class ItemEditDelegate : public QStyledItemDelegate
{
  Q_OBJECT

public:
  ItemEditDelegate(ResourceCalculator::ParamsCollection &PC, const ItemsEditModel &Model, QObject *parent = 0);
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
  bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
private:
  const ItemsEditModel &_Model;
  ResourceCalculator::ParamsCollection &_PC;
};

class ItemsEditDialog : public QDialog
{
  Q_OBJECT

public:
  ItemsEditDialog(ResourceCalculator::ParamsCollection &PC, QWidget *parent = 0);

private:
  ResourceCalculator::ParamsCollection &_PC;
};

#endif // ITEM_EDIT_DIALOG_H
