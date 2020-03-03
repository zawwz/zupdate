#ifndef FETCH_HPP
#define FETCH_HPP

#include "repos.hpp"

//functions
int fetch_update(repo_update* r, const std::string& name, const std::string& command);

int import_sizes(repo_update* ru, const char* ext_size_command, const char* loc_size_command);

#endif //FETCH_HPP
