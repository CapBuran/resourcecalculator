#pragma once

#include <QAbstractItemModel>
#include <FactoryCollection.h>

class FactoryTypesModel: public QAbstractTableModel
{
  Q_OBJECT
public:
  FactoryTypesModel( ResourceCalculator::FactoryTypeCollection& FTC, QObject *parent = 0 );
  int rowCount( const QModelIndex &parent ) const override;
  int columnCount( const QModelIndex &parent ) const override;
  QVariant data( const QModelIndex &index, int role ) const override;
  QVariant headerData( int section, Qt::Orientation orientation, int role ) const override;
  Qt::ItemFlags flags( const QModelIndex &index ) const override;
  bool insertRows( int position, int rows, const QModelIndex &index = QModelIndex() ) override;
  bool removeRows( int position, int rows, const QModelIndex &index = QModelIndex() ) override;
  bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::EditRole ) override;
  void Commit();
  void Select();
  const ResourceCalculator::FactoryTypeCollection& GetRawData() const { return _FTC_Edit; }
private:
  ResourceCalculator::FactoryTypeCollection& _FTC;
  ResourceCalculator::FactoryTypeCollection _FTC_Edit;
};
