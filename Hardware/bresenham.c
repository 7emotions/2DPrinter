#include "stm32f10x.h"                  // Device header

#include "bresenham.h"
#include "Delay.h"
#include <cmath>
#include <stdlib.h>

void EMP_down(){

}

void EMP_up(){
	
}

void Control42(int x,int y,int precision){
	
}


void draw_line(int x1,int y1,int x2,int y2,int precision){
	//精度预处理
	x1 = x1/precision;
	y1 = y1/precision;
	x2 = x1/precision;
	y2 = y2/precision;
	//
	EMP_up();//确保电磁铁提起
	
	int x,y,dx,dy,p1,i;
	dx = x2 - x1;
	dy = y2 - y1;

	
	if(abs(dx) > abs(dy)){
		if(x1 < x2){
			x = x1;
			y = y1;
		}
		else{
			x = x2;
			y = y2;
		}
		
		
		for(int k = 0;k < abs(x)/precision;k++){
			Control42((x>0?1:-1),0,precision);
		}
		
		for(int k = 0;k < abs(y)/precision;k++){
			Control42(0,(y>0?1:-1),precision);
		}
		
		EMP_down();//电磁铁放下
		//printf("(%d,%d)",x,y);
		Delay_ms(50);
		p1 = 2*abs(dy) - abs(dx);
	
		for(i = MIN(x1,x2);i < MAX(x1,x2);i++){
			x = x+1;
			Control42(1,0, precision);
			
			if(p1 >= 0){
				if(dx*dy >= 0){
					y = y+1;
					Control42(0,1, precision);
				}
				else{
					y = y-1;
					Control42(0,-1, precision);
				}
				p1 = p1+2*(abs(dy) - abs(dx));
			
			}
			else{
				y  = y;
				p1 = p1+2*abs(dy);
			}
			//printf("(%d,%d)",0+x,0-y);
			Delay_ms(50);
		}
	}
	
	
	else{
		if(y1 < y2){
			x = x1;
			y = y1;
		}
		else{
			x = x2;
			y = y2;
		}
		//printf("(%d,%d)",x,-y);
		Delay_ms(50);
		
		p1 = 2*abs(dx) - abs(dy);
		for(i = MIN(y1,y2);i < MAX(y1,y2);i++){
			y = y+1;
			
			Control42(0,1,precision);
			
			if(p1 >= 0){
				if(dx*dy >= 0){
					x = x+1;
					Control42(1,0, precision);
				}
				else{
					x = x-1;
					Control42(-1,0, precision);
				}
				p1 = p1+2*(abs(dx) - abs(dy));
			}
			else{
				x = x;
				p1 = p1+2*abs(dx);
			}
			//printf("(%d,%d)",x,y);
			Delay_ms(50);
		}
	}
	for(int k = 0;k < abs(x)/precision;k++){
		Control42((dx>0?1:-1),0,precision);
	}
		
	for(int k = 0;k < abs(y)/precision;k++){
		Control42(0,(dy>0?1:-1),precision);
	}
}



void draw_cricle(int x1,int y1,int r,int precision){
	//精度预处理
	x1 = x1/precision;
	y1 = y1/precision;
	r = r/precision;
	//
	
	int t = 0;
	int a[1024] = {0};
	int x,y,p;
	x = 0;
	y = r;
	p = 3-2*r;
	while(x < y){
		//画x1,x2,x,y
		if(p < 0)
			p = p+4*x+6;
		else{
			p = p+4*(x-y) + 10;
			y -= 1;
			a[t++] = 1;
		}
		x += 1;
		a[t++] = 0;
		
	}
	
	int dx = x1;
	int dy = y1+r;
	
	for(int k = 0;k < abs(dx)/precision;k++){
		Control42((dx>0?1:-1),0,precision);
	}
		
	for(int k = 0;k < abs(dy)/precision;k++){
		Control42(0,(dy>0?1:-1),precision);
	}
	
	draw_line(0,0,dx,dy,precision);
	
	for (int i = 0; i < t; i++) {
		if (a[i] == 0) {
			dx++;
			Control42(1,0, precision);
		}
		else {
			dy--;
			Control42(0,-1, precision);
		}
	}

	for (int i = t - 1; i >= 0; i--) {
		if (a[i] == 0) {
			dy--;
			Control42(0,-1, precision);
		}
		else {
			dx++;
			Control42(1,0, precision);
		}
	}

	for (int i = 0; i < t; i++) {
		if (a[i] == 0) {
			dy--;
			Control42(0,-1, precision);
		}
		else {
			dx--;
			Control42(-1,0, precision);
		}
	}

	for (int i = t-1; i >= 0; i--) {
		if (a[i] == 0) {
			dx--;
			Control42(-1,0, precision);
		}
		else {
			dy--;
			Control42(0,-1, precision);
		}
	}
	//////////////
	for (int i = 0; i < t; i++) {
		if (a[i] == 0) {
			dx--;
			Control42(-1,0, precision);
		}
		else {
			dy++;
			Control42(0,1, precision);
		}
	}	

	for (int i = t-1; i >= 0; i--) {
		if (a[i] == 0) {
			dy++;
			Control42(0,1, precision);
		}
		else {
			dx--;
			Control42(-1,0, precision);
		}
	}

	for (int i = 0; i < t; i++) {
		if (a[i] == 0) {
			dy++;
			Control42(0,1, precision);
		}
		else {
			dx++;
			Control42(1,0, precision);
		}
	}

	for (int i = t-1; i >= 0; i--) {
		if (a[i] == 0) {
			dx++;
			Control42(1,0, precision);
		}
		else {
			dy++;
			Control42(0,1, precision);
		}
	}
	
	for(int k = 0;k < abs(dx)/precision;k++){
		Control42((dx>0?-1:1),0,precision);
	}
		
	for(int k = 0;k < abs(dy)/precision;k++){
		Control42(0,(dy>0?-1:1),precision);
	}
	
}