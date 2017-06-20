
#include "mainwindow.h"
#include <fstream>

#include <QApplication>

#define StandartTestFileJson "C:\\games\\Factorio.json"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  ResourceCalculator::ParamsCollection PC;

  {
    QFile file(StandartTestFileJson);
    if (!file.open(QIODevice::ReadOnly)) {
      return -4;
    }
    QByteArray fileData = file.readAll();
    Json::Value jsonPrRestore;
    Json::Reader JsonReader;
    bool parsingSuccessful = JsonReader.parse(fileData.constData(), jsonPrRestore);
    PC.RC.ReadFromJson(jsonPrRestore["Recipes"]);
    PC.IC.ReadFromJson(jsonPrRestore["Items"]);
  }

  MainWindow mw(PC);
  mw.show();
  return app.exec();
}

