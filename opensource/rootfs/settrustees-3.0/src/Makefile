CFLAGS += -ggdb -I../include -Wall
LDFLAGS := 
CC := gcc
settrustees_objs := settrustees.o dynamic_array.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

default: settrustees

settrustees: $(settrustees_objs)
	$(CC) $(LDFLAGS) -o settrustees $(settrustees_objs) 

clean:
	rm -fr *.o settrustees
