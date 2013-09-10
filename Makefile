CC = g++

CFLAGS = -Wall -g `pkg-config gtkmm-3.0 --cflags`
INCLUDES = -I/usr/include/libircclient -I/usr/include/gtkmm-3.0
LFLAGS =
LIBS = -lircclient `pkg-config gtkmm-3.0 --libs`

SRCS += irc_signal_handler.cpp main_window.cpp
OBJS = ${SRCS:.cpp=.o}

MAIN = Pesterchum

.PHONY: clean

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) *.o $(MAIN)

