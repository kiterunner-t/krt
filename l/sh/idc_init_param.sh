#! /bin/bash
# Copyleft (C) KRT, 2013 by kiterunner_t


_IN_FILEJOB="in/filejob.txt"
_IN_FILELIST="in/filelist.txt"
_IN_JOBPARAM="in/filejobparam.txt"
_IN_FILE="in/t_file.txt"

_OUT_UPDATEOLD=fileold.sql
_OUT_FILECHECK=filecheck.out.sql
_OUT_FILEJOB=filejob.out.sql
_OUT_FILELIST=filelist.out.sql
_OUT_ALL="$_OUT_UPDATEOLD $_OUT_FILECHECK $_OUT_FILEJOB $_OUT_FILELIST"
_RESULT=result.sql

trap "rm -f $_OUT_ALL" EXIT

: > $_OUT_UPDATEOLD
: > $_OUT_FILECHECK
: > $_OUT_FILEJOB
: > $_OUT_FILELIST
: > $_RESULT


_BANK_ID=$(cat <<EOF
  000000
  822000
  823000
  825000
  826000
  827000
  828000
  829000
  831000
  606000
  608000
EOF
);

_PUBLIC_TABLES=$(cat <<EOF
  GAS_BIDW_ACCOUNT_KJ_DA
  GAS_BIDW_CURRENCY_D
  GAS_BIDW_DATE_D
  GAS_BIDW_DATE_MONTH_DA
  CCRD_CUSTR
  GAS_BIDW_ORG_LV6_DA
  GAS_BIDW_ORG_LV3_DA
  GAS_BIDW_LEDGER_D
EOF
);


is_public_tables() {
  local _table=$1

  local _t
  for _t in $_PUBLIC_TABLES; do
    test "$1" = "$_t" && return 0
  done

  return 1
}


get_quotes() {
  local _file=$1
  local _t=
  local _lists=

  while read _t; do
    if [ -z "$_lists" ]; then
	  _lists="'$_t'"
	  continue
    fi

    _lists="$_lists, '$_t'"
  done <$_file

  echo "$_lists"
}


file_is_exist() {
  local _f=
  for _f; do
    if [ ! -s $_f ]; then
      echo "[ERROR] $_f is not exist or is empty ..."
      return 1
    fi
  done
}


! file_is_exist $_IN_FILEJOB $_IN_FILELIST $_IN_JOBPARAM $_IN_FILE && 
  exit 1

echo "    produce update sql ..."
cat >>$_OUT_UPDATEOLD <<EOF
UPDATE IDC.AT_FILEJOB_PARAM SET USEFLAG='1' WHERE JOBNAME IN (\
`get_quotes $_IN_FILEJOB`);
EOF

cat >>$_OUT_UPDATEOLD <<EOF
UPDATE IDC.AT_SFILELIST SET LOADFLAG='0' WHERE SFILENAMEEN IN (\
`get_quotes $_IN_FILELIST`);
EOF

cat >>$_OUT_UPDATEOLD <<EOF
UPDATE IDC.AT_JOB_PARAM SET USEFLAG='1' WHERE JOBNAME IN (\
`get_quotes $_IN_JOBPARAM`);
EOF


echo "    produce IDC.AT_SYSPARAM ..."
while read _filename; do
  for _bank_id in $_BANK_ID; do
    test "$_bank_id" = "000000" && continue

    cat >> $_OUT_FILECHECK <<EOF
INSERT INTO IDC.AT_SYSPARAM(PARAMTYPE, PRARMTYPEDESC, VALUECHAR, VALUEINT, \
UPDATEUSERID, CHECKUSERID, UPDATETIME) \
VALUES('TAR_Z_CHK', '文件包检查', '${_filename}_${_bank_id}', 0, \
'9999999', '9999999', current timestamp);
EOF
  done
done <<EOF
GAS
CCRD
EOF


print_filejob() {
  local _group_id=$1
  local _bank_id=$2
  local _subno=$3
  local _filename=$4

  is_public_tables $_filename && test "$_bank_id" != "822000" && return 1
  test "$_bank_id" = "000000" && return 1

  cat >>$_OUT_FILEJOB <<EOF
INSERT INTO IDC.AT_FILEJOB_PARAM VALUES($_group_id, '$_bank_id', $_subno, '$_filename', \
'1', '/dbdata/filedir', '0');
EOF
}


print_filelist() {
  local _sysid=$1
  local _filename=$2
  local _bank_id=$3
  local _filename_cn=$4
  local _table=$5
  local _loadflag=$6

  is_public_tables $_filename && 
    test "$_bank_id" != "822000" -a "$_bank_id" != "000000" && 
      return 1

  local _id=$_sysid
  if [ "$_bank_id" = "000000" ]; then
    _id="EDW"
    test "$_filename" = "CCRD_ACCT" && _id="EDWAGG"
  fi

  cat >>$_OUT_FILELIST <<EOF
INSERT INTO IDC.AT_SFILELIST(SSYSTEMID, SFILENAMEEN, SBANKID, SFILENAMECN, \
NEWTAB, LOADFLAG, UPDATEUSERID, UPDATETIME) VALUES ('$_id', 'O_${_filename}', \
'$_bank_id', '$_filename_cn', '$_table', '$_loadflag', '9999999', current timestamp);
EOF
}


echo "    produce IDC.AT_FILEJOB_PARAM and IDC.AT_SFILELIST ..."
_GROUP_ID=731
while read _SYSID _FILENAME _TABLE _FILENAME_CN _LOADFLAG _N; do
  if [ "$_LOADFLAG" = "I" ]; then
	_LOADFLAG=1
  elif [ "$_LOADFLAG" = "IU" ]; then
    _LOADFLAG=2
  else
	echo "unkown loadflag: $_FILENAME, $_LOADFLAG"
    exit 1
  fi

  _SUBNO=1
  for _BID in $_BANK_ID; do
    print_filejob $_GROUP_ID $_BID $_SUBNO $_FILENAME
    test $? -eq 0 && _SUBNO=$(expr $_SUBNO + 1)

    print_filelist $_SYSID $_FILENAME $_BID $_FILENAME_CN $_TABLE $_LOADFLAG
  done

  _GROUP_ID=$(expr $_GROUP_ID + 1)
done <$_IN_FILE


echo "    merge all outputs to $_RESULT file ..."
cat $_OUT_ALL >>$_RESULT
echo "    ok."
echo

