#ifndef bpj_toughsweeper_field_h
#define bpj_toughsweeper_field_h

#include <cstdio>
#include <cstdlib>
#include <cstring>

#define FIELD_VISIBLE 0x80
#define FIELD_BOMB 0x40
#define FIELD_FLAGGED 0x20
#define FIELD_NUMBERS 0x1F
class Field {
public:
  Field(int width, int height, int num_mines, int neighbor_size) {
    w = width;
    h = height;
    if (num_mines > w * h) num_mines = w * h;
    nmines = num_mines;
    nsize = neighbor_size;
    cells = new unsigned char [w * h];
    marks = new unsigned char [w * h];

    reset();
  }
  ~Field() {
    if (cells) delete cells;
  }

  void reset() {
    memset(cells, 0, w * h);
    memset(marks, 0, w * h);

    nminesleft = nflagsleft = nmines;
    
    int tgt;
    for (int i = 0; i < nmines; i++) {
      do {
	tgt = rand() % (w * h);
      }	while (cells[tgt]);
      cells[tgt] = FIELD_BOMB;
    }
    
    for (int r = 0; r < h; r++)
      for (int c = 0; c < w; c++)
	cells[r * w + c] += getNeighborhood(r, c, nsize);
  }
  int flags() {
    return nflagsleft;
  }
  int complete() {
    return (!nminesleft && !nflagsleft);
  }
  int width() {return w;}
  int height() {return h;}
  void setMark(int r, int c, unsigned char mark) {
    marks[r * w + c] = mark;
  }
  unsigned char getMark(int r, int c) {
    return marks[r * w + c];
  }
  void setVisible(int r, int c) {
    if (!(cells[r * w + c] & FIELD_VISIBLE)) cells[r * w + c] += FIELD_VISIBLE;
  }
  void traverseZero(int r, int c) {
    for (int nr = r - nsize; nr <= r + nsize; nr++) {
      for (int nc = c - nsize; nc <= c + nsize; nc++) {
	if (nr >= 0 && nr < h && nc >= 0 && nc < w && !checkVisible(nr, nc)) {
	  setVisible(nr, nc);
	  if (neighbors(nr, nc) == 0) traverseZero(nr, nc);
	}
      }
    }
  }
  void flag(int r, int c) {
    if (cells[r * w + c] & FIELD_FLAGGED) {
      cells[r * w + c] -= FIELD_FLAGGED;
      if (cells[r * w + c] & FIELD_BOMB) nminesleft++;
      nflagsleft++;
    }
    else {
      cells[r * w + c] += FIELD_FLAGGED;
      if (cells[r * w + c] & FIELD_BOMB) nminesleft--;
      nflagsleft--;
    }
  }
  int checkVisible(int r, int c) {
    return ((cells[r * w + c] & FIELD_VISIBLE) != 0);
  }
  int checkFlagged(int r, int c) {
    return ((cells[r * w + c] & FIELD_FLAGGED) != 0);
  }
  int checkBomb(int r, int c) {
    return ((cells[r * w + c] & FIELD_BOMB) != 0);
  }
  int neighbors(int r, int c) {
    return (cells[r * w + c] & FIELD_NUMBERS);
  }
  void print() {
    printf("   ");
    for (int i = 0; i < w; i++) printf("%2d ", i);
    printf("\n");
    for (int r = 0; r < h; r++) {
      printf("%2d ", r);
      for (int c = 0; c < w; c++) {
	if (cells[r * w + c] & FIELD_FLAGGED) printf("|> ");
	else if (cells[r * w + c] & FIELD_VISIBLE) {
	  if (cells[r * w + c] & FIELD_BOMB) printf("*  ");
	  else printf("%2d ", cells[r * w + c] - FIELD_VISIBLE);
	}
	else printf("[] ");
      }
      printf("\n");
    }
  }
protected:
  int w, h, nmines, nminesleft, nflagsleft, nsize;
  unsigned char* cells;
  unsigned char* marks;

  int getNeighborhood(int r0, int c0, int n) {
    if (cells[r0 * w + c0] & FIELD_BOMB) return 0;
    int mines = 0;
    for (int r = -n; r <= n; r++)
      for (int c = -n; c <= n; c++) {
	if (r0 + r < 0 || r0 + r >= h || c0 + c < 0 || c0 + c >= w) continue;
	else if (cells[(r0 + r) * w + c0 + c] & FIELD_BOMB) mines++;
      }
    return mines;
  }
};

#endif
