#include <stdio.h>
#include <stdlib.h>
#include <zip.h>

int main(int argc, char **argv)
{
  struct zip *zip_file; // ���������� zip �����
  int err; // ��������� ��� �������� ����� ������
  int files_total; // ���������� ������ � ������

  if (argc < 2) {
    fprintf(stderr, "usage: %s <zipfile>\n", argv[0]);
    return -1;
  };

  // ��������� ���� zip � ������ ���������� � �������� ���������
  zip_file = zip_open(argv[1], 0, &err);
  if (!zip_file) {
    fprintf(stderr, "Error: can't open file %s\n", argv[1]);
    return -1;
  };

  files_total = zip_get_num_files(zip_file); // ���������� ������ � ������
  printf("Files in ZIP: %d\n", files_total);

  zip_close(zip_file);

  return 0;
}
