#ifndef RECIPE_EDIT_DIALOG_H
#define RECIPE_EDIT_DIALOG_H

#include <QtWidgets>

#include "../../ResourceCalculator/ParamsCollection.h"

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
  RecipeListModel(ResourceCalculator::ParamsCollection &PC, QObject *parent = 0);
  int rowCount(const QModelIndex &parent) const override;
  int columnCount(const QModelIndex &parent) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
  bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
  bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
  ResourceCalculator::KEY_RECIPE GetRecipeId(int Num) const;
  void Commit();
  void Select();
private:
  ResourceCalculator::ParamsCollection &_PC;
  std::set<ResourceCalculator::KEY_RECIPE> _RecipesToDelete;
  std::map<ResourceCalculator::KEY_RECIPE, ResourceCalculator::Recipe> _RecipesToAdd;
  QList<std::pair<ResourceCalculator::KEY_RECIPE, ResourceCalculator::Recipe> > _listOfRecipesId;
};

class RecipesEditDelegate : public QStyledItemDelegate
{
  Q_OBJECT
public:
  RecipesEditDelegate(ResourceCalculator::ParamsCollection &PC, QObject *parent = 0);
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
  QWidget *RecipesEditDelegate::createEditor( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const override;
  void setEditorData( QWidget *editor, const QModelIndex &index ) const override;
  void updateEditorGeometry( QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index ) const override;
  bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
  void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
private:
  ResourceCalculator::ParamsCollection &_PC;
};

class RecipesEditDialog : public QDialog
{
  Q_OBJECT
public:
  RecipesEditDialog(ResourceCalculator::ParamsCollection &PC, QWidget *parent = 0);
private:
  QTableView *_tableView;
  RecipeListModel _Model;
  ResourceCalculator::ParamsCollection &_PC;
  QPushButton *_removeButton;
  private Q_SLOTS:
    void add_item();
    void remove_item();
    void editorEventDelegate(const QModelIndex &index);
    void PushButtonOk();
};

#endif // RECIPE_EDIT_DIALOG_H
