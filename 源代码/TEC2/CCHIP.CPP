#include "stdafx.h"
#include "cchip.h"
#include "cbit.h"

REG::REG()
{
	Y &= 0xffff;
	D &= 0xffff;
}

void REG::Input(int D)
{
	this->D = D;
}

void REG::Update()
{
	Y = D;
}

/****************************************************************/

AM2901::AM2901():Sign(0), Zero(0), Over(0), Carry(0)
{
	Y &= 0xffff;
	F &= 0xffff;
	B &= 0xf;
}

bool AM2901::Compute(int D, int I, int A, int B, bool Cn,
	bool &RAM0, bool &RAM15, bool &Q0, bool &Q15)
{
	int R = 0, S = 0, tmp;
	bool changed = 0, btmp;
	this->B = B;
	Modify_Q = 0;
	Modify_B = 1;

	//根据 I2-0 选择操作数
	switch (getbits(I, 2, 0))
	{
	case 0: R = Register[A].Y; S = Q.Y;	break;
	case 1: R = Register[A].Y; S = Register[B].Y; break;
	case 2: R = 0; S = Q.Y; break;
	case 3: R = 0; S = Register[B].Y; break;
	case 4: R = 0; S = Register[A].Y; break;
	case 5: R = D; S = Register[A].Y; break;
	case 6: R = D; S = Q.Y; break;
	case 7: R = D; S = 0;
	}

	btmp = Over;
	Over = 0;
	//根据 I5-3 选择运算类型
	switch (getbits(I, 5, 3))
	{
	case 0:
		F = R + S + Cn;
		if (R <= 32767 && S <= 32767 && F > 32767) Over = 1;
		break;
	case 1:
		F = S - R - Cn;
		if (F < 0) F += 65536;
		if (S > 32767 && R > 0 && F <= 32767) Over = 1;
		break;
	case 2:
		F = R - S - Cn;
		if (F < 0) F += 65536;
		if (R > 32767 && S > 0 && F <= 32767) Over = 1;
		break;
	case 3: F = R | S; break;
	case 4: F = R & S; break;
	case 5: F = R & S; break;
	case 6: F = R ^ S; break;
	case 7: F = ~(R ^ S) & 0xffff;
	}
	if (btmp != Over) changed = 1;

	btmp = Carry;
	Carry = getbit(F, 16);
	setbit(F, 16, 0);
	if (btmp != Carry) changed = 1;

	btmp = Zero;
	Zero = (F==0);
	if (btmp != Zero) changed = 1;

	btmp = Sign;
	Sign = getbit(F, 15);
	if (btmp != Sign) changed = 1;

	//根据 I8-6 选择结果处理方式
	switch (getbits(I, 8, 6))
	{
	case 0:
		Q.Input(F); 
		Modify_Q = 1;
		Modify_B = 0;
		break;
	case 1: Modify_B = 0; break;
	case 2: Register[B].Input(F); break;
	case 3: Register[B].Input(F); break;
	case 4:
		RAM0 = getbit(F, 0);
		tmp = F>>1;
		setbit(tmp, 15, RAM15);
		Register[B].Input(tmp);

		Q0 = getbit(Q.Y, 0);
		tmp = Q.Y>>1;
		setbit(tmp, 15, Q15);
		Q.Input(tmp); Modify_Q = 1;
		break;
	case 5:
		RAM0 = getbit(F, 0);
		tmp = F>>1;
		setbit(tmp, 15, RAM15);
		Register[B].Input(tmp);
		break;
	case 6:
		RAM15 = getbit(F, 15);
		tmp = F;
		setbit(tmp, 15, 0);
		tmp <<= 1;
		setbit(tmp, 0, RAM0);
		Register[B].Input(tmp);

		Q15 = getbit(Q.Y, 15);
		tmp = Q.Y;
		setbit(tmp, 15, 0);
		tmp <<= 1;
		setbit(tmp, 0, Q0);
		Q.Input(tmp); Modify_Q = 1;
		break;
	case 7:
		RAM15 = getbit(F, 15);
		tmp = F;
		setbit(tmp, 15, 0);
		tmp <<= 1;
		setbit(tmp, 0, RAM0);
		Register[B].Input(tmp);
	}

	tmp = Y;
	if (getbits(I, 8, 6) == 2) Y = Register[A].Y;
	else Y = F;
	if (tmp != Y) changed = 1;

	return changed;
}

void AM2901::Update()
{
	if (Modify_B) Register[B].Update();
	if (Modify_Q) Q.Update();
}

/****************************************************************/

AM2910::AM2910()
{
	int i;
	uPC.Input(0); uPC.Update();
	uSP.Input(-1); uSP.Update();
	PL = 0; MAP = VECT = 1;
	for (i=0; i<5; i++)
	{
		uStack[i].Input(0);
		uStack[i].Update();
	}
}

void AM2910::Combine(int D, int I, bool CC, int CI)
{
	//int tmp = PL<<2 | MAP<<1 | VECT;
	//PL = MAP = VECT = 1;
	switch (I)
	{
	case 0: Y = 0; uSP.Y = -1; break;
	case 1:
		if (CC) Y = uPC.Y;
		else
		{
			Y = D;
			uStack[uSP.Y+1].Input(uPC.Y);
			uSP.Input(uSP.Y+1);
		}
		break;
	case 2: Y = D; break;
	case 3: Y = (CC ? uPC.Y:D); break;
	case 4:
		if (!CC) R.Input(D);
		Y = uPC.Y;
		uStack[uSP.Y+1].Input(uPC.Y);
		uSP.Input(uSP.Y+1);
		break;
	case 6: Y = (CC ? uPC.Y:D); break;
	case 8:
		if (R.Y != 0)
		{
			R.Input(R.Y-1);
			Y = uStack[uSP.Y].Y;
		}
		else
		{
			Y = uPC.Y;
			uSP.Input(uSP.Y-1);
		}
		break;
	case 10:
		if (CC) Y = uPC.Y;
		else
		{
			Y = uStack[uSP.Y].Y;
			uSP.Input(uSP.Y-1);
		}
		break;
	case 14: Y = uPC.Y; break;
	case 15:
		if (R.Y != 0)
		{
			R.Input(R.Y-1);
			if (CC) Y = uStack[uSP.Y].Y;
			else
			{
				Y = uPC.Y;
				uSP.Input(uSP.Y-1);
			}
		}
		else
		{
			Y = (CC ? D:uPC.Y);
			uSP.Input(uSP.Y-1);
		}
		break;
	}
	uPC.Input(Y+CI);
	//return tmp != (PL<<2 | MAP<<1 | VECT);
}

void AM2910::Schedule()
{
	uPC.Update();
	uSP.Update();
	if (uSP.Y >= 0) uStack[uSP.Y].Update();
	R.Update();
}
