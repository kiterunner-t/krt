#n

# produce the declaration of the static function in .c

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

