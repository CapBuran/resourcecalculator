#ifndef MODULE_SELECTED_DIALOG_H
#define MODULE_SELECTED_DIALOG_H

#include <QtWidgets>

#include "../../ResourceCalculator/ParamsCollection.h"

class ModuleSelectModel : public QAbstractTableModel
{
  Q_OBJECT
public:
  ModuleSelectModel(
    const ResourceCalculator::ParamsCollection &PC,
    ResourceCalculator::KEY_MODULE OldResult,
    QObject *parent = 0
  );
  int rowCount(const QModelIndex &parent) const override;
  int columnCount(const QModelIndex &parent) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  ResourceCalculator::KEY_MODULE GetResult() const;
private:
  QList<ResourceCalculator::Module> _listOfItemsId;
  ResourceCalculator::KEY_MODULE _Result;
};

class ModuleSelectDelegate : public QStyledItemDelegate
{
  Q_OBJECT
    const ResourceCalculator::ParamsCollection &_PC;
public:
  ModuleSelectDelegate(const ResourceCalculator::ParamsCollection &PC, QObject *parent = 0);
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
  bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
};

class ModuleSelectDialog : public QDialog
{
  Q_OBJECT
public:
  ModuleSelectDialog(const ResourceCalculator::ParamsCollection & PC, ResourceCalculator::KEY_MODULE OldResult, QWidget * parent = 0);
  ResourceCalculator::KEY_MODULE GetResult() const;
private:
  ModuleSelectModel _Model;
  QTableView *_tableView;
};

#endif // MODULE_SELECTED_DIALOG_H
