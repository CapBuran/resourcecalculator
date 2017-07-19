#include "base64.h"

static char encoding_table[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
'w', 'x', 'y', 'z', '0', '1', '2', '3',
'4', '5', '6', '7', '8', '9', '+', '/' };
static char decoding_table[256];
static int mod_table[] = { 0, 2, 1 };
static bool isBuild_decoding_table = false;

void _build_decoding_table() {
  for (int i = 0; i < 64; i++) {
    decoding_table[(unsigned char)encoding_table[i]] = i;
  }
  isBuild_decoding_table = true;
}

void base64_encode(const std::vector<char> &input, std::vector<char> &output)
{
  const size_t input_length = input.size();
  output.clear();

  if (input_length == 0) return;

  const unsigned char *data = reinterpret_cast<const unsigned char *>(&input[0]);

  const size_t output_length = 4 * ((input_length + 2) / 3);
  output.resize(output_length);
  char *encoded_data = &output[0];

  for (int i = 0, j = 0; i < input_length;) {
    uint32_t octet_a = i < input_length ? data[i++] : 0;
    uint32_t octet_b = i < input_length ? data[i++] : 0;
    uint32_t octet_c = i < input_length ? data[i++] : 0;
    uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;
    encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
    encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
    encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
    encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
  }

  for (int i = 0; i < mod_table[input_length % 3]; i++)
    encoded_data[output_length - 1 - i] = '=';
}

void base64_decode(const std::vector<char> &input, std::vector<char> &output)
{

  output.clear();

  if (!isBuild_decoding_table) _build_decoding_table();

  const size_t input_length = input.size();

  if (input_length % 4 != 0) return;

  size_t output_length = input_length / 4 * 3;

  const char * input_data = &input[0];
  
  if (input_data[input_length - 1] == '=') (output_length)--;
  if (input_data[input_length - 2] == '=') (output_length)--;

  output.resize(output_length);
  
  char *decoded_data = &output[0];

  for (int i = 0, j = 0; i < input_length;) {

    uint32_t sextet_a = input_data[i] == '=' ? 0 & i++ : decoding_table[input_data[i++]];
    uint32_t sextet_b = input_data[i] == '=' ? 0 & i++ : decoding_table[input_data[i++]];
    uint32_t sextet_c = input_data[i] == '=' ? 0 & i++ : decoding_table[input_data[i++]];
    uint32_t sextet_d = input_data[i] == '=' ? 0 & i++ : decoding_table[input_data[i++]];

    uint32_t triple = (sextet_a << 3 * 6)
      + (sextet_b << 2 * 6)
      + (sextet_c << 1 * 6)
      + (sextet_d << 0 * 6);

    if (j < output_length) decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
    if (j < output_length) decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
    if (j < output_length) decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
  }

}

size_t base64_get_size_encode_data(size_t org_data_size)
{
  return 4 * ((org_data_size + 2) / 3);
}

