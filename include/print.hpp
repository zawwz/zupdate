#ifndef PRINT_HPP
#define PRINT_HPP

#include <string>

#include <ztd/color.hpp>

#include "repos.hpp"
#include "colors.hpp"

//constants
extern const char* size_suffixes[6];
extern const int size_print_padding;

//tool
std::string strpf(std::string const& format, std::string const& var);

//functions
void help();

std::pair<double, const char*> convertN(const long int size, unsigned int n);

std::pair<double, const char*> convertHReadable(const long int size);

void print_separator(uint32_t length, ztd::color color, char sepchar='=');

void print_update(repo_update& ru, ztd::color color, bool dlsize=false, bool nisize=false, bool nusize=false);

void print_update_sizes(repo_update& ru, ztd::color color, bool dlsize, bool nisize, bool cisize, bool notitle, uint32_t padsize);

void print_size(long int size, bool printTitle=false, std::string title="", int padding=0, ztd::color color=ztd::color::none, unsigned int precision=2, unsigned int sizepow=2, const char* line_end="\n", int sizepad=-1);

void print_listraw(repo_update& ru);

#endif //PRINT_HPP
