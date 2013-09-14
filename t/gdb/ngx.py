#! /usr/bin/env python
# Copyleft (C) KRT, 2013 by kiterunner_t

import gdb


class NgxArray(gdb.Command):
  def __init__(self):
    super(NgxArray, self).__init__("ngx_array_print", gdb.COMMAND_NONE)


  def invoke(self, args, from_tty):
    argv = gdb.string_to_argv(args)
    if len(argv) != 2:
      raise gdb.GdbError("ngx_array_print <array> <array-ele-type>")

    val = gdb.parse_and_eval(argv[0])
    typ = gdb.lookup_type(argv[1])

    if val.type == gdb.lookup_type("ngx_array_t").pointer():
      val = val.dereference()
    elif val.type != gdb.lookup_type("ngx_array_t"):
      raise gdb.GdbError("error")

    size = val["size"]
    p = val["elts"].cast(typ.pointer())
    for i in range(size):
      p = p + 1
      print p.dereference()


NgxArray()

