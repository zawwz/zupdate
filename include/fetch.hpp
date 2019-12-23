#ifndef FETCH_HPP
#define FETCH_HPP

#include "repos.hpp"

//functions
int fetch_update(repo_update* r, const std::string& name, const std::string& command);

int import_sizes(repo_update* ru, const char* ext_info_command, const char* loc_info_command, const char* ext_cut_command, const char* loc_cut_command);

#endif //FETCH_HPP
