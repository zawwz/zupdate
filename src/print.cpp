#include "print.hpp"

#include <iostream>
#include <cmath>

#include <ztd/shell.hpp>

#include "opt_bool.h"

//constants
const char* size_suffixes[7] = { "B", "KiB", "MiB", "GiB", "TiB", "PiB", "EiB"};
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
std::pair<double, const char*> convertN(const int64_t size, uint32_t n)
{
  double ret = size;
  for(uint32_t i=0; i<n; i++)
  {
    ret /= 1024;
  }
  return std::make_pair( ret , size_suffixes[n] );
}

std::pair<double, const char*> convertHReadable(const int64_t size)
{
  double ret = size;
  uint8_t pow1k=0;
  while(std::abs(ret) >= 1024.0)
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

void print_update(repo_update& ru, ztd::color color, bool dlsize, bool nisize, bool nusize, bool print_only_install)
{
  if(ru.packages.size() > 0)
  {
    printf("%s[%s] ",  p_color(color), ru.name.c_str());
    bool print_pkgcount = (ru.n_updates != ru.packages.size());
    if(print_pkgcount)
      printf("%lu package%s", ru.packages.size(), ru.packages.size()>1?"s":"");
    if(print_pkgcount && ru.has_update)
      printf(" : ");
    if(ru.has_update)
      printf("%lu update%s", ru.n_updates, ru.n_updates>1?"s":"");
    printf("%s\n", p_color(no_color));
    int c_padsize = (opt_nocolor ? 0 : 20);
    for(auto it : ru.packages)
    {
      std::string v1,v2;
      if(it.has_update)
      {
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
      }
      else
      {
        if(!opt_nocolor)
          v1 += ztd::color(ztd::color::b_white).code();
        v1 += it.current_version;
        if(!opt_nocolor)
        {
          v1 += ztd::color(ztd::color::b_red).code();
          v1 += ztd::color(ztd::color::none).code();
        }
      }
      printf("  %*s %*s ", -1*(ru.name_max_length + 2), it.name.c_str(), -1*(ru.vcur_max_length + c_padsize) , v1.c_str());
      if(it.has_update) // has update
        printf("-> %*s  | ", -1*(ru.vnew_max_length + c_padsize) , v2.c_str());
      else // no update
        printf("%*s  | ", ru.has_update?ru.vnew_max_length+3:0, "" );
      if(print_only_install)
      {
        if(opt_linstall && nisize)
          print_size(it.current_install_size, true, "", 0, ztd::color::none, 2, size_index, "  : ", (int) std::max(log10(ru.max_install_size) - 3*size_index , 0.0) + 5);
      }
      else
      {
        if(opt_linstall && nisize)
          print_size( it.has_update?it.new_install_size:it.current_install_size , true, "", 0, ztd::color::none, 2, size_index, "  : ", (int) std::max(log10(ru.max_install_size) - 3*size_index , 0.0) + 5);
        if(it.has_update && dlsize)
          print_size(it.download_size, true, "", 0, ztd::color::none, 2, size_index, "  : ", (int) std::max(log10(ru.max_download_size) - 3*size_index , 0.0) + 5 );
        if(it.has_update && nusize)
          print_size(it.net_size, true, "", 0, ztd::color::none, 2, size_index, "", (int) std::max(log10(ru.max_net_size) - 3*size_index , 0.0) + 5 );
      }
      printf("\n");
    }
  }
}

void print_update_sizes(repo_update& ru, ztd::color color, bool dlsize, bool nisize, bool nusize, bool notitle, uint32_t padsize)
{
  if(dlsize)
    print_size(ru.download_size, !notitle, "Total Download Size:", size_print_padding, color, 2, size_index, "\n", padsize);
  if(nisize)
    print_size(ru.new_install_size, !notitle, "Total Install Size:", size_print_padding, color, 2, size_index, "\n", padsize);
  if(nusize)
    print_size(ru.net_size, !notitle, "Net Upgrade Size:", size_print_padding, color, 2, size_index, "\n", padsize);
}

void print_size(int64_t size, bool printTitle, std::string title, int padding, ztd::color color, uint32_t precision, uint32_t sizepow, const char* line_end, int sizepad)
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
    printf("%s %s", it.name.c_str(), it.current_version.c_str());
    if(it.has_update)
      printf(" -> %s", it.new_version.c_str());
    printf("\n");
  }
}
