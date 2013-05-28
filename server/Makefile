PROGRAMS = server 

INCLUDES = -I . -I ../include
COMPILE = $(CC) $(DEFS) $(INCLUDES) $(CFLAGS) $(LIMYSQL)
CC = gcc
CFLAGS = -g -O3 -Wall -posix
LIBSOCKS =  -lpthread -lnsl -lm -lmysqlclient_r
LIBMYSQL = `mysql_config --cflags --libs`
OBJECTS = server.o xlcp_handlers.o xlcom.o queue.o avltree.o client_info.o db.o db_mysql.o

all: $(PROGRAMS)

.SUFFIXES: .c .o

server: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(PROGRAMS) $(OBJECTS) $(LIBSOCKS) $(LIBMYSQL)

.c.o:
	$(COMPILE) -c -o $@ $< $(LIBMYSQL)


clean: clean-PROGRAMS
	rm -f *.o


clean-PROGRAMS:
	@list='$(PROGRAMS)'; for p in $$list; do \
	  f=`echo $$p|sed 's/$$//'`; \
	  echo " rm -f $$p $$f"; \
	  rm -f $$p $$f ; \
	done

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
