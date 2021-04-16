#include <cstdio>

#include <stdlib.h>

#include <thread>

#include "options.hpp"
#include "package_man.hpp"

int main(int argc, char* argv[])
{
  setenv("LANG", "en_US.UTF-8", 1);
  std::vector<std::string> args;
  //option process start
  try
  {
    create_options();
    args=options.process(argc, argv);
    options_bool();
    process_combines();
  }
  catch(ztd::option_error& e)
  {
    printf("%s\n", e.what());
    if(e.type() == ztd::option_error::unknown_option)
      help();
    return 1;
  }

  if( !combine_target ) //no target -> all targets
  {
    opt_repo = true;
    opt_aur = true;
  }
  if( !combine_op_any ) //no operation -> print all
  {
    opt_pall = true;
  }

  if ( opt_pall ) //pall -> list + sizes
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
      return pacman_process(args, exec_find("yay")); break;
    case apt :
      return apt_process(args); break;
    default :
      std::cerr << "Unsupported package manager\n";
      return 1;
      break;
  }
}
