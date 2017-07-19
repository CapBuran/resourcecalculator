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
  ItemEditDelegate(QObject *parent = 0);
  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
  void setEditorData(QWidget *editor, const QModelIndex &index) const override;
  void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
  void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

class ItemsEditDialog : public QDialog
{
  Q_OBJECT

public:
  ItemsEditDialog(ResourceCalculator::ParamsCollection &PC, QWidget *parent = 0);
  //QLineEdit *nameText;
  //QTextEdit *addressText;

private:

  ResourceCalculator::ParamsCollection &_PC;

  //QLabel *nameLabel;
  //QLabel *addressLabel;
  QPushButton *okButton;
  QPushButton *cancelButton;
};

#endif // ITEM_EDIT_DIALOG_H
