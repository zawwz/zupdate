#include "fetch.hpp"

#include <string>
#include <vector>
#include <thread>
#include <utility>

#include <ztd/shell.hpp>

#include "commands.h"

//functions
int fetch_update(repo_update* r, const std::string& name, const std::string& command)
{
  r->packages.clear();
  r->name=name;
  r->name_max_length=0;
  r->vcur_max_length=0;
  r->vnew_max_length=0;
  r->max_download_size=0;
  r->max_net_size=0;
  std::pair<std::string, int> cp = ztd::shp(command);
  if(cp.second != 0)
    return cp.second;
  std::string str=cp.first;

  unsigned long int i=0,j=0;
  while(i<str.size())
  {
    package_update pkg;
    //name
    while( str[i]!=' ' )
      i++;
    pkg.name = str.substr(j,i-j);
    if(pkg.name.size() > r->name_max_length)
      r->name_max_length = pkg.name.size();
    //end name
    i++;
    //current version
    j=i;
    while( str[i]!=' ' )
      i++;
    pkg.current_version = str.substr(j,i-j);
    if(pkg.current_version.size() > r->vcur_max_length)
      r->vcur_max_length = pkg.current_version.size();
    //end current version
    i++;
    //skip arrow
    while( str[i]!=' ' )
      i++;
    //end arrow
    i++;
    //new version
    j=i;
    while( str[i]!='\n' )
      i++;
    pkg.new_version = str.substr(j,i-j);
    if(pkg.new_version.size() > r->vnew_max_length)
      r->vnew_max_length = pkg.new_version.size();
    //end new version
    i++;
    j=i;
    r->packages.push_back(pkg);
  }
  return 0;
}



void get_ext_sizes(package_update* pkg, const char* info_command, const char* cut_command)
{
  std::string sizes=ztd::sh(info_command + pkg->name + cut_command);
  unsigned int i=0, j=0;
  while(sizes[i]!='\n')
    i++;
  pkg->download_size = std::stoul(sizes.substr(0,i));
  i++;
  j=i;
  while(sizes[i]!='\n')
    i++;
  pkg->new_install_size = std::stoul(sizes.substr(j,i-j));
}
void get_loc_size(package_update* pkg, const char* info_command, const char* cut_command)
{
  std::string size=ztd::sh(info_command + pkg->name + cut_command);
  if(size.size() > 0)
    pkg->current_install_size = std::stoul(size);
  else
    pkg->current_install_size = 0;
}

int import_sizes(repo_update* ru, const char* ext_info_command, const char* loc_info_command, const char* ext_cut_command, const char* loc_cut_command)
{
  const unsigned int n=ru->packages.size();
  #pragma omp parallel for
  for(unsigned int i=0; i<n; i++) //parallel
  {
    package_update* pkg = &(ru->packages[i]);
    get_ext_sizes(pkg, ext_info_command, ext_cut_command);
    get_loc_size(pkg, loc_info_command, loc_cut_command);
    pkg->net_size = (long int) pkg->new_install_size - (long int) pkg->current_install_size;
  }
  ru->download_size = 0;
  ru->new_install_size = 0;
  ru->current_install_size = 0;
  for(auto pkg : ru->packages)
  {
    ru->max_download_size=std::max(ru->max_download_size, pkg.download_size);
    ru->max_net_size=std::max(ru->max_net_size, (long unsigned int) labs(pkg.net_size));

    ru->download_size += pkg.download_size;
    ru->new_install_size += pkg.new_install_size;
    ru->current_install_size += pkg.current_install_size;
  }
  ru->net_size = (long int) ru->new_install_size - (long int) ru->current_install_size;
  return 0;
}
