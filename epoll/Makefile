CC = gcc
CFLAGS = -Wall -Wextra -O2 -pthread
LDFLAGS = -lpthread -lssl -lcrypto

TARGET = server
SRCS = server.c queue.c http_response.c http_parser.c router.c handlers.c ssl.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

re: clean all

.PHONY: all clean re
