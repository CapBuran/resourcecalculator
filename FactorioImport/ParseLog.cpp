#include <iostream>
#include <string>
#include <fstream>
//#include <algorithm>
//#include <iterator>
#include <map>

#include "ParseLog.h"


#include "../ResourceCalculator/Item.h"
#include "../ResourceCalculator/Recipe.h"
#include "../ResourceCalculator/Icon.h"

namespace FactorioImport
{

  static inline void ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
      return !std::isspace(ch);
      }));
  }

  // trim from end (in place)
  static inline void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
      return !std::isspace(ch);
      }).base(), s.end());
  }

  // trim from both ends (in place)
  static inline void trim(std::string& s) {
    ltrim(s);
    rtrim(s);
  }

  std::string ParseParam(const std::string param, std::string str)
  {
    auto pos = str.find(param);
    str = str.substr(pos + param.length());
    pos = str.find('[');
    if (pos != std::string::npos) str = str.substr(0, pos);
    trim(str);
    return str;
  }

  void ParseIcons(std::string str, std::list<IconsLog>& out)
  {
    enum class State
    {
      Begin,
      Entry,
      Name,
      Icons,
      Icon,
      IconSize,
      End,
      Error
    } state = State::Begin;

    const std::string findMask = "Script @__recipelister__/data-final-fixes.lua";
    auto pos = str.find(findMask);
    if (pos != std::string::npos)
    {
      pos += findMask.length();
      IconsLog ToAdd;
      IconLog ToAddIcon;
      for (size_t i = pos; i < str.length(); i++)
      {
        if (state == State::End) break;
        if (state == State::Error) break;

        char ch = str[i];
        switch (state)
        {
        case State::Begin:
          if (ch == '{') {
            state = State::Entry;
            continue;
          }
          if (ch == '}') {
            return;
          }
          break;
        case State::Entry:
          if (ch == '{') {
            state = State::Name;
            continue;
          }
          if (ch == '}') {
            return;
          }
          ToAdd.Entry += ch;
          break;
        case State::Name:
          if (ch == '}') {
            state = State::Icons;
            continue;
          }
          if (ch == '{') {
            state = State::End;
            continue;
          }
          ToAdd.Name += ch;
          break;
        case State::Icons:
          if (ch == '}') {
            state = State::End;
            continue;
          }
          if (ch == '{') {
            state = State::Icon;
            continue;
          }
          break;
        case State::Icon:
          if (ch == ':') {
            state = State::IconSize;
            continue;
          }
          if (ch == '{') {
            continue;
          }
          if (ch == '}') {
            if(!ToAddIcon.Path.empty()) ToAdd.Icons.push_back(ToAddIcon);
            ToAddIcon.Path.clear();
            ToAddIcon.Size = 0;
            state = State::Icons;
            continue;
          }
          ToAddIcon.Path += ch;
          break;
        case State::IconSize:
          if (ch == '}') {
            if (!ToAddIcon.Path.empty()) ToAdd.Icons.push_back(ToAddIcon);
            ToAddIcon.Path.clear();
            ToAddIcon.Size = 0;
            state = State::Icons;
            continue;
          }
          if (ch == '{') {
            state = State::Error;
            continue;
          }
          ToAddIcon.Size = ToAddIcon.Size * 10 + ch - '0';
          break;
        case State::End:
        default:
          break;
        }
      }

      if (state == State::End)
      {
        out.push_back(ToAdd);
      }
    }
  }

  bool ParseLog::Parse(std::string FullPath)
  {
    std::string s;
    std::ifstream file(FullPath);

    std::map<std::string, std::string> params =
    {
      {"Read data path: ", ""},
      {"Write data path: ", ""},
      {"Binaries path: ", ""}
    };

    while (std::getline(file, s)) { 
      for (auto& param : params) {
        if (s.find(param.first) != std::string::npos)
        {
          param.second = ParseParam(param.first, s);
        }
      }
      if (s.find("__recipelister__") != std::string::npos)
      {
        ParseIcons(s, Icons);
      }
    }

    if (params.count("Read data path: "))
    {
      ReadData = params["Read data path: "];
    }
    if (params.count("Binaries path: "))
    {
      BinaryData = params["Binaries path: "];
    }
    if (params.count("Write data path: "))
    {
      WriteData = params["Write data path: "];
    }

    return false;
  }

}