#include "ui.h"
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int SCREEN_COLS, SCREEN_ROWS;
int MASTER_COLS, MASTER_ROWS;

void ui_exit_handler(int code, void *args) {
  struct hitlist *hl = (struct hitlist *) args;
  if (hl->wm != NULL) delwin(hl->wm);
  if (hl->fp != NULL) fclose(hl->fp);
  endwin();
  printf("EXIT: %d\n", code);
}

static void ui_init_clr(void) {
  start_color();
  init_pair(COLORS_D,   COLOR_WHITE, COLOR_BLACK);
  init_pair(COLORS_HL,  COLOR_BLACK, COLOR_WHITE);
  init_pair(COLORS_SEL, COLOR_BLACK, COLOR_GREEN);
}

void ui_init_scr(void) {
  initscr();
  if (stdscr == NULL) PANIC(EXIT_ALLOC_FAILED);
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(FALSE);

  SCREEN_COLS = getmaxx(stdscr);
  SCREEN_ROWS = getmaxy(stdscr);

  ui_init_clr();
}

void ui_init_master(WINDOW **w) {
  MASTER_COLS = SCREEN_COLS - 2 * BORDER_WIDTH;
  MASTER_ROWS = SCREEN_ROWS - 2 * BORDER_WIDTH;
  *w = newwin(MASTER_ROWS, MASTER_COLS, 1, 1);
  if (*w == NULL) PANIC(EXIT_ALLOC_FAILED);
  scrollok(*w, TRUE);
  keypad(*w, TRUE);
  set_escdelay(0);
  box(*w, 0, 0);
}

size_t ui_master_get_cols(void) {return (size_t)MASTER_COLS - 2 * BORDER_WIDTH;}
size_t ui_master_get_rows(void) {return (size_t)MASTER_ROWS - 2 * BORDER_WIDTH;}

static void ui_print_line_no(WINDOW *w, size_t abs_line_no, int rel_line_no) {
  wattron(w, A_DIM);
  mvwprintw(w, rel_line_no, BORDER_WIDTH, "%4zu ", abs_line_no);
  wattroff(w, A_DIM);
}

static void ui_page_no_title(WINDOW *w, size_t page_no) {
  char page_no_title[10] = "";
  int offset  = sprintf(page_no_title, "[ %zu ]", page_no);
  int col_end = (int) ui_master_get_cols();
  mvwprintw(w, 0, col_end - offset, "%s", page_no_title);
}

/* static void mvwprintw_lag(WINDOW *w, struct timespec t, const char *fmt, ...) */
/* { */
/*   va_list args; */
/*   va_start(args, fmt); */
/*   char buffer[256]; */
/*   vsnprintf(buffer, sizeof(buffer), fmt, args); */
/*   va_end(args); */
/*   curs_set(TRUE); */
/*   for (char *p = buffer; *p != '\0'; p++) { */
/*     waddch(w, *p); */
/*     wrefresh(w); */
/*     nanosleep(&t, NULL); */
/*   } */
/*   curs_set(FALSE); */
/* } */

void ui_clear_master(WINDOW *w, Page *p) {
  for (size_t l = 0; l < p->page_end; l++) {
    ui_print_line_no(w, p->lines[l].abs_line_no, (int) l + BORDER_WIDTH);
    wprintw(w, "%s", p->lines[l].txt);
    wclrtoeol(w);
    box(w, 0, 0);
  }
  ui_page_no_title(w, p->page_no);
}

#define PAD_LINE_NO 5
void ui_master_update(WINDOW *w, Page *p, size_t line_no) {
  static size_t last_line_no = 0;
  // unhighlight
  mvwchgat(w, (int) last_line_no + BORDER_WIDTH, BORDER_WIDTH + PAD_LINE_NO,
           (int)ui_master_get_cols() - PAD_LINE_NO, A_NORMAL, COLORS_D, NULL);
  // highlight
  mvwchgat(w, (int) line_no + BORDER_WIDTH, BORDER_WIDTH + PAD_LINE_NO,
           (int)ui_master_get_cols() - PAD_LINE_NO, A_BOLD, COLORS_HL, NULL);
  // additional attributes
  for (size_t l = 0; l < p->page_end; l++) {
    if (p->lines[l].selected)
      mvwchgat(w, (int) l + BORDER_WIDTH, BORDER_WIDTH,
               (int)ui_master_get_cols(), A_BOLD, COLORS_SEL, NULL);
  }
  last_line_no = line_no;
}

void ui_select_line(Page *p, size_t line_no) {
  p->lines[line_no].selected = !p->lines[line_no].selected;
}
