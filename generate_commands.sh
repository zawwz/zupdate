#!/bin/sh

cd "$(dirname "$0")"

file=include/commands_gen.h

echo "#ifndef COMMANDS_GEN_H
#define COMMANDS_GEN_H
" > "$file"

for I in $(find commands -type f)
do
  name=$(basename "$I" | tr '[:lower:]' '[:upper:]')_COMMAND
  printf '#define %s "%s"\n' "$name" "$(sed '/^#/d' "$I" | tocstr)" >> "$file"
done

echo "
#endif" >> "$file"
