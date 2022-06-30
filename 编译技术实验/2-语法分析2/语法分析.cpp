
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum symbol
{
	nul,
	zero,
	one,
	period
};

int token; //��ǰ�ķ���
char ch;

FILE *fin;

void gettoken();
void parseS();
void parseN();
void parseB();

int main()
{
	char filename[20];

	printf("������������ļ���:");
	gets(filename);
	// scanf("%s",filename);
	do
	{
		if ((fin = fopen(filename, "r")) == NULL)
		{
			printf("���ܴ��ļ�.\n");
			return 0;
		}
		gettoken(); //����һ�����ʣ�������������token��
		parseS();	// ���ÿ�ʼ���Ŷ�Ӧ���ӳ���
		if (token == nul)
		{
			printf("�﷨��ȷ\n");
		}
		else
		{
			printf("�﷨����\n");
		}
		fclose(fin);

		printf("���������������ļ���������س�");
		// scanf("%s",filename);
		gets(filename);
	} while (strlen(filename) > 0);
	return 0;
}

/*
 * �򻯵Ĵʷ���������ȡһ�����Ŵ浽token��
 */
void gettoken()
{
	ch = fgetc(fin);

	while (ch == ' ' || ch == 10 || ch == 13 || ch == 9) /* ���Կո񡢻��С��س���TAB */
		ch = fgetc(fin);

	if (ch == EOF)
		token = nul;
	else if (ch == '1')
		token = one;
	else if (ch == '0')
		token = zero;
	else if (ch == '.')
		token = period;
	else
	{
		printf("--�ʷ���\n");
		exit(0);
	}
	return;
}

/*
S->N.N
*/
void parseS()
{
	parseN();
	if (token == period)
	{
		gettoken();
		parseN();
	}
	else
	{
		printf("�﷨����\n");
		return;
	}
}

/* N->BN or �� */
void parseN()
{
	parseB();
	while(token == zero || token == one)
	{
		parseB();
	}
}

/*  B->0|1  */
void parseB()
{
	if (token == zero || token == one)
	{
		gettoken();
	}
	else
	{
		printf("�﷨����\n");
		return;
	}
}
