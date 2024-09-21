CFLAGS = -std=c2x -D_GNU_SOURCE -m32 -fPIC -Wall -Wextra

%.so: %.o
	$(CC) $(CFLAGS) -shared $^ -o $@

all: audit.so

clean:
	rm -f audit.so audit.o
