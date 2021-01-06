#pragma once

#include <QtWidgets>

#include <IconCollection.h>

#include <FactorysTypesModel.h>

class FactorysTypesSelectedDialog: public QDialog
{
  Q_OBJECT
public:
  FactorysTypesSelectedDialog(const ResourceCalculator::FactoryTypeCollection&_FTC, const ResourceCalculator::IconCollection& IC, bool isMultiSelect, QWidget* parent = 0);
  std::set<ResourceCalculator::KEY_TYPE_FACTORY> GetResult() const;
  void SetResult(std::set<ResourceCalculator::KEY_TYPE_FACTORY> result);
private:
  const bool _isMultiSelect;
  QTableView* _tableView;
  FactoryTypesModelRead _Model;
};
