
#include <assert.h>
#include <stdio.h>

//////////////////////////////////////
// DO NOT CHANGE THESE CONSTANTS:
const char UNREVEALED = ' ' ;
const char REVEALED[9] = "012345678";
const char FLAG = 'F';
const char MINE = '*';
//////////////////////////////////////

bool flag(struct ms_board *b, int x, int y) {
  assert(b->width > 0);
  assert(b->height > 0);
  assert(1 <= x && x <= b->width);
  assert(1 <= y && y <= b->height);
  int length = (b->width * b->height);
  int cord = (y - 1) * b->width + x - 1;

  if (b->board[cord] == UNREVEALED) { 
    b->board[cord] = FLAG; // a flag is placed
    return true;
  }
  if (b->board[cord] == FLAG) {
    b->board[cord] = UNREVEALED; // a flag is removed
    return true;
  }
  for (int i = 0; i < length; ++i) {
    if(b->board[cord] == REVEALED[i]) { // tile is already revealed
      return false;
    }
  }
  return true;
}

bool reveal(struct ms_board *b, int x, int y) {
  assert(b->width > 0);
  assert(b->height > 0);
  assert(1 <= x && x <= b->width);
  assert(1 <= y && y <= b->height);
  //int length = (b->width * b->height);
  int mine = b->num_mines;
  int cord = (y - 1) * b->width + x - 1;
  int z = 0;
  //All the false cases

  // when (x, y) was already revealed (#f)
  for (int i = 0; i < 9; ++i) {
    if(b->board[cord] == REVEALED[i]) { // tile is already revealed
      return false;
    }
  }

  // when a flag was already placed on (x, y) (#f)
  if (b->board[cord] == FLAG) { 
    return false;
  }

  // when (x, y) revealed, (x, y) is Mine(#f)
  for (int i = 0; i < mine; ++i) {
    if(b->mines[i].x == x && b->mines[i].y == y) {
      b->board[cord] = MINE;
      return false;
    }
  }

  // All true cases

  // count the number of mines (z) adjacent to (x, y)
  for (int x_offset = -1; x_offset <= 1; ++x_offset) {
    for (int y_offset = -1; y_offset <= 1; ++y_offset) {
      for (int i = 0; i < mine; ++i) {
        if ( x + x_offset == b->mines[i].x && y + y_offset == b->mines[i].y ) {
          ++z;
        }
      }
    }
  }

  // when cord replaced with the number of mines (z) adjacent to (x,y)
  if (z > 0) {
    b->board[cord] = REVEALED[z];
    return true;
  }

  // when z = 0, reveal all unrevealed adjacent tiles (neighbours)
  if (z == 0) {
    b->board[cord] = REVEALED[z];
    for(int x_offset = -1; x_offset <= 1; ++x_offset) {
      for (int y_offset = -1; y_offset <= 1; ++y_offset) {
        if ( x + x_offset >= 1 && x + x_offset <= b->width
            && y + y_offset >= 1 && y + y_offset <= b->height ){
          reveal(b, x + x_offset, y + y_offset);
        }
      }
    }
  }
  return true;
}


bool game_won(const struct ms_board *b) {
  assert(b->width > 0);
  assert(b->height > 0);

  int length = (b->width * b->height);
  int mine = b->num_mines;
  int safe_count = 0;

  // all safe (mine_free) tiles have been revealed (#t)
  for (int x = 1; x <= b->width; ++x) {
    for (int y = 1; y <= b->height; ++y) {
      for (int i = 0; i < mine; ++i) {
        int cord = (y - 1) * b->width + x - 1;
        if(!(b->mines[i].x == x && b->mines[i].y == y)) {//only check safe tiles

          if(b->board[cord] != UNREVEALED && b->board[cord] != FLAG) {
            ++safe_count;
          }
        } else {
          if(b->board[cord] == MINE) {
            return false;
          }

        }
      }
    }
  }
 // printf("safe_count is %d\n", safe_count);
 // printf("mine_count is %d\n", mine);
 // printf("length is %d\n", length);
  if(safe_count / mine == (length - mine)) {
    return true;
  } 

  return false;
}


bool game_lost(const struct ms_board *b) {
  assert(b->width > 0);
  assert(b->height > 0);
  int length = (b->width * b->height);
  for (int i = 0; i < length; ++i) {
    if ( b->board[i] == MINE ) {
      return true;
    }
  }
  return false;

}

