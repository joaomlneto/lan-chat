LIBRARIES = xlcom_lib.a
OBJECTS = xlcom_api.o xlcp_api.o

CC = gcc
#CFLAGS 32-bit = $(INCLUDES) -g -O3 -Wall -m32 -ansi -std=c99 -D_GNU_SOURCE
CFLAGS = $(INCLUDES) -g -O3 -Wall -ansi -std=c99 -D_GNU_SOURCE
INCLUDES = -I ../include -I .

AR = ar
ARFLAGS = cru
RANLIB = ranlib

all: $(LIBRARIES)

xlcom_lib.a: $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^
	$(RANLIB) $@

clean: 
	rm -f *.o
	rm -f *.a

