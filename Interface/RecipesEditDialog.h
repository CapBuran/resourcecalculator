#pragma once

#include <QtWidgets>

#include <IconCollection.h>
#include <RecipeCollection.h>
#include <ItemCollection.h>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QTextEdit;
class QLineEdit;
QT_END_NAMESPACE

class RecipeListModel : public QAbstractTableModel
{
  Q_OBJECT
public:
  RecipeListModel(ResourceCalculator::RecipeCollection& RC, const ResourceCalculator::ItemCollection& IC, QObject *parent = 0);
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
  const ResourceCalculator::FactoryTypeCollection& GetFactoryTypes() const;
  const ResourceCalculator::ItemCollection& GetItems() const;
private:
  const ResourceCalculator::ItemCollection& _IC;
  ResourceCalculator::RecipeCollection& _RC;
  ResourceCalculator::RecipeCollection _RC_EDIT;
};

class RecipesEditDelegate: public QStyledItemDelegate
{
  Q_OBJECT
public:
  RecipesEditDelegate(const ResourceCalculator::IconCollection& icons, QObject *parent = 0);
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
  bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
private:
  const ResourceCalculator::IconCollection &_Icons;
};

class RecipesEditDialog : public QDialog
{
  Q_OBJECT
public:
  RecipesEditDialog(
    ResourceCalculator::RecipeCollection& RC,
    const ResourceCalculator::ItemCollection& IC,
    const ResourceCalculator::IconCollection& icons,
    QWidget *parent = 0
  );
private:
  RecipeListModel _Model;
  QTableView *_tableView;
  QPushButton* _removeButton;
private Q_SLOTS:
  void add_item();
  void remove_item();
  void editorEventDelegate(const QModelIndex &index);
  void PushButtonOk();
};
