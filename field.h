#ifndef _BPJ_TOUGHSWEEPER_FIELD_H
#define _BPJ_TOUGHSWEEPER_FIELD_H

#define FIELD_VISIBLE 0x80
#define FIELD_BOMB 0x40
#define FIELD_FLAGGED 0x20
#define FIELD_NUMBERS 0x1F

class Field {
protected:
  int w, h, nmines, nminesleft, nflagsleft, nsize;
  unsigned char *cells, *marks;

  int getNeighborhood(int r0, int c0, int n) const;

public:
  Field(int width, int height, int num_mines, int neighbor_size);
  Field(const Field& f);
  virtual ~Field();

  Field& operator=(const Field& f);

  void reset();

  inline int flags() const {return nflagsleft;}
  inline bool complete() const {return (!nminesleft && !nflagsleft);}
  inline int width() const {return w;}
  inline int height() const {return h;}
  inline void setMark(int r, int c, unsigned char mark) {marks[r * w + c] = mark;}
  inline unsigned char getMark(int r, int c) const {return marks[r * w + c];}
  inline void setVisible(int r, int c) {cells[r * w + c] |= FIELD_VISIBLE;}
  inline bool isVisible(int r, int c) const {return cells[r * w + c] & FIELD_VISIBLE;}
  inline bool isFlagged(int r, int c) const {return cells[r * w + c] & FIELD_FLAGGED;}
  inline bool isBomb(int r, int c) const {return cells[r * w + c] & FIELD_BOMB;}
  inline int neighbors(int r, int c) const {return cells[r * w + c] & FIELD_NUMBERS;}

  void traverseZero(int r, int c);//Expands connected 0-neighbor cells
  void flag(int r, int c);

  void print();
};

#endif
