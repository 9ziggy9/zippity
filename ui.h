#ifndef UI_H_
#define UI_H_
#include <ncurses.h>

#define WINDOW_WIDTH    80
#define WINDOW_HEIGHT   20
#define BORDER_WIDTH    1
#define MAX_LINE_LENGTH 256
#define MAX_LINES       256

#define KEY_ESC 27

typedef struct { char *txt; int row; bool selected; } Line;

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
void ui_clear_master(WINDOW *);
int  ui_read_page(WINDOW *, FILE *);

enum {
  COLORS_D,
  COLORS_HL,
  COLORS_SEL,
};
void ui_master_update(WINDOW *, int);
void ui_hl_line(WINDOW *, int, short);
void ui_toggle_sel(int);

struct hitlist {
  WINDOW *wm;
  FILE *fp;
};
void ui_exit_handler(int code, void *args);

#endif // UI_H_
