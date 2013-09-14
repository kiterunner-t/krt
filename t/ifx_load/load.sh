#! /bin/sh
# Copyleft (C) KRT, 2011 by kiterunner_t
#
# For INFORMIX-SQL Version 11.50.FC4
#     AIX version 6.1

 
usage() {
  echo "Usage: $_SCRIPT_NAME [-d <database>] -t <table> <files>"
  exit
}


del() {
  local _file=
  for _file in $@; do
    test -e $_file && rm -f $_file
  done
}


check_fieldnum() {
  local _file=$1

  # expr the field number of the file
  local _num=`expr $_FIELD_NUM`
  local _last_char=`sed -n '1p' "$_file" | 
                     sed -e 's/\r//g' -e 's/ +$//' |
                       awk '{ print substr($NF, length($NF), 1) }'`
  test "$_last_char" = $DELIMITER_CHAR && _num=`expr $_FIELD_NUM + 1`

  # check the field number of the file by the three lines at the head
  local _field_num1=`sed -n '1p' $_file | awk -F"$DELIMITER_CHAR" '{ print NF }'`
  local _field_num2=`sed -n '2p' $_file | awk -F"$DELIMITER_CHAR" '{ print NF }'`
  local _field_num3=`sed -n '3p' $_file | awk -F"$DELIMITER_CHAR" '{ print NF }'`
  local _cnt=0
  test "$_field_num1" = "$_num" && _cnt=$((_cnt + 1))
  test "$_field_num2" = "$_num" && _cnt=$((_cnt + 1))
  test "$_field_num3" = "$_num" && _cnt=$((_cnt + 1))
 
  if [ $_cnt -lt 2 ]; then
    echo "BAD FILE: The field numbers of the file [$_file] is not matched "
    echo "the field numbers of the table"
    exit 0
  fi
}


db_select_ncols() {
  local $_db=$1
  local $_table=$2
  
  dbaccess $_db <<EOF 1>/dev/null 2>&1
select ncols from systables where tabname='$_table';
EOF
}

 
load_file() {
  local _file=$1
  local _is_check=`expr $2`
  local _cmd_file="$_file.cmd"
 
  test $_is_check -eq 1 && check_fieldnum $_file
 
  local _line_nums=`cat $_file | wc -l`
  _FILES_NUMS=`expr $_FILES_NUMS + $_line_nums`

  echo "Lines of file [$_file]: $_line_nums ($_FILES_NUMS for all files)"
  
  _line_nums=`expr $_line_nums`
  if [ $line_nums -eq 0 ]; then
    echo "BAD FILE: the file [$_file] is empty."
    return
  fi
 
  cat <<EOF > $_cmd_file
file "$_file" delimiter "$_DELIMITER_CHAR" $_FIELD_NUM;
insert into $_TABLE;
EOF
 
  if [ ! -e "$_cmd_file" ]; then
    echo "BAD DBLOAD: the command file is not exist."
    exit 1
  fi
 
  echo -e "\nStart loading data, please wait a moment ..."
  dbload -d $DB_NAME -c $cmd_file -l $ERR_FILE -e $ERR_NUMS \
         -n $COMMIT_NUMS >> $TMP_FILE 2>&1

  echo -e "Finish loading table [$_TABLE] from file [$_file].\n"
  rm -f $cmd_file
}


_SCRIPT_NAME=`basename $0`

_DB="mydb"
_TABLE=
_DELIMITER_CHAR="|"
_COMMIT_NUMS=100000         # 一次提交的数据量
_ERR_NUMS=10000             # 容许错误的数据量
_FILES_NUMS=0               # 装入数据库的总条目数
_FIELD_NUM=0                # 文件中每行由分隔符分隔的字段数
_TMP_FILE="${_file}$$.tmp"
_ERR_FILE="${_file}$$.err"

while getopts d:t: _opt; do
  case $_opt in
    d) _DB=$OPTARG
      ;;
 
    t) _TABLE=$OPTARG
      ;;
  esac
done

shift $((OPTIND - 1))
 
test -z "$_TABLE" && echo -e "Unkown table name\n" && usage
test $# -lt 1 && echo -e "no data files\n" && usage

trap 'del $_TMP_FILE $_ERR_FILE' EXIT QUIT KILL TERM


_FIELD_NUM=`select_cols | awk '{ if ($1 ~/[0-9]+/) print $1 }'`
test $_FIELD_NUM -eq 0 && 
  echo "get the field numbers of table from database error" && 
    exit 1

echo "Loading files ($@) into $_TABLE of the database ($_DB), "
echo "there is $_FIELD_NUM fields in the table called $_TABLE."

for _file in $@; do
  load_file $_file 1

  if [ -s $_ERR_FILE ]; then
    echo "handle load error file: $_ERR_FILE"
    _handle_errfile="$$.hef"
 
    cat $_ERR_FILE |
      awk '{ 
            if ($0 ~/Row number [0-9]* is bad\./) 
              flag=1;
            if (flag == 2) 
              line=$0;
            if (flag==5 && $10~/not/ && $11~/match/) 
              print line;

            flag++ 
          }' |
        sed 's/ ... $//g' >> $_handle_errfile
 
    if [ -s $_handle_errfile ]; then
      filter_5c7c $_handle_errfile
      _DELIMITER_CHAR='!'
 
      load_file ${_handle_errfile}.tmp 0
      rm -f ${_handle_errfile}.tmp
    fi
 
    rm -f $_handle_errfile
  fi
done

_INSERT_COUNT=`grep "Table $_TABLE had " $_TMP_FILE | \
                 awk 'BEGIN{ NUM=0 } { NUM += int($4) } END { print NUM }'`

echo "\n\tInsert $_TABLE_NAME items: $_INSERT_COUNT"
_NUM=`expr $_FILES_ITEM_NUM - $_INSERT_COUNT`
test $_NUM -ne 0 && 
  echo "There are $_NUM items left, please check the file: $_ERR_FILE"

