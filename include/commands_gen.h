#ifndef COMMANDS_GEN_H
#define COMMANDS_GEN_H

#define PACMAN_FETCH_COMMAND "CHECKUPDATES_DB=\"${TMPDIR:-/tmp}/checkup-db-$USER/\"\nDBPath=\"$(pacman-conf DBPath)\"\nmkdir -p \"$CHECKUPDATES_DB\"\nln -sf \"$DBPath/local\" \"$CHECKUPDATES_DB\" > /dev/null 2>&1\nfakeroot pacman -Sy --dbpath \"$CHECKUPDATES_DB\" --logfile /dev/null >/dev/null || exit $?\npacman -Qu --dbpath \"$CHECKUPDATES_DB/\" 2> /dev/null | cut -d ' ' -f1 | grep -v '\\[.*\\]'"
#define PACMAN_EXT_SIZE_COMMAND "pacman -Si --dbpath \"${TMPDIR:-/tmp}/checkup-db-$USER/\" --logfile /dev/null %s | grep -E '^Version|^Download Size|^Installed Size' | cut -d':' -f2- | sed 's/ //g;1!s/iB$//g;1!s/B//g'"
#define APT_LOCAL_SIZE_COMMAND "dpkg -s %s 2>/dev/null| grep -E '^Version:|^Installed-Size:' | awk '{if(NR==1) tmp=$2*1024;else print $2}END{printf \"%%u\\\\n\", tmp}'"
#define PACMAN_LOCAL_SIZE_COMMAND "pacman -Qi %s | grep -E '^Version|^Installed Size' | cut -d':' -f2- | tr -d ' ' | sed 's/ //g;1!s/iB$//g;1!s/B//g'"
#define APT_FETCH_COMMAND "sudo apt-get update >/dev/null || exit $?\napt list --upgradable 2>/dev/null | tail -n +2 | cut -d'/' -f1"
#define APT_FETCH_ALL_COMMAND "sudo apt-get update >/dev/null || exit $?\ndpkg-query -f '${binary:Package}\\n' -W | cut -d ':' -f1"
#define APT_EXT_SIZE_COMMAND "apt-cache show --no-all-versions %s 2>/dev/null| grep -E '^Version:|^Installed-Size:|^Size:' | awk '{if(NR==2) tmp=$2*1024;else print $2}END{printf \"%%u\\\\n\", tmp}'"
#define PACMAN_FETCH_ALL_COMMAND "CHECKUPDATES_DB=\"${TMPDIR:-/tmp}/checkup-db-$USER/\"\nDBPath=\"$(pacman-conf DBPath)\"\nmkdir -p \"$CHECKUPDATES_DB\"\nln -sf \"$DBPath/local\" \"$CHECKUPDATES_DB\" > /dev/null 2>&1\nfakeroot pacman -Sy --dbpath \"$CHECKUPDATES_DB\" --logfile /dev/null >/dev/null || exit $?\npacman -Qn --dbpath \"$CHECKUPDATES_DB/\" 2> /dev/null | cut -d ' ' -f1 | grep -v '\\[.*\\]'"

#endif
