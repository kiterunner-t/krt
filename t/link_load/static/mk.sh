#! /bin/bash
# Copyleft (C) KRT, 2013 by kiterunner_t

if [ $# -lt 1 ]; then
  echo -e "usage: $0 x86_32|x86_64\n"
  exit 1
fi

_CPU=$1
_FILE="print.c exit.c foo.c swap.c"
_TARGET=`echo $_FILE | sed 's/\.c/.o/g'`

_GCC_OPT=
_LIB="libc.a"
_ENTRY_ADDR=
_LD_LDS="foo.lds"

if [ "$_CPU" = "x86_32" ]; then
  _ENTRY_ADDR="0x08048000"
  _GCC_OPT="-Dx86_32"
  _LD_OPT="-T "
elif [ "$_CPU" = "x86_64" ]; then
  _ENTRY_ADDR="0x40000000"
  _GCC_OPT="-Dx86_64"
else
  echo -e "error ...\n"
  exit 1
fi

cat >$_LD_LDS <<EOF
ENTRY(foo)
SEARCH_DIR(.)
INPUT(foo.o swap.o $_LIB)

SECTIONS {
  . = $_ENTRY_ADDR + SIZEOF_HEADERS;

  mytext : { *(.text) *(.data) *(.rodata) }

  /DISCARD/ : { *(.comment) }
}
EOF

gcc -c -fno-builtin $_GCC_OPT $_FILE
ar -rcs $_LIB print.o exit.o
ld -static -T $_LD_LDS -o foo 
rm -f $_TARGET $_LD_LDS $_LIB

