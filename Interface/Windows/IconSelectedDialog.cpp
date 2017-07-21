#include "IconSelectedDialog.h"

#include <QtWidgets>

int SetIconData(QIcon &Icon, QSize Size, int size_data, const char* data) {
  QImage Image;
  const uchar *buf = (const uchar*)data;
  Image.loadFromData(buf, size_data);
  Icon.addPixmap(QPixmap::fromImage(Image.scaled(Size)));
  return 0;
}

IconSelectedDialog::IconSelectedDialog(ResourceCalculator::ParamsCollection &PC, QWidget *parent)
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

  QListWidget *ListWidget = new QListWidget;
  ListWidget->setWrapping(true);
  ListWidget->setGridSize(QSize(216, 64+20));
  ListWidget->setFlow(QListWidget::Flow::LeftToRight);
  ListWidget->setResizeMode(QListWidget::ResizeMode::Adjust);
  ListWidget->setViewMode(QListWidget::IconMode);
  ListWidget->setIconSize(IconSize);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(ListWidget);
  mainLayout->addLayout(buttonLayout);
  setLayout(mainLayout);


  const std::map<std::string, ResourceCalculator::Icon>& Icons = PC.Icons.GetAllIcon();
  for (auto & icon : Icons){
    QListWidgetItem *ListItem = new QListWidgetItem(icon.second.ShortName.c_str());
    const std::vector<char>& RawData = icon.second.GetRawData();
    QIcon qicon;
    SetIconData(qicon, IconSize, (int)RawData.size(), &RawData[0]);
    ListItem->setIcon(qicon);
    ListWidget->addItem(ListItem);
  }

  //setWidget

  //okButton = new QPushButton("OK");
  //cancelButton = new QPushButton("Cancel");

  //_table = new IconSelectedModel(_PC, this);
  //QTableView *tableView = new QTableView;
  //tableView->setModel(_table);

  //QHBoxLayout *buttonLayout = new QHBoxLayout;
  //buttonLayout->addWidget(okButton);
  //buttonLayout->addWidget(cancelButton);

  //QVBoxLayout *mainLayout = new QVBoxLayout;
  //mainLayout->addWidget(tableView);
  //mainLayout->addLayout(buttonLayout);
  //setLayout(mainLayout);

  //connect(okButton, &QAbstractButton::clicked, this, &QDialog::accept);
  //connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);



  setWindowTitle(tr("SelectedIcons"));

}

const ResourceCalculator::Icon * IconSelectedDialog::GetResult() const
{
  return _Result;
}
