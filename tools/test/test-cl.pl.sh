#! /bin/bash


test_cl() {
  local _file=$1
  local _expected_num=$2
  local _title=$3

  local _num=$(cl.pl $_file | head -1 | awk '{ print $1 }')
  if [ $_num -ne $_expected_num ]; then
    echo "[FAILED] test $_file fail: $_title"
    _FAILED=$(expr $_FAILED + 1)
  fi
}


_FAILED=0

test_cl data-cl-1.c 6 "normal case"
test_cl data-cl-2.c 4 "multi comment inline case"

echo
echo "------- test report -------"
if [ $_FAILED -ne 0 ]; then
  echo "    $_FAILED case failed"
else
  echo "    all case test ok"
fi

