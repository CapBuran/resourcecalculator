
#include "mainwindow.h"
#include <fstream>

#include <zlib.h> 

#define SRC_SIZE 512
#define DST_SIZE 512

int main_zlib_use_todo()
{
  // ������� ����� �������� SRC_SIZE
  unsigned char source[SRC_SIZE];
  // ��� �� ����� ������� ������ �������� ������ 
  // (���������� ������ ��� �����������)
  const unsigned long sourceLen = SRC_SIZE;
  // �������� �����
  unsigned char dest[DST_SIZE];
  // ������ ��������� ������
  unsigned long destLen = DST_SIZE;

  // ����� ������� ����� ������� ������
  memset(source, 0, SRC_SIZE * sizeof(source[0]));

  if (Z_OK == compress(dest, &destLen, source, sourceLen)) {
    // ��� �������
    printf("Compress ratio: %.2f\n", destLen / (float)sourceLen);
  } else {
    // ��� ������ ��������� ��� ������ � ������� ����� ��������� ����������
    printf("Compress failed\n");
  }

  return 0;
}

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

