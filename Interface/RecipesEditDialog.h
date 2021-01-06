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
  RecipesEditDelegate(const ResourceCalculator::ItemCollection& IC, const ResourceCalculator::FactoryTypeCollection& FTC, const ResourceCalculator::IconCollection& icons, QObject *parent = 0);
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
  bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
private:
  const ResourceCalculator::ItemCollection& _IC;
  const ResourceCalculator::FactoryTypeCollection& _FTC;
  const ResourceCalculator::IconCollection &_Icons;
};

class RecipesEditDialog: public QDialog
{
  Q_OBJECT
public:
  RecipesEditDialog(
    ResourceCalculator::ItemCollection& IC,
    const ResourceCalculator::FactoryTypeCollection& FTC,
    const ResourceCalculator::IconCollection& icons,
    QWidget *parent = 0
  );
private:
  ResourceCalculator::ItemCollection& _IC;
  const ResourceCalculator::FactoryTypeCollection& _FTC;
  RecipesModel _Model;
  QTableView *_tableView;
  QPushButton* _removeButton;
private Q_SLOTS:
  void add_item();
  void remove_item();
  void editorEventDelegate(const QModelIndex &index);
  void PushButtonOk();
};
