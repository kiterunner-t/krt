#! /bin/sh
# Copyleft (C) KRT, 2013  by kiterunner_t


echo_log() {
  echo "[`date`] $@"
}


rm_large_files() {
  test ! $# -eq 2 && return
  local _path=$1
  local _pattern=$2

  test ! -e $_path && 
    echo_log "rm large files path($_path) isn't exist" &&
      return

  echo_log "rm large files: $_path, $_pattern"
  local _saved_path=`pwd`

  cd $_path
  if [ $? -eq 0 ]; then
    ls . | sed -n "/$_pattern/ p" | xargs rm -f
    cd $_saved_path
  fi
}


rm_files() {
  local _file=
  for _file in "$@"; do
    test -f $_file && rm -f $_file
  done
}


zip_files() {
  local _file=
  for _file in "$@"; do
    ls . | sed -n "/$_file/ p" | xargs gzip -q
  done
}


bak_files() {
  test $# -lt 3 && return
  local _log_path=$(echo $1 | sed 's#\(.\)/$#\1#')
  local _suffix=$2
  shift 2

  test ! -e $_log_path && echo_log "log_path($_log_path) isn't exist" &&
    return

  local _file=
  for _file in $@; do
    test ! -f $_file && continue

    local _file_bak="$_log_path/${_file}.${_suffix}"
    cp $_file $_file_bak
    echo " " > $_file
    zip_files $_file_bak
  done
}


get_lastmonth() {
  local _today=$1
  local _today_month=$(echo $_today | cut -b 5-6)
  local _year=$(echo $_today | cut -b 1-4)
  local _t=$(expr $_today_month - 1)

  if [ $_t -eq 0 ]; then
    _year=$(expr $_year - 1)
    _t=12
  fi

  printf "%04d%02d" $_year $_t
}


get_yesterday() {
  perl -MPOSIX -e 'print strftime "%Y%m%d", localtime time - 86400'
}


get_yesterday_before() {
  perl -MPOSIX -e 'print strftime "%Y-%m-%d", localtime time - 2 * 86400'
}


_DEBUG="false"

test "x$_DEBUG" = "xtrue" && set -x


_TODAY=$(date +%Y%m%d)
_YESTERDAY=$(get_yesterday)
_YESTERDAY_BEFORE=$(get_yesterday_before)

_FILES_TMP=
_FILES_NOHUPS=
_FILES_NEEDEDZIPS=

_HOSTNAME=$(hostname)

case $_HOSTNAME in
  UAT|153)
    test ! $(echo $_TODAY | cut -b 7-8) -eq 1 && exit 0

    _NOHUP_OLD="nohup.out.$(get_lastmonth $_TODAY)01"

    _FILES_TMP=$(cat <<-EOF
      /weblogic/file/recp     recp.*_.*\.xml
      /weblogic/file/pdf      pdf.*_.*\.pdf
      /weblogic/file/image    image.*_.*\.png
      /weblogic/file/ercode   RP.*\.png
      /weblogic/brap_monitor  $_NOHUP_OLD
      /weblogic/brms_batch    $_NOHUP_OLD
      /weblogic/Oracle/Middleware/user_projects/domains/brms_domain/bin  $_NOHUP_OLD
EOF
)

    _FILES_NOHUPS=$(cat <<-EOF
      /weblogic/Oracle/Middleware/user_projects/domains/brms_domain/bin  /weblogic/logs/brms  nohup.out
      /weblogic/brap_monitor  /weblogic/logs/monitor  nohup.out
      /weblogic/brms_batch    /weblogic/logs/batch    nohup.out
EOF
)
    ;;

  DB)
    test ! $(echo $_TODAY | cut -b 7-8) -eq 1 && exit 0

    _NOHUP_OLD="nohup.out.$(get_lastmonth $_TODAY)01"

    _FILES_TMP=$(cat <<-EOF
      /brmsapp/logs/batch   $_NOHUP_OLD
      /brmsapp/logs/unload  $_NOHUP_OLD
EOF
)

    _FILES_NOHUPS=$(cat <<-EOF
      /brmsapp/brms_batch   /brmsapp/logs/batch   nohup.out
      /brmsapp/brms_unload  /brmsapp/logs/unload  nohup.out
EOF
)
    ;;

  A|B)
    _MID="A"
    test "x$_HOSTNAME" = "xB" && _MID="B"

    _FILES_TMP=$(cat <<-EOF
      /weblogic/file/recp    recp.*_.*${_YESTERDAY}.*\.xml
      /weblogic/file/ercode  RP.*\.png
      /brms/ftpuser/recp     recp.*_${_MID}${_YESTERDAY}.*\.xml
      /brms/ftpuser/pdf      pdf.*_${_MID}${_YESTERDAY}.*\.pdf
      /brms/ftpuser/image    image.*_${_MID}${_YESTERDAY}.*\.png
EOF
)

    #  _WEBLOGIC_PID=$(ps -u weblogic -o pid,ppid,args |
    #                    grep -e "-Dweblogic.Name=AdminServer.*weblogic.Server$" |
    #                      awk '{ print $2 }')
    #  _WEBLOGIC_LOGFILE="weblogic.${_WEBLOGIC_PID}.log"
    #  _WEBLOGIC_GCFILE="gc.${_WEBLOGIC_PID}.log"

    # /weblogic/brms/brms_domain/bin  /weblogic/brms/brms_domain/log  $_WEBLOGIC_LOGFILE
    # /weblogic/brms/brms_domain/bin  /weblogic/brms/brms_domain/log  $_WEBLOGIC_GCFILE

    _FILES_NOHUPS=$(cat <<-EOF
      /weblogic/brms/brms_domain/bin  /weblogic/brms/brms_domain/log  nohup.out
      /weblogic/brap_monitor          /weblogic/logs/monitor          nohup.out
EOF
)

    _FILES_NEEDEDZIPS=$(cat <<-EOF
      /weblogic/logs  common-\(all\|dao\|warn\|error\).log.${_YESTERDAY_BEFORE}
EOF
)
    ;;


  krt)
    _FILES_TMP=$(cat <<-EOF
      /home/krt/work/l/sh/test  .*\.txt
EOF
)

    _FILES_NOHUPS=$(cat <<-EOF
      /home/krt/work/l/sh/test  /home/krt/work/l/sh/bak  nohup.out
EOF
)

    _FILES_NEEDEDZIPS=$(cat <<-EOF
      /home/krt/work/l/sh/test  common-\(all\|dao\).log$
EOF
)
    ;;


  *)
    echo_log "unkown hostname: $_HOSTNAME"
	exit -1
    ;;
esac


echo_log "start clean weblogic logs and tmp files ..."

# clean tmp files
while read -r _path _pattern; do
  test -z "$_path" -o -z "$_pattern" && continue

  echo_log "clean $_path \"$_pattern\""
  rm_large_files $_path $_pattern
done <<-EOF
  $_FILES_TMP
EOF


# clean weblogic log
while read -r _path _bak_path _file; do
  test -z "$_path" -o -z "$_bak_path" -o -z "$_file" && continue

  echo_log "clean and bak file $_file from $_path to $_bak_path"
  cd $_path && test $? -eq 0 && 
    bak_files $_bak_path $_TODAY $_file
done <<-EOF
  $_FILES_NOHUPS
EOF


# zip files
while read -r _path _files; do
  test -z "$_path" -o -z "$_files" && continue

  echo_log "zip the files: $_files in $_path"
  cd $_path && test $? -eq 0 && 
    zip_files $_files
done <<-EOF
  $_FILES_NEEDEDZIPS
EOF

