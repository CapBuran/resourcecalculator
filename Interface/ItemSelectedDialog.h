#pragma once

#include <QtWidgets>

#include <ItemsModel.h>

#include <IconCollection.h>
#include <ItemCollection.h>
#include <RecipeCollection.h>

enum class ItemSelectedDialogMode{
  ForRecipeSelectItemsRequired,
  ForRecipeSelectItemsResult,
  ForSelectOneItem
};

class ItemSelectedDelegate: public QStyledItemDelegate
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
    const ResourceCalculator::ItemCollection& IC,
    const ResourceCalculator::IconCollection& icons,
    QWidget* parent);
  std::set<ResourceCalculator::CountsItem> GetResult() const;
private:
  ItemsModelRead _Model;
  QTableView *_tableView;
};
