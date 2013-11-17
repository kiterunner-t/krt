#! /bin/bash
# Copyleft (C) KRT, 2013 by kiterunner_t


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


for _bin in $_ALL_BINS; do
  echo "test $_bin"
  echo "---------------------------------"

  for _shared_max in 50000 100000 2000000 4000000 8000000; do
    if [ "$_bin" = "t_lock_peterson" ]; then
      echo "(shared_max: $_shared_max)"
      time bin/$_bin 2 $_shared_max
      echo
      echo
      continue
    fi

    for _thread_num in 2 4 8 16 32; do
      echo "(thread_num: $_thread_num, shared_max: $_shared_max)"
      time bin/$_bin 4 $_shared_max
      echo
    done

    echo
    echo
  done
done

