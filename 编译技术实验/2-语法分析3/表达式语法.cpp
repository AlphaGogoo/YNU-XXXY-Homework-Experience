#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define al 10 // ��ʶ������󳤶�

//���嵥�ʵ�����
enum tokenkind
{
	errtoken,
	endfile,
	id,
	num,
	letter,
	addtoken,
	multoken,
	ltctoken,
	gtctoken,
	eqctoken,
	lparen,
	rparen,
	subtoken,
	divtoken,
	becomes,
};

enum tokenkind token; //��ǰ���ʵĴʷ��ǺŻ��ߵ��ʵ����
char ch;
char idname[al + 1]; // ��ʶ��������

FILE *fin;

void gettoken();
void factor();
void term();
void expr();
void bexpr();

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
		gettoken();			  //����һ�����ʣ�������������sym��
		bexpr();			  // ���ÿ�ʼ���Ŷ�Ӧ���ӳ���
		if (token == endfile) //��ȡ���ļ�����û�д���
		{
			printf("�Ϸ����ʽ\n");
		}
		else
		{
			printf("�Ƿ����ʽ\n");
		}
		fclose(fin);

		printf("���������������ļ���������س�");
		// scanf("%s",filename);
		gets(filename);
	} while (strlen(filename) > 0);
	return 0;
}

/*
 * �ʷ�����,��һ�����ʵ�token
 */
void gettoken()
{
	int i, k;
	char a[al + 1];
	ch = fgetc(fin); //��ȡ��һ���ַ�

	while (ch == ' ' || ch == 10 || ch == 13 || ch == 9)
	{ /* ���Կո񡢻��С��س���TAB */
		ch = fgetc(fin);
	}
	if (isalpha(ch))
	{ // ���ֻ�������a..z��ͷ
		k = 0;
		do
		{
			if (k < al) // ���ŵ���󳤶�Ϊal �������ͽ�β����
			{
				a[k] = ch;
				k++;
			}
			ch = fgetc(fin);
		} while (isalpha(ch) || isdigit(ch));
		if(ch == EOF){
			//ʲô������
		}else{
			fseek(fin, -1, 1);
		}
	
		a[k] = 0;
		token = id;
	}
	else if (ch == EOF) //������
	{
		token = endfile;
	}
	else if (ch == '+')
	{
		token = addtoken;
	}
	else if (ch == '-')
	{
		token = subtoken;
	}
	else if (ch == '*')
	{
		token = multoken;
	}
	else if (ch == '/')
	{
		token = divtoken;
	}
	else if (ch == '(')
	{
		token = lparen;
	}
	else if (ch == ')')
	{
		token = rparen;
	}
	else if (ch == '<')
	{
		token = ltctoken;
	}
	else if (ch == '>')
	{
		token = gtctoken;
	}
	else if (ch == '=') // ��⸳ֵ����
	{
		ch = fgetc(fin);
		if (ch == '=')
		{
			token = eqctoken;
		}
		else
		{
			token = becomes;
			fseek(fin, -1, 1);
		}
	}
	else
	{
		printf("�ʷ�����\n");
		exit(0);
	}
}
bool isalpha(char c) //�жϽ����ַ��Ƿ�Ϊ��ĸ
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return 1;
	else
		return 0;
}
bool isalnum(char c) //�жϽ����ַ��Ƿ�Ϊ��ĸ��������
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
		return 1;
	else
		return 0;
}
void bexpr()
{
	expr();
	if (token == ltctoken || token == gtctoken || token == eqctoken) //
	{
		while (token == ltctoken || token == gtctoken || token == eqctoken)
		{
			gettoken(); //��ȡһ�����Ŵ浽sym��
			expr();
		}
	}
}
/*
expr->term{ + term | - term}
*/
void expr()
{
	term();
	if (token == addtoken || token == subtoken) //�����ǰ����Ϊaddtoken���ͽ���
	{
		while (token == addtoken || token == subtoken) //ѭ���жϣ�ֱ��term���ú���һλ����addtoken
		{
			gettoken(); //��ȡһ�����Ŵ浽sym��
			term();
		}
	}
}

/*
factor ->(expr)| id
*/
void factor()
{
	if (token == lparen) //�����ǰ����Ϊlparen������Ϊfactor ->(expr)�ж��Ƿ�Ϊ'('
	{
		gettoken(); //��ȡһ�����Ŵ浽token��
		expr();
		if (token == rparen) //�����ǰ����Ϊrparen����,�ж��Ƿ�Ϊ')'
		{
			gettoken(); //��ȡһ�����Ŵ浽token��
		}
		else
		{ //���expr���治��)'���﷨����
			printf("�﷨����1\n ");
			exit(0);
		}
	}
	else if (token == id)
	{
		gettoken();
	}
	else
	{
		printf("�﷨����2\n ");
		exit(0);
	}
}

/*
term �� factor { *  factor | / factor}
*/
void term()
{
	factor();
	if (token == multoken || token == divtoken)
	{
		while (token == multoken || token == divtoken)
		{
			gettoken();
			factor();
		}
	}
}