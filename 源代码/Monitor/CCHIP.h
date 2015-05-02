#ifndef _CCHIP_H
#define _CCHIP_H


//16λ�Ĵ���
class REG
{
public:
	REG();
	void Update();
	void Input(int D);
	int Y;			//�����
private:
	int D;
};

/****************************************************************/

//AM2901������(�����AM2901ֱ�������16λ)
class AM2901
{
public:
	int Y;							//�����ͳ��������ź�
	bool Sign, Zero, Over, Carry;	//״̬��־λ
	REG Register[16];				//�Ĵ�����

	AM2901();
	bool Compute(int D, int I, int A, int B, bool Cn,
		bool &RAM0, bool &RAM15, bool &Q0, bool &Q15);
	void Update();

private:
	REG Q;							//Q�Ĵ���
	int F;							//������
	bool Modify_Q, Modify_B;
	int B;
};

/****************************************************************/

//AM2910������
class AM2910
{
public:
	int Y;		//�µ�ַ
	bool FULL;	//΢��ջ���źţ��͵�ƽ��Ч
	bool PL, MAP, VECT;

	AM2910();
	void Combine(int D, int I, bool CC, int CI);
	void Schedule();

private:
	REG uPC, uSP, uStack[5];
	REG R;		//�Ĵ���/������
};

#endif
