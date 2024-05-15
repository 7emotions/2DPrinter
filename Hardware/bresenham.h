#ifndef __BRESENHAM_H__
#define __BRESENHAM_H__


#define MIN(i, j) (((i) < (j)) ? (i) : (j))
#define MAX(i, j) (((i) > (j)) ? (i) : (j))
void draw_cricle(int xc,int yc,int r,int precision);
void draw_line(int x1,int y1,int x2,int y2,int precision);
void EMP_down();
void EMP_up();
void Control42(int x,int y,int precision);

#endif