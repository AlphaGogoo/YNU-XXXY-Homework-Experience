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
bool parseS();
int parseA();
int parseB();

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
		bool is_equal = parseS();	// ���ÿ�ʼ���Ŷ�Ӧ���ӳ���
		if (token == nul)
		{
			printf("�﷨��ȷ\n");
            if(is_equal){
                printf("Accepted\n");
            }else{
                printf("Refused\n");
            }
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
bool parseS()
{
    int Anum, Bnum;
	if (token == atoken || token == ctoken)
	{
		Anum = parseA();
		Bnum = parseB();
        if (Anum == Bnum){
            return true;
        }else{
            return false;
        }
	}
	else
	{
		printf("�﷨��1: ȱa����c\n ");
		exit(0);
	}
}

/* A-->aA|c */
int parseA()
{
    int Anum = 0;
	if (token == atoken)
	{
		gettoken();
        Anum = parseA() + 1;
	}
	else if (token == ctoken)
	{
        Anum = 0;
		gettoken();
	}else
    {
        printf("�﷨��2: ȱa����c\n");
        exit(0);
    }
    return Anum;
}

/* A-->bB|d */
int parseB()
{
    int Bnum;
	if (token == btoken)
	{
		gettoken();
        Bnum = parseB() + 1;
	}
	else if (token == dtoken)
	{
		gettoken();
        Bnum = 0;
	}
    else
    {
        printf("�﷨��3: ȱb����d\n");
        exit(0);
    }
    return Bnum;
}
