#ifndef PACKAGE_MAN_HPP
#define PACKAGE_MAN_HPP

#include <string>

// types
enum package_manager { none, pacman, apt, dnf };

// globals
extern package_manager cur_pkgman;

// functions
bool exec_find(const std::string& name);

int pacman_process(bool yay);

int apt_process();

#endif //PACKAGE_MAN_HPP
