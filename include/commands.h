#ifndef COMMANDS_H
#define COMMANDS_H

// pacman
#define PACMAN_FETCH_COMMAND              "CHECKUPDATES_DB=\"${TMPDIR:-/tmp}/checkup-db-${USER}/\"\nDBPath=\"$(pacman-conf DBPath)\"\nmkdir -p \"$CHECKUPDATES_DB\"\nln -sf \"${DBPath}/local\" \"$CHECKUPDATES_DB\" > /dev/null 2>&1\nfakeroot pacman -Sy --dbpath \"$CHECKUPDATES_DB\" --logfile /dev/null >/dev/null || return $?\npacman -Qu --dbpath \"$CHECKUPDATES_DB/\" 2> /dev/null | grep -v '\\[.*\\]'\nreturn 0"
#define AUR_FETCH_COMMAND                 "yay -Qau"

#define PACMAN_UPDATE_COMMAND             "sudo pacman -Syu"
#define PACMAN_UPDATE_COMMAND_NOCONFIRM   "sudo pacman -Syu --noconfirm"
#define AUR_UPDATE_COMMAND                "yay -Sau"
#define AUR_UPDATE_COMMAND_NOCONFIRM      "yay -Sau --noconfirm"

#define PACMAN_EXT_INFO_COMMAND           "pacman -Si --dbpath \"${TMPDIR:-/tmp}/checkup-db-${USER}/\" --logfile /dev/null "
#define PACMAN_LOCAL_INFO_COMMAND         "pacman -Qi "

#define PACMAN_EXT_SIZE_CUT_COMMAND       " |grep -E 'Download Size|Installed Size'|cut -d':' -f2|tr -d ' '|cut -d'i' -f1 | tr -d 'B'|numfmt --from=iec"
#define PACMAN_LOCAL_SIZE_CUT_COMMAND     " |grep 'Installed Size'|cut -d':' -f2|tr -d ' '|cut -d'i' -f1 | tr -d 'B'|numfmt --from=iec|tr -d '\n'"

// apt/dpkg
#define APT_FETCH_COMMAND                 "sudo apt update >/dev/null || return $?\napt list --upgradable 2>/dev/null | tail -n +2 | awk -F \"/\" '{print $1\" \"$2}' | tr -d ']' | awk '{print $1\" \"$7\" -> \"$3}'"
#define APT_UPDATE_COMMAND                "sudo apt upgrade"
#define APT_UPDATE_COMMAND_NOCONFIRM      "echo y | sudo apt upgrade"

#define APT_EXT_INFO_COMMAND              "apt show "
#define APT_LOCAL_INFO_COMMAND            "dpkg -s "

#define APT_EXT_SIZE_CUT_COMMAND          " 2>/dev/null| grep -E 'Installed-Size:|Download-Size:' | cut -d' ' -f2- | tr -d ', B' | tr 'k' 'K' | numfmt --from=iec | awk '{s=$0;getline;s=$0\"\\n\"s;print s}'"
#define APT_LOCAL_SIZE_CUT_COMMAND        " 2>/dev/null| grep 'Installed-Size:' | cut -d' ' -f2 | xargs echo '1024 *' | bc"



#endif //COMMANDS_H
