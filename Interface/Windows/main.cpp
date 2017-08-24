
#include "mainwindow.h"
#include <fstream>

//#include <QApplication>
//#include <QtGui>
//#include <QtCore>
#include <QtWidgets>

#define StandartTestFileJson "C:\\games\\Factorio.json"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  ResourceCalculator::ParamsCollection PC;

  {
    Q_INIT_RESOURCE(ResurceGui);
    QFile file(":/Res/IconsDefault");
    if (file.open(QIODevice::ReadOnly)) {
      QByteArray RawData = file.readAll();
      PC.Icons.ReadRawDataAsJson(RawData.constData());
    }
  }

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

  //{
  //  //std::string files_preffix = "C:/games/icons";
  //  std::string files_preffix = "C:/games/icons/";
  //  std::list<std::string> files;
  //  QStringList filtres_file; filtres_file << "*.png";
  //  QDirIterator it(files_preffix.c_str(), filtres_file, QDir::Files, QDirIterator::Subdirectories | QDirIterator::FollowSymlinks);
  //  while (it.hasNext()) {
  //    QFileInfo fi = it.fileInfo();
  //    std::string filename = fi.absoluteFilePath().toStdString();
  //    QString dd = fi.absoluteFilePath();
  //    if (filename.length() > 0) {
  //      files.push_back(fi.absoluteFilePath().toStdString());
  //    }
  //    it.next();
  //  }
  //  //PC.Icons.ReadAll(files_preffix + "/", files);
  //  PC.Icons.ReadAll("__base__/graphics/icons/", files_preffix, files);
  //  std::ofstream out;
  //  out.open(StandartTestFileJson);
  //  Json::Value jsonPr;
  //  PC.WriteToJson(jsonPr);
  //  Json::StyledWriter styledWriter;
  //  out << styledWriter.write(jsonPr);
  //  out.close();
  //}

  QTranslator *qtTranslator = new QTranslator(qApp);
  bool IsOk = qtTranslator->load(":/ResurceWin_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
  qApp->installTranslator(qtTranslator);

  MainWindow mw(PC);
  //mw.show();
  mw.showMaximized();
  return app.exec();
}

