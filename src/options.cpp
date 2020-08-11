#include "options.hpp"

ztd::option_set options;

void help()
{
  printf("zupdate [option...] [package...]\n");
  printf("Safely view package updates in a fancy list format.\n");
  printf("Supported package managers: pacman, apt\n");
  printf("\n");
  printf("Options:\n");
  options.print_help(5,23);
  printf("\n");
  printf("Option order does not matter\n");
  printf("No print or operation options suggests -p\n");
  printf("No target specified will target all\n");
  printf("Default size in MB\n");
}

void create_options()
{
  options.add(
    ztd::option("\r  [HELP]"),
    ztd::option('h', "help",          false, "Print this help message"),
    ztd::option("\r  [PRINT]"),
    ztd::option('p', "print",         false, "Print all update info"),
    ztd::option('l', "list",          false, "Print a detailed list of packages"),
    ztd::option('L', "list-raw",      false, "Print a raw list of packages"),
    ztd::option('s', "size",          false, "Print all sizes"),
    ztd::option('d', "download-size", false, "Download size (no AUR)"),
    ztd::option('i', "install-size",  false, "Install size (no AUR)"),
    ztd::option('n', "net-size",      false, "Net difference size (no AUR)"),
    ztd::option('k', "no-titles",     false, "Don't print titles on sizes"),
    ztd::option('I', "list-install",  false, "Print install size on list"),
    ztd::option('C', "no-color",      false, "Don't print colors"),
    ztd::option("\r  [OPERATION]"),
    ztd::option('u', "update",        false, "Update targeted repositories"),
    ztd::option('y', "noconfirm",     false, "Doesn't ask for confirmation"),
    ztd::option("\r  [PKGMAN]"),
    ztd::option(     "pacman",        false, "Force pacman as package manager"),
    ztd::option(     "apt",           false, "Force apt as package manager"),
    ztd::option("\r  [TARGET]"),
    ztd::option('A', "all",           false, "Target all packages instead of just updated ones"),
    ztd::option('r', "repo",          false, "Target official repository (pacman only)"),
    ztd::option('a', "aur",           false, "Target AUR (pacman only, yay required)"),
    ztd::option("\r  [SIZE]"),
    ztd::option('B', "byte",          false, "Print sizes in bytes"),
    ztd::option('K', "kilobyte",      false, "Print sizes in kilobytes"),
    ztd::option('M', "megabyte",      false, "Print sizes in megabytes"),
    ztd::option('G', "gigabyte",      false, "Print sizes in gigabytes")
  );
}

void options_bool()
{
  //help
  opt_help = options['h'];
  //targets
  opt_repo = options['r'];
  opt_aur = options['a'];
  //print
  opt_pall = options['p'];
  opt_notitles = options['k'];
  opt_nocolor = options['C'];
  opt_plist = options['l'];
  opt_plistraw = options['L'];
  opt_psizes = options['s'];
  opt_pdownload = options['d'];
  opt_pinstall = options['i'];
  opt_pnet = options['n'];
  opt_linstall = options['I'];
  //size
  if(options['B']) size_index = 0;
  if(options['K']) size_index = 1;
  if(options['M']) size_index = 2;
  if(options['G']) size_index = 3;
  //operation
  opt_rall = options['A'];
  opt_update = options['u'];
  opt_noconfirm = options['y'];
  //packageman
  opt_pacman = options["pacman"];
  opt_apt = options["apt"];
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
