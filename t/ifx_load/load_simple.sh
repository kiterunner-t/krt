#! /bin/sh
# Copyleft (C) KRT, 2011 by kiterunner_t
 

if [ $# -lt 2 ]; then
  echo "Usage: `basename $0` tablename files"
  exit 1
fi
 
_DB="mydb"
_TABLE=$1
shift

db_load() {
  local _db=$1
  local _db_table=$2
  local _file=$3
 
  dbaccess $_db <<EOF
load from $_file delimiter '!' insert into $_db_table;
EOF
}
 
for _file in $@; do
  if [ ! -e $_file ]; then
    echo "$_file is not exsit..."
    continue
  fi
 
  filter_5c7c $_file
  _tmpfile=${_file}.tmp
 
  db_load $_DB $_TABLE $_tmpfile
  rm -f $_tmpfile
done

