PROGRAMS = stress_connect

INCLUDES = -I . -I ../include
COMPILE = $(CC) $(DEFS) $(INCLUDES) $(CFLAGS)
CC = gcc
CFLAGS = -g -O3 -Wall -ansi -std=c99
LIBXLCOM = ../client_api/xlcom_lib.a
OBJECTS = $(addsuffix .o, $(basename $(PROGRAMS)))


all: libs $(PROGRAMS)

.SUFFIXES: .c .o

# PART XLCOM CLI CLIENT

$(PROGRAMS): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(PROGRAMS) $(addsuffix .o, $@) $(INCLUDES) $(LIBXLCOM)

.c.o:
	$(COMPILE) -c $(addsuffix .c, $(basename $@))


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
