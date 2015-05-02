#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <conio.h>
#include <windows.h>
#include "ctec2.h"
#include "cbit.h"

CTEC2 tec2;
char str[1000];
char inst[64][20];
int num;
HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
bool os;		//true时为Windows_NT
bool db;

//0: 经典怀旧 1:黑白分明 2: 白纸黑字 3: 蓝天白云 4: 春意盎然
WORD color[5][2] = {{0x7, 0x17}, {0xf, 0x9f}, {0xf0, 0x70},
					{0x1f, 0x79}, {0x3f, 0x2f}};
int selcolor;

void init();
void ProcStr(char *str);
int HexValue(char *str, char end=0);
int GetReg(char *str, char end=0);
void InputHex(char *str, int len=4);

void clrscr();
void Prompt();
void ErrCmd();
void Scroll();
int ViewInst(int addr);
void ViewStatus();
void ReplaceCND(char *str);

void CmdA();
void CmdB();
void CmdU();
void CmdG();
void CmdT();
void CmdP();
void CmdR(bool u = true);
void CmdC();
void CmdD();
void CmdE();
void CmdH();
void CmdS();
void CmdTT();
void CmdHelp();
void OnF10();

int main(int argc, char *argv[])
{
	unsigned char c, c2;
	int i;

	selcolor = 3;
	if (argc==2)
	{
		if (argv[1][0]>='0' && argv[1][0]<='4') selcolor = argv[1][0]-'0';
	}

	LPTSTR lpszSystemInfo;     // pointer to system information string 
	TCHAR tchBuffer[256];
	lpszSystemInfo = tchBuffer;
	ExpandEnvironmentStrings("%OS%", lpszSystemInfo, 256);
	os = (strstr(lpszSystemInfo, "NT") != NULL);
	if (!os) selcolor = 0;

	SetConsoleTextAttribute(hout, color[selcolor][0]);

	init();

	printf("TEC-2 CRT MONITOR\nVersion 1.2 ,Jun.2005\n\n");
	Prompt();

	while (1)
	{
		printf(">");
		i = 0;
		while (c = getch(), c!='\r')
		{
			if (c==0 || c==0xE0)
			{
				c2 = getch();
				if (c==0 && c2==68)
				{
					OnF10(); i = 0; break;
				}
			}
			if (i > 77) continue;
			if (c=='\b' && i>0)
			{
				printf("\b \b");
				i--;
				continue;
			}
			if (isgraph(c) || c==' ')
			{
				str[i++] = c;
				putchar(c);
			}
		}
		str[i] = 0;
		printf("\n");
		Scroll();
		ProcStr(str);
		if (str[1] && (str[0]=='S' || str[0]=='Q' || str[0]=='C'))
		{
			ErrCmd();
			continue;
		}
		switch (str[0])
		{
		case 0: break;
		case 'A': CmdA(); break;
		case 'U': CmdU(); break;
		case 'G': CmdG(); break;
		case 'T':
			if (str[1]=='T') CmdTT();
			else CmdT();
			break;
		case 'P': CmdP(); break;
		case 'R': CmdR(); break;
		case 'C': CmdC(); break;
		case 'D': CmdD(); break;
		case 'E': CmdE(); break;
		case 'S': CmdS(); break;
		case '?': CmdHelp(); break;
		case 'H': CmdH(); break;
		case 'Q': clrscr(); return 1;
		case 'V': ViewStatus(); break;
		case 'B': CmdB(); break;
		default: ErrCmd();
		}
	}
	return 0;
}

void init()
{
	COORD size = {80, 25};
	SetConsoleTitle("TEC-2 实验系统监控程序");
	SetConsoleScreenBufferSize(hout,size);

	clrscr();

	FILE *f = fopen("INST.ROM", "r");
	int i = 0, j;
	if (f == NULL)
	{
		MessageBox(NULL, "初使化错误! 找不到文件INST.ROM!", "错误", 0);
		exit(2);
	}
	fscanf(f, "%d", &num);
	fgets(inst[0], 20, f);
	for (i=0; i<num; i++)
	{
		fgets(inst[i], 20, f);
		j = strlen(inst[i])-1;
		while (inst[i][j]=='\n' || inst[i][j]=='\r') inst[i][j--]=0;
	}
	fclose(f);
	if (!tec2.LDMC_RESET())
	{
		MessageBox(NULL, "初始化错误! 请确认所在目录下有文件 MCM.ROM 和 MAPROM.ROM", "错误", 0);
		exit(2);
	};

	db = 0;
	tec2.SetReg(4, 0xffff);
}

int seeki(char *ins)
{
	int i = 0;
	while (i<num && strcmp(ins, inst[i])!=0) i++;
	return (i<num ? i: -1);
}

void ProcStr(char *str)
{
	int i, j, pre, len;
	len = strlen(str);
	for (i=0; i<len; i++) if (str[i] == '\t') str[i] = ' ';
	i = pre = 0;
	//多个空格合并为一个
	while (i<len)
	{
		while (i<len && str[i]==' ') i++;
		for (j=i; j<=len; j++) str[j-i+pre] = str[j];
		len -= (i-pre);
		i = pre;
		while (i<len && str[i]!=' ') i++;
		i++;
		pre = i;
	}
	if (str[len-1]==' ') str[--len] = 0;		//去末尾空格
	if (str[1]==' ')		//去命令后空格
	{
		for (i=2; i<=len; i++) str[i-1] = str[i];
		len--;
	}
	i = 0; while (i<len && str[i]!=',') i++;
	if (i<len)
	{
		if (i>0 && str[i-1]==' ')
		{
			j = i--;
			while (j <= len) str[j-1] = str[j], j++;
			len--;
		}
		if (i<len-1 && str[i+1]==' ')
		{
			j = i + 2;
			while (j <= len) str[j-1] = str[j], j++;
			len--;
		}
	}
	for (i=0; i<len; i++) str[i] = toupper(str[i]);
}

int HexValue(char *str, char end)
{
	bool valid = true;
	int ret = 0;
	if (*str == end) valid = false;
	while (*str != end && valid)
	{
		if (isdigit(*str) || (*str>='A' && *str<='F'))
		{
			ret <<= 4;
			ret += (isdigit(*str) ? *str-'0' : *str-'A'+10);
			if (ret > 65535) valid = false;
		}
		else valid = false;
		str++;
	}
	return (valid ? ret : -1);
}

void InputHex(char *str, int len)
{
	char c;
	int j = 0;
	while (c = getch(), c!='\r')
	{
		if (c=='\b')
		{
			if (j > 0)
			{
				printf("\b \b");
				j--;
			}
		}
		else if (j<len && (isdigit(c) || (c>='a' && c<='f') || (c>='A' && c<='F')))
		{
			putchar(c);
			str[j++] = c;
		}
	}
	str[j] = 0;
	ProcStr(str);
	printf("\n"); Scroll();
}

int GetReg(char *str, char end)
{
	int n;
	n = -1;
	if (str[0]=='R')
	{
		if (isdigit(str[1]) && str[2]==end) n = str[1] - '0';
		else if (str[1]=='1' && str[2]>='0' && str[2]<='5' && str[3]==end)
		{
			n = 10 + str[2] - '0';
		}
	}
	else if (str[1] && str[2]==end)
	{
		if (str[0]=='S' && str[1]=='P') n = 4;
		else if (str[0]=='P' && str[1]=='C') n = 5;
		else if (str[0]=='I' && str[1]=='P') n = 6;
	}
	return n;
}

void clrscr()
{
	COORD pos = {0, 0};
	CONSOLE_SCREEN_BUFFER_INFO bInfo; // 窗口信息
	GetConsoleScreenBufferInfo(hout, &bInfo );
	//FillConsoleOutputCharacter(hout, ' ', bInfo.dwSize.X * bInfo.dwSize.Y, pos, NULL);//Win98 不兼容！
	SetConsoleCursorPosition(hout, pos);
	int i = bInfo.dwSize.X * bInfo.dwSize.Y;
	for (; i>0; i--) putchar(' ');
	SetConsoleCursorPosition(hout, pos);
}

void ErrCmd()
{
	printf("Unknown command! Type ? for help.\n");
	Scroll();
}

void Prompt()
{
	COORD pos = {1, 24};
	CONSOLE_SCREEN_BUFFER_INFO bInfo; // 窗口缓冲区信息
	char str[] =  " Monitor of Virtual TEC-2 By Guiheng Zhou, Jun. 2005, Sun Yat-sen University  ";

	GetConsoleScreenBufferInfo( hout, &bInfo ); // 获取窗口缓冲区信息
	SetConsoleTextAttribute(hout, color[selcolor][1]);
	SetConsoleCursorPosition(hout, pos);
	//WriteConsole(hout, str, 78, NULL, NULL);	//Win98 不兼容！
	printf("%s", str);
	SetConsoleTextAttribute(hout, bInfo.wAttributes);
	SetConsoleCursorPosition(hout, bInfo.dwCursorPosition);
}

void Scroll()
{
	SMALL_RECT rcScroll;
	COORD crDest = {0, -1};
	CHAR_INFO chFill;
	CONSOLE_SCREEN_BUFFER_INFO bInfo; // 窗口缓冲区信息

	GetConsoleScreenBufferInfo( hout, &bInfo ); // 获取窗口缓冲区信息
	if (bInfo.dwCursorPosition.Y >= bInfo.dwSize.Y - 1)
	{
		rcScroll.Left = rcScroll.Top = 0;
		rcScroll.Right = bInfo.dwSize.X - 1;
		rcScroll.Bottom = bInfo.dwSize.Y - 2;
		chFill.Attributes = bInfo.wAttributes;
		chFill.Char.AsciiChar = ' ';
		ScrollConsoleScreenBuffer(hout, &rcScroll, &rcScroll, crDest, &chFill);
		bInfo.dwCursorPosition.Y = bInfo.dwSize.Y - 2;
		SetConsoleCursorPosition(hout, bInfo.dwCursorPosition);
	}
}

int ViewInst(int addr)
{
	int j, len, op, ret = 1, r;
	printf("%04X: ", addr);
	printf("%04X ", tec2.MEM[addr]);
	if ((op=getbits(tec2.MEM[addr], 15, 10)) < num)
	{
		if ((strstr(inst[op], "ADR")!=NULL && strncmp("JR", inst[op], 2)!=0)
			|| strstr(inst[op], "DATA")!=NULL)
		{
			printf("%04X  ", tec2.MEM[addr+1]);
			ret = 2;
		} else printf("      ");
		len = strlen(inst[op]);
		j = 0;
		while (j<len && inst[op][j]!=' ') printf("%c", inst[op][j++]);
		while (j < len)
		{
			if (strncmp("DR", inst[op]+j, 2)==0)
			{
				r = getbits(tec2.MEM[addr], 7, 4);
				if (r == 4) printf("SP");
				else if (r == 5) printf("PC");
				else if (r == 6) printf("IP");
				else printf("R%d", r);
				j += 2;
			}
			else if (strncmp("SR", inst[op]+j, 2)==0)
			{
				r = getbits(tec2.MEM[addr], 3, 0);
				if (r == 4) printf("SP");
				else if (r == 5) printf("PC");
				else if (r == 6) printf("IP");
				else printf("R%d", r);
				j += 2;
			}
			else if (strncmp("ADR", inst[op]+j, 3)==0)
			{
				if (strncmp("JR", inst[op], 2)==0)
				{
					printf("%02X", getbits(tec2.MEM[addr], 7, 0));
				}
				else printf("%04X", tec2.MEM[addr+1]);
				j += 3;
			}
			else if (strncmp("DATA", inst[op]+j, 4)==0)
			{
				printf("%04X", tec2.MEM[addr+1]);
				j += 4;
			}
			else if (strncmp("PORT", inst[op]+j, 4)==0)
			{
				printf("%02X", getbits(tec2.MEM[addr], 7, 0));
				j += 4;
			}
			else if (strncmp("CND", inst[op]+j, 3)==0)
			{
				if (!getbit(tec2.MEM[addr], 10)) printf("N");
				switch (getbits(tec2.MEM[addr], 9, 8))
				{
				case 0: printf("C"); break;
				case 1: printf("Z"); break;
				case 2: printf("V"); break;
				case 3: printf("S");
				}
				j += 3;
			}
			else
			{
				if (inst[op][j] == ',') printf(",\t");
				else if (inst[op][j] == ' ') printf("\t");
				else printf("%c", inst[op][j]);
				j++;
			}
		}
		printf("\n"); Scroll();
	}
	else
	{
		printf("      DW     %04X\n", tec2.MEM[addr]); Scroll();
	}
	return ret;
}

void Replace(char *p, int size, int num)
{
	if (p == NULL) return;
	int i, len;
	len = strlen(p);
	for (i=size; i<=len; i++) p[i-size+1] = p[i];
	p[0] = char(num+'0');
}

void ReplaceCND(char *str)
{
	Replace(strstr(str, "NC,"), 2, 0);
	Replace(strstr(str, "NZ,"), 2, 1);
	Replace(strstr(str, "NV,"), 2, 2);
	Replace(strstr(str, "NS,"), 2, 3);
	Replace(strstr(str, "C,"), 1, 4);
	Replace(strstr(str, "Z,"), 1, 5);
	Replace(strstr(str, "V,"), 1, 6);
	Replace(strstr(str, "S,"), 1, 7);
}

void CmdA()
{
	int i, j, len, first, value1, value2, data;
	static int p = 0x800;
	int ir = 0, op, ir2;
	bool valid;
	if (str[1])
	{
		i = HexValue(str+1);
		if (i < 0)
		{
			printf("Error\n"); Scroll();
			return;
		}
		p = i;
	}
	printf("%04X: ", p);
	if (!os) getch();		//为了兼容Win98
	while (1)
	{
		gets(str);
		Scroll();
		ProcStr(str);
		ReplaceCND(str);
		ir2 = -1;
		valid = true;
		len = strlen(str);
		if (len == 0) return;
		i=0; while (i < len && str[i] != ' ') i++;
		if (i < len)
		{
			i++;
			first = i;
			if ((value1 = GetReg(str+first)) >= 0)
			{
				if (strncmp("MUL", str, 3)==0 || strncmp("DIV", str, 3)==0
					|| strncmp("JP", str, 2)==0 || strncmp("CALL", str, 4)==0)
				{
					strcpy(str+first, "SR");
					if ((op=seeki(str)) < 0) valid = false;
					ir = op<<10 | value1;
				}
				else
				{
					strcpy(str+first, "DR");
					if ((op=seeki(str)) < 0) valid = false;
					ir = op<<10 | value1<<4;
				}
			}
			else if ((value1=GetReg(str+first, ',')) >= 0)
			{
				i = first;
				while (str[i] != ',') i++;
				i++;
				if ((value2=GetReg(str+i)) >= 0)
				{
					strcpy(str+first, "DR,SR");
					if ((op=seeki(str)) < 0) valid = false;
					ir = op<<10 | value1<<4 | value2;
				}
				else if ((data=HexValue(str+i)) >= 0)
				{
					strcpy(str+first, "DR,DATA");
					if ((op=seeki(str)) < 0) valid = false;
					ir = op<<10 | value1<<4;
					ir2 = data;
				}
				else if ((data=HexValue(str+i, '[')) >= 0)
				{
					j = i;
					while (str[j] != '[') j++;
					if ((value2=GetReg(str+j+1, ']')) >= 0)
					{
						strcpy(str+first, "DR,DATA[SR]");
						if ((op=seeki(str)) < 0) valid = false;
						ir = op<<10 | value1<<4 | value2;
						ir2 = data;
					}
					else valid = false;
				}
				else if (str[i]=='[')
				{
					if ((value2=GetReg(str+i+1, ']'))>=0)
					{
						strcpy(str+first, "DR,[SR]");
						if ((op=seeki(str)) < 0) valid = false;
						ir = op<<10 | value1<<4 | value2;
					}
					else if ((data=HexValue(str+i+1, ']')) >= 0)
					{
						strcpy(str+first, "DR,[ADR]");
						if ((op=seeki(str)) < 0) valid = false;
						ir = op<<10 | value1<<4;
						ir2 = data;
					}
					else valid = false;
				}
				else valid = false;
			}
			else if ((data=HexValue(str+first, '[')) >= 0)
			{
				i = first;
				while (str[i] != '[') i++;
				if ((value2=GetReg(str+i+1, ']')) >= 0)
				{
					j = i+1;
					while (str[j] != ']') j++;
					if ((str[++j] == ',') && (value1=GetReg(str+j+1))>=0)
					{
						strcpy(str+first, "DATA[SR],DR");
						if ((op=seeki(str)) < 0) valid = false;
						ir = op<<10 | value1<<4 | value2;
						ir2 = data;
					}
					else valid = false;
				}
				else valid = false;
			}
			else if (str[first]=='[' && (value1=GetReg(str+first+1, ']'))>=0)
			{
				i = first;
				while (str[i] != ']') i++;
				if (str[++i] == ',' && (value2=GetReg(str+i+1))>=0)
				{
					strcpy(str+first, "[DR],SR");
					if ((op=seeki(str)) < 0) valid = false;
					ir = op<<10 | value1<<4 | value2;
				}
				else valid = false;
			}
			else if (str[first]=='[' && (data=HexValue(str+first+1, ']'))>=0)
			{
				i = first;
				while (str[i] != ']') i++;
				if (str[++i]==',' && (value2=GetReg(str+i+1))>=0)
				{
					strcpy(str+first, "[ADR],SR");
					if ((op=seeki(str)) < 0) valid = false;
					ir = op<<10 | value2;
					ir2 = data;
				}
			}
			else if ((data=HexValue(str+first)) >= 0)
			{
				if (strncmp("JP", str, 2)!=0 && data>=256) valid = false;
				else if (strncmp("IN", str, 2)==0 || strncmp("OUT", str, 3)==0)
				{
					strcpy(str+first, "PORT");
					if ((op=seeki(str)) < 0) valid = false;
					ir = op<<10 | data;
				}
				else if (strncmp("JP", str, 2)==0)
				{
					strcpy(str+first, "ADR");
					if ((op=seeki(str)) < 0) valid = false;
					ir = op<<10;
					ir2 = data;
				}
				else
				{
					strcpy(str+first, "ADR");
					if ((op=seeki(str)) < 0) valid = false;
					ir = op<<10 | data;
				}
			}
			else if ((data=HexValue(str+first, ',')) >= 0)
			{
				if (data >= 8) valid = false;
				else if ((value1=GetReg(str+first+2)) >= 0)
				{
					strcpy(str+first, "CND,SR");
					if ((op=seeki(str)) < 0) valid = false;
					ir = op<<10 | data<<8 | value1;
				}
				else if ((value1=HexValue(str+first+2)) >= 0)
				{
					strcpy(str+first, "CND,ADR");
					if ((op=seeki(str)) < 0) valid = false;
					ir = op<<10 | data<<8;
					ir2 = value1;
				}
				else valid = false;
			}
			else valid = false;
		}
		else if ((op=seeki(str)) >= 0)
		{
			ir = op<<10;
		}
		else valid = false;

		if (!valid) { printf("\tError\n"); Scroll(); }
		else
		{
			tec2.MEM[p++] = ir;
			if (ir2 >= 0) tec2.MEM[p++] = ir2;
		}
		printf("%04X: ", p);
	}
}

void CmdU()
{
	int i;
	static int p = 0x800;
	if (str[1])
	{
		if ((i=HexValue(str+1)) < 0)
		{
			printf("Error\n"); Scroll();
			return;
		}
		p = i;
	}
	for (i=0; i<16; i++)
	{
		p += ViewInst(p);
		p &= 0xffff;
	}
}

void CmdB()
{
	int i;
	if (str[1])
	{
		if ((i=HexValue(str+1)) < 0)
		{
			printf("Error\n"); Scroll();
			return;
		}
		tec2.bp = i;
	}
	else tec2.bp = -1;
}

void CmdG()
{
	int i;
	if (str[1])
	{
		if ((i=HexValue(str+1)) < 0)
		{
			printf("Error\n"); Scroll();
			return;
		}
		tec2.SetReg(5, i);
	}
	else
	{
		i = -1;
	}
	if (!tec2.Cont(i))
	{
		printf("Dead cycle!\n"); Scroll();
	}
}

void CmdTT()
{
	if (str[2])
	{
		printf("Error\n"); Scroll();
		return;
	}
	if (!db)
	{
		db = 1;
		tec2.SetStatus(5, 0, 0x29, 0x0300, 0x90f0, 0);
		while (tec2.MCTL.Y != 0x1a) tec2.Run(1);
	}
	tec2.Run(1);
	if (tec2.MCTL.Y == 0xa4)
	{
		db = 0;
		while (tec2.MCTL.Y != 0x1a) tec2.Run(1);
		tec2.Run(1);
	}
	ViewStatus();
	//ViewInst(tec2.ALU.Register[6].Y);
	str[1] = 0;
	CmdR(false);
}

void ViewStatus()
{
	int i;
	printf("Next: %03X ", tec2.MCTL.Y);
	for (i=3; i>=0; i--) printf("  %04X", tec2.PLR[i]);
	printf("\n"); Scroll();
	printf("Analyse: ");

	int B3, B2, B1, B0;
	int MI, MI876, MI543, MI210, MI0, REQ, WE;
	int MRA, CI, SCC, SC, SST, MRW, A, B, SCI, SSH, SA, SB, DC1, DC2;

	B0 = tec2.PLR[0].Y; B1 = tec2.PLR[1].Y;
	B2 = tec2.PLR[2].Y; B3 = tec2.PLR[3].Y;
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

	printf("AR=%04X  MEM=%04X  MRA=%03X  A=%02X  B=%02X\n", 
		tec2.AR.Y, tec2.MEM[tec2.AR.Y], MRA, A, B); Scroll();
	printf("C=%d  Z=%d  V=%d  S=%d\n", getbit(tec2.Flags.Y, 7),
		getbit(tec2.Flags.Y, 6), getbit(tec2.Flags.Y, 5), getbit(tec2.Flags.Y, 4)); Scroll();

}

void CmdT()
{
	tec2.Step();
	CmdR();
}

void CmdP()
{
	tec2.Step2();
	CmdR();
}

void CmdR(bool u)
{
	int i, n;
	int r[16], flags;
	tec2.GetReg(r);
	if (!str[1])
	{
		for (i=0; i<4; i++) printf("R%d=%04X  ", i, r[i]);
		printf("SP=%04X  PC=%04X  IP=%04X  ", r[4], r[5], r[6]);
		printf("R7=%04X  R8=%04X\n", r[7], r[8]); Scroll();
		for (i=9; i<16; i++) printf("R%d=%04X ", i, r[i]);
		flags = tec2.GetFlags();
		printf("  F=");
		for (i=7; i>=0; i--) printf("%d", getbit(flags, i));
		printf("\n"); Scroll();
		if (u) ViewInst(r[5]);
		return;
	}
	n = GetReg(str+1);
	if (n<0)
	{
		printf("Error\n"); Scroll();
		return;
	}
	printf("%04X:-", r[n]);
	InputHex(str);
	if (str[0]) tec2.SetReg(n, HexValue(str));
}

void CmdC()
{
	clrscr();
	Prompt();
}

void CmdD()
{
	static int p = 0x800;
	int i, j;
	char c;
	if (str[1])
	{
		i = HexValue(str+1);
		if (i < 0)
		{
			printf("Error!\n"); Scroll();
			return;
		}
		p = i;
	}
	for (i=0; i<15; i++)
	{
		printf("%04X    ", p);
		for (j=0; j<8; j++) printf("%04X  ", tec2.MEM[(p+j)&0xffff]);
		for (j=0; j<8; j++)
		{
			c = (tec2.MEM[p] & 0xff00) >> 8;
			if (!isgraph(c)) c = '.';
			printf("%c", c);
			c = (tec2.MEM[p] & 0xff);
			if (!isgraph(c)) c = '.';
			printf("%c", c);
			p++;
			p &= 0xffff;
		}
		printf("\n"); Scroll();
	}
}

void CmdE()
{
	int i, j;
	static int p = 0x800;
	char c;
	if (str[1])
	{
		if ((i=HexValue(str+1))<0)
		{
			printf("Error\n"); Scroll();
			return;
		}
		p = i;
	}
	i = p;
	while (1)
	{
		if ((i-p) % 5 == 0)
		{
			if (i > p) { printf("\n"); Scroll(); }
			printf("%04X\t", i);
		}
		printf("%04X:", tec2.MEM[i]);
		j = 0;
		while (c = getch(), c!=' ' && c!='\r')
		{
			if (c=='\b')
			{
				if (j > 0)
				{
					printf("\b \b");
					j--;
				}
			}
			else if (j<4 && (isdigit(c) || (c>='a' && c<='f') || (c>='A' && c<='F')))
			{
				putchar(c);
				str[j++] = c;
			}
		}
		str[j] = 0;
		ProcStr(str);
		if (j>0) tec2.MEM[i] = HexValue(str);
		if (c == '\r')
		{
			p = i + 1;
			printf("\n"); Scroll();
			break;
		}
		while (j++<4) printf(" ");
		printf("  ");
		i++;
	}
}

void CmdH()
{
	int value1, value2, i, len = strlen(str);
	i = 1; while (i<len && str[i]==' ') i++;
	if (i<len && (value1=HexValue(str+i, ' ')) >=0)
	{
		while (i<len && str[i]!=' ') i++;
		while (i<len && str[i]==' ') i++;
		if (i<len && (value2=HexValue(str+i)) >= 0)
		{
			i = value1 + value2;
			if (i > 65535) i -= 65536;
			printf("%04X  ", i);
			i = value1 - value2;
			if (i < 0) i += 65536;
			printf("%04X\n", i);
		}
		else printf("Error\n");
	}
	else printf("Error\n");
	Scroll();
}

void CmdS()
{
	char c;
	bool yes = false;
	printf("Are you sure to save the MCM? [N]\b\b");
	while (c=getch(), c!='\r')
	{
		if (c=='Y' || c=='y')
		{
			printf("Y\b");
			yes = true;
		}
		else if (c=='N' || c=='n')
		{
			printf("N\b");
			yes = false;
		}
	}
	printf("\n");
	if (yes) tec2.SaveMCM();
}

void CmdHelp()
{
	printf("Assemble\tA [adr]\n"); Scroll();
	printf("Unassemble\tU [adr]\n"); Scroll();
	printf("Go\t\tG [adr]\n"); Scroll();
	printf("Trace\t\tT [adr]\n"); Scroll();
	printf("Proceed\t\tP [adr]\n"); Scroll();
	printf("Register\tR [reg]\n"); Scroll();
	printf("Dump\t\tD [adr]\n"); Scroll();
	printf("Enter\t\tE [adr]\n"); Scroll();
	printf("Hex\t\tH value1 value2\n"); Scroll();
	printf("SaveMCM\t\tS\n"); Scroll();
	printf("MStep\t\tTT\n"); Scroll();
	printf("Clear\t\tC\n"); Scroll();
	printf("Quit\t\tQ\n"); Scroll();
}

void OnF10()
{
	int i, bytes, from, len;
	char c, temp, name[256];

	clrscr();
	Prompt();
	for (i=0; i<5; i++) printf("\n");
	printf("\t\t0---Return to TEC-2 CRT Monitor\n");
	printf("\t\t1---Send a file to TEC-2\n");
	printf("\t\t2---Receive a file from TEC-2\n");
	printf("\t\t3---Return to PC(MS)-DOS\n\n");
	printf("\t\tEnter your choice:[0]\b\b");
	c = '0';
	while (temp=getch(), temp!='\r')
	{
		if (temp>='0' && temp<='3') printf("%c\b", c=temp);
	}
	printf("\n");
	switch (c)
	{
	case '0': return;
	case '1':
		printf("\n\t\tFile name for read:");
		gets(name);
		bytes = tec2.ReadFile(name);
		if (bytes < 0)
		{
			printf("\n00000 bytes\n\nFile not found\n\n");
			break;
		}
		else
		{
			printf("\n%05d bytes\n\n", bytes);
		}
		break;
	case '2':
		printf("\n\t\tFile name for write:");
		gets(name);
		printf("\t\tBegin from: ");
		InputHex(str);
		from = HexValue(str);
		printf("\t\tData length: ");
		InputHex(str);
		len = HexValue(str);

		bytes = tec2.WriteFile(name, from, len);
		if (bytes < 0)
		{
			printf("\n\nFile not found\n\n");
			break;
		}
		else
		{
			printf("\n%05d bytes\n\n", bytes);
		}
		break;
	case '3':
		clrscr();
		exit(0);
	}
}
