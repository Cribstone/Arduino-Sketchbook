TOPDIR  := $(shell cd ..; pwd)
include $(TOPDIR)/Rules.make

APP = simple

all: $(APP)

$(APP): main.c	
	$(CC) -o $(APP) main.c $(CFLAGS)	
	
clean:
	rm -f *.o ; rm $(APP)
