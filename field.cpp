#include "field.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

int Field::getNeighborhood(int r0, int c0, int n) const {
  if (cells[r0 * w + c0] & FIELD_BOMB) return 0;
  int mines = 0;
  for (int r = -n; r <= n; r++)
    for (int c = -n; c <= n; c++) {
      if (r0 + r < 0 || r0 + r >= h || c0 + c < 0 || c0 + c >= w) continue;
      else if (cells[(r0 + r) * w + c0 + c] & FIELD_BOMB) mines++;
    }
  return mines;
}

Field::Field(int width, int height, int num_mines, int neighbor_size) {
  w = width;
  h = height;
  if (num_mines > w * h) num_mines = w * h;
  nmines = num_mines;
  nsize = neighbor_size;
  cells = new unsigned char [w * h];
  marks = new unsigned char [w * h];

  reset();
}

Field::Field(const Field& f) {
  cells = marks = NULL;
  *this = f;
}

Field::~Field() {
  if (cells) delete [] cells;
  if (marks) delete [] marks;
}

Field& Field::operator=(const Field& f) {
  w = f.w; h = f.h;
  nmines = f.nmines; nminesleft = f.nminesleft;
  nflagsleft = f.nflagsleft; nsize = f.nsize;
  if (cells) delete [] cells;
  if (marks) delete [] marks;
  cells = new unsigned char [w * h];
  marks = new unsigned char [w * h];
  memcpy(cells, f.cells, w * h);
  memcpy(marks, f.marks, w * h);
  return *this;
}

void Field::reset() {
  memset(cells, 0, w * h);
  memset(marks, 0, w * h);

  nminesleft = nflagsleft = nmines;
    
  //This should be rewritten. This is a terrible way to do this.
  int tgt;
  for (int i = 0; i < nmines; i++) {
    do {
      tgt = rand() % (w * h);
    }	while (cells[tgt]);
    cells[tgt] = FIELD_BOMB;
  }
    
  for (int r = 0; r < h; r++)
    for (int c = 0; c < w; c++)
      cells[r * w + c] |= getNeighborhood(r, c, nsize);
}

void Field::traverseZero(int r, int c) {
  for (int nr = r - nsize; nr <= r + nsize; nr++) {
    for (int nc = c - nsize; nc <= c + nsize; nc++) {
      if (nr >= 0 && nr < h && nc >= 0 && nc < w && !isVisible(nr, nc)) {
	setVisible(nr, nc);
	if (neighbors(nr, nc) == 0) traverseZero(nr, nc);
      }
    }
  }
}

void Field::flag(int r, int c) {
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

void Field::print() {
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
