#include <FactorysTypesSelectedDialog.h>

FactorysTypesSelectedDialog::FactorysTypesSelectedDialog(
  const ResourceCalculator::FactoryTypeCollection& _FTC,
  const ResourceCalculator::IconCollection& IC,
  bool isMultiSelect,
  QWidget* parent)
  : QDialog(parent)
  , _Model(_FTC)
  , _isMultiSelect(isMultiSelect)
{
  setMinimumSize(700, 600);

  QPushButton* okButton = new QPushButton(tr("OK"), this);
  QPushButton* cancelButton = new QPushButton(tr("Cancel"), this);
  _tableView = new QTableView(this);
  _tableView->setSelectionMode(_isMultiSelect ? QTableView::SelectionMode::MultiSelection : QTableView::SelectionMode::SingleSelection);
  _tableView->setSelectionBehavior(QTableView::SelectionBehavior::SelectRows);
  _tableView->setModel(&_Model);
  _tableView->sortByColumn(1, Qt::AscendingOrder);
  //  _tableView->setItemDelegate(new ItemSelectedDelegate(PC, _Mode));
    //_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  _tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

  //_tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
  //_tableView->setColumnWidth(0, 60);

  QHBoxLayout* buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(okButton);
  buttonLayout->addWidget(cancelButton);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(_tableView);
  mainLayout->addLayout(buttonLayout);
  setLayout(mainLayout);

  connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);
  connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);

  if (_isMultiSelect)
    setWindowTitle(tr("Select an types factory"));
  else
    setWindowTitle(tr("Select an type factory"));
}

std::set<ResourceCalculator::KEY_TYPE_FACTORY> FactorysTypesSelectedDialog::GetResult() const
{
  std::set<ResourceCalculator::TYPE_KEY> indexes;
  const auto indexes_qt = _tableView->selectionModel()->selectedRows();
  for (auto index : indexes_qt)
    indexes.insert(static_cast<ResourceCalculator::TYPE_KEY>(index.row()));
  return _Model.GetRawData().ConvertToKey(indexes);
}

void FactorysTypesSelectedDialog::SetResult(std::set<ResourceCalculator::KEY_TYPE_FACTORY> result)
{
  std::set<ResourceCalculator::TYPE_KEY> indexses = _Model.GetRawData().ConvertToIndex(result);
  for (auto index : indexses)
    _tableView->selectRow(static_cast<int>(index));
}
