#ifndef PACKAGE_MAN_HPP
#define PACKAGE_MAN_HPP

#include <string>
#include <vector>

// types
enum package_manager { none, pacman, apt, dnf };

// globals
extern package_manager cur_pkgman;

// functions
bool exec_find(const std::string& name);

int pacman_process(const std::vector<std::string>& args, bool yay);

int apt_process(const std::vector<std::string>& args);

#endif //PACKAGE_MAN_HPP
