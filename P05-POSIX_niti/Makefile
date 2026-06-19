# Makefile za poglavlje P05 - POSIX niti
# Konvencije su iste kao u ostalim poglavljima: CC, CFLAGS, LDFLAGS, TARGETS,
# implicitno pravilo .c.o te pseudo-pravila default, all i clean.
# Za rad s nitima nuzna je zastavica -pthread (i pri prevodjenju i pri linkanju).

CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -pthread
LDFLAGS = -pthread

TARGETS = pozdrav_nit dijeljena_memorija argument_int argument_struktura \
          greska_petlja ispravak_polje rezultat_malloc utrka mutex suma_niti

default: all
all: $(TARGETS)

# Prevodjenje: .c -> .o
.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

# Linkanje: svaki program nastaje iz istoimene objektne datoteke
$(TARGETS): %: %.o
	$(CC) $(LDFLAGS) $< -o $@

clean:
	rm -f $(TARGETS) *.o

.PHONY: default all clean
