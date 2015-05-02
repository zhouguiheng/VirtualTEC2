#ifndef _CBIT_H
#define _CBIT_H

//返回F的第p位
bool getbit(int F, int p);

//返回F的low与high之间所有位
int getbits(int F, int high, int low);

//设置F的第p位为bit
void setbit(int &F, int p, bool bit);


#endif
