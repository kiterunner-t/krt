CC       = gcc
CC_FLAGS = -g


all: dyn_foo.so dyn_foo_extern.so dyn


dyn: dyn.c
	$(CC) $(CC_FLAGS) -o $@ ./dyn_foo.so ./dyn_foo_extern.so $^


dyn_foo.so: dyn_foo.c
	$(CC) $(CC_FLAGS) -fPIC -shared -o $@ $^


dyn_foo_extern.so: dyn_foo_extern.c
	$(CC) $(CC_FLAGS) -fPIC -shared -o $@ $^


clean:
	-rm -f dyn dyn_foo.so dyn_foo_extern.so

