#! /bin/bash
# Copyleft (C) KRT, 2011 2012 by kiterunner_t


usage() {
  cat <<EOF
Usage: 
    $_FILE_NAME [OPTION] all debug|release [clean]
    $_FILE_NAME [OPTION] 125 debug|release [clean]
    $_FILE_NAME [OPTION]  95 debug|release [clean]
    $_FILE_NAME [OPTION]  58 debug|release [clean]
    $_FILE_NAME [OPTION] simware_32|simware_64 mpu|cen|lpu debug|release [clean]

OPTIONS: "
    -d          ## display compile commands, but not execute, just for debug
    -h             display help information
    -l num         display error messages in 'num' lines accoding to error file
    -k             compile kernel codes
    -m [clean]     my compile options (equal to: -u simware_32 mpu debug [clean])
    -p             product a compile report package (*.tar.gz)
    -u             compile user codes
EOF

  exit 0
}


del() {
  local _file
  for _file in $@; do
    test -e $_file && rm -rf $_file
  done
}


create_dir() {
  local _dir
  for _dir in $@; do
    test ! -e $_dir && mkdir $_dir
  done
}


decho() {
  test "$_DEBUG" = "debug" && 
    echo "$@"
}


mk_exe() {
  local _log=$1
  local _log_err=$2

  local _line=
  while read _line; do
    _mk_exe $_log $_log_err $_line
  done
  
  echo -n "\n"
}


# global flags: 
#   _KERN         0 all, 1 user, 2 kernel
#   _PACK         0 nopack, 1 pack
#   _DEBUG
#
# kern_flag pack_flag command
# 0         0         cd ../make/
# 
# ignore empty lines and comments
_mk_exe() {
  local _log=$1
  local _log_err=$2
  local _level=$3
  local _pack=$4
  shift 4
  local _cmd="$@"
 
  test "$_log" = "" -o "$_log_err" = "" && echo "INTERNAL ERROR" && exit 1
  test "$_level" = "" -o "$_level" = "#" && return

  if [ "$_level" != "0" ]; then
    test "$_KERN" = "kernel" -a "$_level" != "2" && return
    test "$_KERN" = "user" -a "$_level" != "1" && return
  fi
  
  if [ "$_pack" != "0" ]; then
    test "$_PACK" = "pack" -a "$_pack" != "1" && return
  fi

  local _log_redirect="1>>$_log 2>>$_log_err"
  test "$_PACK" = "pack" && _log_redirect="1>>$_log 2>&1"
  
  local _cmdlog="$_cmd $_log_redirect"
  decho $_cmdlog && return
 
  echo "$_cmd, please wait moment ..."
  eval "$_cmdlog"
 
  if [ ! $? -eq 0 ]; then
    echo -e "---COMPILE ERROR!"
    echo -e "please refer $_log_err for details\n\n\n"
    tail -$_LOG_ERR_LINES $_log_err
    exit 1
  fi
}


mk_simware() {
  local _support=$1
  local _version=$2
  local _dev=$3
  local _product=$4
  
  local _log="$_COMPILE_DIR/_${_product}_${_dev}_${_version}.txt"
  local _log_err="`echo $_log | sed 's/\.txt$//'`_err.txt"
  del_files $_log $_log_err
  
  local _arch="x86_32"
  test "$_product" = "simware_64" && _arch="x86_64"

  mk_exe $_log $_log_err <<EOF
0 0 cd $_PWD/make/
1 1 ./make_user_onearch.sh $_product $_arch $_dev $_version

2 0 ./make_kernel.sh $_support $_product $_dev $_version
0 0 cd ../target/kernel_lib/
2 0 ./make_plat.sh $_version $_product $_dev
0 0 cd ../../product/$_product/$_dev/prj/
2 2 ./build_product.sh
0 0 cd ../../../../make/
2 2 ./make_ko.sh $_product $_dev $_version

0 0 cd ../product/$_product/$_dev/prj/
0 2 ./mkelf.sh

0 2 cp ./${_version}/simware_*.img $
EOF
}


isskip() {
  local _line="$@"
  
  test "$_line" = "" && echo "yes" && return
  test "$_CLEAN" = "clean" && echo "$_line" | grep -q "^clean" &&
    echo "yes" && return
  test "$_EXCLUDE" = "" && echo "no" && return
  
  local _pattern=
  local _tmp=
  local _exclude=
  local _exclude_dev=
  local _pattern=
  for _tmp in $_EXCLUDE; do
    decho $_tmp
    _exclude=`echo $_tmp | sed 's/,.*$//'`
    _exclude_dev=`echo $_tmp | awk -F, '{ print $2 }'`
    
    _pattern="^(mk|clean)_${_exclude}.*"
    if echo $_exclude | grep -q "simware_(32|64)"; then
      _pattern="^(mk|clean)_simware8*"
      test -n "$_exclude_dev" && _pattern="($_exclude_dev) $_exclude\$"
    fi
    
    echo "$_line" | grep -Pq "$_pattern" && echo "yes" && return
  done
  
  echo "no"
}


_mk_all() {
  local _support=$1
  local _version=$2
  
  local _line=
  while read _line; do
    # isskip
  done <<EOF
echo "CLEANING ..."
clean_simware $_support $_version mpu simware_32
clean_simware $_support $_version cen simware_32
clean_simware $_support $_version lpu simware_32
clean_simware $_support $_version mpu simware_64
clean_simware $_support $_version cen simware_64
clean_simware $_support $_version lpu simware_64
clean_125 $_support $_version
clean_95 $_support $_version
clean_58 $_support $_version

echo "COMPILING ..."
mk_simware $_support $_version mpu simware_32
mk_simware $_support $_version cen simware_32
mk_simware $_support $_version lpu simware_32
mk_simware $_support $_version mpu simware_64
mk_simware $_support $_version cen simware_64
mk_simware $_support $_version lpu simware_64
mk_125 $_support $_version
mk_95 $_support $_version
mk_58 $_support $_version
EOF
}


mk_all() {
  local _support=$1
  local _version=$2
  
  _mk_all $_support $_version
  test "$_DEBUG" = "debug" && return
  
  local _compile_pack="$_COMPILE_DIR/compile_report.tar.gz"
  del $_compile_pack
  (cd $_COMPILE_DIR &&
     tar -czf $_compile_pack \
       `ls *.txt | awk '{ if ($0 !~/.*(_err|_clean)\.txt/) print $0 }'`)
}


_FILE_NAME=`basename $0`

_SUPPORT="support"
_PRODUCT=
_DEV=
_VERSION=
_CLEAN=

# _mk_exe flags
_FUNC=
_KERN="all"
_PACK=
_DEBUG=
_LOG_ERR_LINES=20

_PWD=`pwd`
_LOCK_DIR="$_PWD/.lock"
_LOCK_MKONE=".lock_mkone"
_COMPILE_DIR="$_PWD/compile_report"

create_dir $_COMPILE_DIR $_LOCK_DIR
if [ -e $_LOCK_DIR/$_LOCK_MKONE ]; then
  echo "SORRY, some guy is compiling, please wait a moment and try again."
  exit 0
fi

touch $_LOCK_DIR/$_LOCK_MKONE
trap 'del $_LOCK_DIR/$_LOCK_MKONE' EXIT QUIT

while getopts dhl:kmpu _opt; do
  case $_opt in
    d) _DEBUG="debug"
      ;;

    h) usage
      ;;

    l) _LOG_ERR_LINES=`expr $OPTARG`
      ;;
    
    k) _KERN="kernel"
      ;;

    m) _PRODUCT="default"
      ;;

    p) _PACK="pack"
      ;;

    u) _KERN="user"
      ;;

    *) usage
      ;;
  esac
done
shift $((OPTIND - 1))

test "$_PRODUCT" = "" && _PRODUCT=$1
case $_PRODUCT in
  all)
    _PRODUCT=$1
    _VERSION=$2
    test "$3" = "clean" && _CLEAN=$3
    test "$_VERSION" != "debug" -o "$_VERSION" != "release" && usage
    ;;

  simware_32|simware_64)
    test $# -lt 3 -o $# -gt 4 && usage
    ! echo $2 | grep -q "[mpu|cen|lpu]" && usage
    ! echo $3 | grep -q "[debug|release]" && usage
    test $# -eq 4 -a "$4" != "clean" && usage
    _PRODUCT=$1
    _DEV=$2
    _VERSION=$3
    test $# -eq 4 && _CLEAN="clean"
    ;;

  125|95|58)
    test $# -lt 2 -o $# -gt 3 && usage
    ! echo $2 | grep -q "[debug|release]" && usage
    test $# -eq 3 -a "$3" != "clean" && usage
    _PRODUCT=$1
    _VERSION=$2
    test $# -eq 3 && _CLEAN="clean"
    ;;

  default)
    test $# -gt 1 && usage
    test $# -eq 1 -a "$1" != "clean" && usage
    _SUPPORT="support"
    _DEV="mpu"
    _PRODUCT="simware_32"
    _VERSION="debug"
    _KERN="user"
    _CLEAN=$1
    ;;

  *) usage
    ;;
esac

if [ "$_CLEAN" = "clean" -a "$_PRODUCT" != "all" ]; then
  _FUNC="clean_${_PRODUCT}"
  echo $_FUNC | grep -q simware && _FUNC="clean_simware"
else
  _FUNC="mk_${_PRODUCT}"
  echo $_FUNC | grep -q simware && _FUNC="mk_simware"
fi

test "$_PRODUCT" = "" -o "$_VERSION" = "" &&
  echo "INTERNAL ERROR: \$_SUPPORT or \$_VERSION is empty string ($_SUPPORT/$_VERSION)" &&
    exit 1

echo $_FUNC | grep -q "_simware$" &&
  test "$_DEV" = "" -o "$_PRODUCT" = "" &&
    echo "INTERNAL ERROR: \$_DEV or \$_PRODUCT is empty string ($_DEV/$_VERSION)" &&
      exit 1

eval "$_FUNC $_SUPPORT $_VERSION $_DEV $_PRODUCT $_CLEAN"

