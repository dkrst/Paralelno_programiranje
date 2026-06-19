# Makefile za poglavlje P04 - Procesi i niti
# Iste konvencije kao ostala poglavlja: CC, CFLAGS, LDFLAGS, TARGETS,
# implicitno pravilo .c.o te pseudo-pravila default, all i clean.

CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -pthread
LDFLAGS = -pthread

TARGETS = proces_vs_nit deadlock deadlock_ispravak master_radnik \
          mnozenje_matrica proizvodjac_potrosac pipeline

default: all
all: $(TARGETS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGETS): %: %.o
	$(CC) $(LDFLAGS) $< -o $@

clean:
	rm -f $(TARGETS) *.o

.PHONY: default all clean
