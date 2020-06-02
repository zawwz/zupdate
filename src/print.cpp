#include "print.hpp"

#include <iostream>
#include <cmath>

#include <ztd/shell.hpp>

#include "opt_bool.h"

//constants
const char* size_suffixes[6] = { "B", "KiB", "MiB", "GiB", "TiB", "PiB"};
const int size_print_padding=-21;

//tool
std::string strpf(std::string const& format, std::string const& var)
{
  std::string ret;
  size_t bufsize = format.size()-1 + var.size();
  char* buf = (char*) malloc( sizeof(char)*bufsize );
  snprintf(buf, bufsize, format.c_str(), var.c_str());
  ret = buf;
  free(buf);
  return ret;
}

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

static const char* p_color(const ztd::color c)
{
  if(!opt_nocolor)
    return c.code();
  else
    return "";
}

void print_separator(uint32_t length, ztd::color color, char sepchar)
{
  std::string sep;
  for(uint32_t i=0 ; i<length ; i++)
    sep += sepchar;
  std::cout << p_color(color) << sep << p_color(no_color) << std::endl;
}

void print_update(repo_update& ru, ztd::color color, bool dlsize, bool nisize, bool nusize)
{
  if(ru.packages.size() > 0)
  {
    printf("%s[%s] %lu updates%s\n", p_color(color), ru.name.c_str(), ru.packages.size(), p_color(no_color) );
    for(auto it : ru.packages)
    {
      std::string v1;
      std::string v2;
      if(!opt_nocolor)
      {
        auto pv = color_diff(it.current_version, it.new_version);
        v1 = pv.first;
        v2 = pv.second;
      }
      else
      {
        v1 = it.current_version;
        v2 = it.new_version;
      }
      int c_padsize = (opt_nocolor ? 0 : 20) ;
      printf("  %*s %*s ->  %*s  | ", -1*(ru.name_max_length + 2), it.name.c_str(), -1*(ru.vcur_max_length + c_padsize) , v1.c_str(), -1*(ru.vnew_max_length + c_padsize) , v2.c_str());
      if(dlsize)
        print_size(it.download_size, true, "", 0, ztd::color::none, 2, size_index, "  : ", (int) std::max(log10(ru.max_download_size) - 3*size_index + 0 , 0.0) + 5 );
      if(nusize)
        print_size(it.net_size, true, "", 0, ztd::color::none, 2, size_index, "", (int) std::max(log10(ru.max_net_size) - 3*size_index + 0 , 0.0) + 5 );
      printf("\n");
    }
  }
}

void print_update_sizes(repo_update& ru, ztd::color color, bool dlsize, bool nisize, bool nusize, bool notitle, uint32_t padsize)
{
  bool separator = !notitle && (dlsize || nisize || nusize) ;
  if(dlsize)
    print_size(ru.download_size, !notitle, "Total Download Size:", size_print_padding, color, 2, size_index, "\n", padsize);
  if(nisize)
    print_size(ru.new_install_size, !notitle, "Total Install Size:", size_print_padding, color, 2, size_index, "\n", padsize);
  if(nusize)
    print_size(ru.net_size, !notitle, "Net Upgrade Size:", size_print_padding, color, 2, size_index, "\n", padsize);
}

void print_size(long int size, bool printTitle, std::string title, int padding, ztd::color color, unsigned int precision, unsigned int sizepow, const char* line_end, int sizepad)
{
  auto tpair = convertN(size, sizepow);
  if( printTitle )
  {
    printf("%s%*s%s", p_color(color), padding, title.c_str(), p_color(no_color) );
  }
  if(sizepad<0)
    sizepad=precision+5;
  if(sizepow == 0)
  {
    precision = 0;
//    sizepad = 9;
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
