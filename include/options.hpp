#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <ztd/options.hpp>

#include "opt_bool.h"

extern ztd::option_set options;

void help();

void create_options();

void process_options(int argc, char** argv);

void options_bool();

void process_combines();


#endif //OPTIONS_HPP
