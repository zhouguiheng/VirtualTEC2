#ifndef _CTEC2_H
#define _CTEC2_H

#include "cchip.h"

class CTEC2
{
public:
	int LED;			//显示灯的状态
	int MEM[65536];		//主存

	CTEC2();
	void Offline(int D, int SW2, int SW1, int CK);
	bool Cont();
	void Step();

	//B3, B2, B1, B0 分别是微指令中的 63-48, 47-32, 31-16, 15-0 位
	void SetStatus(int FS, int D, int B3, int B2, int B1, int B0);
	void SetLED(int S);
	int ReadFile(char *filename);
	int WriteFile(char *filename);

	bool LDMC_RESET();
	void Run(int CK);

	int GetY();
	int GetFlags();
	void GetReg(int *R);
	void SetReg(int R, int value);

private:
	AM2901 ALU;
	AM2910 MCTL;

	REG Flags;			//标志寄存器，从高到低位依次为：C,Z,V,S,INTE,P2,P1,P0
	REG AR;				//地址寄存器
	REG IR;				//指令寄存器
	REG PLR[4];			//微指令寄存器
	REG LDR[4];

	int IB;				//内部总线
	int AB;				//地址总线
	int D;				//手动开关输入
	int FS;
	bool Cin;			//进位输入
	bool RAM0, RAM15;
	bool Q0, Q15;
	int MCM[2048];		//控存
	int MapRom[64];
	bool WAIT, INT;
	bool Online;

	int B3, B2, B1, B0;
	int MRA;			//下地址
	int CI;				//AM2910命令
	int SCC;			//AM2910条件码
	int SC;				//AM2910条件码
	bool CC;			//AM2910条件码
	int SST;			//状态位产生
	int MRW;			//控制内存外设读写及微码装入
	int MI;				//AM2901控制码
	int A;				//A口地址
	int B;				//B口地址
	int SCI;			//AM2901最低位进位
	int SSH;			//移位控制
	int SA;
	int SB;
	int DC1;			//向IB总线发送控制
	int DC2;			//寄存器接收控制
	int SW1;			//微型开关SW1
	int SW2;			//微型开关SW2

	bool ProcSCI();
	bool ProcSSH();
	void ProcSST();
	bool ProcMRW();
	bool ProcDC1();
	void ProcDC2();

	void GenerateCC();
	int SelectD(int CI);
	void SetPLR();

	bool LoadMCM();
	bool LoadMapRom();

	void RunCombine();
	void RunSchedule();
};

#endif
