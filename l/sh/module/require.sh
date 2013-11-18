#! /bin/bash
# Copyleft (C) KRT, by kiterunner_t


delcare -A _FUNCTOINS


function _load_file() {
  local _pathes=
  declare -a _pathes

  local _path=`echo $PATH | 
                 sed -e 's#/:#:#g' -e 's/:/" "/g' -e 's/^/"/' -e 's/$/"/'`
  eval "_pathes=(\"$(pwd)\" $path)"

  local _file=
  local _dir=
  local _src=
  local _is_found=
  for _file in "$@"; do
	_is_found=false
	_src=

	for _dir in "${_pathes[@]}"; do
	  test -r "$_dir/$_file.bash" && _src="$_dir/$_file.bash"
	  test -r "$_dir/$_file.sh" && _src="$_dir/$_file.sh"

	  if [ "$_src" != "" ]; then
		source $_src
		_is_found=true
		break
	  fi
	done

	if [ "$_is_found" = "false" ]; then
	  echo "not found file: $_file"
	  return 1
	fi
  done

  return 0
}


function _delfunc() {
  unset -f string.toupper
}


function ktrap() {
  declare -A _traps

  _traps["EXIT"]="echo hello; _delfunc;"
  trap "$_traps['EXIT']" EXIT
  trap "$_traps['KILL']" KILL
}


function require() {
  _load_file "$@"

  eval "$(declare -f toupper | sed 's/toupper ()/function string.toupper()/')"
  unset -f toupper

  ktrap "_delfunc" EXIT
  ktrap "echo hello" EXIT
}

