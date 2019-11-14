#ifndef OPT_BOOL_H
#define OPT_BOOL_H

// OPTIONS
//help
extern bool opt_help;
//targets
extern bool opt_repo;
extern bool opt_aur;
//print
extern bool opt_pall;
extern bool opt_notitles;
extern bool opt_plist;
extern bool opt_plistraw;
extern bool opt_psizes;
extern bool opt_pdownload;
extern bool opt_pinstall;
extern bool opt_pnet;
//size
extern unsigned int size_index;
//operation
extern bool opt_update;
extern bool opt_noconfirm;
//package man
extern bool opt_pacman;
extern bool opt_apt;

// COMBINES
//target
extern bool combine_target;
//print
extern bool combine_list;
extern bool combine_size;
extern bool combine_print;
//operation
extern bool combine_op;
extern bool combine_op_any;
//fetch required
extern bool combine_fetch;

#endif //OPT_BOOL_H
