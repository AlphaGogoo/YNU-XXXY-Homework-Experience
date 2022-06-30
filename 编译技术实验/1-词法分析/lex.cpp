#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define norw 11 // norw-1���ؼ���
#define al 20	//��Ĺؼ��ֵĳ���

#define ID 10			  //��ʶ���Ĵʷ��Ǻ�
#define inttoken 11		  //�����Ĵʷ��Ǻ�
#define comma 12		  //����
#define lparen 13		  //��С����
#define rparen 14		  //��С����
#define lbrace 15		  //�������
#define rbrace 16		  //�Ҵ�����
#define becomes 17		  //�Ⱥ�
#define addtoken 18		  //�Ӻ�
#define multoken 19		  //�˺�
#define ltctoken 20		  //С�ں�
#define gtctoken 21		  //���ں�
#define eqctoken 22		  //���ں�
#define semicolon 23	  //�ֺ�
#define minustoken 24	  //����
#define divtoken 25		  //����
#define quotationtoken 26 //����
#define floattoken 27	  //С������
#define chartoken 28 //�ַ�����

char TOKEN[20]; //�ַ������������δ�ŵ��ʴ��ĵĸ����ַ�

extern int lookup(char *); //��TOKEN�ַ����鱣���ֱ�
bool isalpha(char);		   //�жϽ����ַ��Ƿ�Ϊ��ĸ
bool isalnum(char);		   //�жϽ����ַ��Ƿ�Ϊ��ĸ��������
bool isdigit(char);		   //�жϽ����ַ��Ƿ�Ϊ����

FILE *fin;
FILE *fout;

void scanner()
{ //�ʷ�������������򣬶�������ı��ļ����дʷ�����
	char ch;
	char ch2;
	int i, c;
	int error = 0;	 //��¼�ļ��дʷ�����ĸ���
	ch = fgetc(fin); //�������ļ��ж�ȡһ���ַ�

	while (ch != EOF)
	{ //���������ļ����յ��ַ������ļ�������ʱ��ִ��ѭ��
		if (isalpha(ch))
		{ //����������ļ����յĵ�һ���ַ�����ĸ
			TOKEN[0] = ch;
			ch = fgetc(fin);
			i = 1;
			while (isalnum(ch))
			{
				TOKEN[i] = ch;
				i++;
				ch = fgetc(fin);
			}
			TOKEN[i] = '\0';
			fseek(fin, -1, 1);
			c = lookup(TOKEN); //�鱣���ֱ�
			if (c == 0)
			{
				fprintf(fout, "(%d,%s)\n", ID, TOKEN);
			} //�����ʶ��
			else
				fprintf(fout, "(%d,%s)\n", c, TOKEN); //������յ���Ϊ������
		}
		//�����һ���ַ�������
		else if (isdigit(ch))
		{
			//���������ֶ��浽TOKEN��
			TOKEN[0] = ch;
			ch = fgetc(fin);
			i = 1;
			while (isdigit(ch))
			{
				TOKEN[i] = ch;
				i++;
				ch = fgetc(fin);
			}
			//�������'.'��˵��������С��
			if (ch == '.')
			{
				//������������С����
				TOKEN[i++] = ch;
				ch = fgetc(fin);
				while (isdigit(ch))
				{
					TOKEN[i++] = ch;
					ch = fgetc(fin);
				}
				//������治����ĸ��С���㣬����������С�������floattoken
				if (ch != '.' && !isalpha(ch))
				{
					TOKEN[i] = '\0';
					fseek(fin, -1, 1);
					fprintf(fout, "(%d,%s)\n", floattoken, TOKEN);
				}else{
					while( isalpha(ch) || ch == '.' ){
						TOKEN[i++] = ch;
						ch = fgetc(fin);
					}
					fseek(fout, -1, 1);
					printf("%s is error\n",TOKEN);
				}
			}
			//�������������ĸ����ô�϶�Ҫ�����������ַ�������
			else if (isalpha(ch))
			{
				while (isalpha(ch) || isdigit(ch))
				{
					TOKEN[i++] = ch;
					ch = fgetc(fin);
				}
				TOKEN[i] = '\0';
				fseek(fin, -1, 1);
				error++;
				printf("%s is error\n", TOKEN);
			}
			//������Ǵ����������inttoken
			else
			{
				TOKEN[i] = '\0';
				fseek(fin, -1, 1);
				fprintf(fout, "(%d,%s)\n", inttoken, TOKEN);
			}
		}
		else
		{ //����������ļ����յĵ�һ���ַ��Ȳ�����ĸ�ֲ�������
			switch (ch)
			{ //�������յ��ķ����ַ����з��࣬��ȡһ��һ��
			case '=':
				ch = fgetc(fin);
				if (ch == '=')
					fprintf(fout, "(%d,==)\n", becomes); //������շ���Ϊ��ֵ��
				else
				{
					fseek(fin, -1, 1); //�ļ������ַ�����һ���ַ�
					fprintf(fout, "(%d,'=')\n", eqctoken);
				}
				break;
			case ',':
				fprintf(fout, "(%d,',')\n", comma);
				break;
			case ';':
				fprintf(fout, "(%d,';')\n", semicolon);
				break;
			case '(':
				fprintf(fout, "(%d,'(')\n", lparen);
				break;
			case ')':
				fprintf(fout, "(%d,')')\n", rparen);
				break;
			case '{':
				fprintf(fout, "(%d,'{')\n", lbrace);
				break;
			case '}':
				fprintf(fout, "(%d,'}')\n", rbrace);
				break;
			case '<':
				fprintf(fout, "(%d,'<')\n", ltctoken);
				break;
			case '>':
				fprintf(fout, "(%d,'>')\n", gtctoken);
				break;
			case '+':
				fprintf(fout, "(%d,'+')\n", addtoken);
				break;
			case '-':
				fprintf(fout, "(%d,'-')\n", minustoken);
				break;
			case '*':
				fprintf(fout, "(%d,'*')\n", multoken);
				break;
			case '\'':
				ch = fgetc(fin);
				ch2 = fgetc(fin);
				if( ch2 == '\''){
					fprintf(fout, "(%d,'\'%c%c')\n", chartoken, ch, ch2);
				}else{
					error++;
					printf("\' is error\n");
				}
				break;
			case '%':
				ch = fgetc(fin);
				char buffer[1024];
				if (ch == '%')
					fgets(buffer, 1024, fin); //�����ж�ȡ��ϣ��ƶ�����һ��
				else
				{
					fseek(fin, -1, 1);
					printf("%% is error\n"); //���շ������ַ����򱨸�ʷ�����
					error++;
				}
				break;
			case '/':
				ch = fgetc(fin);
				if (ch == '*')
				{
					ch = fgetc(fin);
					ch2 = fgetc(fin);
					while( ch != '*' || ch2 != '/' ){
						// printf("%c---%c", ch, ch2);
						ch = ch2;
						ch2 = fgetc(fin);
						if( ch2 == EOF ){
							printf("'/*' is error, without matched '*/'\n");
							error++;
							break;
						}
					}
				}
				else
				{
					fprintf(fout, "(%d,'/')\n", divtoken);
				}
				break;
			case ' ':
				break;
			case '\n':
				break;
			case '\t':
				break;
			default:
				printf("%c is error\n", ch); //���շ������ַ����򱨸�ʷ�����
				error++;
				break;
			}
		}
		ch = fgetc(fin); //�������ļ��ж�ȡ��һ�����ʣ�ֱ���ļ�����
	}					 // whileѭ������

	printf("������%d ���ʷ�����!", error);
	return;
}

int lookup(char *token)
{
	int j;
	char word[norw][al];
	strcpy(&(word[1][0]), "int");
	strcpy(&(word[2][0]), "char");
	strcpy(&(word[3][0]), "bool");
	strcpy(&(word[4][0]), "if");
	strcpy(&(word[5][0]), "then");
	strcpy(&(word[6][0]), "else");
	strcpy(&(word[7][0]), "while");
	strcpy(&(word[8][0]), "do");
	strcpy(&(word[9][0]), "true");
	strcpy(&(word[10][0]), "false");

	for (j = 1; j <= norw - 1; j++)
		if (strcmp(token, word[j]) == 0)
			return j; //��TOKEN�ַ����鱣���ֱ����鵽���ر����������
	return 0;		  // TOKEN���Ǳ����֣�����0
}

bool isalpha(char c)
{ //�жϽ����ַ��Ƿ�Ϊ��ĸ
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return 1;
	else
		return 0;
}

bool isalnum(char c)
{ //�жϽ����ַ��Ƿ�Ϊ��ĸ��������
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
		return 1;
	else
		return 0;
}

bool isdigit(char c)
{ //�жϽ����ַ��Ƿ�Ϊ����
	if (c >= '0' && c <= '9')
		return 1;
	else
		return 0;
}

int main()
{
	char filename[20];
	printf("�������ļ���:");
	scanf("%s", filename);
	if ((fin = fopen(filename, "r")) == NULL) //��Ҫ��ȡ���ı��ļ�
	{
		printf("���ܴ��ļ�.\n");
		exit(0);
	}

	printf("�����뱣�����������ļ���:");
	scanf("%s", filename);
	if ((fout = fopen(filename, "w")) == NULL)
	{
		printf("���ܴ��ļ�.\n");
		exit(0);
	}

	scanner(); //���ôʷ���������
	getchar();
	getchar();

	fclose(fin);
	fclose(fout);
	return 0;
}
