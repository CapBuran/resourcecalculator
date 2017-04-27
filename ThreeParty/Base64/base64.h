#include <string>

#ifdef WIN32
#ifdef BASE64_SHARED
#    if defined _WIN32 || defined _CYGWIN_
#        define BASE64_API __declspec(dllexport) //Windows
#    else
#        define BASE64_API __attribute__ ((visibility ("default"))) //UNIX
#    endif
#else
#    define BASE64_API
#endif
#else
#    define BASE64_API
#endif

#define DEPRECATED_MSG "Old version is not suitable with large strings."


#ifdef WIN32
#    define DEPRECATED_FUNC __declspec(deprecated(DEPRECATED_MSG)) //Windows
#else
#    if defined(__INTEL_COMPILER)
#        define DEPRECATED_FUNC __attribute__((deprecated)) //INTEL
#    elif defined(__GNUC__)
#        define DEPRECATED_FUNC __attribute__((deprecated(DEPRECATED_MSG))) //GCC
#    else
#        pragma message("!WARNING!: Unable to determine compiler. DEPRECATED feature will be disabled.")
#    endif
#endif


/*
 *    Memory-optimized version of Base64 encoding
 * @param[in] string_to_encode
 * @param[out] encoded_string output
 */
BASE64_API void base64_encode(const std::string& string_to_encode, std::string& encoded_string);

/*
 *    Memory-optimized version of Base64 decoding
 * @param[in] encoded_string input
 * @param[out] decoded_string output
 */
BASE64_API void base64_decode(const std::string& encoded_string, std::string& decoded_string);

BASE64_API DEPRECATED_FUNC std::string base64_encode(unsigned char const* , unsigned int len);
BASE64_API DEPRECATED_FUNC std::string base64_decode(std::string const& s);
