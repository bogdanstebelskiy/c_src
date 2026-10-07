CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c11 -g

# Automatically include all header directories
INC_DIRS := $(shell find include src/internal src/utils -type d)
CFLAGS += $(addprefix -I,$(INC_DIRS))

LDFLAGS = 

# Source groups
SRC_HTTP     = src/http/parser.c src/http/response.c src/http/headers.c src/http/mime.c
SRC_UTILS    = src/utils/logger.c src/utils/fileio.c
SRC_SERVER   = src/server/server.c
SRC_HANDLERS = src/handlers/static.c
SRC_MAIN     = src/main.c

# Object files
OBJ = $(SRC_HTTP:.c=.o) \
      $(SRC_UTILS:.c=.o) \
      $(SRC_SERVER:.c=.o) \
      $(SRC_HANDLERS:.c=.o) \
      $(SRC_MAIN:.c=.o)

TARGET = webserver

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

# Generic compile rule
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
