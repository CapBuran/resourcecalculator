#include <FactorysEditDialog.h>
#include <IconSelectedDialog.h>
#include <FactoryTypesEditDialog.h>
#include <FactorysTypesSelectedDialog.h>

#pragma region DELEGATES

#pragma region FactorysEditDialogDelegate

FactorysEditDialogDelegate::FactorysEditDialogDelegate(const ResourceCalculator::IconCollection& IC, QObject *parent)
  : QStyledItemDelegate(parent)
  , _Icons(IC)
{
}

void FactorysEditDialogDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex& index) const
{
  //emit(ThePermissionToDeleteAnObjectHasBeenChanged(index, _ItemSelectionModel->selectedRows().size() > 0));
  switch (index.column()) {
  case 0:
  {
    using namespace ResourceCalculator;
    const ResourceCalculator::Icon &icon = _Icons.GetIcon(index.data(Qt::DisplayRole).toString().toStdString());
    if (icon.GetRawData().size() > 0) {
      QPixmap pixmap;
      bool dd = pixmap.loadFromData((uchar*)&icon.GetRawData()[0], (uint)icon.GetRawData().size());
      const int MinCoord = std::min(option.rect.width(), option.rect.height());
      const int MaxCoord = std::max(option.rect.width(), option.rect.height());
      const int Sub1 = (MaxCoord - MinCoord) / 2;
      QRect rect;
      if (MaxCoord == option.rect.width()) {
        rect.setCoords(
          option.rect.left() + Sub1, option.rect.top(),
          option.rect.left() + Sub1 + MinCoord, option.rect.bottom()
        );
      } else {
        rect.setCoords(
          option.rect.left(), option.rect.top() + Sub1,
          option.rect.right(), option.rect.top() + Sub1 + MinCoord
        );
      }
      painter->drawPixmap(rect, pixmap);
    }
    break;
  }
  case 2:
  {
    const QT_KeysFactoryType& types = index.data().value<QT_KeysFactoryType>();
    const FactorysModel& model = dynamic_cast<const FactorysModel&>(*index.model());
    const auto& FTC = model.GetTypesData();
    QString Text = types.empty() ? tr("Types not set!") : "";
    for (const auto& type: types)
    {
      if (!Text.isEmpty()) Text += tr(", ");
      Text += QString::fromStdString(FTC[type].GetName());
    }
    QStyleOptionButton button;
    button.text = Text;
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

QSize FactorysEditDialogDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  if (index.column() == 2)
  {
    const QT_KeysFactoryType& types = index.data().value<QT_KeysFactoryType>();
    const FactorysModel& model = dynamic_cast<const FactorysModel&>(*index.model());
    const auto& FTC = model.GetTypesData();
    QString Text = types.empty() ? tr("Types not set!") : "";
    for (const auto& type : types)
    {
      if (!Text.isEmpty()) Text += ", ";
      Text += QString::fromStdString(FTC[type].GetName());
    }
    QFontMetrics fm(option.font);
    int width = fm.horizontalAdvance(Text) + fm.overlinePos();
    return QSize(width, option.rect.height());
  }
  return QStyledItemDelegate::sizeHint(option, index);
}

bool FactorysEditDialogDelegate::editorEvent(QEvent * event, QAbstractItemModel* model, const QStyleOptionViewItem & option, const QModelIndex& index)
{
  if (event->type() == QEvent::MouseButtonPress)
  {
    switch (index.column()) {
    case 0:
    {
      IconSelectedDialog _IconSelectedDialog(_Icons);
      if (_IconSelectedDialog.exec()) {
        const ResourceCalculator::Icon* Icon = _IconSelectedDialog.GetResult();
        if (Icon != nullptr) {
          model->setData(index, QString::fromStdString(Icon->GetKeyPath()), Qt::EditRole);
        }
      }
      return false;
      break;
    }
    case 2:
    {
      const QT_KeysFactoryType& types = index.data().value<QT_KeysFactoryType>();
      FactorysModel& modelFactory = dynamic_cast<FactorysModel&>(*model);
      FactorysTypesSelectedDialog dialog(modelFactory.GetTypesData(), _Icons, true, nullptr);
      dialog.SetResult(types);
      if (dialog.exec()) {
        QVariant V; V.setValue<QT_KeysFactoryType>(dialog.GetResult());
        modelFactory.setData(index, V);
      }
    }
      break;
    default:
      return QStyledItemDelegate::editorEvent(event, model, option, index);
      break;
    }
  }
  return QStyledItemDelegate::editorEvent(event, model, option, index);
}

#pragma endregion FactorysEditDialogDelegate

#pragma endregion DELEGATES

FactorysEditDialog::FactorysEditDialog(
  ResourceCalculator::FactoryTypeCollection& FTC,
  const ResourceCalculator::IconCollection& IC,
  QWidget *parent
)
  : QDialog(parent)
  , _Model(FTC, parent)
  , _Delegate(IC, parent)
  , _Icons(IC)
{
  setMinimumSize(1000, 600);

  QPushButton *okButton = new QPushButton(tr("OK"));
  QPushButton *addButton = new QPushButton(tr("Add"));
  _remoteButton = new QPushButton(tr("Remote"));
  QPushButton *cancelButton = new QPushButton(tr("Cancel"));
  QPushButton *typesFactorysButton = new QPushButton(tr("Factory types"));

  _tableView = new QTableView;
  _tableView->setSelectionMode(QTableView::SelectionMode::SingleSelection);
  _tableView->setSelectionBehavior(QTableView::SelectionBehavior::SelectRows);
  _tableView->setModel(&_Model);
  _tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  _tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
  _tableView->setColumnWidth(0, 60);

  _tableView->setItemDelegate(&_Delegate);

  QHBoxLayout *buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(okButton);
  buttonLayout->addWidget(typesFactorysButton);
  buttonLayout->addWidget(addButton);
  buttonLayout->addWidget(_remoteButton);
  buttonLayout->addWidget(cancelButton);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(_tableView);
  mainLayout->addLayout(buttonLayout);
  setLayout(mainLayout);

  connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);
  connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);
  connect(addButton, &QAbstractButton::clicked, this, &FactorysEditDialog::add_item);
  connect(typesFactorysButton, &QAbstractButton::clicked, this, &FactorysEditDialog::typesFactory);
  connect(_remoteButton, &QAbstractButton::clicked, this, &FactorysEditDialog::remove_item);
  connect(
    &_Delegate, &FactorysEditDialogDelegate::ThePermissionToDeleteAnObjectHasBeenChanged,
    this, &FactorysEditDialog::ThePermissionToDeleteAnObjectHasBeenChanged
  );
  setWindowTitle(tr("Editor factorys"));
}

void FactorysEditDialog::Commit()
{
  _Model.Commit();
}

void FactorysEditDialog::remove_item()
{
  QModelIndexList RowsSelected = _tableView->selectionModel()->selectedRows();
  if (RowsSelected.size() > 0) {
    _Model.removeRow(RowsSelected[0].row());
  }
}

void FactorysEditDialog::ThePermissionToDeleteAnObjectHasBeenChanged(const QModelIndex & index, bool IsAllowed)
{
  _remoteButton->setEnabled(IsAllowed);
}

void FactorysEditDialog::add_item()
{
  QModelIndexList Rows = _tableView->selectionModel()->selectedRows();
  if (Rows.size() > 0) {
    _Model.insertRow(Rows[0].row());
  } else {
    _Model.insertRow(0);
  }
}

void FactorysEditDialog::typesFactory()
{
  _Model.BeginReset();
  FactoryTypesEditDialog FTED(_Model.GetTypesData(), _Icons, this);
  if (FTED.exec()) {
    FTED.Commit();
  }
  _Model.EndReset();
}
