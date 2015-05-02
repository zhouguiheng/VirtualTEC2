#ifndef _CCHIP_H
#define _CCHIP_H


//16位寄存器
class REG
{
public:
	REG();
	void Update();
	void Input(int D);
	int Y;			//输出端
private:
	int D;
};

/****************************************************************/

//AM2901运算器(这里把AM2901直接扩充成16位)
class AM2901
{
public:
	int Y;							//向外送出的数据信号
	bool Sign, Zero, Over, Carry;	//状态标志位
	REG Register[16];				//寄存器组

	AM2901();
	bool Compute(int D, int I, int A, int B, bool Cn,
		bool &RAM0, bool &RAM15, bool &Q0, bool &Q15);
	void Update();

private:
	REG Q;							//Q寄存器
	int F;							//计算结果
	bool Modify_Q, Modify_B;
	int B;
};

/****************************************************************/

//AM2910控制器
class AM2910
{
public:
	int Y;		//下地址
	bool FULL;	//微堆栈满信号，低电平有效
	bool PL, MAP, VECT;

	AM2910();
	void Combine(int D, int I, bool CC, int CI);
	void Schedule();

private:
	REG uPC, uSP, uStack[5];
	REG R;		//寄存器/计数器
};

#endif
