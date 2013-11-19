#! /bin/bash
# Copyleft (C) KRT, 2013 by kiterunner_t

if [ $# -eq 2 ]; then
  test ! -x bin/$1 &&
    echo "bin/$1 is not existing or executing" &&
      exit 2

  _ALL_BINS=$1
  _SHARED_MAX=$2

elif [ $# -eq 1 ]; then
  _SHARED_MAX=$1
  _ALL_BINS=$(sed -n -e '
    /all:/ {
    :AGAIN
      /\\\s*$/ {
      N
      b AGAIN
    }

    s/^all://
    s/\\//g
    p
  }' makefile)

else
  echo "Usage: $(basename $0) [program] <shared_max>"
  exit 2
fi


for _bin in $_ALL_BINS; do
  echo "test $_bin"
  echo "---------------------------------"

  for _shared_max in $_SHARED_MAX; do
    if [ "$_bin" = "lock_peterson" ]; then
      echo "(shared_max: $_shared_max)"
      time bin/$_bin 2 $_shared_max
      echo
      echo
      continue
    fi

    for _thread_num in 2 4 8 16 32; do
      echo "(thread_num: $_thread_num, shared_max: $_shared_max)"
      time bin/$_bin $_thread_num $_shared_max
      echo
    done
  done

  echo
  echo
done

