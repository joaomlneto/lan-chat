PROGRAMS = client

INCLUDES = -I . -I ../include
COMPILE = $(CC) $(DEFS) $(INCLUDES) $(CFLAGS)
CC = gcc
#CFLAGS 32-bit = -g -O3 -Wall -m32 -ansi -std=c99
CFLAGS = -g -O3 -Wall -ansi -std=c99
LIBSOCKS = -lpthread -lnsl
LIBXLCOM = ../client_api/xlcom_lib.a
OBJECTS = client.o


all: libs $(PROGRAMS)

.SUFFIXES: .c .o

# PART XLCOM CLI CLIENT

client: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(PROGRAMS) $(OBJECTS) $(LIBSOCKS) $(LIBXLCOM)

.c.o:
	$(COMPILE) -c -o $@ $<


clean: clean-PROGRAMS
	rm -f *.o


clean-PROGRAMS:
	@list='$(PROGRAMS)'; for p in $$list; do \
	  f=`echo $$p|sed 's/$$//'`; \
	  echo " rm -f $$p $$f"; \
	  rm -f $$p $$f ; \
	done


# PART LINKED LIBRARIES

libs:
	$(MAKE) xlcom_lib.a -C ../client_api


# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
