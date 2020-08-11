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

  uint64_t download_size;
  uint64_t new_install_size;
  uint64_t current_install_size;
  int64_t net_size;

  bool has_update;

} package_update;

typedef struct repo_update
{
  std::vector<package_update> packages;
  std::string name;
  uint32_t name_max_length;
  uint32_t vcur_max_length;
  uint32_t vnew_max_length;

  uint64_t download_size;
  uint64_t new_install_size;
  uint64_t current_install_size;
  int64_t net_size;

  uint64_t max_install_size;
  uint64_t max_download_size;
  uint64_t max_net_size;

  uint64_t n_updates;
  bool has_update;

} repo_update;

extern repo_update repo, aur;

#endif //REPOS_HPP
