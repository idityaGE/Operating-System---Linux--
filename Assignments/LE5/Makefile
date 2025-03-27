CC = gcc
CFLAGS = -Wall -Wextra

SRCS = main.c src/common.c src/display.c src/fcfs.c src/sjf.c src/rr.c src/srtn.c src/hrrn.c src/lottery.c src/edf.c src/priority.c
OBJS = $(SRCS:.c=.o)
TARGET = scheduler

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
