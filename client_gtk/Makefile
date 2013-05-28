PROGRAMS = client

INCLUDES = -I . -I ../include
COMPILE = $(CC) $(DEFS) $(INCLUDES) $(CFLAGS) $(LIBGTK2)
CC = gcc
CFLAGS = -g -O3 -Wall -ansi -std=c99
LIBXLCOM = ../client_api/xlcom_lib.a
LIBGTK2 = `pkg-config --cflags gtk+-2.0` `pkg-config --libs gtk+-2.0`
OBJECTS = client.o ui_window_main.o ui_menu.o ui_toolbar.o ui_window_connect.o ui_window_about.o ui_window_status.o


all: libs $(PROGRAMS)

.SUFFIXES: .c .o

# PART XLCOM CLI CLIENT

client: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(PROGRAMS) $(OBJECTS) $(LIBXLCOM) $(LIBGTK2)

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
