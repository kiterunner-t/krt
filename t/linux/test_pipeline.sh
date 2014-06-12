#! /bin/bash


_NUM=$1
i=1

while [ $i -le $_NUM ]; do
  echo "the number of theads is $i"
  bin/test_pipeline $i $2 1
  i=$((i + 1))
done

