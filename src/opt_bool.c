#include "opt_bool.h"

// OPTIONS
//help
bool opt_help=false;
//targets
bool opt_repo=false;
bool opt_aur=false;
//print
bool opt_pall=false;
bool opt_notitles=false;
bool opt_nocolor=false;
bool opt_plist=false;
bool opt_plistraw=false;
bool opt_psizes=false;
bool opt_pdownload=false;
bool opt_pinstall=false;
bool opt_pnet=false;
//size
unsigned int size_index=2;
//operation
bool opt_update=false;
bool opt_noconfirm=false;
//package man
bool opt_pacman=false;
bool opt_apt=false;

// COMBINES
//target
bool combine_target=false;
//print
bool combine_list=false;
bool combine_size=false;
bool combine_print=false;
//operation
bool combine_op=false;
bool combine_op_any=false;
//fetch required
bool combine_fetch=false;
