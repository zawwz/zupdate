#include "package_man.hpp"

#include <cstdio>
#include <csignal>
#include <cmath>

#include <algorithm>

#include "fetch.hpp"
#include "print.hpp"
#include "repos.hpp"

#include "opt_bool.h"
#include "commands.h"

#include <ztd/shell.hpp>

package_manager cur_pkgman = none;

bool exec_find(const std::string& name)
{
  std::string which=ztd::sh("which "+name+" 2>/dev/null");
  return which != "";
}

uint32_t req_pad_size(repo_update& ru)
{
  int pd, pi, pn;
  pd=std::max(0.0, log10(ru.download_size)    - 3*size_index);
  pi=std::max(0.0, log10(ru.new_install_size) - 3*size_index);
  pn=std::max(0.0, log10(ru.net_size)         - 3*size_index + (ru.net_size<0?1:0));
  return std::ceil(std::max(std::max(pd, pi), pn)) + (size_index>0?7:2);
}

void repo_print_process(repo_update& ru, ztd::color cl, bool print_size=true)
{
  //only if there are packages
  if(opt_plistraw)
    print_listraw(repo);
  else if(ru.packages.size() > 0)
  {
    //list
    if( opt_plist )
      print_update(ru, cl, print_size && opt_pdownload, print_size && opt_pinstall, print_size && opt_pnet);
    // raw list

    uint32_t padsize=req_pad_size(ru);
    if( opt_plist && print_size )
      print_separator(padsize+22, cl);

    padsize -= size_index>0 ? 3 : 1;
    //sizes
    if( print_size )
      print_update_sizes(ru, cl, opt_pdownload, opt_pinstall, opt_pnet, opt_notitles, padsize);
  }
}

int pacman_process(bool yay)
{
  int r=0, r2=0;

  if(!exec_find("pacman"))
  {
    std::cerr << "pacman not found\n";
    return 1;
  }
  //fetch
  if(combine_fetch)
  {
    #pragma omp parallel sections
    {
      #pragma omp section
      {
        if(opt_repo)
          r = fetch_update(&repo, "REPO", PACMAN_FETCH_COMMAND);
      }
      #pragma omp section
      {
        if(opt_aur && yay)
          r2 = fetch_update(&aur, "AUR", AUR_FETCH_COMMAND);
      }
    }
    if(r!=0)
      return r;
    if(r2!=0)
      return r2;
  }

  //process
  if(opt_repo)
  {
    //size fetch
    if( combine_size )
    {
      r = import_sizes(&repo, PACMAN_EXT_SIZE_COMMAND, PACMAN_LOCAL_SIZE_COMMAND);
    }
    if(r!=0)
      return r;

    repo_print_process(repo, ztd::color::b_white);

    if(opt_update)
    {
      signal(SIGINT, SIG_IGN);
      if(opt_noconfirm)
        r = ztd::shr(PACMAN_UPDATE_COMMAND_NOCONFIRM);
      else
        r = ztd::shr(PACMAN_UPDATE_COMMAND);

      if(r!=0)
        return r;
    }


  }
  if(opt_aur && yay)
  {
    repo_print_process(aur, ztd::color::b_cyan, false);

    if(opt_update)
    {
      signal(SIGINT, SIG_IGN);
      if(opt_noconfirm)
        r = ztd::shr(AUR_UPDATE_COMMAND_NOCONFIRM);
      else
        r = ztd::shr(AUR_UPDATE_COMMAND);

      if(r!=0)
        return r;
    }
  }
  return 0;
}

std::string apt_getrepo()
{
  if ( ztd::sh("apt-cache policy | grep '/main' -A2 | grep '=exp'").size()>0)
    return "EXPERIMENTAL";
  if ( ztd::sh("apt-cache policy | grep '/main' -A2 | grep '=unstable'").size()>0)
    return "UNSTABLE";
  if ( ztd::sh("apt-cache policy | grep '/main' -A2 | grep '=stable'").size()>0)
    return "STABLE";
  if ( ztd::sh("apt-cache policy | grep '/main' -A2 | grep '=oldstable'").size()>0)
    return "OLDSTABLE";

  return "UNKNOWN";
}

int apt_process()
{
  int r=0;

  if(!exec_find("apt"))
  {
    std::cerr << "apt not found\n";
    return 1;
  }
  if(!exec_find("apt-get"))
  {
    std::cerr << "apt-get not found\n";
    return 1;
  }
  if(!exec_find("apt-cache"))
  {
    std::cerr << "apt-cache not found\n";
    return 1;
  }
  if(!exec_find("dpkg"))
  {
    std::cerr << "dpkg not found\n";
    return 1;
  }

  if( combine_fetch )
  {
    r = fetch_update(&repo, apt_getrepo(), APT_FETCH_COMMAND);
  }
  if(r!=0)
    return r;

  if( combine_size )
  {
    r = import_sizes(&repo, APT_EXT_SIZE_COMMAND, APT_LOCAL_SIZE_COMMAND);
  }
  if(r!=0)
    return r;

  repo_print_process(repo, ztd::color::b_white);

  if(opt_update)
  {
    signal(SIGINT, SIG_IGN);
    if(opt_noconfirm)
      r = ztd::shr(APT_UPDATE_COMMAND_NOCONFIRM);
    else
      r = ztd::shr(APT_UPDATE_COMMAND);

    if(r!=0)
      return r;
  }
  return 0;
}
