
#include "RecipeModel.h"
#include <QComboBox>
#include <QTabWidget>
#include <QItemDelegate>

namespace ChainsCalcModel {

#pragma region RecipeModelNames
  RecipeModelNames::RecipeModelNames(FactorioCalculator::RecipeCollection &RC, QObject *parent)
    : QAbstractTableModel(parent), _RC(RC)
  {
    //listOfPairs = pairs;
  }

  int RecipeModelNames::rowCount(const QModelIndex &parent) const
  {
    Q_UNUSED(parent);
    //return listOfPairs.size();
    return 0;
  }

  int RecipeModelNames::columnCount(const QModelIndex &parent) const
  {
    Q_UNUSED(parent);
    return 1;
  }

  QVariant RecipeModelNames::data(const QModelIndex &index, int role) const
  {
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

  QVariant RecipeModelNames::headerData(int section, Qt::Orientation orientation, int role) const
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

  bool RecipeModelNames::insertRows(int position, int rows, const QModelIndex &index)
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

  bool RecipeModelNames::removeRows(int position, int rows, const QModelIndex &index)
  {
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row) {
      //listOfPairs.removeAt(position);
    }

    endRemoveRows();
    return true;
  }

  bool RecipeModelNames::setData(const QModelIndex &index, const QVariant &value, int role)
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

  Qt::ItemFlags RecipeModelNames::flags(const QModelIndex &index) const
  {
    if (!index.isValid())
      return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
  }

  QList< QPair<QString, QString> > RecipeModelNames::getList()
  {
    QList<QPair<QString, QString> > RetVal;
    return RetVal;
  }
  
#pragma endregion


}