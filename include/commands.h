#ifndef COMMANDS_H
#define COMMANDS_H

// pacman
#define PACMAN_FETCH_COMMAND              "CHECKUPDATES_DB=\"${TMPDIR:-/tmp}/checkup-db-${USER}/\"\nDBPath=\"$(pacman-conf DBPath)\"\nmkdir -p \"$CHECKUPDATES_DB\"\nln -sf \"${DBPath}/local\" \"$CHECKUPDATES_DB\" > /dev/null 2>&1\nfakeroot pacman -Sy --dbpath \"$CHECKUPDATES_DB\" --logfile /dev/null >/dev/null || return $?\npacman -Qu --dbpath \"$CHECKUPDATES_DB/\" 2> /dev/null | grep -v '\\[.*\\]'\nexit 0"
#define AUR_FETCH_COMMAND                 "yay -Qau"

#define PACMAN_UPDATE_COMMAND             "sudo pacman -Syu"
#define PACMAN_UPDATE_COMMAND_NOCONFIRM   "sudo pacman -Syu --noconfirm"
#define AUR_UPDATE_COMMAND                "yay -Sau"
#define AUR_UPDATE_COMMAND_NOCONFIRM      "yay -Sau --noconfirm"

#define PACMAN_EXT_SIZE_COMMAND       "pacman -Si --dbpath \"${TMPDIR:-/tmp}/checkup-db-${USER}/\" --logfile /dev/null %s |grep -E 'Download Size|Installed Size'|cut -d':' -f2|tr -d ' '|cut -d'i' -f1 | tr -d 'B'|numfmt --from=iec"
#define PACMAN_LOCAL_SIZE_COMMAND     "pacman -Qi %s |grep 'Installed Size'|cut -d':' -f2|tr -d ' '|cut -d'i' -f1 | tr -d 'B'|numfmt --from=iec|tr -d '\n'"

// apt/dpkg
#define APT_FETCH_COMMAND                 "sudo apt update >/dev/null 2>&1 || return $?\napt list --upgradable 2>/dev/null | tail -n +2 | awk -F \"/\" '{print $1\" \"$2}' | tr -d ']' | awk '{print $1\" \"$7\" -> \"$3}'"
#define APT_UPDATE_COMMAND                "sudo apt update && sudo apt upgrade"
#define APT_UPDATE_COMMAND_NOCONFIRM      "sudo apt update && sudo apt -y upgrade"

#define APT_EXT_SIZE_COMMAND          "apt show %s 2>/dev/null| grep -E 'Installed-Size:|Download-Size:' | cut -d' ' -f2- | tr -d ', B' | tr 'k' 'K' | numfmt --from=iec | awk '{s=$0;getline;s=$0\"\\n\"s;print s}'"
#define APT_LOCAL_SIZE_COMMAND        "dpkg -s %s 2>/dev/null| grep 'Installed-Size:' | cut -d' ' -f2 | xargs echo '1024 *' | bc"



#endif //COMMANDS_H
