all:
	gcc -O2 -o linkedin_daemon linkedin_daemon.c

install:
	mkdir -p $(DESTDIR)/bin
	cp linkedin_daemon $(DESTDIR)/bin/
	cp run-daemon.sh $(DESTDIR)/bin/
	cp setup.sh $(DESTDIR)/bin/
	chmod +x $(DESTDIR)/bin/run-daemon.sh
	chmod +x $(DESTDIR)/bin/setup.sh
