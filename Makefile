SRC = $(wildcard *.c)
HEADERS=$(wildcard *.h)
DEP=$(src) $(HEADERS)

ls8: $(DEPS)
        gcc -wall -wextra -g -o $@ $(SRC)

.PHONY: clean

clean:
        rm -rf ls8 *.dSYM
