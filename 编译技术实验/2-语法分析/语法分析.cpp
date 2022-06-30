
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum symbol
{
	nul,
	atoken,
	btoken,
	ctoken,
	dtoken
};

int token; //��ǰ�ķ���
char ch;

FILE *fin;

void gettoken();
void parseS();
void parseA();
void parseB();

int main()
{
	char filename[20];

	printf("������������ļ���:");
	gets(filename);
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

	else if (ch == 'a')
		token = atoken;
	else if (ch == 'b')
		token = btoken;
	else if (ch == 'c')
		token = ctoken;
	else if (ch == 'd')
		token = dtoken;
	else
	{
		printf("--�ʷ���\n");
		exit(0);
	}
	return;
}

/*
S->AB
*/
void parseS()
{
	if (token == atoken || token == ctoken)
	{
		parseA();
		parseB();
	}
	else
	{
		printf("�﷨��1: ȱa����c\n ");
		exit(0);
	}
}

/* A-->aA|c */
void parseA()
{
	if (token == atoken)
	{
		gettoken();
		parseA();
	}
	else if (token == ctoken)
	{
		gettoken();
	}
	else
	{
		printf("�﷨��2: ȱa����c\n");
		exit(0);
	}
}

/* A-->bB|d */
void parseB()
{
	if (token == btoken)
	{
		gettoken();
		parseB();
	}
	else if (token == dtoken)
	{
		gettoken();
	}else{
		printf("�﷨��4: ȱb����d\n");
		exit(0);
	}
}
