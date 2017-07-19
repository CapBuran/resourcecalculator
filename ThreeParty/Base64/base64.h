#include <vector>

void base64_encode(const std::vector<char> &input, std::vector<char> &output);
void base64_decode(const std::vector<char> &input, std::vector<char> &output);

size_t base64_get_size_encode_data(size_t org_data_size);