PROG     =  sysinfo
PREFIX  ?= /usr/local

LIBS     =  -lalpm `pkg-config --cflags --libs glib-2.0`
CFLAGS   =  -g -Os -Wall -Wextra

${PROG}: ${PROG}.c
	@${CC} ${CFLAGS} ${LIBS} -o ${PROG} ${PROG}.c
	@strip ${PROG}

clean:
	rm -f ${PROG}

install:
	install -Dm755 ${PROG} ${DESTDIR}${PREFIX}/bin/${PROG}

uninstall:
	rm -f ${PREFIX}/bin/${PROG}
