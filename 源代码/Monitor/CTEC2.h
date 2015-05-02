#ifndef _CTEC2_H
#define _CTEC2_H

#include "cchip.h"

class CTEC2
{
public:
	int LED;			//��ʾ�Ƶ�״̬
	int MEM[65536];		//����
	AM2901 ALU;
	AM2910 MCTL;
	REG PLR[4];			//΢ָ��Ĵ���
	int bp;				//�ϵ�
	bool bping;

	CTEC2();
	void Offline(int D, int SW2, int SW1, int CK);
	bool Cont(int addr = -1);
	void Step();
	void Step2();

	//B3, B2, B1, B0 �ֱ���΢ָ���е� 63-48, 47-32, 31-16, 15-0 λ
	void SetStatus(int FS, int D, int B3, int B2, int B1, int B0);
	void SetLED(int S);
	int ReadFile(char *filename);
	int WriteFile(char *filename, int addr, int n);
	void SaveMCM();

	bool LDMC_RESET();
	void Run(int CK);

	int GetY();
	int GetFlags();
	void GetReg(int *R);
	void SetReg(int R, int value);

//private:

	REG Flags;			//��־�Ĵ������Ӹߵ���λ����Ϊ��C,Z,V,S,INTE,P2,P1,P0
	REG AR;				//��ַ�Ĵ���
	REG IR;				//ָ��Ĵ���
	REG LDR[4];

	int IB;				//�ڲ�����
	int AB;				//��ַ����
	int D;				//�ֶ���������
	int FS;
	bool Cin;			//��λ����
	bool RAM0, RAM15;
	bool Q0, Q15;
	int MCM[4096];		//�ش�
	int MapRom[64];
	bool WAIT, INT;
	bool Online;

	int B3, B2, B1, B0;
	int MRA;			//�µ�ַ
	int CI;				//AM2910����
	int SCC;			//AM2910������
	int SC;				//AM2910������
	bool CC;			//AM2910������
	int SST;			//״̬λ����
	int MRW;			//�����ڴ������д��΢��װ��
	int MI;				//AM2901������
	int A;				//A�ڵ�ַ
	int B;				//B�ڵ�ַ
	int SCI;			//AM2901���λ��λ
	int SSH;			//��λ����
	int SA;
	int SB;
	int DC1;			//��IB���߷��Ϳ���
	int DC2;			//�Ĵ������տ���
	int SW1;			//΢�Ϳ���SW1
	int SW2;			//΢�Ϳ���SW2

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
