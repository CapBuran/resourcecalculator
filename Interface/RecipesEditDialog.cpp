#include "RecipesEditDialog.h"
#include "ItemSelectedDialog.h"
#include "IconSelectedDialog.h"
#include "FactorysEditDialog.h"

#pragma region DELEGATE

RecipesEditDelegate::RecipesEditDelegate(
  const ResourceCalculator::IconCollection& icons,
  FactoryTypesModel& factoryTypesModel, 
  ItemsModel& itemModel,
  QObject *parent)
  : QStyledItemDelegate(parent)
  , _Icons(icons)
  , _FactoryTypesModel(factoryTypesModel)
  , _ItemModel(itemModel)
{
}

void RecipesEditDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  switch (index.column()) {
  case 0:
  {
    QString IcopPath = index.data().toString();
    const ResourceCalculator::Icon &icon = _Icons.GetIcon(IcopPath.toStdString());
    if (icon.GetRawData().size() > 0) {
      QPixmap pixmap;
      pixmap.loadFromData((uchar*)&icon.GetRawData()[0], (uint)icon.GetRawData().size());
      const int MinCoord = std::min(option.rect.width(), option.rect.height());
      const int MaxCoord = std::max(option.rect.width(), option.rect.height());
      const int Sub1 = (MaxCoord - MinCoord) / 2;
      QRect rect;
      if (MaxCoord == option.rect.width()) {
        rect.setCoords(
          option.rect.left() + Sub1, option.rect.top(),
          option.rect.left() + Sub1 + MinCoord, option.rect.bottom());
      } else {
        rect.setCoords(
          option.rect.left(), option.rect.top() + Sub1,
          option.rect.right(), option.rect.top() + Sub1 + MinCoord);
      }
      painter->drawPixmap(rect, pixmap);
    }
    break;
  }
  case 3:
  case 4:
  {
    QStyleOptionButton button;
    button.rect = option.rect;
    button.text = index.column() == 3 ? tr("Result") : tr("Ingredients");
    button.state = QStyle::State_Enabled;
    QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter);
    break;
  }
  case 5:
  {
    ResourceCalculator::KEY_TYPE_FACTORY keyFactory = static_cast<ResourceCalculator::KEY_TYPE_FACTORY>(index.data().toInt());
    const RecipesModel& model = dynamic_cast<const RecipesModel&>(*index.model());

    QStyleOptionButton button;
    button.text = QString::fromStdString(model.GetFactoryTypes()[keyFactory].GetName());
    button.rect = option.rect;
    button.state = QStyle::State_Enabled;
    QApplication::style()->drawControl(QStyle::CE_PushButton, &button, painter, 0);
    break;
  }
  default:
    QStyledItemDelegate::paint(painter, option, index);
    return;
    break;
  }
}

bool RecipesEditDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index)
{
  if (event->type() == QEvent::MouseButtonPress) {
    switch (index.column()) {
    case 0:
    {
      IconSelectedDialog _IconSelectedDialog(_Icons);
      if (_IconSelectedDialog.exec()) {
        const ResourceCalculator::Icon * Icon = _IconSelectedDialog.GetResult();
        if (Icon != nullptr) {
          model->setData(index, QString::fromStdString(Icon->GetKeyPath()));
        }
      }
      return false;
      break;
    }
    case 3:
    {
      const RecipesModel& modelRecipes = dynamic_cast<const RecipesModel&>(*index.model());
      ItemSelectedDialog _ItemSelectedDialog(_ItemModel, _Icons, ItemSelectedDialogMode::ForRecipeSelectItemsResult, model->data(index).value<QT_CountsItem>(), nullptr);
      if (_ItemSelectedDialog.exec()) {
        QVariant V; V.setValue<QT_CountsItem>(_ItemSelectedDialog.GetResult());
        model->setData(index, V);
      }
      return false;
      break;
    }
    case 4:
    {
      const RecipesModel& modelRecipes = dynamic_cast<const RecipesModel&>(*index.model());

      ItemSelectedDialog _ItemSelectedDialog(_ItemModel, _Icons, ItemSelectedDialogMode::ForRecipeSelectItemsRequired, model->data(index).value<QT_CountsItem>(), nullptr);
      if (_ItemSelectedDialog.exec()) {
        QVariant V; V.setValue<QT_CountsItem>(_ItemSelectedDialog.GetResult());
        model->setData(index, V);
      }
      return false;
      break;
    }
    case 5:
    {
      const RecipesModel& modelRecipes = dynamic_cast<const RecipesModel&>(*index.model());
      FactorysTypesSelectedDialog factorysTypesSelectedDialog(_FactoryTypesModel, _Icons, false, nullptr);
      QVariant V = model->data(index);
      factorysTypesSelectedDialog.SetResult({static_cast<ResourceCalculator::KEY_TYPE_FACTORY>(V.value<int>())});
      if (factorysTypesSelectedDialog.exec())
      {
        std::set<ResourceCalculator::KEY_TYPE_FACTORY> result = factorysTypesSelectedDialog.GetResult();
        if (!result.empty())
          model->setData(index, QVariant::fromValue<int>(static_cast<int>(*result.begin())));
      }
      return false;
      break;
    }
    default:
      return QStyledItemDelegate::editorEvent(event, model, option, index);
      break;
    }
  }
  return QStyledItemDelegate::editorEvent(event, model, option, index);
}

#pragma endregion DELEGATE

RecipesEditDialog::RecipesEditDialog(
  ResourceCalculator::RecipeCollection& RC,
  ResourceCalculator::ItemCollection& IC,
  const ResourceCalculator::IconCollection& icons,
  ResourceCalculator::FactoryTypeCollection& FTC,
  QWidget *parent)
  : QDialog(parent)
  , _Model(RC, IC, FTC, parent)
  , _FactoryTypesModel(FTC)
  , _ItemModel(IC)
{
  setMinimumSize(1000, 600);

  QPushButton *okButton = new QPushButton(tr("Ok"));
  QPushButton *addButton = new QPushButton(tr("Add"));
  _removeButton = new QPushButton(tr("Remove"));
  QPushButton *cancelButton = new QPushButton(tr("Cancel"));

  _tableView = new QTableView();
  _tableView->setModel(&_Model);
  _tableView->setItemDelegate(new RecipesEditDelegate(icons, _FactoryTypesModel, _ItemModel, _tableView));
  _tableView->setSelectionMode(QTableView::SelectionMode::MultiSelection);
  _tableView->setSelectionBehavior(QTableView::SelectionBehavior::SelectRows);
  _tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  _tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
  _tableView->setColumnWidth(0, 60);

  QHBoxLayout *buttonLayout = new QHBoxLayout();
  buttonLayout->addWidget(okButton);
  buttonLayout->addWidget(addButton);
  buttonLayout->addWidget(_removeButton);
  buttonLayout->addWidget(cancelButton);

  QVBoxLayout *mainLayout = new QVBoxLayout();
  mainLayout->addWidget(_tableView);
  mainLayout->addLayout(buttonLayout);
  setLayout(mainLayout);

  connect(okButton, SIGNAL(clicked()), SLOT(PushButtonOk()));
  //connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);
  connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);
  connect(addButton, &QAbstractButton::clicked, this, &RecipesEditDialog::add_item);
  connect(_removeButton, &QAbstractButton::clicked, this, &RecipesEditDialog::remove_item);
  //connect(Delegate, &RecipesEditDelegate::editorEventDelegate, this, &RecipesEditDelegate::editorEventDelegate);

  setWindowTitle(tr("Recipes Edit"));

}

void RecipesEditDialog::add_item()
{
  QModelIndexList Rows = _tableView->selectionModel()->selectedRows();
  if (Rows.size() > 0) {
    _Model.insertRow(Rows[0].row());
  } else {
    _Model.insertRow(0);
  }
}

void RecipesEditDialog::remove_item()
{
  QModelIndexList RowsSelected = _tableView->selectionModel()->selectedRows();
  for (const auto& it: RowsSelected)
  {
    _Model.removeRow(it.row());
  }
}

void RecipesEditDialog::editorEventDelegate(const QModelIndex & index)
{
  QModelIndexList Rows = _tableView->selectionModel()->selectedRows();
  _removeButton->setEnabled(Rows.size() > 0);
}

void RecipesEditDialog::PushButtonOk()
{
  _Model.Commit();
  emit(&QDialog::accept);
  emit(close());
}
