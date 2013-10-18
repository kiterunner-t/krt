#n

# produce the declaration of the static function in .c
# cause I'm not be skilled in emacs lisp for now

/^static .*[^;]$/ {
:AGAIN
  N
  /{$/!b AGAIN

  s/{$//
  s/\n/ /
  s/\* \(.*\)(/*\1(/
  s/\n*$/;/
  p
}

