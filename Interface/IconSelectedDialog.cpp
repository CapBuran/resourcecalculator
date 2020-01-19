#include "IconSelectedDialog.h"

class QListWidgetItemData : public QListWidgetItem {
  public:
    const ResourceCalculator::Icon *IconRC;
    explicit QListWidgetItemData(const ResourceCalculator::Icon *icon, 
      const QString &text, QListWidget *view = Q_NULLPTR, int type = Type):
      QListWidgetItem(text, view, type), IconRC(icon)
    {
    }
};

int SetIconData(QIcon &Icon, QSize Size, int size_data, const char* data) {
  QImage Image;
  const uchar *buf = (const uchar*)data;
  Image.loadFromData(buf, size_data);
  Icon.addPixmap(QPixmap::fromImage(Image.scaled(Size)));
  return 0;
}

IconSelectedDialog::IconSelectedDialog(const ResourceCalculator::ParamsCollection &PC, QWidget *parent)
  : QDialog(parent), _PC(PC), _Result(nullptr)
{
  setMinimumSize(800, 600);
  setWindowState(Qt::WindowStates::enum_type::WindowMaximized);
  
  QPushButton *okButton = new QPushButton(tr("OK"));;
  QPushButton *cancelButton = new QPushButton(tr("Cancel"));

  QHBoxLayout *buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(okButton);
  buttonLayout->addWidget(cancelButton);

  //QSize IconSize(32, 32);
  QSize IconSize(64, 64);

  _ListWidget = new QListWidget;
  _ListWidget->setWrapping(true);
  _ListWidget->setGridSize(QSize(216, 64+20));
  _ListWidget->setFlow(QListWidget::Flow::LeftToRight);
  _ListWidget->setResizeMode(QListWidget::ResizeMode::Adjust);
  _ListWidget->setViewMode(QListWidget::IconMode);
  _ListWidget->setIconSize(IconSize);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(_ListWidget);
  mainLayout->addLayout(buttonLayout);
  setLayout(mainLayout);

  const std::map<std::string, ResourceCalculator::Icon>& Icons = PC.Icons.GetAllIcon();
  for (auto & icon : Icons){
    QListWidgetItemData *ListItem = new QListWidgetItemData(&icon.second, icon.second.GetShortName().c_str());
    const std::vector<char>& RawData = icon.second.GetRawData();
    QIcon qicon;
    SetIconData(qicon, IconSize, (int)RawData.size(), &RawData[0]);
    ListItem->setIcon(qicon);
    
    _ListWidget->addItem(ListItem);
  }

  connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);
  connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);

  setWindowTitle(tr("SelectedIcons"));
}

const ResourceCalculator::Icon * IconSelectedDialog::GetResult() const
{
  return dynamic_cast<const QListWidgetItemData *>(_ListWidget->currentItem())->IconRC;
}

