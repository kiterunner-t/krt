#! /bin/bash
# This script is based on http://poormansprofiler.org/


print_stack() {
  local _sym=
  local _pid=

  test -n $_SYMFILE && _sym="symbol-file $_SYMFILE"
  for _pid in $@; do
    echo
    echo $_pid
    echo -------------
    gdb -ex "$_sym" -ex "set pagination 0" -ex "thread apply all bt" -batch -p $_pid | \
      awk 'BEGIN { s = ""; } 
            /Thread/ { print s; s = ""; } 
            /^\#/ { if ($3 != "in") { $4 = $2 } }
            /^\#/ { if (s != "" ) { s = s "," $4} else { s = $4 } } 
            END { print s }' | \
        sort | uniq -c | \
          perl -e 'while (<>) { print if ! /^\s*\d+\s*$/; }' | \
            sort -r -n -k 1,1
  done
}


nsamples=1
sleeptime=0
pid=$(pidof $redis-server)
_SYMFILE=$2

for x in $(seq 1 $nsamples); do
  print_stack $pid
  sleep $sleeptime
done 

