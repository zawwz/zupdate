#include <stdio.h>

#include <thread>

#include "options.hpp"
#include "package_man.hpp"

long unsigned int new_download_size=0;
long unsigned int new_install_size=0;
long unsigned int current_install_size=0;
long int net_size=0;

int main(int argc, char* argv[])
{
  //option process start
  try
  {
    create_options();
    process_options(argc, argv);
    options_bool();
    process_combines();
  }
  catch(ztd::option_error& e)
  {
    printf("%s\n", e.what());
    return 1;
  }

  if( !combine_target ) //no target -> all
  {
    opt_repo = true;
    opt_aur = true;
  }
  if( !combine_op_any ) //no operation -> print all
  {
    opt_pall = true;
  }

  if ( opt_pall ) //all -> list + sizes
  {
    opt_plist=true;
    opt_psizes=true;
  }
  if ( opt_psizes ) //sizes -> download, install, net
  {
    opt_pdownload=true;
    opt_pinstall=true;
    opt_pnet=true;
  }
  process_combines();
  //option process end

  //halt options
  if( opt_help )
  {
    help();
    return 0;
  }

  if(opt_pacman)
    cur_pkgman=pacman;
  else if(opt_apt)
    cur_pkgman=apt;

  if(cur_pkgman == none)
  {
    if(exec_find("pacman"))
    {
      cur_pkgman=pacman;
    }
    else if(exec_find("apt") && exec_find("dpkg"))
    {
      cur_pkgman=apt;
    }
  }

  switch(cur_pkgman)
  {
    case pacman :
      pacman_process(exec_find("yay")); break;
    case apt :
      apt_process(); break;
    default : std::cerr << "Unsupported package manager\n"; break;
  }


  return 0;
}
