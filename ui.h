#ifndef UI_H_
#define UI_H_
#include "pages.h"
#include <ncurses.h>

#define BORDER_WIDTH 1
#define KEY_ESC 27

enum {
  EXIT_ALLOC_FAILED = 100,
};

#define PANIC(ERR)                                               \
  do {                                                           \
    fprintf(stderr, "[PANIC] %s() :: %s\n", __func__, ""#ERR""); \
    exit(ERR);                                                   \
  } while(0);                                                    \


void ui_init_scr(void);
void ui_init_master(WINDOW **);
void ui_clear_master(WINDOW *, Page *p);
void ui_master_update(WINDOW *, Page *, size_t);

size_t ui_master_get_cols(void);
size_t ui_master_get_rows(void);

void ui_select_line(Page *, size_t);

enum {
  COLORS_D,
  COLORS_HL,
  COLORS_SEL,
};


struct hitlist {
  WINDOW *wm;
  FILE *fp;
};
void ui_exit_handler(int code, void *args);

#endif // UI_H_
