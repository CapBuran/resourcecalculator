#ifndef MODULES_SELECTED_DIALOG_H
#define MODULES_SELECTED_DIALOG_H

#include <QtWidgets>

#include "../../ResourceCalculator/ParamsCollection.h"

class ModulesSelectModel : public QAbstractTableModel
{
  Q_OBJECT
public:
  ModulesSelectModel(
    const ResourceCalculator::ParamsCollection &PC,
    int CountSlotsForModules,
    QObject *parent = 0
  );
  ModulesSelectModel(
    const ResourceCalculator::ParamsCollection &PC,
    const ResourceCalculator::FactoryModules &OldResult,
    QObject *parent = 0
  );
  int rowCount(const QModelIndex &parent) const override;
  int columnCount(const QModelIndex &parent) const override;
  QVariant data(const QModelIndex &index, int role) const override;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
  const ResourceCalculator::FactoryModules &GetResult() const;
private:
  const ResourceCalculator::ParamsCollection &_PC;
  ResourceCalculator::FactoryModules _Result;
};

class ModulesSelectDelegate : public QStyledItemDelegate
{
  Q_OBJECT
  const ResourceCalculator::ParamsCollection &_PC;
public:
  ModulesSelectDelegate(const ResourceCalculator::ParamsCollection &PC, QObject *parent = 0);
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
  bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
};

class ModulesSelectDialog : public QDialog
{
  Q_OBJECT
public:
  ModulesSelectDialog(
    const ResourceCalculator::ParamsCollection & PC,
    int CountSlotsForModules,
    QWidget * parent = 0);
  ModulesSelectDialog(
    const ResourceCalculator::ParamsCollection & PC,
    const ResourceCalculator::FactoryModules &OldResult,
    QWidget * parent = 0);
  const ResourceCalculator::FactoryModules &GetResult() const;
private:
  const ResourceCalculator::ParamsCollection &_PC;
  ModulesSelectModel _Model;
  QTableView *_tableView;
};

#endif // MODULES_SELECTED_DIALOG_H
