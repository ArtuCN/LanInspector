SRC = main.cpp src/local.cpp src/arp.cpp src/NetworkInterface.cpp
COMPILE = g++
CFLAGS = -Ilib
RM = rm -f
EXEC = laninspector

.PHONY: all run clean re

all:
	$(COMPILE) $(CFLAGS) $(SRC) -o $(EXEC)

run: all
	sudo ./$(EXEC)

clean:
	$(RM) $(EXEC)

re:
	$(MAKE) clean
	$(MAKE) all