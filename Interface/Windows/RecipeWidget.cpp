//#include "adddialog.h"

#include "RecipeWidget.h"
//
//RecipeWidget::RecipeWidget(ResourceCalculator::ParamsCollection &PC, QWidget *parent)
//  : QTabWidget(parent), _PC(PC)
//{
//  table = new ProductionChainModelLeft(_PC, this);
//  //newAddressTab = new NewAddressTab(this);
//  //connect(newAddressTab, &NewAddressTab::sendDetails, this, &AddressWidget::addEntry);
//  //addTab(newAddressTab, "Address Book");
//  //setupTabs();
//}

//void RecipeWidget::showAddEntryDialog()
//{
  //AddDialog aDialog;
  //if (aDialog.exec()) {
  //  QString name = aDialog.nameText->text();
  //  QString address = aDialog.addressText->toPlainText();
  //  addEntry(name, address);
  //}
//}
//
//void RecipeWidget::addEntry(QString name, QString address)
//{
  //QList<QPair<QString, QString> >list = table->getList();
  //QPair<QString, QString> pair(name, address);

  //if (!list.contains(pair)) {
  //  table->insertRows(0, 1, QModelIndex());

  //  QModelIndex index = table->index(0, 0, QModelIndex());
  //  table->setData(index, name, Qt::EditRole);
  //  index = table->index(0, 1, QModelIndex());
  //  table->setData(index, address, Qt::EditRole);
  //  removeTab(indexOf(newAddressTab));
  //}
  //else {
  //  QMessageBox::information(this, tr("Duplicate Name"),
  //    tr("The name \"%1\" already exists.").arg(name));
  //}
//}
//
//void RecipeWidget::editEntry()
//{
  //QTableView *temp = static_cast<QTableView*>(currentWidget());
  //QSortFilterProxyModel *proxy = static_cast<QSortFilterProxyModel*>(temp->model());
  //QItemSelectionModel *selectionModel = temp->selectionModel();

  //QModelIndexList indexes = selectionModel->selectedRows();
  //QString name;
  //QString address;
  //int row = -1;

  //foreach(QModelIndex index, indexes) {
  //  row = proxy->mapToSource(index).row();
  //  QModelIndex nameIndex = table->index(row, 0, QModelIndex());
  //  QVariant varName = table->data(nameIndex, Qt::DisplayRole);
  //  name = varName.toString();

  //  QModelIndex addressIndex = table->index(row, 1, QModelIndex());
  //  QVariant varAddr = table->data(addressIndex, Qt::DisplayRole);
  //  address = varAddr.toString();
  //}

  //AddDialog aDialog;
  //aDialog.setWindowTitle(tr("Edit a Contact"));

  //aDialog.nameText->setReadOnly(true);
  //aDialog.nameText->setText(name);
  //aDialog.addressText->setText(address);

  //if (aDialog.exec()) {
  //  QString newAddress = aDialog.addressText->toPlainText();
  //  if (newAddress != address) {
  //    QModelIndex index = table->index(row, 1, QModelIndex());
  //    table->setData(index, newAddress, Qt::EditRole);
  //  }
  //}
//}
//
//void RecipeWidget::removeEntry()
//{
  //QTableView *temp = static_cast<QTableView*>(currentWidget());
  //QSortFilterProxyModel *proxy = static_cast<QSortFilterProxyModel*>(temp->model());
  //QItemSelectionModel *selectionModel = temp->selectionModel();

  //QModelIndexList indexes = selectionModel->selectedRows();

  //foreach(QModelIndex index, indexes) {
  //  int row = proxy->mapToSource(index).row();
  //  table->removeRows(row, 1, QModelIndex());
  //}

  //if (table->rowCount(QModelIndex()) == 0) {
  //  insertTab(0, newAddressTab, "Address Book");
  //}
//}

//void RecipeWidget::setupTabs()
//{
  //QStringList groups;
  //groups << "ABC" << "DEF" << "GHI" << "JKL" << "MNO" << "PQR" << "STU" << "VW" << "XYZ";
  //for (int i = 0; i < groups.size(); ++i) {
  //  QString str = groups.at(i);
  //  QString regExp = QString("^[%1].*").arg(str);
  //  proxyModel = new QSortFilterProxyModel(this);
  //  proxyModel->setSourceModel(table);
  //  proxyModel->setFilterRegExp(QRegExp(regExp, Qt::CaseInsensitive));
  //  proxyModel->setFilterKeyColumn(0);
  //  QTableView *tableView = new QTableView;
  //  tableView->setModel(proxyModel);
  //  tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  //  tableView->horizontalHeader()->setStretchLastSection(true);
  //  tableView->verticalHeader()->hide();
  //  tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  //  tableView->setSelectionMode(QAbstractItemView::SingleSelection);
  //  tableView->setSortingEnabled(true);
  //  connect(tableView->selectionModel(), &QItemSelectionModel::selectionChanged,  this, &RecipeWidget::selectionChanged);
  //  addTab(tableView, str);
  //}
//}
