#pragma once

#include <QtWidgets>

#include <IconCollection.h>
#include <ItemCollection.h>
#include <RecipeCollection.h>

enum class ItemSelectedDialogMode{
  ForRecipeSelectItemsRequired,
  ForRecipeSelectItemsResult,
  ForSelectOneItem
};

class ItemSelectedModel: public QAbstractTableModel
{
  Q_OBJECT
public:
  ItemSelectedModel(
    const ResourceCalculator::ItemCollection& IC,
    const std::set<ResourceCalculator::CountsItem>& oldValues,
    QObject *parent = 0
  );
  ItemSelectedModel(
    const ResourceCalculator::ItemCollection& IC,
    QObject *parent = 0
  );
  int rowCount(const QModelIndex &parent) const override;
  int columnCount(const QModelIndex &parent) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

  std::set<ResourceCalculator::CountsItem> GetResult(const QModelIndexList& Rows) const;
private:
  const bool _IsOneItem;
  const ResourceCalculator::ItemCollection& _IC;
  QVector<ResourceCalculator::CountsItem> _Status;
};

class ItemSelectedDelegate : public QStyledItemDelegate
{
  Q_OBJECT
  const ItemSelectedDialogMode _Mode;
  const ResourceCalculator::IconCollection& _Icons;
public:
  ItemSelectedDelegate(const ResourceCalculator::IconCollection& icons, ItemSelectedDialogMode Mode, QObject *parent = 0);
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
  QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const override;
};

class ItemSelectedDialog : public QDialog
{
  Q_OBJECT
public:
  ItemSelectedDialog(
    const ResourceCalculator::ItemCollection& IC,
    const ResourceCalculator::IconCollection& icons,
    ItemSelectedDialogMode Mode,
    const std::set<ResourceCalculator::CountsItem>& select,
    QWidget* parent);
  ItemSelectedDialog(
    const ResourceCalculator::ItemCollection& PC,
    const ResourceCalculator::IconCollection& icons,
    QWidget* parent);
  std::set<ResourceCalculator::CountsItem> GetResult() const;
private:
  ItemSelectedModel _Model;
  QTableView *_tableView;
};
