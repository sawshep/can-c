CC = clang
CFLAGS = -pipe -ansi -pedantic -g -O0
DEPS = arg.h env.h trash.h
OBJ = arg.c can.c env.c trash.c

# Black magic, this is
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

can: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

#clean:
#	rm $(TARGET)
