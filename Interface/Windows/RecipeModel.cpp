
#include "RecipeModel.h"

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

#pragma region RecipeListModel

  RecipeListModel::RecipeListModel(FactorioCalculator::RecipeCollection &RC, QObject *parent)
    : QAbstractTableModel(parent), _RC(RC)
  {
    //listOfPairs = pairs;
  }

  int RecipeListModel::rowCount(const QModelIndex &parent) const
  {
    Q_UNUSED(parent);
    return _RC.GetData().size();
  }

  int RecipeListModel::columnCount(const QModelIndex &parent) const
  {
    Q_UNUSED(parent);
    //1) name
    //2) Time
    //3) Зависимость
    //4) Factorys allowes
    return 4;
  }

  QVariant RecipeListModel::data(const QModelIndex &index, int role) const
  {
    using namespace FactorioCalculator;

    if (!index.isValid())
      return QVariant();

    const std::map<KEY_RECIPE, Recipe> &DATA = _RC.GetData();

    if (index.row() >= DATA.size() || index.row() < 0)
        return QVariant();

    const int row = index.row();
    
    std::map<KEY_RECIPE, Recipe>::const_iterator it = DATA.cbegin();
    for (size_t i = 0; i < row; i++, it++);

    if (role == Qt::DisplayRole) {
      QString retval;
      RecipeParams RP = it->second.GetRecipeParams();

      switch (index.column())
      {
      case 0:
        retval = QString::fromUtf8(it->second.GetName().c_str());
        return retval;
        break;
      case 1:
        retval = QString::number(RP.Time);
        return retval;
        break;
      case 2:
        retval = "TODO: Зависимость";
        return retval;
        break;
      case 3:
        retval = "TODO: Factorys allowes";
        return retval;
        break;
      default:
        return QVariant();
        break;
      }
    }

    return QVariant();
  }

  QVariant RecipeListModel::headerData(int section, Qt::Orientation orientation, int role) const
  {
    if (role != Qt::DisplayRole)
      return QVariant();

    if (orientation == Qt::Horizontal) {
      switch (section) {
      case 0:
        return tr("Recipe Name");
      case 1:
        return tr("Recipe time"); 
      case 2:
        return tr("Recipe Зависимость");
      case 3:
        return tr("Factorys allowes");
      default:
        return QVariant();
      }
    }
    return QVariant();
  }

  bool RecipeListModel::insertRows(int position, int rows, const QModelIndex &index)
  {
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row) {
      //QPair<QString, QString> pair(" ", " ");
      //listOfPairs.insert(position, pair);
    }

    endInsertRows();
    return true;
  }

  bool RecipeListModel::removeRows(int position, int rows, const QModelIndex &index)
  {
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row) {
      //listOfPairs.removeAt(position);
    }

    endRemoveRows();
    return true;
  }

  bool RecipeListModel::setData(const QModelIndex &index, const QVariant &value, int role)
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

  Qt::ItemFlags RecipeListModel::flags(const QModelIndex &index) const
  {
    if (!index.isValid())
      return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
  }

  //QList< QPair<QString, QString> > RecipeListModel::getList()
  //{
  //  QList<QPair<QString, QString> > RetVal;
  //  return RetVal;
  //}

#pragma endregion

}