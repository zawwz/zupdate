#include "fetch.hpp"

#include <string>
#include <vector>
#include <thread>
#include <utility>

#include <ztd/shell.hpp>

#include "commands.h"
#include "print.hpp"

#define KB_MULT 1024.0
#define MB_MULT 1048576.0
#define GB_MULT 1073741824.0
#define TB_MULT 1099511627776.0
#define PB_MULT 1125899906842624.0
#define EB_MULT 1152921504606846976.0

uint64_t sizeconvert(std::string const& in)
{
  if(in.size()==0)
    return 0;
  double num=std::stod(in.substr(0, in.size()-1));
  switch(in[in.size()-1])
  {
    case 'K': return (uint64_t) (num * KB_MULT); break;
    case 'M': return (uint64_t) (num * MB_MULT); break;
    case 'G': return (uint64_t) (num * GB_MULT); break;
    case 'T': return (uint64_t) (num * TB_MULT); break;
    case 'P': return (uint64_t) (num * PB_MULT); break;
    case 'E': return (uint64_t) (num * EB_MULT); break;
    default: return std::stoul(in);
  }
}

package_update new_package()
{
  package_update ret;
  ret.download_size = 0;
  ret.new_install_size = 0;
  ret.current_install_size = 0;
  ret.net_size = 0;
  ret.has_update = false;
  return ret;
}

//functions
int fetch_update(repo_update* r, const std::string& name, const std::string& command, std::vector<std::string> const& args)
{
  r->packages.clear();
  r->name=name;
  r->download_size = r->new_install_size = r->current_install_size = 0;
  r->name_max_length = r->vcur_max_length = r->vnew_max_length = 0;
  r->max_install_size = r->max_download_size = r->max_net_size = r->n_updates = 0;
  r->has_update=false;

  std::string sup;
  if(args.size()>0)
  {
    sup += "|grep -E '";
    for(auto it: args)
    {
      sup+="(^"+it+"$)|";
    }
    sup.pop_back();
    sup += '\'';
  }

  std::pair<std::string, int> cp = ztd::shp(command+sup+"\nexit 0");
  if(cp.second != 0)
    return cp.second;
  std::string str=cp.first;

  size_t i=0,j=0;
  while(j<str.size())
  {
    package_update pkg = new_package();
    i=str.find('\n',j);
    pkg.name = str.substr(j,i-j);
    j=i+1;
    r->packages.push_back(pkg);
  }
  return 0;
}


void get_ext_sizes(package_update* pkg, const char* command)
{
  std::string str=ztd::sh(strpf(command, pkg->name));
  uint32_t i=0, j=0;
  i=str.find('\n');
  pkg->new_version = str.substr(0,i);
  j=i+1;
  i=str.find('\n',j);
  pkg->download_size = sizeconvert(str.substr(j,i-j));
  j=i+1;
  i=str.find('\n',j);
  pkg->new_install_size = sizeconvert(str.substr(j,i-j));
}
void get_loc_size(package_update* pkg, const char* command)
{
  std::string str=ztd::sh(strpf(command, pkg->name));
  uint32_t i=0, j=0;
  i=str.find('\n',i);
  pkg->current_version = str.substr(0,i);
  j=i+1;
  i=str.find('\n',j);
  pkg->current_install_size = sizeconvert(str.substr(j,i-j));
}

int import_sizes(repo_update* ru, const char* ext_size_command, const char* loc_size_command)
{
  const uint32_t n=ru->packages.size();
  #pragma omp parallel for
  for(uint32_t i=0; i<n; i++) //parallel
  {
    package_update* pkg = &(ru->packages[i]);

    // sizes import
    if(ext_size_command!=NULL) get_ext_sizes(pkg, ext_size_command);
    if(loc_size_command!=NULL) get_loc_size(pkg, loc_size_command);

    // has new version
    if(pkg->current_version != "" && pkg->new_version != "" && pkg->new_version != pkg->current_version)
    {
      pkg->has_update=true;
      ru->has_update=true;
    }
    else
      pkg->has_update=false;

    // net size
    pkg->net_size = (int64_t) pkg->new_install_size - (int64_t) pkg->current_install_size;
  }

  // post compute
  for(auto pkg : ru->packages)
  {
    // string lengths
    ru->name_max_length = std::max(ru->name_max_length, (uint32_t) pkg.name.size());
    ru->vnew_max_length = std::max(ru->vnew_max_length, (uint32_t) pkg.new_version.size());
    ru->vcur_max_length = std::max(ru->vcur_max_length, (uint32_t) pkg.current_version.size());

    // biggest sizes
    ru->max_install_size  = std::max(ru->max_install_size, pkg.has_update ? pkg.new_install_size : pkg.current_install_size);
    ru->max_download_size = std::max(ru->max_download_size, pkg.download_size);
    ru->max_net_size      = std::max(ru->max_net_size, (uint64_t) labs(pkg.net_size));

    // size sums
    if(pkg.has_update) ru->download_size += pkg.download_size;
    ru->new_install_size += pkg.has_update ? pkg.new_install_size : pkg.current_install_size;
    ru->current_install_size += pkg.current_install_size;

    // count updates
    if(pkg.has_update) ru->n_updates++;
  }

  // global net size
  ru->net_size = (int64_t) ru->new_install_size - (int64_t) ru->current_install_size;

  return 0;
}
