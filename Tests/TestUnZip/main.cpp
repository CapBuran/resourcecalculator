#include <stdio.h>
#include <stdlib.h>
#include <zip.h>

int main(int argc, char **argv)
{
  struct zip *zip_file; // дескриптор zip файла
  int err; // переменая для возврата кодов ошибок
  int files_total; // количество файлов в архиве

  if (argc < 2) {
    fprintf(stderr, "usage: %s <zipfile>\n", argv[0]);
    return -1;
  };

  // открываем файл zip с именем переданным в качестве параметра
  zip_file = zip_open(argv[1], 0, &err);
  if (!zip_file) {
    fprintf(stderr, "Error: can't open file %s\n", argv[1]);
    return -1;
  };

  files_total = zip_get_num_files(zip_file); // количество файлов в архиве
  printf("Files in ZIP: %d\n", files_total);

  zip_close(zip_file);

  return 0;
}
