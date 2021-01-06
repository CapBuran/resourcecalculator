#pragma once

#include <QAbstractItemModel>
#include <FactoryCollection.h>

class FactoryTypesModelRead: public QAbstractTableModel
{
  Q_OBJECT
public:
  FactoryTypesModelRead( const ResourceCalculator::FactoryTypeCollection& FTC, QObject *parent = 0 );
  int rowCount( const QModelIndex &parent ) const override;
  int columnCount( const QModelIndex &parent ) const override;
  QVariant data( const QModelIndex &index, int role ) const override;
  QVariant headerData( int section, Qt::Orientation orientation, int role ) const override;
  Qt::ItemFlags flags( const QModelIndex &index ) const override;
  const ResourceCalculator::FactoryTypeCollection& GetRawData() const { return _FTC_Edit; }
protected:
  ResourceCalculator::FactoryTypeCollection _FTC_Edit;
};

class FactoryTypesModel: public FactoryTypesModelRead
{
  Q_OBJECT
public:
  FactoryTypesModel(ResourceCalculator::FactoryTypeCollection& FTC, QObject* parent = 0);
  Qt::ItemFlags flags(const QModelIndex& index) const override;
  bool insertRows(int position, int rows, const QModelIndex& index = QModelIndex()) override;
  bool removeRows(int position, int rows, const QModelIndex& index = QModelIndex()) override;
  bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
  void Commit();
private:
  ResourceCalculator::FactoryTypeCollection& _FTC;
};
