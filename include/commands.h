#ifndef COMMANDS_H
#define COMMANDS_H

#include "commands_gen.h"

// pacman
#define AUR_FETCH_COMMAND                 "yay -Qau | cut -d' ' -f1"
#define AUR_FETCH_ALL_COMMAND             "yay -Qm | cut -d' ' -f1"

#define PACMAN_UPDATE_COMMAND             "sudo pacman -Syu"
#define PACMAN_UPDATE_COMMAND_NOCONFIRM   "sudo pacman -Syu --noconfirm"
#define AUR_UPDATE_COMMAND                "yay -Sau"
#define AUR_UPDATE_COMMAND_NOCONFIRM      "yay -Sau --noconfirm"
#define YAY_UPDATE_COMMAND                "yay -Syu"
#define YAY_UPDATE_COMMAND_NOCONFIRM      "yay -Syu --noconfirm"

// apt/dpkg
#define APT_UPDATE_COMMAND                "sudo apt-get update && sudo apt-get full-upgrade"
#define APT_UPDATE_COMMAND_NOCONFIRM      "sudo apt-get update && sudo apt-get -y full-upgrade"


#endif //COMMANDS_H
