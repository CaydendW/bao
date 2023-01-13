LD = gcc
CC = gcc

LIBS = -lm

CFLAGS := \
	-Iinclude \
	-Wall \
	-Wextra \
	-Werror \
	-g \
	-std=gnu99 \
	-Wno-unused-result \

CFILES := $(shell find src/ -name '*.c')
OFILES := $(CFILES:.c=.o)

TARGET = bao

all: clean compile

compile: ld
	@ echo "Done!"
	
ld: $(OFILES)
	@ echo "[LD] $^"
	@ $(LD) $^ $(LIBS) -o $(TARGET)

%.o: %.c
	@ echo "[CC] $<"
	@ $(CC) $(CFLAGS) -c $< -o $@

clean:
	@ echo "[CLEAN]"
	@ rm -rf $(OFILES) $(TARGET)

