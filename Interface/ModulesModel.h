#pragma once

#include <QAbstractItemModel>

#include <ModuleCollection.h>

class ModulesModelRead: public QAbstractTableModel {
  Q_OBJECT
public:
  ModulesModelRead( const ResourceCalculator::ModuleCollection& MC, QObject *parent = 0 );
  int rowCount( const QModelIndex &parent ) const override;
  int columnCount( const QModelIndex &parent ) const override;
  QVariant data( const QModelIndex &index, int role ) const override;
  QVariant headerData( int section, Qt::Orientation orientation, int role ) const override;
  Qt::ItemFlags flags( const QModelIndex &index ) const override;
protected:
  ResourceCalculator::ModuleCollection _MC_EDIT;
};

class ModulesModel : public ModulesModelRead {
  Q_OBJECT
public:
  ModulesModel(ResourceCalculator::ModuleCollection& MC, QObject* parent = 0);
  Qt::ItemFlags flags(const QModelIndex& index) const override;
  bool insertRows(int position, int rows, const QModelIndex& index = QModelIndex()) override;
  bool removeRows(int position, int rows, const QModelIndex& index = QModelIndex()) override;
  bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
  void Commit();
private:
  ResourceCalculator::ModuleCollection& _MC;
};
