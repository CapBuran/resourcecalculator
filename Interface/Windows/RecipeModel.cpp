
#include "RecipeModel.h"
#include <QComboBox>
#include <QTabWidget>
#include <QItemDelegate>

#pragma region Production—hainModelLeft
ProductionChainModelLeft::ProductionChainModelLeft(ResourceCalculator::ParamsCollection &PC, QObject *parent)
    : QAbstractTableModel(parent), _PC(PC)
  {
    //listOfPairs = pairs;
  }

  int ProductionChainModelLeft::rowCount(const QModelIndex &parent) const
  {
    Q_UNUSED(parent);
    //return listOfPairs.size();
    return 0;
  }

  int ProductionChainModelLeft::columnCount(const QModelIndex &parent) const
  {
    Q_UNUSED(parent);
    return 1;
  }

  QVariant ProductionChainModelLeft::data(const QModelIndex &index, int role) const
  {
    //this->dataChanged()
    if (!index.isValid())
      return QVariant();

    //if (index.row() >= listOfPairs.size() || index.row() < 0)
    //    return QVariant();

    //if (role == Qt::DisplayRole) {
    //    QPair<QString, QString> pair = listOfPairs.at(index.row());

    //    if (index.column() == 0)
    //        return pair.first;
    //    else if (index.column() == 1)
    //        return pair.second;
    //}
    return QVariant();
  }

  QVariant ProductionChainModelLeft::headerData(int section, Qt::Orientation orientation, int role) const
  {
    if (role != Qt::DisplayRole)
      return QVariant();

    if (orientation == Qt::Horizontal) {
      switch (section) {
      case 0:
        return tr("Name");

      case 1:
        return tr("Address");

      default:
        return QVariant();
      }
    }
    return QVariant();
  }

  bool ProductionChainModelLeft::insertRows(int position, int rows, const QModelIndex &index)
  {
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row) {
      QPair<QString, QString> pair(" ", " ");
      //listOfPairs.insert(position, pair);
    }

    endInsertRows();
    return true;
  }

  bool ProductionChainModelLeft::removeRows(int position, int rows, const QModelIndex &index)
  {
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row) {
      //listOfPairs.removeAt(position);
    }

    endRemoveRows();
    return true;
  }

  bool ProductionChainModelLeft::setData(const QModelIndex &index, const QVariant &value, int role)
  {
    if (index.isValid() && role == Qt::EditRole) {
      int row = index.row();

      //QPair<QString, QString> p = listOfPairs.value(row);

      //if (index.column() == 0)
      //    p.first = value.toString();
      //else if (index.column() == 1)
      //    p.second = value.toString();
      //else
      //    return false;

      //listOfPairs.replace(row, p);
      //emit(dataChanged(index, index));

      return true;
    }

    return false;
  }

  Qt::ItemFlags ProductionChainModelLeft::flags(const QModelIndex &index) const
  {
    if (!index.isValid())
      return Qt::ItemIsEnabled;
    
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
  }

  
#pragma endregion


#pragma region Production—hainModelRight

  int ProductionChainModelRight::rowCount(const QModelIndex & parent) const
  {
    return 0;
  }

  int ProductionChainModelRight::columnCount(const QModelIndex & parent) const
  {
    return 5;
  }

  QVariant ProductionChainModelRight::data(const QModelIndex & index, int role) const
  {
    return QVariant();
  }

  QVariant ProductionChainModelRight::headerData(int section, Qt::Orientation orientation, int role) const
  {
    
    return QVariant();
  }

  Qt::ItemFlags ProductionChainModelRight::flags(const QModelIndex & index) const
  {
    return Qt::ItemFlags();
  }

  bool ProductionChainModelRight::setData(const QModelIndex & index, const QVariant & value, int role)
  {
    return false;
  }

  bool ProductionChainModelRight::insertRows(int position, int rows, const QModelIndex & index)
  {
    return false;
  }

  bool ProductionChainModelRight::removeRows(int position, int rows, const QModelIndex & index)
  {
    return false;
  }

  ResourceCalculator::KEY_RECIPE ProductionChainModelRight::GetRecipeId(int Num) const
  {
    return ResourceCalculator::KEY_RECIPE();
  }

#pragma endregion
