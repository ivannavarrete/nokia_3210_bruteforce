
MAKE = make


all:
	@cd ./src; $(MAKE);
	@cp ./src/nokia ./bin;
	@cp ./src/hc11/nokia.s19 ./bin/hc11;

clean:
	@cd ./src; $(MAKE) clean;
	@cd ./bin; rm -f nokia hc11/nokia.s19;
