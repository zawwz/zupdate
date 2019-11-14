#ifndef PACKAGE_MAN_HPP
#define PACKAGE_MAN_HPP

#include <string>

// types
enum package_manager { none, pacman, apt, dnf };

// globals
extern package_manager cur_pkgman;

// functions
bool exec_find(const std::string& name);

void pacman_process(bool yay);

void apt_process();

#endif //PACKAGE_MAN_HPP
