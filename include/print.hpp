#ifndef PRINT_HPP
#define PRINT_HPP

#include <string>

#include <ztd/color.hpp>

#include "repos.hpp"
#include "colors.hpp"

//constants
extern const char* size_suffixes[6];
extern const int size_print_padding;

//functions
void help();

std::pair<double, const char*> convertN(const long int size, unsigned int n);

std::pair<double, const char*> convertHReadable(const long int size);

void print_update(repo_update& ru, ztd::color color, bool dlsize=false, bool nisize=false, bool nusize=false);

void print_update_sizes(repo_update& ru, ztd::color color, bool dlsize, bool nisize, bool cisize, bool notitle);

void print_size(long int size, bool printTitle=false, std::string title="", int padding=0, ztd::color color=ztd::color::none, unsigned int precision=2, unsigned int sizepow=2, const char* line_end="\n");

void print_listraw(repo_update& ru);

#endif //PRINT_HPP
