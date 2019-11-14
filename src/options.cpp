#include "options.hpp"

ztd::option_set options;

void help()
{
  printf("zupdate [options]\n\nOptions:\n");
  options.print_help(5,23);
  printf("\n");
  printf("Option order does not matter\n");
  printf("No print or operation options suggests -p\n");
  printf("No target specified will target all\n");
  printf("Default size in MB\n");
}

void create_options()
{
  options.add(ztd::option("\r  [HELP]"));
  options.add(ztd::option('h', "help",          false, "Print this help message"));
  options.add(ztd::option("\r  [PRINT]"));
  options.add(ztd::option('p', "print",         false, "Print all update info"));
  options.add(ztd::option('l', "list",          false, "Print a detailed list of packages"));
  options.add(ztd::option('L', "list-raw",      false, "Print a raw list of packages"));
  options.add(ztd::option('s', "size",          false, "Print all sizes"));
  options.add(ztd::option('d', "download-size", false, "Download size (repo only)"));
  options.add(ztd::option('i', "install-size",  false, "Install size (repo only)"));
  options.add(ztd::option('n', "net-size",      false, "Net difference size (repo only)"));
  options.add(ztd::option('k', "no-titles",     false, "Don't print titles on -d -i and -n"));
  options.add(ztd::option("\r  [OPERATION]"));
  options.add(ztd::option('u', "update",        false, "Update targeted repositories"));
  options.add(ztd::option('y', "noconfirm",     false, "Doesn't ask for confirmation"));
  options.add(ztd::option("\r  [PKGMAN]"));
  options.add(ztd::option(     "pacman",        false, "Force pacman as package manager"));
  options.add(ztd::option(     "apt",           false, "Force apt as package manager"));
  options.add(ztd::option("\r  [TARGET]"));
  options.add(ztd::option('r', "repo",          false, "Target official repository (pacman only)"));
  options.add(ztd::option('a', "aur",           false, "Target AUR (pacman only, yay required)"));
  options.add(ztd::option("\r  [SIZE]"));
  options.add(ztd::option('B', "byte",          false, "Print sizes in bytes"));
  options.add(ztd::option('K', "kilobyte",      false, "Print sizes in kilobytes"));
  options.add(ztd::option('M', "megabyte",      false, "Print sizes in megabytes"));
  options.add(ztd::option('G', "gigabyte",      false, "Print sizes in gigabytes"));
}

void process_options(int argc, char** argv)
{
  options.process(argc, argv);
}

void options_bool()
{
  //help
  opt_help = options.find('h')->activated;
  //targets
  opt_repo = options.find('r')->activated;
  opt_aur = options.find('a')->activated;
  //print
  opt_pall = options.find('p')->activated;
  opt_notitles = options.find('k')->activated;
  opt_plist = options.find('l')->activated;
  opt_plistraw = options.find('L')->activated;
  opt_psizes = options.find('s')->activated;
  opt_pdownload = options.find('d')->activated;
  opt_pinstall = options.find('i')->activated;
  opt_pnet = options.find('n')->activated;
  //size
  if(options.find('B')->activated) size_index = 0;
  if(options.find('K')->activated) size_index = 1;
  if(options.find('M')->activated) size_index = 2;
  if(options.find('G')->activated) size_index = 3;
  //operation
  opt_update = options.find('u')->activated;
  opt_noconfirm = options.find('y')->activated;
  //packageman
  opt_pacman = options.find("pacman")->activated;
  opt_apt = options.find("apt")->activated;
}

void process_combines()
{
  //target
  combine_target = opt_repo || opt_aur;
  //print
  combine_list = opt_pall || opt_plist;
  combine_size = opt_pall || opt_psizes || opt_pdownload || opt_pinstall || opt_pnet;
  combine_print = combine_list || combine_size || opt_plistraw;
  //operation
  combine_op = opt_update;
  combine_op_any = combine_print || combine_op;
  //fetch required
  combine_fetch = combine_print;
}
