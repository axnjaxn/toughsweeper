#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <ctime>
#include "field.h"

#define TITLE "ToughSweeper by Brian Jackson"

#define CELLSIZE 30
void displayField(SDL_Surface* screen, const Field& field, TTF_Font* font, int mr, int mc) {
  SDL_Rect rect;
  SDL_Surface* text = NULL;
  char label[8];
  SDL_Color bgcolor = {0xFF, 0xFF, 0xFF};
  SDL_Color fgcolor = {0, 0, 0};

  SDL_FillRect(screen, NULL, 0);
  for (int r = 0; r < field.height(); r++)
    for (int c = 0; c < field.width(); c++) {
      rect.x = c * CELLSIZE + 1;
      rect.y = r * CELLSIZE + 1;
      rect.w = rect.h = CELLSIZE - 2;

      bgcolor.r = bgcolor.g = bgcolor.b = 0xFF;
      fgcolor.r = fgcolor.g = fgcolor.b = 0;

      if (r <= mr + 2 && r >= mr - 2 && c <= mc + 2 && c >= mc - 2) {
	bgcolor.r = bgcolor.g = 0xFF;
	if (field.isVisible(r, c)) bgcolor.b = 0xA0;
	else bgcolor.b = 0x80;
      }

      if (field.isFlagged(r, c)) {
	bgcolor.b = 0xFF;
	bgcolor.r = bgcolor.g = 0;
      }
      else if (field.isVisible(r, c)) {
	if (field.isBomb(r, c)) {
	  bgcolor.r = 0xFF;
	  bgcolor.g = bgcolor.b = 0;
	}
	else {
	  sprintf(label, "%d", field.neighbors(r,c));
	  text = TTF_RenderText_Shaded(font, label, fgcolor, bgcolor);
	}
      }      
      else if (field.getMark(r, c)) {
	sprintf(label, "%d", field.getMark(r,c));
	fgcolor.r = fgcolor.g = 0;
	fgcolor.b = 0xFF;
 	text = TTF_RenderText_Shaded(font, label, fgcolor, bgcolor);
      }
      
      
      SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, bgcolor.r, bgcolor.g, bgcolor.b));
      if (text) {
	rect.x = rect.x + (CELLSIZE - text->w) / 2;
	rect.y = rect.y + (CELLSIZE - text->h) / 2;
	SDL_BlitSurface(text, NULL, screen, &rect);
	SDL_FreeSurface(text);
	text = NULL;
      }

      
    }
}

void readConfig(int* nr, int* nc, int* nm) {
  char fn[200];
  sprintf(fn, "%s/.toughsweeper/config.txt", getenv("HOME"));
  FILE* fp = fopen(fn, "r");
  if (!fp) {
    *nr = 10;
    *nc = 10;
    *nm = 15;
    fp = fopen(fn, "w");
    if (fp) {
      fprintf(fp, "%d %d %d", *nr, *nc, *nm);
      fclose(fp);
    }
  }
  else {
    fscanf(fp, "%d %d %d", nr, nc, nm);
    fclose(fp);
  }
}

TTF_Font* getFont(const char* fn, int sz) {
  TTF_Font* font = NULL;
  char name[200];
  sprintf(name, "%s/.toughsweeper/%s", getenv("HOME"), fn);
  font = TTF_OpenFont(name, sz);
  if (!font) TTF_OpenFont(fn, sz);
  return font;
}

void win(SDL_Surface* screen) {
  TTF_Font* font = getFont("FreeSans.ttf", 24);
  SDL_Rect rect;
  SDL_Color white = {0xFF, 0xFF, 0xFF};
  SDL_Color blue = {0, 0, 0x80};
  rect.x = 0;
  rect.y = screen->h / 2 - 25;
  rect.w = screen->w;
  rect.h = 50;
  SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, 0, 0, 0x80));
  SDL_Surface* text = TTF_RenderText_Shaded(font, "Success!", white, blue);
  rect.x = (screen->w - text->w) / 2;
  rect.y = (screen->h - text->h) / 2;
  rect.w = text->w;
  rect.h = text->h;
  SDL_BlitSurface(text, NULL, screen, &rect);
  SDL_FreeSurface(text);
  SDL_Flip(screen);

  SDL_Event event;
  for(;;) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) exit(0);
      else if (event.type == SDL_KEYDOWN) {
	if (event.key.keysym.sym == SDLK_ESCAPE) exit(0);
	else if (event.key.keysym.sym == SDLK_F2) return;
      }
    }
    SDL_Delay(100);
  }
}

int main(int argc, char* argv[]) {
  int nr, nc, nm;
  readConfig(&nr, &nc, &nm);

  //Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL INIT ERROR");
    exit(1);
  }

  //Initialize TTF
  if(TTF_Init() < 0) {
    printf("TTF_Init: %s\n", TTF_GetError());
    exit(1);
  }

  atexit(TTF_Quit);
  atexit(SDL_Quit);
  
  SDL_WM_SetCaption(TITLE, NULL);
  srand(time(NULL));
  Field field(nr, nc, nm, 2);
  SDL_Surface* screen = SDL_SetVideoMode(CELLSIZE * field.width(), 
					 CELLSIZE * field.height(),
					 32, SDL_HWSURFACE|SDL_DOUBLEBUF);

  TTF_Font* font = getFont("FreeSans.ttf", 12);
  if (!font) {
    printf("Can't load font! Exiting.\n");
    return 1;
  }
 
  displayField(screen, field, font, -99, -99);
  SDL_Flip(screen);

  int r, c, dead = 0;
  SDL_Event event;
  for (;;) {
    if (field.complete()) {
      win(screen);
      
      srand(time(NULL));
      field.reset();
      dead = 0;
      
      displayField(screen, field, font, r, c);
      SDL_Flip(screen);
    }

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) exit(0);
      else if (event.type == SDL_KEYDOWN) {
	if (event.key.keysym.sym == SDLK_ESCAPE) exit(0);
	else if (event.key.keysym.sym == SDLK_F2) {
	  srand(time(NULL));
	  field.reset();
	  dead = 0;

	  displayField(screen, field, font, r, c);
	  SDL_Flip(screen);
	}
	else if (event.key.keysym.sym >= SDLK_0 && event.key.keysym.sym <= SDLK_9) {
	  field.setMark(r, c, event.key.keysym.sym - SDLK_0);

	  displayField(screen, field, font, r, c);
	  SDL_Flip(screen);
	}
	else if (event.key.keysym.sym == SDLK_1) {
	  field.setMark(r, c, 1);

	  displayField(screen, field, font, r, c);
	  SDL_Flip(screen);
	}
      }
      else if (event.type == SDL_MOUSEMOTION) {
	r = event.motion.y / CELLSIZE;
	c = event.motion.x / CELLSIZE;

	displayField(screen, field, font, r, c);
	SDL_Flip(screen);
      }
      else if (!dead && event.type == SDL_MOUSEBUTTONDOWN) {
	if (event.button.button == SDL_BUTTON_RIGHT) {
	  if (!field.isVisible(r, c)) field.flag(r,c);
	}
	else if (event.button.button == SDL_BUTTON_LEFT) {
	  field.setVisible(r, c);
	  if (field.isBomb(r, c)) dead = 1;
	  else if (field.neighbors(r, c) == 0)
	    field.traverseZero(r, c);
	}

	displayField(screen, field, font, r, c);
	SDL_Flip(screen);
      }
    }

    SDL_Delay(100);
  }
  
  return 0;
}
