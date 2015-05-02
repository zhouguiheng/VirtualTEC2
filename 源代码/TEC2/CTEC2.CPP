#include "stdafx.h"
#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include "ctec2.h"
#include "cbit.h"

CTEC2::CTEC2():A(0), B(0), INT(0), FS(0), Online(0)
{
	Flags.Input(0xf); Flags.Update();
	IB &= 0xffff; AB &= 0xffff;
	D &= 0xffff;
	for (int i=0; i<65536; i++) MEM[i] &= 0xffff;
}

void CTEC2::Offline(int D, int SW2, int SW1, int CK)
{
	int B1, B0;
	B1 = 1<<15 | getbits(SW2, 11, 9)<<12 | getbits(SW2, 8, 6)<<8 |
		 1<<7 | getbits(SW2, 5, 3)<<4 | getbits(SW1, 11, 8);
	B0 = getbits(SW1, 7, 0)<<8;
	SetStatus(8, D, 0, 1, B1, B0);
	Run(CK);
}

bool CTEC2::Cont()
{
	int start = clock();
	do
	{
		if (clock() - start > 2000) return false;
		Run(1);
	} while (MCTL.Y != 0x84);		//RET
	return true;
}

void CTEC2::Step()
{
	Run(1);
	do
	{
		Run(1);
	} while (MCTL.Y != 0xa4);
}
/*
void CTEC2::Step2()
{
	Run(1);
	do
	{
		Run(1);
		if (MCTL.Y == 0x74)		//CALL SR
		{
			Cont();
			SetStatus(5, 0, 0x29, 0x0300, 0x90F0, 0);
			Run(1);
		}
	} while (MCTL.Y != 0xa4);
}
*/
//B3, B2, B1, B0 分别是微指令中的 63-48, 47-32, 31-16, 15-0 位
void CTEC2::SetStatus(int FS, int D, int B3, int B2, int B1, int B0)
{
	int MI876, MI543, MI210, MI0, REQ, WE;

	this->FS = FS; this->D = D;
	this->B3 = B3; this->B2 = B2;
	this->B1 = B1; this->B0 = B0;
	PLR[0].Y = B0; PLR[1].Y = B1;
	PLR[2].Y = B2; PLR[3].Y = B3;
	Online = !getbit(FS, 3);
	MRA = getbits(B3, 7, 0)<<2 | getbits(B2, 15, 14);
	CI = getbits(B2, 11, 8);
	SCC = getbits(B2, 7, 5);
	SC = getbit(B2, 4);
	SST = getbits(B2, 2, 0);
	MI0 = getbit(B1, 15);
	REQ = getbit(B1, 11);
	WE = getbit(B1, 7);
	MRW = MI0<<2 | REQ<<1 | WE;
	MI876 = getbits(B1, 14, 12);
	MI543 = getbits(B1, 10, 8);
	MI210 = getbits(B1, 6, 4);
	MI = MI876<<6 | MI543<<3 | MI210;
	A = getbits(B1, 3, 0);
	B = getbits(B0, 15, 12);
	SCI = getbits(B0, 11, 10);
	SSH = getbits(B0, 9, 8);
	SA = getbit(B0, 7);
	SB = getbit(B0, 3);
	DC1 = getbits(B0, 6, 4);
	DC2 = getbits(B0, 2, 0);
	//SW1 = A<<8 | B<<4 | SCI<<2 | SSH;
	//SW2 = MI<<3 | SST;
}

void CTEC2::Run(int CK)
{
	RunCombine();
	if (CK)
	{
		RunSchedule();
		//RunCombine();
	}
}

bool CTEC2::LDMC_RESET()
{
	if (!LoadMCM() || !LoadMapRom()) return 0;
	PLR[2].Input(0);
	PLR[2].Update();
	CI = 0;
	return 1;
}

//更新组合电路状态
void CTEC2::RunCombine()
{
	bool changed;
	int AA, BB;
	if (Online) SetStatus(FS, D, PLR[3].Y, PLR[2].Y, PLR[1].Y, PLR[0].Y);
	AA = (SA ? getbits(IR.Y, 3, 0):A);
	BB = (SB ? getbits(IR.Y, 7, 4):B);
	do
	{
		changed = 
		ProcSCI() |
		ProcSSH() |
		ALU.Compute(IB, MI, AA, BB, Cin, RAM0, RAM15, Q0, Q15) |
		ProcMRW() |
		ProcDC1();
	} while (changed);
	ProcSST();
	ProcDC2();

	if (Online)
	{
		GenerateCC();
		MCTL.Combine(SelectD(CI), CI, CC, 1);
		SetPLR();
	}
}

//触发时序电路
void CTEC2::RunSchedule()
{
	int i;
	ALU.Update();
	Flags.Update();
	AR.Update();
	AB = AR.Y;
	IR.Update();
	if (Online)
	{
		MCTL.Schedule();
		for (i=0; i<4; i++)
		{
			PLR[i].Update();
			LDR[i].Update();
		}
	}
}

//加载微码到控存
bool CTEC2::LoadMCM()
{
	FILE *f = fopen("MCR.ROM", "r");
	char line[1024];
	int i, j, p=0, data;
	if (f == NULL) return 0;
	while (fgets(line, 1024, f) != NULL)
	{
		j = 0;
		for (i=0; i<4; i++)
		{
			data = 0;
			while (isdigit(line[j]) || (line[j]>='A' && line[j]<='F'))
			{
				line[j] = char(toupper(line[j]));
				data <<= 4;
				data += (isdigit(line[j]) ? line[j]-'0':line[j]-'A'+10);
				j++;
			}
			j++;
			MCM[p++] = data;
		}
	}
	fclose(f);
	return 1;
}

bool CTEC2::LoadMapRom()
{
	FILE *f = fopen("MAPROM.ROM", "r");
	int data, p=0, i;
	char temp[10];
	if (f == NULL) return 0;
	while (fgets(temp, 10, f) != NULL)
	{
		data = i = 0;
		while (isdigit(temp[i]) || (temp[i]>='A' && temp[i]<='F'))
		{
			data <<= 4;
			data += (isdigit(temp[i]) ? temp[i]-'0':temp[i]-'A'+10);
			i++;
		}
		MapRom[p++] = data;
	}
	fclose(f);
	return 1;
}

int CTEC2::ReadFile(char *filename)
{
	FILE *f = fopen(filename, "rb");
	int data, addr, n, bytes;
	unsigned char high, low;
	if (f == NULL) return -1;
	bytes = 5;
	fseek(f, 1, 0);
	fscanf(f, "%c%c", &low, &high);
	addr = high<<8 | low;
	fscanf(f, "%c%c", &low, &high);
	n = high<<8 | low;
	while (n--)
	{
		fscanf(f, "%c%c", &high, &low);
		bytes += 2;
		data = high<<8 | low;
		MEM[addr++] = data;
	}
	fclose(f);
	return bytes;
}

int CTEC2::WriteFile(char *filename)
{
	FILE *f = fopen(filename, "wb");
	int i, addr, n, bytes;
	unsigned char high, low;
	if (f == NULL) return -1;
	addr = MEM[0x0F80];
	n = MEM[0x0F81];
	bytes = 5;
	fprintf(f, "%c", 1);
	high = getbits(addr, 15, 8); low = getbits(addr, 7, 0);
	fprintf(f, "%c%c", low, high);
	high = getbits(n, 15, 8); low = getbits(n, 7, 0);
	fprintf(f, "%c%c", low, high);
	for (i=0; i<n && addr<65536; i++)
	{
		high = getbits(MEM[addr], 15, 8);
		low = getbits(MEM[addr], 7, 0);
		fprintf(f, "%c%c", high, low);
		addr++;
		bytes += 2;
	}
	fclose(f);
	return bytes;
}

void CTEC2::SetLED(int S)
{
	int i;
	switch (S)
	{
	case 0:
		LED = 0;
		for (i=0; i<4; i++) setbit(LED, i, getbit(Flags.Y, 7-i));
		LED |= (getbits(Flags.Y, 3, 0)<<4 | getbits(B0, 7, 0)<<8);
		break;
	case 1:
		LED = getbits(B1, 7, 0)<<8 | getbits(B0, 15, 8);
		break;
	case 2:
		LED = getbits(B2, 7, 0)<<8 | getbits(B1, 15, 8);
		break;
	case 3:
		LED = getbits(B3, 7, 0)<<8 | getbits(B2, 15, 8);
		break;
	case 4: LED = MCTL.Y; break;
	case 5:	LED = AB; break;
	case 6:	LED = ALU.Y; break;
	case 7:	LED = IB;
	}
}

//AM2901最低位进位
bool CTEC2::ProcSCI()
{
	bool orgCin = Cin;
	switch (SCI)
	{
	case 0: Cin = 0; break;
	case 1: Cin = 1; break;
	case 2: Cin = getbit(Flags.Y, 7); break;
	case 3: Cin = 0;				//进位为方波，待完善
	}
	return orgCin != Cin;
}

//移位控制
bool CTEC2::ProcSSH()
{
	bool tRAM0=RAM0, tRAM15=RAM15, tQ0=Q0, tQ15=Q15;
	bool shl = (getbits(MI, 8, 6)>5 ? 1:0);
	switch (SSH)
	{
	case 0: (shl ? RAM0:RAM15) = 0; break;
	case 1: (shl ? RAM0:RAM15) = getbit(Flags.Y, 7); break;
	case 2:
		if (shl) Q0 = !ALU.Sign, RAM0 = Q15;
		else Q15 = RAM0, RAM15 = ALU.Carry;
		break;
	case 3:
		if (!shl)
		{
			RAM15 = ALU.Sign ^ ALU.Over;
			Q15 = RAM0;
		}
	}
	return (tRAM0!=RAM0 || tRAM15!=RAM15 || tQ0!=Q0 || tQ15!=Q15);
}

//状态位产生
void CTEC2::ProcSST()
{
	int f = Flags.Y;
	switch (SST)
	{
	case 0: break;
	case 1:
		f = getbits(f, 3, 0) |
			int(ALU.Sign)<<4 |
			int(ALU.Over)<<5 |
			int(ALU.Zero)<<6 |
			int(ALU.Carry)<<7;
		break;
	case 2:
		f = getbits(IB, 7, 4)<<4 | getbits(f, 3, 0);
		break;
	case 3: setbit(f, 7, 0); break;
	case 4: setbit(f, 7, 1); break;
	case 5: setbit(f, 7, RAM0); break;
	case 6: setbit(f, 7, RAM15); break;
	case 7: setbit(f, 7, Q0);
	}
	Flags.Input(f);
}

//控制内存外设读写及微码装入
bool CTEC2::ProcMRW()
{
	bool changed = 0;
	switch (MRW)
	{
	case 0: MEM[AB] = IB; break;		//存储器写
	case 1:								//存储器读
		if (IB != MEM[AB]) changed = 1, IB = MEM[AB];
		break;
	case 2: break;		//IO写
	case 3: break;		//IO读
	case 4:
	case 5:	break;		//不操作
	case 6:
	case 7:				//装入微码
		for (int i=3; i>=0; i--) MCM[(AB<<2)+3-i] = LDR[i].Y;
		break;
	}
	return changed;
}

//向IB总线发送控制
bool CTEC2::ProcDC1()
{
	int orgIB = IB;
	switch (DC1)
	{
	case 0:							//SWTOIB, 开关手拔数据
		if (MRW != 1 && MRW != 3) IB = D;
		break;
	case 1: IB = ALU.Y; break;		//RTOIB, 运算器的输出
	case 2: IB = getbits(IR.Y, 7, 0); break;	//ITOIB, 指令的低8位
	case 3: IB = Flags.Y; break;	//FTOIB, 状态寄存器
	case 4: break;					//INTA, 中断向量	待加！！
	case 5: break;					//NC, 未使用
	case 6:	break;					//EI, 转用于开中断	待加！！
	case 7:	break;					//DI, 转用于开中断	待加！！
	}
	return orgIB != IB;
}

//寄存器接收控制
void CTEC2::ProcDC2()
{
	switch (DC2)
	{
	case 0: break;						//NC, 未使用
	case 1: IR.Input(IB); break;		//GIR, 指令寄存器IR
	//case 2: AR.Input(IB); break;		//GAR, 地址寄存器AR
	case 2: AR.Input(ALU.Y); break;
	case 3:	break;						//GINTP, 中断优先级
	case 4: LDR[0].Input(IB); break;
	case 5: LDR[1].Input(IB); break;
	case 6: LDR[2].Input(IB); break;
	case 7: LDR[3].Input(IB);
	}
}

//CC条件码形成
void CTEC2::GenerateCC()
{
	switch (SCC)
	{
	case 0: CC = 0; break;
	case 1: CC = 1; break;
	case 2: CC = !(SC ? getbit(Flags.Y, 7):getbit(FS, 0)); break;
	case 3: CC = !(SC ? getbit(Flags.Y, 6):getbit(FS, 1)); break;
	case 4: CC = !(SC ? getbit(Flags.Y, 5):getbit(FS, 2)); break;
	case 5: CC = !(SC ? getbit(Flags.Y, 4):WAIT); break;
	case 6: CC = !INT; break;
	case 7:
		switch (getbits(IR.Y, 9, 8))
		{
		case 0: CC = getbit(Flags.Y, 7); break;
		case 1: CC = getbit(Flags.Y, 6); break;
		case 2: CC = getbit(Flags.Y, 5); break;
		case 3: CC = getbit(Flags.Y, 4); break;
		}
		if (getbit(IR.Y, 10)==0) CC = !CC;
	}
}

int CTEC2::SelectD(int CI)
{
	if (CI==2) return MapRom[getbits(IR.Y, 15, 10)];
	else if (CI==6) return D;
	else return MRA;
	//if (!MCTL.PL) return MRA;
	//if (!MCTL.MAP) return MapRom[getbits(IR.Y, 15, 10)];
	//return D;
}

void CTEC2::SetPLR()
{
 	int i, p = MCTL.Y << 2;
	for (i=3; i>=0; i--) PLR[i].Input(MCM[p++]);
}


int CTEC2::GetY() { return ALU.Y; }

int CTEC2::GetFlags() { return Flags.Y; }

void CTEC2::GetReg(int *R)
{
	int i;
	for (i=0; i<16; i++) R[i] = ALU.Register[i].Y;
}

void CTEC2::SetReg(int R, int value)
{
	ALU.Register[R].Input(value);
	ALU.Register[R].Update();
}
