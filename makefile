SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:src/%.c=obj/%.o)
DEPS := $(wildcard src/*.h)

test: bin/test
bin/test: ${OBJS} obj/test.o
	@mkdir -p bin
	gcc -o $@ $+

# Installs to standard locations /usr/local/include and /usr/local/lib.
# To build and install manually, use \"make library\".
install:
ifneq ($(shell id -u), 0)
	@echo "You are not root. Use sudo or run target as root."
	@echo "Use \"make lib\" to build library and install manually." 
	@exit 1
endif
	@cp lib/fnv32_ht.h /usr/local/include
	@cp lib/libfnv32_ht.a /usr/local/lib

uninstall:
ifneq ($(shell id -u), 0)
	@echo "You are not root. Use sudo or run target as root."
	@exit 1
endif
	@rm /usr/local/include/fnv32_ht.h /usr/local/lib/libfnv32_ht.a

# Build library to lib/ folder.
build: lib/libfnv32_ht.a
lib/libfnv32_ht.a: ${OBJS}
	@mkdir -p lib
	ar rcs $@ $^
	@cp src/fnv32_ht.h lib/

obj/%.o: src/%.c ${DEPS}
	@mkdir -p obj
	gcc -c $< -o $@

obj/test.o: test/test.c
	gcc -c $< -o $@

help:
	@printf "make test\n"
	@printf "\t- builds test binay to bin/test\n"

	@printf "make build\n"
	@printf "\t- builds library to lib/libfnv32_ht.a\n"
	@printf "\t- moves copy of fnv32_ht.h to lib/fnv32_ht.h\n"

	@printf "make install\n\t!REQUIRES ROOT\n"
	@printf "\t- copies libfnv32_ht.a to /usr/local/lib\n"
	@printf "\t- copies fnv32_ht.h to /usr/local/include\n"

	@printf "make uninstall\n\t!REQUIRES ROOT\n"
	@printf "\t- removes /usr/local/lib/fnv32_ht.a\n"
	@printf "\t- removes /usr/local/include/fnv32_ht.h\n"

	@printf "make clean\n"
	@printf "\t- removes all build files\n"
	@printf "\t- does not uninstall\n"

	@printf "make help\n"
	@printf "\t- displays this help text\n"

.PHONY: clean
clean:
	@rm -rf bin/ obj/ lib/
