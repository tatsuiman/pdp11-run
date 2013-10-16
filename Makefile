CC            = gcc
CFLAGS        = -g #-O4 -Wall -I/usr/local/include
DEST          = /usr/local/bin
LDFLAGS       = #-L/usr/local/lib
LIBS          = -lm
OBJS          = main.o file.o vm.o print_disas.o
PROGRAM       = run
TARGET        = /usr/local/v6root/bin/nm bin/test0
MEM           = 0xFFFF

all:            $(PROGRAM)

$(PROGRAM):     $(OBJS)
	                $(CC) $(OBJS) $(LDFLAGS) $(LIBS) -o $(PROGRAM)

clean:;         rm -f *.o *~ $(PROGRAM)

install:        $(PROGRAM)
	                install -s $(PROGRAM) $(DEST)

diff: $(PROGRAM)
	-v6run -m $(MEM) -v $(TARGET) > v6run.txt 2>&1
	./run -m $(MEM) -v $(TARGET)|sed 's/ $$//g'|\
		sed 's/r6/sp/g'|sed 's/r7/pc/g'|head -n 1000|\
		diff -u - v6run.txt| vim -R - ;rm v6run.txt
v6run:
	v6run -v $(TARGET)
v6runm:
	v6run -m $(MEM) -v $(TARGET)
myrun: $(PROGRAM)
	./run -m $(MEM) -v $(TARGET)
