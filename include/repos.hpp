#ifndef REPOS_HPP
#define REPOS_HPP

#include <string>
#include <vector>

//types
typedef struct package_update
{
  std::string name;
  std::string current_version;
  std::string new_version;

  long unsigned int download_size;
  long unsigned int new_install_size;
  long unsigned int current_install_size;
  long int net_size;
} package_update;

typedef struct repo_update
{
  std::vector<package_update> packages;
  std::string name;
  unsigned int name_max_length;
  unsigned int vcur_max_length;
  unsigned int vnew_max_length;

  long unsigned int download_size;
  long unsigned int new_install_size;
  long unsigned int current_install_size;
  long int net_size;

  long unsigned int max_download_size;
  long unsigned int max_net_size;

} repo_update;

extern repo_update repo, aur;

#endif //REPOS_HPP
