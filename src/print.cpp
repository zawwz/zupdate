#include "print.hpp"

#include <iostream>
#include <cmath>

#include <ztd/shell.hpp>

#include "opt_bool.h"

//constants
const char* size_suffixes[6] = { "B", "KiB", "MiB", "GiB", "TiB", "PiB"};
const int size_print_padding=-21;

//functions
std::pair<double, const char*> convertN(const long int size, unsigned int n)
{
  double ret = size;
  for(unsigned int i=0; i<n; i++)
  {
    ret /= 1024;
  }
  return std::make_pair( ret , size_suffixes[n] );
}

std::pair<double, const char*> convertHReadable(const long int size)
{
  double ret = size;
  unsigned char pow1k=0;
  while(abs(ret) >= 1024.0)
  {
    ret /= 1024;
    pow1k++;
  }
  return std::make_pair( ret, size_suffixes[pow1k] );
}

void print_update(repo_update& ru, ztd::color color, bool dlsize, bool nisize, bool nusize)
{
  if(ru.packages.size() > 0)
  {
    printf("%s[%s] %lu updates%s\n", color.code(), ru.name.c_str(), ru.packages.size(), no_color.code());
    for(auto it : ru.packages)
    {
      std::string v1;
      std::string v2;
      if(ztd::sh("command -v zdiffcolor").size() > 0)
      {
        std::string command="zdiffcolor -n b_white -c b_red -s b_green " + it.current_version + ' ' + it.new_version;
        std::string str=ztd::sh(command);
        auto newline = str.find('\n');
        v1=str.substr(0, newline);
        newline++;
        v2=str.substr(newline, str.size()-newline-1);
      }
      else
      {
        ztd::color v1c = ztd::color::b_red, v2c = ztd::color::b_green, noc = ztd::color::none;
        v1 = v1c.code() + it.current_version + noc.code();
        v2 = v2c.code() + it.new_version + noc.code();
      }
      printf("  %*s %*s ->  %*s  | ", -1*(ru.name_max_length + 2), it.name.c_str(), -1*(ru.vcur_max_length + 20), v1.c_str(), -1*(ru.vnew_max_length + 20), v2.c_str());
      if(dlsize)
        print_size(it.download_size, true, "", 0, ztd::color::none, 2, size_index, "  : ");
      if(nusize)
        print_size(it.net_size, true, "", 0, ztd::color::none, 2, size_index, "");
      printf("\n");
    }
    std::cout << color;
    std::cout << "================================";
    std::cout << no_color << std::endl;
  }
}

void print_update_sizes(repo_update& ru, ztd::color color, bool dlsize, bool nisize, bool nusize, bool notitle)
{
  if(dlsize)
    print_size(ru.download_size, !notitle, "Total Download Size:", size_print_padding, color, 2, size_index);
  if(nisize)
    print_size(ru.new_install_size, !notitle, "Total Install Size:", size_print_padding, color, 2, size_index);
  if(nusize)
    print_size(ru.net_size, !notitle, "Net Upgrade Size:", size_print_padding, color, 2, size_index);
  if(!notitle && (dlsize || nisize || nusize))
  {
    std::cout << color;
    std::cout << "================================";
    std::cout << no_color << std::endl;
  }
}

void print_size(long int size, bool printTitle, std::string title, int padding, ztd::color color, unsigned int precision, unsigned int sizepow, const char* line_end)
{
  auto tpair = convertN(size, sizepow);
  if( printTitle )
  {
    printf("%s%*s%s", color.code(), padding, title.c_str(), no_color.code());
  }
  unsigned int sizepad=precision+5;
  if(sizepow == 0)
  {
    precision = 0;
    sizepad = 9;
  }
  if(!printTitle)
  {
    sizepad = 0;
  }
  printf("%*.*f %s%s", sizepad, precision, tpair.first, tpair.second, line_end);
}

void print_listraw(repo_update& ru)
{
  for(auto it : ru.packages)
  {
    printf("%s %s -> %s\n", it.name.c_str(), it.current_version.c_str(), it.new_version.c_str());
  }
}
