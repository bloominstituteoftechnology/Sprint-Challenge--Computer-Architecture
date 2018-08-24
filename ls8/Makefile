SRC=$(wildcard *.c)
HEADERS=$(wildcard *.h)
DEPS=$(SRC) $(HEADERS)

ls8: $(DEPS)
	gcc -Wall -Wextra -g -o $@ $(SRC)

.PHONY: clean

clean:
	rm -rf ls8 *.dSYM
