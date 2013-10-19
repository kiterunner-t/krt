#n
# Copyleft (C) KRT, 2013 by kiterunner_t
# sed -f c.sed *.c

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

