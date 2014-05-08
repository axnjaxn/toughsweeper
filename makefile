APPNAME = toughsweeper

LFLAGS = `sdl-config --libs` -lSDL_ttf
DFLAGS = -O2 -Wno-unused-result
EXT = 

APP = $(APPNAME)$(EXT)

OBJECT_FILES = field.o main.o

all: $(APP)

.cpp.o:
	$(CXX) -c $< $(DFLAGS)

$(APP): $(OBJECT_FILES)
	$(CXX) $(OBJECT_FILES) -o $(APP) $(LFLAGS) $(DFLAGS)

run: $(APP)
	./$(APP)

clean:
	rm -f *.o *~ $(APPNAME)

install: $(APPNAME) FreeSans.ttf
	cp $(APPNAME) -t /usr/local/bin

install_font:
	mkdir -p ~/.toughsweeper
	cp FreeSans.ttf -t ~/.toughsweeper