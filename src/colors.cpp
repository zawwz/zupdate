#include "colors.hpp"

const ztd::color no_color(ztd::color::none);
const ztd::color repo_color(ztd::color::b_white);
const ztd::color aur_color(ztd::color::b_cyan);
const ztd::color error_color(ztd::color::b_red);

const ztd::color new_version_color(ztd::color::b_green);
const ztd::color old_version_color(ztd::color::b_red);
const ztd::color neutral_version_color(ztd::color::b_white);

std::string version_delimiters = ".-+_:";

static bool isDelim(const char c)
{
  return version_delimiters.find(c) != std::string::npos;
}


std::pair<std::string, std::string> color_diff(std::string const& str1, std::string const& str2)
{
  std::string r1, r2;
  {
    //CORE FUNCTION
    uint32_t i=0, j=0;

    while( i < str1.size() && i < str2.size() )
    {
      while( i < str1.size() && i < str2.size() && str1[i] == str2[i] && !isDelim(str1[i]) )
        i++;

      if( str1[i] != str2[i] ) //there is a difference
      {

        r1 += neutral_version_color;
        r1 += str1.substr(0, j); //previous section
        r1 += old_version_color;
        r1 += str1.substr(j, str1.size() - j); //changed section and rest
        r1 += no_color;

        r2 += neutral_version_color;
        r2 += str2.substr(0, j); //previous section
        r2 += new_version_color;
        r2 += str2.substr(j, str2.size() - j); //changed section and rest
        r2 += no_color;

        break;
      }
      i++;
      j=i;
    }
  }
  return std::make_pair(r1,r2);

}
