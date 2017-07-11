
#include "mainwindow.h"
#include <fstream>

#include <QApplication>
#include <QtGui>
#include <QtCore>

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
    PC.ReadFromJson(jsonPrRestore);
  }

  //Q_INIT_RESOURCE(resources);

  //QFile file(":/test.txt");
  //QByteArray dd = file.readAll();

  //std::vector<char> d(dd.size());

  //for (size_t i = 0; i < dd.size(); i++)
  //{
  //  d[i] = dd.operator const char *()[i];
  //}

  QTranslator *qtTranslator = new QTranslator(qApp);
  bool IsOk = qtTranslator->load(":/ResurceWin_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
  qApp->installTranslator(qtTranslator);

  MainWindow mw(PC);
  mw.show();
  return app.exec();
}

