#include "pages.h"
#include <string.h>

Page *page_load(FILE *fp, size_t page_no, size_t max_lines, size_t max_txt)
{
  rewind(fp);

  Page *p = (Page *) malloc(sizeof(Page));
  if (p == NULL) exit(EXIT_FAILURE);
  p->lines     = malloc(sizeof(Line) * max_lines);
  p->page_no   = page_no;
  p->page_end  = 0;
  p->lines_max = max_lines;
  p->prev      = NULL;
  p->next      = NULL;

  size_t line_no = 0;
  size_t file_line = 0;
  char txt_buf[max_txt];

  size_t start_line = page_no * max_lines;
  while (file_line < start_line &&
         fgets(txt_buf, (int)sizeof(txt_buf), fp) != NULL) file_line++;
  while
  (line_no < max_lines && fgets(txt_buf, (int)sizeof(txt_buf), fp) != NULL)
  {
    txt_buf[strcspn(txt_buf, "\n")] = '\0';
    p->lines[line_no] = (Line) {
      .txt          = strdup(txt_buf),
      .abs_line_no  = p->page_no * max_lines + line_no + 1,
      .line_max     = max_txt,
      .selected     = false,
    };
    p->page_end++;
    line_no++;
  }
  return p;
}

#define MAX_PAGES 10
void turn_page_prev(Page **pg) { if ((*pg)->page_no != 0) *pg = (*pg)->prev; }
void turn_page_next(FILE *fp, Page **pg, size_t rows, size_t cols) {
  if ((*pg)->page_no == MAX_PAGES) return;
  if ((*pg)->next == NULL) {
    (*pg)->next = page_load(fp, (*pg)->page_no + 1, rows, cols);
    (*pg)->next->prev = (*pg);
  }
  *pg = (*pg)->next;
}
