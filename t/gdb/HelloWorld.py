# Copyleft (C) KRT, 2013 by kiterunner_t

import gdb


class HelloWorld(gdb.Command):
  def __init__(self):
    super(HelloWorld, self).__init__("hello-world", gdb.COMMAND_NONE)


  def invoke(self, args, from_tty):
    argv = gdb.string_to_argv(args)
    if len(argv) != 0:
      raise gdb.GdbError("hello-world takes no arguments")

    print "Hello, World!"


HelloWorld()

