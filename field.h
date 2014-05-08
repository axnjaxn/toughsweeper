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

  int getNeighborhood(int r0, int c0, int n);

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

  void setVisible(int r, int c);
  void traverseZero(int r, int c);//?
  void flag(int r, int c);//?
  
  //These should all be const
  int checkVisible(int r, int c);//repl. with isVisible
  int checkFlagged(int r, int c);//repl. with isFlagged
  int checkBomb(int r, int c);//isBomb
  int neighbors(int r, int c);

  void print();
};

#endif
