#ifndef PRINT_HPP
#define PRINT_HPP

#include <string>

#include <ztd/color.hpp>

#include "repos.hpp"
#include "colors.hpp"

//constants
extern const char* size_suffixes[7];
extern const int size_print_padding;

//tool
std::string strpf(std::string const& format, std::string const& var);

//functions
void help();

std::pair<double, const char*> convertN(const int64_t size, uint32_t n);

std::pair<double, const char*> convertHReadable(const int64_t size);

void print_separator(uint32_t length, ztd::color color, char sepchar='=');

void print_update(repo_update& ru, ztd::color color, bool dlsize=false, bool nisize=false, bool nusize=false, bool print_only_install=false);

void print_update_sizes(repo_update& ru, ztd::color color, bool dlsize, bool nisize, bool cisize, bool notitle, uint32_t padsize);

void print_size(int64_t size, bool printTitle=false, std::string title="", int padding=0, ztd::color color=ztd::color::none, uint32_t precision=2, uint32_t sizepow=2, const char* line_end="\n", int sizepad=-1);

void print_listraw(repo_update& ru);

#endif //PRINT_HPP
