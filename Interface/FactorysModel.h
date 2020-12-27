#pragma once

#include <QAbstractItemModel>
#include <FactoryCollection.h>

using QT_KeysFactoryType = std::set<ResourceCalculator::KEY_TYPE_FACTORY>;
Q_DECLARE_METATYPE(QT_KeysFactoryType);

class FactorysModel: public QAbstractTableModel
{
public:
  FactorysModel( ResourceCalculator::FactoryTypeCollection& FTC, QObject *parent = 0 );
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
  ResourceCalculator::FactoryTypeCollection& GetTypesData();
  const ResourceCalculator::FactoryTypeCollection& GetTypesData() const;
private:
  ResourceCalculator::FactoryTypeCollection& _FTC;
  ResourceCalculator::FactoryCollection _FC_Edit;
  ResourceCalculator::FactoryTypeCollection _FTC_Edit;
};
