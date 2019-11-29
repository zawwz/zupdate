#ifndef COLORS_HPP
#define COLORS_HPP

#include <ztd/color.hpp>

#include <utility>
#include <string>

// Global constants
extern const ztd::color no_color;
extern const ztd::color repo_color;
extern const ztd::color aur_color;
extern const ztd::color error_color;

extern const ztd::color new_version_color;
extern const ztd::color old_version_color;
extern const ztd::color neutral_version_color;

std::pair<std::string, std::string> color_diff(std::string const& str1, std::string const& str2);

#endif //COLORS_HPP
