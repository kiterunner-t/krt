#! /bin/sh

awk 'BEGIN {
  is_print=0
}

/^ Total execution time / {
  if ($5 > 0.5) {
    is_print=NR
    print $5
  }
}

/^ Statement text/ {
  if (NR==is_print+5) {
    print $0
  }
}
' syn.out
