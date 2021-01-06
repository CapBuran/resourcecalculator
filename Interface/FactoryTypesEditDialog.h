#pragma once

#include <QtWidgets>

#include <IconCollection.h>

#include <FactorysTypesModel.h>

class FactoryTypesEditDelegate: public QStyledItemDelegate {
  Q_OBJECT
public:
  FactoryTypesEditDelegate(const ResourceCalculator::IconCollection& icons, QObject* parent = 0);
  void paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const override;
  bool editorEvent( QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index ) override;
private:
  const ResourceCalculator::IconCollection& _Icons;
Q_SIGNALS:
  void editorEventDelegate( const QModelIndex & index ) const;
};

class FactoryTypesEditDialog: public QDialog {
  Q_OBJECT

public:
  FactoryTypesEditDialog( ResourceCalculator::FactoryTypeCollection& FTC, const ResourceCalculator::IconCollection& IC, QWidget* parent = 0 );
  void Commit();
private:

  QPushButton* _remoteButton;
  QTableView* _tableView;

  FactoryTypesModel _Model;
  FactoryTypesEditDelegate _Delegate;

private Q_SLOTS:
  void add_item();
  void remove_item();
  void editorEventDelegate( const QModelIndex &index );
};
