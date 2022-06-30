/**
 * @file �������.cpp
 * @author ����ǿ (ynubwq@mail.ynu.edu.cn)
 * @�����������
 * @version 1
 * @date 2022-05-20
 *
 * @copyright Copyright (c) 2022
 *
 * ����ģʽ���£�
	start�� DS.
    D�� B; D
    D����
    B�� int L { L.type := int } | real L { L.type := real }
    L�� id { A.type := L.type enter(v.entry, L.type)} A
    A�� ,idA { A1.type := A.type enter(v.entry,A.type)}
    A����
    S�� V := E { gen( ":=", E.place,0,V.place) } H
    H��;S | ��
    E��T { R.place:=T.place; R.type=T.type} R {E.place:=R.place; E.type=R.type}
    R��+T { R1.place:= newtemp; gen( "+", R.place, T.place , R1.place) } R {R.place=R1.place; R.type=R1.type}
    R��-T { R1.place:= newtemp; gen( "-", R.place, T.place , R1.place) } R {R.place:=R1.place; R.type=R1.type}
    R�� �� {R.place=R.place; R.type=R.type}
    T��F { P.place:=F.place; P.type=F.type} P { T.place:=P.place; T.type=P.place}
    P��* F { P1.place:= newtemp; gen( "*", P.place, F.place ,P1.place) } P{P.place:= P1.place;P.type=P1.type }
    P��>/ F { P1.i:= newtemp; gen( "/", P.place, F.place , P1.place) } P {P.place:= P1.place;P.type=P1.type }
    P���� {P.place=P.place; P.type=P.type}
    F��( E ) { F.place := E.place; F.type=E.type}
    F��id {F.place := found(id).place; F.type=found(id).type}
    V��id {V.place:=found(id).place; V.type=found(id).type}
 */

#include "test.h"

int main() {
    char filename[20];

    printf("������������ļ���:");
    gets(filename);
    if ((fin = fopen(filename, "r")) == NULL) {
        printf("���ܴ��ļ�.\n");
        exit(0);
    }

    init();      //��ʼ��
    getsym(); //����һ�����ʣ�������������sym�У����ִ�ֵ����id��
    start();  //��ʼ��start->DS.  ����

    if (sym == eeof) {
        printf("�﷨��ȷ\n\n���м���뱣�浽�ļ��������ļ���������س�");
        gets(filename);
        if (strlen(filename) <= 0)
            return 0;
        if ((fout = fopen(filename, "w")) == NULL) {
            printf("���ܴ��ļ�.\n");
            exit(0);
        }
        for (int cx1 = 0; cx1 < cx; cx1++)
            fprintf(fout, "(%s,%s,%s,%s)\n", code[cx1].f, code[cx1].l, code[cx1].r, code[cx1].t);
    } else {
        printf("�﷨��1:  . �������о���");
        exit(0);
    }
    fclose(fin);
    fclose(fout);
    return 0;
}

//��ʼ������
void init() {
    int i;

    /* ���õ��ַ����� */
    for (i = 0; i <= 255; i++) {
        ssym[i] = nul; //����ȷ�ĵ��ַ�����Ϊnul����Ԥ�ó�ֵnul
    }
    ssym['+'] = plus;
    ssym['-'] = minus;
    ssym['*'] = times;
    ssym['/'] = divide;
    ssym['('] = lparen;
    ssym[')'] = rparen;
    ssym['.'] = period;
    ssym[','] = comma;
    ssym[';'] = semicolon;

    /* ���ñ��������� */
    strcpy(&(word[0][0]), "real");
    strcpy(&(word[1][0]), "int");

    /* ���ñ����ַ��� */
    wsym[0] = realsym;
    wsym[1] = intsym;

    tv = 100; //��ʱ����ָ���ֵ����Tx��tv��ȡֵû�н��������𵽵�����ʱ������������ı���
    tx = 0;      //��ָ���ֵ
    cx = 0;      //ָ�������
}

/**
 * ��ȡ��һ������
 */
int getsym() {
    int i, k;
    ch = fgetc(fin);

    if (ch == EOF) {
        sym = eeof;
        return 0;
    } //�ļ�����

    while (ch == ' ' || ch == 10 || ch == 13 || ch == 9) /* ���Կո񡢻��С��س���TAB */
        ch = fgetc(fin);

    if (ch >= 'a' && ch <= 'z') { /* ���ֻ�������a..z��ͷ ��*/
        k = 0;
        do {
            if (k < al) /* ���ŵ���󳤶�Ϊal �������ͽ�β����*/
            {
                a[k] = ch;
                k++;
            }
            ch = fgetc(fin);
        } while (ch >= 'a' && ch <= 'z' || ch >= '0' && ch <= '9');

        a[k] = 0;
        strcpy(id, a);
        fseek(fin, -1, 1);

        for (i = 0; i < norw; i++) /* ������ǰ�����Ƿ�Ϊ������ */
            if (strcmp(id, word[i]) == 0)
                break;
        if (i < norw) {
            sym = wsym[i];
        } else {
            sym = ident; /* ����ʧ���������Ǳ�ʶ�� */
        }
    } else if (ch == ':') /* ��⸳ֵ���� */
    {
        ch = fgetc(fin);
        if (ch == '=') {
            sym = becomes;
        } else {
            sym = nul; /* ����ʶ��ķ��� */
        }
    } else
        sym = ssym[ch]; /* �����Ų�������������ʱ��ȫ�����յ��ַ����Ŵ��� */
    return 0;
}

/**
 * �ڷ��ű�������һ��
 * @param type ���ŵ�����
 */
void enter(enum symbol type) {
    tx = tx + 1;
    if (tx > txmax) {
        printf(" ���ű�Խ�� "); /* ���ű�Խ�� */
        return;
    }
    //����ظ�����
    for (int i = 1; i < tx; i++) {
        if (strcmp(table[i].name, id) == 0) {
            printf("�ظ�����: %s\n", id);
            exit(0);
        }
    }
    strcpy(table[tx].name, id); /* ȫ�ֱ���id���Ѵ��е�ǰ���ֵ�����,TxΪ���뵱ǰ����֮ǰ��βָ�� */
    table[tx].type = type;
}

/**
 * �������ֵ�λ��.
 * �ҵ��򷵻������ֱ��е�λ��,���򷵻�0.
 * tx: ��ǰ���ֱ�βָ�룬ȫ�ֱ���
 * @param idt Ҫ���ҵ�����
 *
 */
info found(char *idt) {
    info inf;
    int i;
    enum symbol type;
    strcpy(table[0].name, idt);
    i = tx;
    while (strcmp(table[i].name, idt) != 0) {
        i--;
    }
    inf.place = i;
    inf.type = table[i].type;
    return inf;
}

/**
 * ģ���м��������
 */
int gen(enum symbol op, int arg1, int arg2, int result) {
    char temp1[al + 1], temp2[al + 1], temp3[al + 1];
    if (arg1 >= 100) //ģ��������ʱ����
    {
        wsprintf(temp1, "T%d", arg1);
    } else {
        strcpy(temp1, table[arg1].name);
    }

    if (arg2 >= 100) {
        wsprintf(temp2, "T%d", arg2);
    } else {
        strcpy(temp2, table[arg2].name);
    }

    if (result >= 100) {
        wsprintf(temp3, "T%d", result);
    } else {
        strcpy(temp3, table[result].name);
    }

    if (op == becomes) {

        printf("(:=,%s,%s,%s)\n", temp1, temp2, temp3);
        writecode(":=", temp1, temp2, temp3);
    } else if (op == plus) //+����
    {
        writecode("+", temp1, temp2, temp3);
        printf("(+,%s,%s,%s)\n", temp1, temp2, temp3);
    } else if (op == minus) //-����
    {

        writecode("-", temp1, temp2, temp3);
        printf("(-,%s,%s,%s)\n", temp1, temp2, temp3);
    } else if (op == divide) // /����
    {

        writecode("/", temp1, temp2, temp3);
        printf("(/,%s,%s,%s)\n", temp1, temp2, temp3);
    } else if (op == times) // *����
    {

        writecode("*", temp1, temp2, temp3);
        printf("(*,%s,%s,%s)\n", temp1, temp2, temp3);
    }

    return 0;
}

//����һ������
void writecode(char op[al + 1], char arg1[al + 1], char arg2[al + 1], char result[al + 1]) {
    if (cx >= cxmax) {
        printf("Program too long"); /* ������� */
        return;
    }
    strcpy(code[cx].f, op);
    strcpy(code[cx].l, arg1);
    strcpy(code[cx].r, arg2);
    strcpy(code[cx].t, result);
    cx++;
}

/**
 * P->DS.
 */
void start() {
    if (sym == intsym || sym == realsym) {
        D();
        S();
        if (sym == period) {
            getsym();
            return;
        } else {
            printf("�﷨��2�� ȱ�ٳ������.");
            exit(0);
        }
    } else {
        printf("�﷨��3: ����ֻ����int,��real��ʼ���������ִ�Сд");
        exit(0);
    }
}


/**
 * D-> B; D
 * D->��
 */
void D() {
    if (sym == intsym || sym == realsym) {
        B();
        if (ch == ';') {
            getsym();
            D();
        } else {
            printf("�﷨������βӦ��';'");
            exit(0);
        }
    } else if (sym == ident || sym == period) {
        return;
    } else {
        printf("�﷨�����忪ͷӦ������");
        exit(0);
    }
}

/**
 * ����һ������
 * B�� int L { L.type := int }  |  real L { L.type := real }
*/
void B() {
    if (sym == intsym) {
        getsym();
        L(intsym);
    } else if (sym == realsym) {
        getsym();
        L(realsym);
    } else {
        printf("�﷨��δ��ʶ����");
        exit(0);
    }
}

/**
 * L-> id   { A.Type := L.type  enter(v.entry,L.type)} A
 * V.entryͨ��ȫ�ֱ���tx���Դ���
 */
void L(enum symbol type) {
    if (sym == ident) {
        enter(type);
        getsym();
        A(type);
    } else {
        printf("�﷨�����Ǳ�ʶ��");
        exit(0);
    }
}

/**
 * A-> ��id  A   { A1.Type := A.type  enter(v.entry,A.type)}
 * A->��
 * ����һ����ʶ��
 * @param type A������
 */
void A(enum symbol type) {
    if (sym == comma) //��ǰ����Ϊ��
    {
        getsym();
        if (sym == ident) {
            enter(type);
            getsym();
            A(type);
        } else {
            printf("�﷨�����Ǳ�ʶ��");
            exit(0);
        }
    } else if (sym == semicolon)
        return; //��ǰ����Ϊ����A��follow��Ԫ�أ��൱�ڽ���A->��
    else {
        printf("�﷨�����Ƕ��Ż�ֺ�");
        exit(0);
    }
}

/**
 * S�� V := E { gen( ":=", E.place,0,V.place) } H
 * ����ֵ���
 */
void S() {
    info V_info, E_info;
    if (sym == ident) {
        V_info = V();
        if (sym == becomes) //��ǰ����Ϊ:=
        {
            getsym();
            E_info = E();
            if (V_info.type == E_info.type) {
                gen(becomes, E_info.place, -1, V_info.place);
            } else {
                printf("�������ֵ�������Ͳ�һ��");
                exit(0);
            }
            H();
        } else {
            printf("�﷨��ȱ�ٵ��ں�");
            exit(0);
        }
    } else {
        printf("�﷨�����Ǳ�ʶ��");
        exit(0);
    }
}

/**
 * H����S | ��
 */
void H() {
    if (sym == semicolon) //;
    {
        getsym();
        S();
    } else if (sym == period)
        return;
    else {
        printf("�﷨�����ǷֺŻ�������");
        exit(0);
    }
}

/**
 * E->T   { R.i:=T.place} R {E.place:=R.s}
 */
info E() {
    info E_info, T_info, R_info;
    if (sym == ident || sym == lparen) {
        T_info = T();
        R_info = R(T_info);
        if (T_info.type == R_info.type) {
            E_info.place = R_info.place;
            E_info.type = R_info.type;
        } else {
            printf("������������Ͳ�һ��");
            exit(0);
        }
    } else {
        printf("�﷨��,���Ǳ�ʶ����������");
        exit(0);
    }
    return E_info;
}

/**
 * R->+T { R1.i:= newtemp; gen( "+", R.i, T.place , R1.i) }  R {R.s:= R1.s; }
 * R->-T { R1.i:= newtemp; gen( "-", R.i, T.place , R1.i) }  R {R.s:=R1.s; }
 * R-> ��  {R.s=R.i}
 */
info R(info Ri) {
    info R_info;
    info T_info;
    info temp;
    if (sym == plus) {
        getsym();
        T_info = T();
        if (T_info.type == Ri.type) {
            tv = tv + 1; //������ʱ����
            gen(plus, Ri.place, T_info.place, tv);
            temp.place = tv;
            temp.type = T_info.type;
            R_info = R(temp);
        } else if (sym == minus) {
            tv = tv + 1; //������ʱ����
            gen(minus, Ri.place, T_info.place, tv);
            temp.place = tv;
            temp.type = T_info.type;
            R_info = R(temp);
        } else {
            printf("������������Ͳ�һ��");
            exit(0);
        }
    } else if (sym == semicolon || sym == rparen || sym == period) {
        R_info = Ri;
    } else {
        printf("�﷨���ӻ����Ҳ���Ǿ��ӽ���");
        exit(0);
    }
    return R_info;
}

/**
 *T->F { P.i:=F.place} P {T.place := P.s}
 */
info T() {
    info T_info, F_info, P_info;
    if (sym == lparen || sym == ident) {
        F_info = F();
        P_info = P(F_info);
        if (P_info.type == F_info.type) {
            T_info.type = P_info.type;
            T_info.place = P_info.place;
        } else {
            printf("������������Ͳ�һ��");
            exit(0);
        }
    } else {
        printf("�﷨�����������Ż��ʶ��");
        exit(0);
    }
    return T_info;
}

/**
 * F->(E){ F.place := E.place}
 * F->id {F.place:=found (id)}
 */
info F() {
    info F_info;
    if (sym == ident) {
        F_info = found(id);
        if (F_info.place == 0) {
            printf("����û������");
            exit(0);
        }
        getsym();
    } else if (sym == lparen) {
        getsym();
        F_info = E();
        if (sym == rparen)
            getsym();
        else {
            printf("�﷨��ȱ)");
            exit(0);
        }
    } else {
        printf("�﷨��,ȱ(");
        exit(0);
    }
    return F_info;
}

/**
 * P-> * F { p1.i:= newtemp; gen( "*", P.i, F.place , T) } P {P.s:= p1.s; }
 * P��> / F { p1.i:= newtemp; gen( "/", P.i, F.place , T) } P {P.s:= p1.s; }
 * P���� {P.s=P.i}
 */
info P(info Pi) {
    info P_info;
    info F_info;
    info temp;
    if (sym == times) {
        getsym();
        F_info = F();
        if (Pi.type == F_info.type) {
            tv = tv + 1;
            gen(times, Pi.place, F_info.place, tv);
            P_info = P(F_info);
        } else {
            printf("������������Ͳ�һ��");
            exit(0);
        }
    } else if (sym == divide) {
        getsym();
        F_info = F();
        if (Pi.type == F_info.type) {
            tv = tv + 1;
            gen(divide, Pi.place, F_info.place, tv);
            P_info = P(F_info);
        } else {
            printf("������������Ͳ�һ��");
            exit(0);
        }
    } else {
        P_info = Pi;
    }
    return P_info;
}

/**
 * V->id {V.place:=found(id)}
 */
info V() {
    info inf;
    if (sym == ident) {
        inf = found(id);
        if (inf.place == 0) {
            printf("����û������");
            exit(0);
        }
        getsym();
    } else {
        printf("�﷨�����Ǳ�ʶ��");
        exit(0);
    }
    return inf;
}
