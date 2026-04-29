CC = gcc
CFLAGS = -Wall -O2
BIN_DIR = bin

all: $(BIN_DIR)/daemon $(BIN_DIR)/agent

$(BIN_DIR)/daemon: daemon.c
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ daemon.c

$(BIN_DIR)/agent: agent.c
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ agent.c

clean:
	rm -rf $(BIN_DIR)/daemon $(BIN_DIR)/agent

install: all
	install -d $(DESTDIR)/bin
	install -m 755 $(BIN_DIR)/daemon $(DESTDIR)/bin/
	install -m 755 $(BIN_DIR)/agent $(DESTDIR)/bin/
	install -m 755 run-daemon.sh $(DESTDIR)/bin/
	install -m 755 run-agent.sh $(DESTDIR)/bin/
	install -m 755 setup.sh $(DESTDIR)/bin/
