#ifndef ITEM_SELECTED_DIALOG_H
#define ITEM_SELECTED_DIALOG_H

#include <set>

#include <QDialog>
#include <QtWidgets>
#include <QStyledItemDelegate>
#include <QAbstractTableModel>

#include "../../ResourceCalculator/ParamsCollection.h"

enum class ItemSelectedDialogMode{
  ForRecipeSelectItemsRequired,
  ForRecipeSelectItemsResult,
  ForSelectOneItem
};

class ItemSelectedModel : public QAbstractTableModel
{
  Q_OBJECT
public:
  ItemSelectedModel(
    const ResourceCalculator::ParamsCollection &PC,
    ItemSelectedDialogMode Mode,
    ResourceCalculator::KEY_RECIPE recipe_key,
    QObject *parent = 0
  );
  int rowCount(const QModelIndex &parent) const override;
  int columnCount(const QModelIndex &parent) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  ResourceCalculator::CountsItem GetItemData(int Num) const;
  int GetItemRow(ResourceCalculator::KEY_ITEM ItemKey);
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

  ResourceCalculator::KEY_ITEM GetResultOne() const;
  const std::set<ResourceCalculator::CountsItem> &GetResult() const;

  void InserResult(ResourceCalculator::CountsItem CI);
  void EraseResult(ResourceCalculator::CountsItem CI);

private:
  const ItemSelectedDialogMode _Mode;
  const ResourceCalculator::ParamsCollection &_PC;
  QList<ResourceCalculator::CountsItem> _listOfItemsId;
  std::set<ResourceCalculator::CountsItem> _Result;
  ResourceCalculator::KEY_ITEM _ResultOne;

};

class ItemSelectedDelegate : public QStyledItemDelegate
{
  Q_OBJECT
  const ItemSelectedDialogMode _Mode;
  const ItemSelectedModel &_Model;
  ResourceCalculator::ParamsCollection &_PC;
public:
  ItemSelectedDelegate(ResourceCalculator::ParamsCollection &PC, const ItemSelectedModel &_Model, ItemSelectedDialogMode Mode, QObject *parent = 0);
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

class ItemSelectedDialog : public QDialog
{
  Q_OBJECT

public:
  ItemSelectedDialog(
    ResourceCalculator::ParamsCollection & PC,
    ItemSelectedDialogMode Mode,
    ResourceCalculator::KEY_RECIPE recipe_key = ResourceCalculator::KEY_RECIPE::ID_RECIPE_NoFindRecipe,
    QWidget * parent = 0);
  ResourceCalculator::KEY_ITEM GetResultOne() const;
  const std::set<ResourceCalculator::CountsItem> &GetResult() const;

private:
  const ItemSelectedDialogMode _Mode;
  ResourceCalculator::ParamsCollection &_PC;
  ItemSelectedModel *_Model;

private slots:
  void ChangedItems(const QItemSelection & selected, const QItemSelection & deselected);

};

#endif // ITEM_SELECTED_DIALOG_H
