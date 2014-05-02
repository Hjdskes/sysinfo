PROG       = sysinfo
CC         = gcc
PREFIX    ?= /usr/local
BINPREFIX  = ${PREFIX}/bin

LIBS       = -lX11 -lalpm -liniparser
CFLAGS    += -Os -Wall

debug: CFLAGS += -O0 -g -pedantic -Wextra
debug: ${PROG}

${PROG}: ${PROG}.c
	@${CC} ${CFLAGS} ${LIBS} ${PROG}.c -o ${PROG}

install:
	install -Dm755 ${PROG} ${DESTDIR}${BINPREFIX}/

uninstall:
	rm -f ${BINPREFIX}/${PROG}

clean:
	rm -f ${PROG}

.PHONY: debug clean install uninstall
