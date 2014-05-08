APPNAME = toughsweeper
LFLAGS = -lSDL -lSDLmain -lSDL_ttf

build: clean $(APPNAME)

$(APPNAME):
	g++ -c *.cpp
	g++ *.o -o $(APPNAME) $(LFLAGS)

run: build
	./$(APPNAME)

clean:
	rm -f *.o *~ $(APPNAME)

debug: $(APPNAME)
	gdb $(APPNAME)

install: $(APPNAME) FreeSans.ttf
	cp $(APPNAME) -t /usr/local/bin

install_font:
	mkdir -p ~/.toughsweeper
	cp FreeSans.ttf -t ~/.toughsweeper