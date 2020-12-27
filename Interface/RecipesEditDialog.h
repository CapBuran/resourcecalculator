#pragma once

#include <QtWidgets>

#include <IconCollection.h>
#include <RecipeCollection.h>
#include <RecipesModel.h>
#include <ItemsModel.h>
#include <FactorysTypesModel.h>
#include <FactoryCollection.h>

class RecipesEditDelegate: public QStyledItemDelegate
{
  Q_OBJECT
public:
  RecipesEditDelegate(const ResourceCalculator::IconCollection& icons, FactoryTypesModel& factoryTypesModel, ItemsModel& itemModel, QObject *parent = 0);
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
  bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
private:
  ItemsModel& _ItemModel;
  FactoryTypesModel& _FactoryTypesModel;
  const ResourceCalculator::IconCollection &_Icons;
};

class RecipesEditDialog : public QDialog
{
  Q_OBJECT
public:
  RecipesEditDialog(
    ResourceCalculator::RecipeCollection& RC,
    ResourceCalculator::ItemCollection& IC,
    const ResourceCalculator::IconCollection& icons,
    ResourceCalculator::FactoryTypeCollection& FTC,
    QWidget *parent = 0
  );
private:
  FactoryTypesModel _FactoryTypesModel;
  RecipesModel _Model;
  ItemsModel _ItemModel;
  QTableView *_tableView;
  QPushButton* _removeButton;
private Q_SLOTS:
  void add_item();
  void remove_item();
  void editorEventDelegate(const QModelIndex &index);
  void PushButtonOk();
};
