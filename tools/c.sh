#! /bin/bash
# Copyleft (C) KRT, 2013-2014 by kiterunner_t

if [ $# -ne 1 ]; then
  echo "Usage: $(basename $0) <c-file>"
  exit 1
fi

_FILE=$1

# produce the declaration of the static function in .c
# cause I'm not skilled in emacs lisp for now

# only support the style of declaration such as
#   static void test()
#   {
#   }
# or
#   static void
#   test(int a,
#        int b)
#   {
#   }

# not support, I don't use the style
#   static void test() {
#   }
sed -n '
  /^static .*[^;]\s*$/ {
  :AGAIN
    N
    /{$/!b AGAIN

    s/{$//
    s/\n/ /
    s/\* \(.*\)(/*\1(/
    s/\n*\s*$/;/
    p
  }
' $_FILE

