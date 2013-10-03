#! /bin/bash
# Copyleft (C) KRT, 2013 by kiterunner_t

# set -x

_EXT_FILENAMES=$(cat <<-EOF
  sh
  bash
  pl
  erl
  [c|h]
  java
  lua
  el
  tcl
EOF
)

_EXCLUSIVE_DIRS=$(cat <<-EOF
  .git
  p 
  packages
EOF
)


_FIND_DIR=${1:-"."}
_FIND_FILE_PATTERN=
_SED_EXCLUSIVE_DIRS=


for _ext in $_EXT_FILENAMES; do
  _FIND_FILE_PATTERN+="-or -name \"*.$_ext\" "
done

_FIND_FILE_PATTERN=${_FIND_FILE_PATTERN/#-or /}


for _exclusive in $_EXCLUSIVE_DIRS; do
  _SED_EXCLUSIVE_DIRS+="/\\/$_exclusive\\// d\n"
done


printf "\n   the lines of all codes are: "
eval "find $_FIND_DIR -type f -and \( $_FIND_FILE_PATTERN \)" |
  sed -n "$(echo -e $_SED_EXCLUSIVE_DIRS)
          s/^/\"/
          s/$/\"/
          p" |
    xargs cat |
      sed -n '/^[ \t]*$/! p' |
        wc -l

echo

