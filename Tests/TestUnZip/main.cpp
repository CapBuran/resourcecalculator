#include <zip.h>

#include <assert.h>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#define ZIPFILE "..\\..\\..\\WorkDir\\VoidChestPlus_2.2.2.zip"
#define ZIPFILE_IN_ACHIVE "VoidChestPlus_2.2.2/graphics/icon/voidchest.png"
#define FIND_FILE_NAME "graphics/icon/voidchest.png"

int main(int argc, char **argv)
{
  struct zip *zip_file; // дескриптор zip файла
  struct zip_stat file_info; // информация о файле
  int err; // переменая для возврата кодов ошибок
  int files_total; // количество файлов в архиве

  const std::string fileZip = ZIPFILE;
  
  // открываем файл zip с именем переданным в качестве параметра
  zip_file = zip_open(fileZip.c_str(), 0, &err);
  if (!zip_file) {
    std::cout << "Error: can't open file" << fileZip << std::endl;
    return -1;
  };

  files_total = zip_get_num_files(zip_file); // количество файлов в архиве
  std::cout << "Files in ZIP: " << files_total << std::endl;

  const std::string findFileValue = FIND_FILE_NAME;

  std::vector<char> DataOut;

  for (int i = 0; i < files_total; i++) {
    // получаем информацию о файле с номером i в структуру file_info
    zip_stat_index(zip_file, i, 0, &file_info);
    std::cout
      << "index: " << file_info.index // номер файла
      << "; name: " << file_info.name // имя файла
      << "; size: " << file_info.size // размер файла
      << std::endl;
    
    std::string fileName = file_info.name;

    if (fileName.size() >= findFileValue.size()) {
      size_t CountToDelete = fileName.size() - findFileValue.size();
      fileName.erase(0, CountToDelete);

      if (findFileValue == fileName) {

        struct zip_file *file_in_zip; // дексриптор файла внутри архива
        
                                      // открываем файл внутри архива по номеру file_number
        file_in_zip = zip_fopen_index(zip_file, file_info.index, 0);

        DataOut.resize(file_info.size);

        // читаем содержимое файла
        zip_int64_t reading = zip_fread(file_in_zip, DataOut.data(), file_info.size);
        assert(reading == DataOut.size());

        // закрываем файл внутри архива
        zip_fclose(file_in_zip);

        std::ofstream myFile("incon.png", std::ios::out | std::ios::binary);
        myFile.write(DataOut.data(), DataOut.size());
      }
    }
  };

  zip_close(zip_file);

  return 0;
};
