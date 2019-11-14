#include "package_man.hpp"

#include <stdio.h>
#include <signal.h>

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

void repo_print_process(repo_update& ru, ztd::color cl, bool print_size=true)
{
  //only if there are packages
  if(ru.packages.size() > 0)
  {
    //list
    if( opt_plist )
      print_update(ru, cl, print_size && opt_pdownload, print_size && opt_pinstall, print_size && opt_pnet);
    //sizes
    if( print_size )
      print_update_sizes(ru, cl, opt_pdownload, opt_pinstall, opt_pnet, opt_notitles);
  }
}

void pacman_process(bool yay)
{
  if(!exec_find("pacman"))
  {
    std::cerr << "pacman not found\n";
    return;
  }
  //fetch
  if(combine_fetch)
  {
    #pragma omp parallel sections
    {
      #pragma omp section
      {
        if(opt_repo)
          fetch_update(&repo, "REPO", PACMAN_FETCH_COMMAND);
      }
      #pragma omp section
      {
        if(opt_aur && yay)
          fetch_update(&aur, "AUR", AUR_FETCH_COMMAND);
      }
    }
  }

  //process
  if(opt_repo)
  {
    //size fetch
    if( combine_size )
    {
      import_sizes(&repo, PACMAN_EXT_INFO_COMMAND, PACMAN_LOCAL_INFO_COMMAND, PACMAN_EXT_SIZE_CUT_COMMAND, PACMAN_LOCAL_SIZE_CUT_COMMAND);
    }

    repo_print_process(repo, ztd::color::b_white);

    if(opt_plistraw)
    {
      print_listraw(repo);
    }

    if(opt_update)
    {
      signal(SIGINT, SIG_IGN);
      if(opt_noconfirm)
        system(PACMAN_UPDATE_COMMAND_NOCONFIRM);
      else
        system(PACMAN_UPDATE_COMMAND);
    }


  }
  if(opt_aur && yay)
  {
    repo_print_process(aur, ztd::color::b_cyan, false);

    if(opt_plistraw)
    {
      print_listraw(aur);
    }

    if(opt_update)
    {
      signal(SIGINT, SIG_IGN);
      if(opt_noconfirm)
        system(AUR_UPDATE_COMMAND_NOCONFIRM);
      else
        system(AUR_UPDATE_COMMAND);
    }
  }
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

void apt_process()
{
  if(!exec_find("apt"))
  {
    std::cerr << "apt not found\n";
    return;
  }
  if(!exec_find("apt-cache"))
  {
    std::cerr << "apt-cache not found\n";
    return;
  }
  if(!exec_find("dpkg"))
  {
    std::cerr << "dpkg not found\n";
    return;
  }
  if(combine_fetch)
  {
    fetch_update(&repo, apt_getrepo(), APT_FETCH_COMMAND);
  }
  if( combine_size )
  {
    import_sizes(&repo, APT_EXT_INFO_COMMAND, APT_LOCAL_INFO_COMMAND, APT_EXT_SIZE_CUT_COMMAND, APT_LOCAL_SIZE_CUT_COMMAND);
  }

  repo_print_process(repo, ztd::color::b_white);

  if(opt_plistraw)
  {
    print_listraw(repo);
  }

  if(opt_update)
  {
    signal(SIGINT, SIG_IGN);
    if(opt_noconfirm)
      system(APT_UPDATE_COMMAND_NOCONFIRM);
    else
      system(APT_UPDATE_COMMAND);
  }
}
