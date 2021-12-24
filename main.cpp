#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdlib>
#include <cstddef>
#include <cstring>
#include <queue>
#include <stack>
#include <vector>
#include <string>
#include <climits>
#include <algorithm>
#include <io.h>
#include <windows.h>
#include <fstream>


using namespace std;

class GRAPH
{
public:
	GRAPH(); //�����������
	char input[100];

	int **matrix_smezhnosti;//������� ���������
	int len_smeg = 0;//������ ������� ��������� � ���������� ������ (������� � 1)
	int **matrix_rate;//������ ���������
	int **matrix_incedent;//������� �������������
	int len_inc_x = 0, len_inc_y = 0;//������� ������� ������������� (x-�����,y-�������)
	int *rate_order;//��������� ������������������
	int *exce;//��� ��������������� (������� ���������� � 0)
	int *cs;//������ � ������ �������� � ����� �� ����������� �������
	int *radius;//������ ��� ������ �� (������� � 0)
	int *diametr;//������� ��� ������ �� (������� � 0)
	bool graf_orientirovan;		//���� �����������������
	int kss;//���-�� ����������� ������
	int *excent;//��������������� ������ ��� ������� ����������
	int circl;//��������������� ���������� �� ���������� ��������� ������
	int **matrix_weight;//������� ����
	int **matrix_weight_ost;//����������� ����� (���� �������� prim)
	int weight;

	//����
	void translate_inc_in_smeg();//������� �� ������� ������������� � ������� ���������
	void translate_smeg_in_inc();//������� �� ������� ��������� � ������� �������������
	void translate_smeg_in_rate();//������� �� ������� ��������� � ������� ������ ���������
	void read_rate(char str[]);//������ ������ ��������� �� �����
	void read_smegnost(char str[]);//������ ������� ��������� �� �����
	void read_incedent(char str[]);//������ ������� ������������� �� �����
	void read_weight(char str[]);//������ ������� �����

	//������ �� �����
	void print_matrix(char c);//������ ������ ���������, �������������, ������ ���������

	//��� �������������� �����
	void create_rate_order();//��������� ������������������
	int create_rate_top(int top);//����������� ������� ������������ �������
	void order_onefriend_tops();//������ ������� ������
	void order_alone_tops();//������ ������������� ������
	int find_shortest_way(int from, int to);//���������� ����������� ���� � ������ �������
	void finder(int *mark, int *seen, int from, int to);//��������������� ������� ��� �������(�������� ��������)
	void exc();//������� ��������������� ���� ������
	void find_diametr();//������� ������� � ������
	int finder_diametr(int ks);//��������������� ������� ��� �������� (�� ���������� � ������)
	int finder_radius(int ks);//��������������� ������� ��� �������
	void find_peref();//��������� � �����
	void compass();//������

	//��������
	void add_top();//�������� �������
	void delete_top(int num);//������� �������
	void add_way(int from, int to);//�������� �����
	void delete_way(int from, int to);//������� �����
	void full();//����������
	void broke_in_half(int from, int to);//������������ ����� (?)
	void double_top(int top);//������������
	void full_double_top(int top);//�����������
	void make_it_one(int top1, int top2);//��������������
	void full_make_it_one(int *tops, int len);//����������
	void unite( GRAPH B);//�����������
	void unite_full(GRAPH B);//����������
	void equl(GRAPH B);//������������
	void stoks();//�����
	void isstoks();//������

	//������
	int finder_wfs(int *check, int kolv, int mark, int from);
	void wfs();
	int finder_prim(int *choce, int mark, int kolv);
	void prim();

	//�����
	void print_in(GRAPH A, char c);//�������� ��� ������
	void in_file(GRAPH A);//���� ������

};

GRAPH::GRAPH()
{
	memset(input, '\0', sizeof(input));

	matrix_smezhnosti = nullptr;//������� ���������
	len_smeg = 0;//������ ������� ��������� � ��������� ������ (������� � 1)
	matrix_rate = nullptr;//������ ���������
	matrix_incedent = nullptr;//������� �������������
	len_inc_x = 0;//����������� ������� �������������
	len_inc_y = 0;
	rate_order = nullptr;//��������� ������������������
	exce = nullptr;//��� ��������������� (������� ���������� � 0)
	cs = nullptr;//������ � ������� �������� � ����� �� ����������� �������
	radius = nullptr;//������ ��� ������ �� (������� � 0)
	diametr = nullptr;//������� ��� ������ �� (������� � 0)
	kss = 0;
}

void GRAPH::print_matrix( char c) {
	int i, j,x,y;
	if (c == 'i') {
		x = len_inc_x; y = len_inc_y;
		cout << "������� �������������:\n";
		cout << " ";
		if (len_smeg >= 10) cout << " ";
		char c = 'a';
		for (i = 0; i < x; i++) cout << "  " << char(c + i);
		cout << "\n";
		for (i = 0; i < y; i++) {
			cout << i + 1 << " "; if (len_smeg >= 10 && i < 10) cout << " ";
			for (j = 0; j < x; j++) {
				if (matrix_incedent[i][j] >= 0&&matrix_incedent[i][j]<10) cout << " ";
				cout << matrix_incedent[i][j] << " ";
			}
			cout << "\n";
		}
	}
	else if (c == 's') {
		x = len_smeg;
		cout << "�������� ���������:\n";
		cout << " ";
		if (len_smeg >= 10) cout << " ";
		for (i = 1; i <= x; i++) {
			if(i<10)cout << "  " << i; else cout << " " << i;
		}
		cout << "\n";
		for (i = 0; i < x; i++) {
			cout << i + 1 << " "; if (len_smeg >= 10 && i < 9) cout << " ";
			for (j = 0; j < x; j++) {
				if (matrix_smezhnosti[i][j] < 10) cout << " ";
				cout << matrix_smezhnosti[i][j] << " ";
			}
			cout << "\n";
		}
	}
	else if (c == 'r') {
		x = len_smeg;
		cout << "������ ���������:\n";
		for (i = 0; i < x; i++) {
			cout << "{ ";
			for (j = 0; j < x; j++) if (matrix_rate[i][j] != 0) cout << matrix_rate[i][j] << " ";
			cout << "}\n";
		}
	}
	return;
}

void GRAPH::read_rate(char str[]) {
	FILE *file;
	file = fopen(str, "r");
	int count = 0, line = 0, check = 0, i;
	char c;
	char pred = 'a';
	char predpred;
	while (!feof(file)) {//������� ���-�� ������ � ��������� ���� �� ������������
		c = fgetc(file);
		if (((int)c >= 48 && (int)c <= 57) || c == '{' || c == '}' || c == ' ' || c == '\n') {//��� ����� ������� ������ ������� ������ ��� ����� �����
			if ((line == 0 && c != '{') || (c == '{'&&line != 0)) check = 1;//������ ������ � ������ - {
			line++;
			if (c == '\n') {
				count++; line = 0;
				if (pred != '}') check = 1;
				if (predpred == ' ' || predpred == '}' || predpred == '{') check = 1;
			}
			predpred = pred;
			pred = c;
			if (check == 1) break;
		}
		else {
			check = 1;
			if (c == -1) { check = 0; }
			if (check != 0) break;
		}
	}
	fclose(file);
	if (check == 1) { cout << "������ � �����!"; return; }
	//else cout << "smegnost on " << count << "\n";

	len_smeg = count + 1;
	matrix_smezhnosti = new int *[count + 1];
	for (int i = 0; i <= count; i++)
		matrix_smezhnosti[i] = new int[count + 1];

	int j;
	for (i = 0; i < len_smeg; i++) for (j = 0; j < len_smeg; j++) matrix_smezhnosti[i][j] = 0;

	int rez = 0, number = 0;
	file = fopen(str, "r");
	int line_rate = -1, count_rate = 0;
	//if (!file) cout<<"pizdetc\n"; else cout<<"ok\n";
	while (!feof(file)) {
		c = fgetc(file);
		if (c == '{') {
			number = 0;//cout<<"\n";
			//count_rate=0;line_rate++;
		}
		if (c == ' ' || c == '}') {//matrix_rate[line_rate][count_rate]=rez;count_rate++;
			if (number == 0) { line = rez;/*cout<<line<<"\n"*/; number++; rez = 0; }
			else {
				if (line - 1 == rez - 1) {
					matrix_smezhnosti[line - 1][rez - 1] += 2;
				}
				else {
					matrix_smezhnosti[line - 1][rez - 1] = 1;
				}
				/*cout<<line-1<<" "<<rez-1<<" "<<"\n";*/number++; rez = 0;
			}
		}
		if ((int)c >= 48 && (int)c <= 57) { rez = rez * 10 + (int)c - 48; }
	}
	if(check!=1&&graf_orientirovan==false)
		for (i = 0; i <= count; i++)
			for (j = 0; j <= count; j++) if (matrix_smezhnosti[i][j] != matrix_smezhnosti[j][i]) check = 1;
	fclose(file);
	if (check == 1) {cout << "������ � �����!\n"; return;}
	excent = new int[len_smeg];
	for (int k = 0; k < len_smeg; k++)	excent[k] = 0;
	print_matrix('r');
	translate_smeg_in_inc();
}

void GRAPH::read_incedent(char str[]) {
	FILE *file;
	file = fopen(str, "r");
	int count = 0, line = 0, check = 0;
	char c;
	while (!feof(file)) {
		c = fgetc(file);
		if (c == ' '&&count == 0)line++;
		if (c == '\n' || c == -1) {
			if (count == 0) line++;
			count++;
		}
	}
	fclose(file);
	cout << count << " " << line;
	len_inc_x = line; len_inc_y = count;
	matrix_incedent = new int *[count];
	for (int i = 0; i < count; i++) matrix_incedent[i] = new int[line];
	int rez = 0, number = 0;
	file = fopen(str, "r");
	count = 0;
	//if (!file) cout<<"pizdetc\n"; else cout<<"ok\n";
	line = 0;
	int minus = 0;
	while (!feof(file)) {
		c = fgetc(file);
		if (c == '-') { minus = 1; c = fgetc(file); }
		if (((int)c >= 48 && (int)c <= 57) || c == ' ' || c == '\n' || c == -1) {
			if (c == '\n' || c == -1) {
				if ((rez > 2) || rez < 0) check = 1;
				if (minus == 1) {
					if (graf_orientirovan == true) { rez *= -1; minus = 0; }
					else check = 1;
				}
				matrix_incedent[count][line] = rez;
				//cout<<rez<<"\n";
				rez = 0; count++; line = 0;
			}
			if (c == ' ') {
				if ((rez > 2) || rez < 0) check = 1;
				if (minus == 1) {
					if (graf_orientirovan == true) { rez *= -1; minus = 0; }
					else check = 1;
				}
				matrix_incedent[count][line] = rez;//cout<<count<<" "<<line<<" "<<rez<<"\n";
				line++; rez = 0;
			}
			if ((int)c >= 48 && (int)c <= 57) { rez = rez * 10 + (int)c - 48; }
			if (check == 1) break;
		}
		else {
			check = 1; break;
		}
	}
	fclose(file);
	if (check == 1) { cout << "������ � �����!\n"; return; }
	translate_inc_in_smeg();
	print_matrix('i');
}

void GRAPH::read_smegnost(char str[]) {
	FILE *file;
	file = fopen(str, "r");
	int count = 0, line = 0, check = 0;
	char c;
	c = fgetc(file);
	while (c != '\n' && !feof(file)) {//������ ������� ������
		if (c == ' ')count++;
		c = fgetc(file);
	}
	fclose(file);

	len_smeg = count + 1;
	matrix_smezhnosti = new int *[count + 1];
	for (int i = 0; i <= count; i++) matrix_smezhnosti[i] = new int[count + 1];

	int rez = 0, number = 0;
	len_smeg = count + 1;

	file = fopen(str, "r");
	count = 0;
	while (!feof(file)) {
		c = fgetc(file);
		if (((int)c >= 48 && (int)c <= 57) || c == ' ' || c == '\n' || c == -1) {
			if (c == '\n' || c == -1) {
				if ((rez % 2 != 0&&count==line) || rez < 0) check = 1;//�������� �� ������������� � �� ������� ���� �� ���������
				matrix_smezhnosti[count][line] = rez;
				//cout<<rez<<"\n";
				rez = 0; count++; line = 0;
			}
			if (c == ' ') {
				if ((rez % 2 != 0 && count == line) || rez < 0) check = 1;//�������� �� ������������� � �� ������� ���� �� ���������
				matrix_smezhnosti[count][line] = rez;//cout<<rez<<" ";
				line++; rez = 0;
			}
			if ((int)c >= 48 && (int)c <= 57) { rez = rez * 10 + (int)c - 48; }
			if (check == 1) break;
		}
		else {
			check = 1; break;
		}
	}
	fclose(file);
	//cout << graf_orientirovan << " " << check << "\n";
	if (graf_orientirovan == false && check != 1) {
		//cout << "here\n";
		for (int i = 0; i < len_smeg; i++)
			for (int j = 0; j < len_smeg; j++) if (matrix_smezhnosti[i][j] != matrix_smezhnosti[j][i]) check = 1;
	}
	if (check == 1) { cout << "������ � �����!\n"; return; }
	excent = new int[len_smeg];
	for (int k = 0; k < len_smeg; k++)	excent[k] = 0;
	print_matrix('s');
	translate_smeg_in_inc();
}

void GRAPH::translate_inc_in_smeg() {
	int i, j, count = 0, line;
	free(matrix_smezhnosti);
	matrix_smezhnosti = new int *[len_inc_y];
	for (int i = 0; i < len_inc_y; i++) matrix_smezhnosti[i] = new int[len_inc_y];
	for (i = 0; i < len_inc_x; i++)
		for (j = 0; j < len_inc_y; j++)matrix_smezhnosti[i][j] = 0;
	int rez = 0, number = 0;
	for (i = 0; i < len_inc_x; i++) {
		for (j = 0; j < len_inc_y; j++) {
			if (matrix_incedent[j][i] != 0) {
				if (matrix_incedent[j][i] == 2) matrix_smezhnosti[j][j] += 2; else {
					if (count == 0) {
						line = j; count++;//cout<<line<<" ";
					}
					else {
						if (graf_orientirovan == false) {
							matrix_smezhnosti[line][j] += 1;
							matrix_smezhnosti[j][line] += 1;
						}
						else {
							if (matrix_incedent[j][i] == -1) matrix_smezhnosti[line][j] += 1; else matrix_smezhnosti[j][line] += 1;
						}
						count = 0;
					}
				}
			}
		}
	}
	len_smeg = len_inc_y;
	excent = new int[len_smeg];
	for (int k = 0; k < len_smeg; k++)	excent[k] = 0;
	translate_smeg_in_rate();
}

void GRAPH::translate_smeg_in_inc() {
	int i, j, sum = 0, orient;
	for (i = 0; i < len_smeg; i++) {
		if (graf_orientirovan == true)orient = 0; else orient = i;
		for (j = orient; j < len_smeg; j++) {
			if (matrix_smezhnosti[i][j] % 2 == 0 && i == j)sum += matrix_smezhnosti[i][j] / 2; else
				sum += matrix_smezhnosti[i][j];
		}
	}
	matrix_incedent = new int *[len_smeg];
	for (int i = 0; i < len_smeg; i++) matrix_incedent[i] = new int[sum];
	for (i = 0; i < len_smeg; i++)
		for (j = 0; j < sum; j++) matrix_incedent[i][j] = 0;
	int line = 0, k, control;
	for (i = 0; i < len_smeg; i++) {
		if (graf_orientirovan == true)orient = 0; else orient = i;
		for (j = orient; j < len_smeg; j++) {
			if (matrix_smezhnosti[i][j] != 0) {
				if (i == j && matrix_smezhnosti[i][j] != 0) //�����
					control = matrix_smezhnosti[i][j] / 2;
				else control = matrix_smezhnosti[i][j];
				for (k = 0; k < control; k++) {//������� ����� ������� ���� ����
					matrix_incedent[i][line] += 1;
					if(graf_orientirovan==false) matrix_incedent[j][line] += 1; else  matrix_incedent[j][line] -= 1;
					if (graf_orientirovan == true && (control != 1||(i == j))) matrix_incedent[j][line] = 2;
					line++;
				}
			}
		}
	}
	len_inc_x = sum; len_inc_y = len_smeg;
	translate_smeg_in_rate();
}

void GRAPH::translate_smeg_in_rate() {
	int i, j, line = 0, k;
	matrix_rate = new int *[len_smeg + 1];
	for (i = 0; i <= len_smeg; i++)
		matrix_rate[i] = new int[len_smeg + 1];
	for (i = 0; i < len_smeg; i++)
		for (j = 0; j < len_smeg; j++)matrix_rate[i][j] = 0;
	for (i = 0; i < len_smeg; i++) {
		matrix_rate[i][0] = i + 1; line = 1;
		for (j = 0; j < len_smeg; j++) {
			if (matrix_smezhnosti[i][j] >= 1) {
				if (matrix_smezhnosti[i][j] == 1) matrix_rate[i][line] = j + 1; else for (k = 0; k < matrix_smezhnosti[i][j] / 2; k++) {
					matrix_rate[i][line] = j + 1; line++;
				}
				line++;
			}
		}
	}
}

void GRAPH::create_rate_order() {
	cout << "��������� ������������������:\n";
	rate_order = new int[len_smeg];
	int i, j, sum;
	if (graf_orientirovan == false) {
		for (i = 0; i < len_smeg; i++) {
			sum = 0;
			for (j = 0; j < len_smeg; j++) {
				sum += matrix_smezhnosti[i][j];
			}
			rate_order[i] = sum; cout << rate_order[i] << " ";
		}
		cout << "\n";
	}
	else {
		cout << "����������� ������:\n";
		for (i = 0; i < len_smeg; i++) {
			cout << "������� " << i + 1 << " ����������� ";
			sum = 0;
			for (j = 0; j < len_smeg; j++) {
				if (i == j && matrix_smezhnosti[i][j] == 2) sum += 1;else
				sum += matrix_smezhnosti[i][j];
			}
			cout << sum << "\n";
		}
		cout << "����������� ������:\n";
		for (i = 0; i < len_smeg; i++) {
			cout << "������� " << i + 1 << " ����������� ";
			sum = 0;
			for (j = 0; j < len_smeg; j++) {
				if (i == j && matrix_smezhnosti[i][j] == 2) sum += 1; else
				sum += matrix_smezhnosti[j][i];
			}
			cout << sum << "\n";
		}
	}
	return;
}

int GRAPH::create_rate_top(int top) {
	int i, sum = 0;
	for (i = 0; i < len_smeg; i++) sum += matrix_smezhnosti[top - 1][i];
	return sum;
}

void GRAPH::order_onefriend_tops() {
	cout << "������ ������� ������:";
	int i, j, sum;
	for (i = 0; i < len_smeg; i++) {
		sum = 0;
		for (j = 0; j < len_smeg; j++) if(matrix_smezhnosti[i][j]>0)sum ++;
		if (sum == 1) cout << i + 1 << " ";
	}
	cout << "\n";
	return;
}

void GRAPH::order_alone_tops() {
	cout << "������ ������������� ������:";
	int i, j, sum;
	for (i = 0; i < len_smeg; i++) {
		sum = 0;
		for (j = 0; j < len_smeg; j++) if(matrix_smezhnosti[i][j]!=0) sum=1;
		if (sum == 0&&graf_orientirovan==false) cout << i + 1 << " ";
		if (sum == 0 && graf_orientirovan == true) {
			for (j = 0; j < len_smeg; j++) if (matrix_smezhnosti[j][i] != 0) sum = 1;
			if (sum==0) cout << i + 1 << " ";
		}
	}
	cout << "\n";
	return;
}

void GRAPH::finder(int *mark, int *seen, int from, int to) {
	if (from == to) return;
	if (circl == len_smeg) return;
	int check, i, j;
	circl++;

	for (i = 0; i < len_smeg; i++) {
		if (matrix_smezhnosti[from][i] != 0 && !seen[i] && from != i) {
			check = mark[from] + 1;
			if (mark[i] < 0) mark[i] = mark[from] + 1;
			else {
				if (mark[i] > check) mark[i] = check;
			}
			seen[i] = 1;
			finder(mark, seen, i, to);
			seen[i] = 0;
		}
	}
	return;
}

int GRAPH::find_shortest_way(int from, int to) {
	int i, j, k;
	int *mark = new int[len_smeg];
	for (i = 0; i < len_smeg; i++)	mark[i] = -1;
	int *seen = new int[len_smeg];
	for (i = 0; i < len_smeg; i++)seen[i] = 0;

	mark[from - 1] = 0;
	for (i = 0; i < len_smeg; i++) {
		if (matrix_smezhnosti[from - 1][i] != 0) {
			for (k = 0; k < len_smeg; k++)seen[k] = 0;
			seen[i] = 1; mark[i] = 1; circl = 1; finder(mark, seen, i, to - 1);
		}
	}

	for (j = 0; j < len_smeg; j++) {
		excent[j] = mark[j];//cout<<j<<"="<<excent[j]<<" ";
	}
	return mark[to - 1];
}

void GRAPH::exc() {
	int i, j, k, max, line;
	exce = new int[len_smeg];
	cs = new int[len_smeg];
	for (i = 0; i < len_smeg; i++) cs[i] = 0;
	int ks = 0;

	for (i = 1; i <= len_smeg; i++) {
		line = 0; max = 0;
		for (k = 0; k < len_smeg; k++)	excent[k] = 0;

		int a = find_shortest_way(i, len_smeg - 1);
		for (k = 0; k < len_smeg; k++) {
			if (excent[k] > max) max = excent[k];
			if (excent[k] != -1 && cs[k] == 0) {
				if (cs[i - 1] == 0) {
					ks++; cs[i - 1] = ks;
				}
				cs[k] = cs[i - 1];
			}
		}
		exce[i - 1] = max;
	}
}

int GRAPH::finder_diametr(int ks) {
	int i, max = 0;
	for (i = 0; i < len_smeg; i++) if (cs[i] == ks && exce[i] > max) max = exce[i];
	return max;
}

void GRAPH::find_diametr() {
	int i, ks = 1;
	for (i = 0; i < len_smeg; i++) if (cs[i] > ks) ks = cs[i];
	kss = ks;
	diametr = new int[ks];
	radius = new int[ks];
	for (i = 1; i <= ks; i++) {
		diametr[i - 1] = finder_diametr(i);
		radius[i - 1] = finder_radius(i);
	}
}

int GRAPH::finder_radius(int ks) {
	int i, min = len_smeg + 1;
	for (i = 0; i < len_smeg; i++) if (cs[i] == ks && exce[i] < min) min = exce[i];
	return min;
}

void GRAPH::find_peref() {
	int i, j,ks=1;
	for (i = 0; i < len_smeg; i++) if (cs[i] > ks) ks = cs[i];
	for (i = 1; i <= ks; i++) {
		cout << "�� " << i << "���������: ";
		for (j = 0; j < len_smeg; j++) {
			if (cs[j] == i && exce[j] == diametr[i - 1]) cout << j + 1 << " ";
		}
		cout << "; ����: ";
		for (j = 0; j < len_smeg; j++) {
			if (cs[j] == i && exce[j] == radius[i - 1]) cout << j + 1 << " ";
		}
		cout << "\n";
	}
}

void GRAPH::compass() {
	int i, j, k, q, min = len_smeg + 1, curent, l;
	int *mark = new int[len_smeg];
	for (i = 0; i < len_smeg; i++)	mark[i] = -1;
	int *seen = new int[len_smeg];
	int ks = 1;
	for (i = 0; i < len_smeg; i++) if (cs[i] > ks) ks = cs[i];
	for (i = 1; i <= ks; i++) {//��� ��
		min = len_smeg + 1;
		for (j = 0; j < len_smeg; j++) {//����� ������ �������
			if (cs[j] == i) {
				curent = len_smeg + 1;
				if (matrix_smezhnosti[j][j] > 0) curent = 0; else
					for (q = 0; q < len_smeg; q++) { //����� ������ �������
						if (matrix_smezhnosti[j][q] > 1) curent = 1; else
							if (matrix_smezhnosti[j][q] == 1 && j != q) {
								matrix_smezhnosti[j][q] = 0;
								matrix_smezhnosti[q][j] = 0;
								for (k = 0; k < len_smeg; k++)seen[k] = 0;//��������� ������� ���������
								seen[q] = 1; mark[q] = 1; l = find_shortest_way(q + 1, j + 1);
								matrix_smezhnosti[j][q] = 1;
								matrix_smezhnosti[q][j] = 1;
								//cout<<"from "<<j+1<<" to from "<<q+1<<" to "<<j+1<<" get "<<l<<"\n";
								if (l != -1 && l < curent) curent = l;
							}

					}

				if (curent < min) min = curent;
			}
		}
		if (min == len_smeg + 1) min = -1;
		cout << "�� " << i << " ������=" << min + 1 << "\n";//+1 ������ ��� 1 ����� �� ����� ����������
	}
}

void GRAPH::delete_top(int top) {
	int i, j;
	for (i = top; i < len_smeg; i++)
		for (j = 0; j < len_smeg; j++) matrix_smezhnosti[j][i - 1] = matrix_smezhnosti[j][i];
	for (i = top; i < len_smeg; i++)
		for (j = 0; j < len_smeg; j++) matrix_smezhnosti[i - 1][j] = matrix_smezhnosti[i][j];
	len_smeg--;

	translate_smeg_in_inc();
}

void GRAPH::add_top() {
	int i, j;
	int **matrix = new int *[len_smeg];
	for (int i = 0; i < len_smeg; i++) matrix[i] = new int[len_smeg];
	for (i = 0; i < len_smeg; i++)
		for (j = 0; j < len_smeg; j++) matrix[i][j] = matrix_smezhnosti[i][j];
	len_smeg++;
	matrix_smezhnosti = new int *[len_smeg];
	for (int i = 0; i <= len_smeg - 1; i++) matrix_smezhnosti[i] = new int[len_smeg];
	for (i = 0; i < len_smeg - 1; i++) {
		for (j = 0; j <= len_smeg - 1; j++) matrix_smezhnosti[i][j] = matrix[i][j];
	}
	for (i = 0; i < len_smeg; i++) matrix_smezhnosti[len_smeg - 1][i] = 0;
	for (i = 0; i < len_smeg; i++) matrix_smezhnosti[i][len_smeg - 1] = 0;
	//print_matrix(len_smeg, len_smeg, 's');

	translate_smeg_in_inc();
	//translate_smeg_in_rate();
}

void GRAPH::add_way(int from, int to) {
	from--; to--;
	if (from < len_smeg&&to < len_smeg) {
		matrix_smezhnosti[from][to]++;
		if (graf_orientirovan==false)matrix_smezhnosti[to][from]++;
		len_inc_y++;

		translate_smeg_in_inc();
	}
	else cout << "������ ����� ���!\n";
}

void GRAPH::delete_way(int from, int to) {
	from--; to--;
	if (from < len_smeg&&to < len_smeg&&matrix_smezhnosti[from][to] != 0) {
		matrix_smezhnosti[from][to]--;
		matrix_smezhnosti[to][from]--;
		len_inc_y--;

		translate_smeg_in_inc();
	}
	else cout << "������ ����� ���\n";
}

void GRAPH::full() {
	int i, j;
	for (i = 0; i < len_smeg; i++)
		for (j = 0; j < len_smeg; j++) {
			if (matrix_smezhnosti[i][j] == 0) matrix_smezhnosti[i][j] = 1;else matrix_smezhnosti[i][j] = 0;
			if (i == j) matrix_smezhnosti[i][j] = 0;
		}
	translate_smeg_in_inc();
}

void GRAPH::broke_in_half(int from, int to) {
	if (from < len_smeg&&to < len_smeg&&matrix_smezhnosti[from - 1][to - 1] != 0) {
		add_top();
		delete_way(from, to);
		add_way(from, len_smeg);
		add_way(to, len_smeg);

		translate_smeg_in_inc();
	}
	else cout << "������ ����� ���!\n";
}

void GRAPH::double_top(int top) {
	top--;
	if (top < len_smeg) {
		add_top();
		int i;
		for (i = 0; i < len_smeg; i++) {
			if (matrix_smezhnosti[top][i] > 0 && top != i) {
				matrix_smezhnosti[len_smeg - 1][i] = 1;
				matrix_smezhnosti[i][len_smeg - 1] = 1;
			}
		}
		translate_smeg_in_inc();
	}
	else cout << "����� ������� ���!\n";
}

void GRAPH::full_double_top(int top) {
	if (top <= len_smeg) {
		double_top(top);
		add_way(top, len_smeg);
		translate_smeg_in_inc();
	}
	else cout << "����� ������� ���!\n";
}

void GRAPH::make_it_one(int top1, int top2) {
	if (top1 <= len_smeg && top2 <= len_smeg) {
		int i;
		double_top(top1);
		for (i = 0; i < len_smeg - 1; i++) {
			if (matrix_smezhnosti[top2 - 1][i] > 0 && (top2 - 1) != i) {
				matrix_smezhnosti[len_smeg - 1][i] = 1;
				matrix_smezhnosti[i][len_smeg - 1] = 1;
			}
		}
		if (top1 > top2) {
			i = top1;
			top1 = top2;
			top2 = i;
		}
		delete_top(top1);
		delete_top(top2 - 1);//������ ��� ����� ������� �������� ����� ������ ����������� �� 1
	}
	else cout << "������� �������, ������� ���!\n";
}

void GRAPH::full_make_it_one(int *tops, int len) {
	if (len >= 2) {
		int i, start = len_smeg;
		add_top();
		for (i = 0; i < len; i++) {
			if (tops[i] <= start) {
				make_it_one(tops[i], len_smeg);
				for (int j = i + 1; j < len; j++) tops[j]--;
			}
			else {
				cout << "������� �������, ������� ���!\n"; return;
			}
		}
	}

}

void GRAPH::unite(GRAPH B) {
	int i,j,len=len_smeg, control;
	for (i = 0; i < B.len_smeg; i++) add_top();
	for (i = 0; i < B.len_smeg; i++)
		for (j = i; j < B.len_smeg; j++)
			if (B.matrix_smezhnosti[i][j] > 0) {
				if(i==j) control= B.matrix_smezhnosti[i][j]/2; else control=B.matrix_smezhnosti[i][j];
				for (int k = 0; k < control; k++) add_way(i + len + 1, j + len + 1);
			}
	translate_smeg_in_inc();
	return;
}

void GRAPH::unite_full(GRAPH B) {
	int i, j,len=len_smeg;
	unite( B);
	for (i = len; i < len_smeg; i++)
		for (j = 0; j < len; j++) matrix_smezhnosti[i][j] = 1;
	for (i = 0; i < len; i++)
		for (j = len; j < len_smeg; j++) matrix_smezhnosti[i][j] = 1;

	translate_smeg_in_inc();
	return;
}

void GRAPH::equl(GRAPH B) {
	int len1 = len_smeg, len2 = B.len_smeg,i,j,k;

	for (k = 1; k < len2; k++) {
		int len = len_smeg;
		for (i = 0; i < len1; i++) add_top();
		for (i = 0; i < len1; i++)
			for (j = i; j < len1; j++)
				if (matrix_smezhnosti[i][j] > 0) add_way(i + len + 1, j + len + 1);

	}
	for (i = 0; i < len2; i++)
		for (j = 0; j < len2; j++)
			if (B.matrix_smezhnosti[i][j] > 0)
				for(k=0;k<len1;k++) matrix_smezhnosti[i*len1+k][j*len1+k] += B.matrix_smezhnosti[i][j];
	translate_smeg_in_inc();
	return;
}

void GRAPH::print_in(GRAPH A,char c) {
	FILE *f;
	if (c == 's') {
		char name[] = "m_smegnosti.txt";
		f=fopen(name, "w");
		for (int i = 0; i < A.len_smeg; i++) {
			for (int j = 0; j < A.len_smeg; j++) { fprintf(f,"%c ",A.matrix_smezhnosti[i][j] + 48); }
			fprintf(f, "\n");
		}
		fclose(f);
	}
	if (c == 'i') {
		char name[] = "m_incedentnosti.txt";
		f = fopen(name, "w");
		for (int i = 0; i < A.len_inc_y; i++) {
			for (int j = 0; j < A.len_inc_x; j++) { fprintf(f, "%c ", A.matrix_incedent[i][j] + 48); }
			fprintf(f,"\n");
		}
		fclose(f);
	}
	if (c == 'r') {
		char name[] = "s_rate.txt";
		f = fopen(name, "w");
		for (int i = 0; i < A.len_smeg; i++) {
			for (int j = 0; j < A.len_smeg; j++) if (A.matrix_rate[i][j] != 0) { fprintf(f,"%c ",A.matrix_rate[i][j] + 48); }
			fprintf(f, "\n");
		}
		fclose(f);
	}
	return;
}

void GRAPH::in_file(GRAPH A) {
	int res = 0;
	cout << "������� ������� ���������? (0-���, 1-��)\n";
	cin >> res;
	if (res == 1) print_in(A,'s');
	cout << "������� ������� �������������? (0-���, 1-��)\n";
	cin >> res;
	if (res == 1) print_in(A,'i');
	cout << "������� ������ ���������? (0-���, 1-��)\n";
	cin >> res;
	if (res == 1) print_in(A,'r');
	return;
}

void GRAPH::stoks() {
	if (graf_orientirovan == false) { cout << "���� �� �������������!!\n"; return; }
	int vertical = 0, horizont = 0, colv = 0;;
	cout << "�����: ";
	for (int i = 0; i < len_smeg; i++) {
		vertical = 0; horizont = 0;
		for (int j = 0; j < len_smeg; j++) {
			vertical += matrix_smezhnosti[i][j];//����� � ������
			horizont += matrix_smezhnosti[j][i];//����� � �������
		}
		if (vertical == 0 && horizont != 0) {
			cout << i + 1; colv++;
		}
	}
	if (colv == 0) cout << "������ ���!\n"; else cout << "\n";
	return;
}

void GRAPH::isstoks() {
	if (graf_orientirovan == false) { return; }
	int vertical = 0, horizont = 0, colv = 0;;
	cout << "������: ";
	for (int i = 0; i < len_smeg; i++) {
		vertical = 0; horizont = 0;
		for (int j = 0; j < len_smeg; j++) {
			vertical += matrix_smezhnosti[i][j];//����� � ������
			horizont += matrix_smezhnosti[j][i];//����� � �������
		}
		if (vertical != 0 && horizont == 0) {
			cout << i + 1; colv++;
		}
	}
	if (colv == 0) cout << "������� ���!\n"; else cout << "\n";
	return;
}

int GRAPH::finder_wfs(int *check, int kolv, int mark, int from) {
	int i, j, cur = -1;

	mark++;
	for (j = 0; j < len_smeg; j++)
		if (check[j] == mark - 1)
			for (i = 0; i < len_smeg; i++) {
				if (matrix_smezhnosti[j][i] != 0 && !check[i]) {
					check[i] = mark; matrix_weight_ost[j][i]= matrix_weight_ost[i][j] = 1; kolv++;
					cur = i;
				}

			}
	if (cur != -1)kolv = finder_wfs(check, kolv, mark, cur); else return kolv;
	return kolv;
}

void GRAPH::wfs() {
	exc();
	int kolv = 0, mark = 0, i, j=0, cur;
	for (i = 0; i < len_smeg; i++) if (cs[i] == cs[0]) kolv++;

	matrix_weight_ost = new int *[kolv];
	for (int i = 0; i < kolv; i++) matrix_weight_ost[i] = new int[kolv];
	for (i = 0; i < kolv; i++) for (j = 0; j < kolv; j++) matrix_weight_ost[i][j] = 0;
	int col = kolv;

	kolv = 0;
	int *check = new int[len_smeg];
	for (int i = 0; i < len_smeg; i++) check[i] = 0;

	mark = 1; check[0] = mark; kolv++;
	kolv = finder_wfs(check, kolv, mark, 0);
	//for (i = 0; i < len_smeg; i++)cout << i << " " << check[i] << "\n";
//����� �� �����
	for (int i = 0; i < col; i++)
		if (col >= 10) {
			if (i == 0)cout << "    ";
			if (i <= 8) cout << i + 1 << "  "; else cout << i + 1 << " ";
		}
		else {
			if (i == 0)cout << "  " << i + 1; else cout << " " << i + 1;
		}
	cout << "\n";
	for (int i = 0; i < col; i++) {
		if (i <= 8) cout << i + 1 << " "; else cout << i + 1;
		for (int j = 0; j < col; j++) {
			if (matrix_weight_ost[i][j] >= 10 || matrix_weight_ost[i][j] < 0) cout << " " << matrix_weight_ost[i][j]; else
				cout << "  " << matrix_weight_ost[i][j];
		}
		cout << "\n";
	}

}

int GRAPH::finder_prim(int *choce, int mark, int kolv) {
	int i, j, cur = -1, y, min;
	mark++;
	for (i = 0; i < len_smeg; i++) {
		if (choce[i] != 0) {
			for (j = 0; j < len_smeg; j++)
				if (matrix_weight[i][j] != 0 && choce[j] == 0)
				{
					if (cur == -1) {
						min = matrix_weight[i][j]; cur = j; y = i;
					}
					else {
						if (matrix_weight[i][j] < min) {
							cur = j; y = i, min = matrix_weight[i][j];
						}
					}
				}
		}
	}
	if (cur > -1) {
		choce[cur] = mark; kolv++; weight += matrix_weight[y][cur];
		matrix_weight_ost[y][cur] = 1;
		matrix_weight_ost[cur][y] = 1;
		kolv = finder_prim(choce, mark, kolv);
	}
	else return kolv;
	return kolv;
}

void GRAPH::prim() {
	cout << "\n";
	int i, j, kolv = 0;
	weight = 0;
	int *choce = new int[len_smeg];

	matrix_weight_ost = new int *[len_smeg];
	for (int i = 0; i < len_smeg; i++) matrix_weight_ost[i] = new int[len_smeg];

	int col= 2147483647, cur;
	int mark = 1,k;
//������� � ����� ������� ���� ������, ����� �������� ����� ��� ����
	for (i = 0; i < len_smeg; i++){
		for (j = 0; j < len_smeg; j++) choce[j] = 0;
		for (j = 0; j < len_smeg; j++) for (k = 0; k < len_smeg; k++) matrix_weight_ost[j][k] = 0;
			choce[i] = 1; kolv=1; weight = 0; mark = 1;
			kolv = finder_prim(choce, mark, kolv);
			if (weight < col) { cur = i; col = weight; }
	}
//��������� �������� � ��������� �������, ����� �������� ������� ����������� �������
	weight = 0;
	for (i = 0; i < len_smeg; i++) choce[i] = 0;
	for (i = 0; i < len_smeg; i++) for (j = 0; j < len_smeg; j++) matrix_weight_ost[i][j] = 0;
	choce[cur] = 1; kolv++; weight = 0;
	kolv = finder_prim(choce, mark, kolv);
//����� �� �����
	cout << "���: " << weight<<"\n";
	for (int i = 0; i < len_smeg; i++)
		if (len_smeg >= 10) {
			if (i == 0)cout << "    ";
			if (i <= 8) cout << i + 1 << "  "; else cout << i + 1 << " ";
		}
		else {
			if (i == 0)cout << "  " << i + 1; else cout << " " << i + 1;
		}
	cout << "\n";
	for (int i = 0; i < len_smeg; i++) {
		if (len_smeg >= 10) {
			if (i <= 8) cout << i + 1 << " "; else cout << i + 1;
		}
		for (int j = 0; j < len_smeg; j++) {
			if (matrix_weight_ost[i][j] >= 10 || matrix_weight_ost[i][j] < 0) cout << " " << matrix_weight_ost[i][j]; else
				cout << "  " << matrix_weight_ost[i][j];
		}
		cout << "\n";
	}
}

void GRAPH::read_weight(char str[]) {
	FILE *file;
	file = fopen(str, "r");
	int count = 0, line = 0, check = 0;
	char c;
	c = fgetc(file);
	while (c != '\n' && !feof(file)) {//������ ������� ������
		if (c == ' ')count++;
		c = fgetc(file);
	}
	fclose(file);

	len_smeg = count + 1;
	matrix_weight = new int *[count + 1];
	for (int i = 0; i <= count; i++) matrix_weight[i] = new int[count + 1];

	int rez = 0, number = 0, minus = 0;
	len_smeg = count + 1;

	file = fopen(str, "r");
	count = 0;
	while (!feof(file)) {
		c = fgetc(file);
		if (((int)c >= 48 && (int)c <= 57) || c == ' ' ||c=='-'|| c == '\n' || c == -1) {
			if (c == '\n' || c == -1) {
				if (minus == 1) rez *= -1; minus = 0;
				matrix_weight[count][line] = rez;
				rez = 0; count++; line = 0;
			}
			if (c == ' ') {
				if (minus == 1) rez *= -1; minus = 0;
				matrix_weight[count][line] = rez;
				line++; rez = 0;
			}
			if (((int)c >= 48 && (int)c <= 57)) { rez = rez * 10 + (int)c - 48; }
			if (c == '-') minus = 1;
			if (check == 1) break;
		}
		else {
			check = 1; break;
		}
	}
	fclose(file);
	if (check != 1) {
		//cout << "here\n";
		for (int i = 0; i < len_smeg; i++)
			for (int j = 0; j < len_smeg; j++) if (matrix_weight[i][j] != matrix_weight[j][i]) { cout << i << " " << j; check = 1; }
	}
	if (check == 1) { cout << "������ � �����!\n"; return; }
	excent = new int[len_smeg];
	for (int k = 0; k < len_smeg; k++)	excent[k] = 0;
	for (int i = 0; i < len_smeg; i++)
		if (len_smeg >= 10) {
			if (i == 0)cout << "    ";
			if (i <= 8) cout << i + 1 << "  "; else cout << i + 1<<" ";
		}
		else {
			if (i == 0)cout << "  " << i + 1; else cout << " " << i + 1;
		}
	cout << "\n";
	for (int i = 0; i < len_smeg; i++) {
		if (len_smeg >= 10) {
			if (i <= 8) cout << i + 1 << " "; else cout<<i+1;
		}
		for (int j = 0; j < len_smeg; j++) {
			if(matrix_weight[i][j]>=10|| matrix_weight[i][j]<0) cout << " " << matrix_weight[i][j]; else
			cout << "  " << matrix_weight[i][j];
		}
		cout << "\n";
	}
}

int graph = 0;
GRAPH A,B,C;

void ad(int opt)
{
	char name[100];
L:;
	cout << "������� ���: ";
	cin >> name;
	FILE *S;
	S = fopen(name, "r");
	while (!S) {
		if (!S)
		{
			cout << "����� �� ����������, ���������� �����\n";
			goto L;
		}
		cout << "������� ���: ";
		cin >> name;
		S = fopen(name, "r");
	}
	fclose(S);
	cout << "����� ���������������? (1-��, ���������-���)\n";
	int o=0;
	cin >> o;
	if (o == 1) A.graf_orientirovan = true; else A.graf_orientirovan = false;
	system("cls");
	if (graph == 0) {
		if (opt == 1) { A.read_smegnost(name); }
		else if (opt == 2) { A.read_incedent(name); }
		else { A.read_rate(name); }
	}
	else {
		if (opt == 1) B.read_smegnost(name);
		else if (opt == 2) B.read_incedent(name); else B.read_rate(name);
	}
	graph++;
	return;
}

void ad_weight() {
	char name[100];
L2:;
	cout << "������� ���: ";
	cin >> name;
	FILE *S;
	S = fopen(name, "r");
	while (!S) {
		if (!S)
		{
			cout << "����� �� ����������, ���������� �����\n";
			goto L2;
		}
		cout << "������� ���: ";
		cin >> name;
		S = fopen(name, "r");
	}
	fclose(S);
	C.read_weight(name);
}

int main()
{
	setlocale(LC_ALL, "Russian");
	int opt = 0,num;
	while (1)
	{
		switch (opt)
		{
		case 0:
			cout << "1  - ����������� ����� ������\n";
			cout << "2  - ����������� ����� ����� (���)\n";
			cout << "3  - ����������� ������� �������\n";
			cout << "4  - ����������� ��������� ������������������ �����\n";
			cout << "5  - ����������� ������� ���������\n";
			cout << "6  - ����������� ������� �������������\n";
			cout << "7  - ����������� ������ ���������\n";
			cout << "8  - ����������� ������� � ������������� ������\n";
			cout << "9  - ����������� � ������� ������� � ������\n";
			cout << "10 - ����������� ���������� ����� ����� ���������\n";
			cout << "11 - ����������� ��������������� �������\n";
			cout << "12 - ����������� �������� � ������� ����� (��� ���� ��������� ���������)\n";
			cout << "13 - ����������� ��������� � ������ ����� (��� ���� ��������� ���������)\n";
			cout << "14 - ����������� ������� �����\n";
			cout << "15 - ���������� ������� � ����\n";
			cout << "16 - �������� ������� � ����\n";
			cout << "17 - ���������� ����� � ����\n";
			cout << "18 - �������� ����� � ����\n";
			cout << "19 - ����������� ���������� �����\n";
			cout << "20 - ������������ �����\n";
			cout << "21 - ���������� �����\n";
			cout << "22 - �������������� ������\n";
			cout << "23 - ������������ �������\n";
			cout << "24 - ����������� �������\n";
			cout << "25 - ����������� (�������������) ������\n";
			cout << "26 - ���������� ������\n";
			cout << "27 - ������������ ������\n";
			cout << "28 - ���� ������ �� ���������� �����\n";
			cout << "29 - ����� ������ � ��������� ����\n";
			cout << "30 - ����� ����������� �����\n";
			cout << "31 - ����� �����\n";
			cout << "��� �����: ";
			cin >> opt;
			break;
		case 1:
			cout << "1  - ����������� ����� ������\n";
			if (graph < 1) {
				cout << "������� ������� ����\n";
				opt = 28;
			}
			else {
				if (graph == 1) {
					cout << A.len_smeg<<"\n";
				}
				else {
					cout << "������� 2 �����, ��� ������ ��������� ��������?\n";
					cin >> num;
					if (num == 1) cout << A.len_smeg<<"\n"; else if (num == 2) cout << B.len_smeg<<"\n";
				}
				cout << "0 - ����� � ����: ";
				cin >> opt;
			}
			break;
		case 2:
			cout << "2  - ����������� ����� ����� (���)\n";
			if (graph < 1) {
				cout << "������� ������� ����\n";
				opt = 28;
			}
			else {
				if (graph == 1) {
					cout << A.len_inc_x << "\n";
				}
				else {
					cout << "������� 2 �����, ��� ������ ��������� ��������?\n";
					cin >> num;
					if (num == 1) cout << A.len_inc_x << "\n"; else if (num == 2) cout << B.len_inc_x << "\n";
				}
				cout << "0 - ����� � ����: ";
				cin >> opt;
			}
			break;
		case 3:
			cout << "3  - ����������� ������� �������\n";
			if (graph < 1) {
				cout << "������� ������� ����\n";
				opt = 28;
			}
			else {
				cout << "������� ����� �������?\n";
				int top;
				cin >> top;
				if (graph == 1) {
					while (top > A.len_smeg&&top!=-10) {
						cout << "����� ������� ���!\n������� ������ ��� ������� (-10), ���� ���������� ������ ������� �������:)\n";
						cin >> top;
					}
					if (top != -10) {
						system("cls");
						cout << A.create_rate_top(top) << "\n";
					}
				}
				else {
					cout << "������� 2 �����, ��� ������ ��������� ��������?\n";
					cin >> num;
					if (num == 1) cout << A.create_rate_top(top) << "\n"; else if (num == 2) cout << B.create_rate_top(top) << "\n";
				}
				cout << "0 - ����� � ����: ";
				cin >> opt;
			}
			break;
		case 4:
			cout << "4  - ����������� ��������� ������������������ �����\n";
			if (graph < 1) {
				cout << "������� ������� ����\n";
				opt = 28;
			}
			else {
				if (graph == 1) {
					 A.create_rate_order();
				}
				else {
					cout << "������� 2 �����, ��� ������ ��������� ��������?\n";
					cin >> num;
					if (num == 1) A.create_rate_order(); else if (num == 2) B.create_rate_order();
				}
				cout << "0 - ����� � ����: ";
				cin >> opt;
			}
			break;
		case 5:
			cout << "5  - ������� ���������\n";
			if (graph < 1) {
				cout << "������� ������� ����\n";
				opt = 28;
			}
			else {
				if (graph == 1) {
					A.print_matrix('s');
				}
				else {
					cout << "������� 2 �����, ��� ������ ��������� ��������?\n";
					cin >> num;
					if (num == 1) A.print_matrix( 's'); else if (num == 2) B.print_matrix( 's');
				}
				cout << "0 - ����� � ����: ";
				cin >> opt;
			}
			break;
		case 6:
			cout << "6  - ������� �������������\n";
			if (graph < 1) {
				cout << "������� ������� ����\n";
				opt = 28;
			}
			else {
				if (graph == 1) {
					A.print_matrix('i');
				}
				else {
					cout << "������� 2 �����, ��� ������ ��������� ��������?\n";
					cin >> num;
					if (num == 1) A.print_matrix('i'); else if (num == 2) B.print_matrix( 'i');
				}
				cout << "0 - ����� � ����: ";
				cin >> opt;
			}
			break;
		case 7:
			cout << "7  - ������ ���������\n";
			if (graph < 1) {
				cout << "������� ������� ����\n";
				opt = 28;
			}
			else {
				if (graph == 1) {
					A.print_matrix('r');
				}
				else {
					cout << "������� 2 �����, ��� ������ ��������� ��������?\n";
					cin >> num;
					if (num == 1) A.print_matrix('r'); else if (num == 2) B.print_matrix('r');
				}
				cout << "0 - ����� � ����: ";
				cin >> opt;
			}
			break;
		case 8:
			cout << "8  - ����������� ������� � ������������� ������\n";
			if (graph < 1) {
				cout << "������� ������� ����\n";
				opt = 28;
			}
			else {
				if (graph == 1) {
					A.order_onefriend_tops();
					A.order_alone_tops();
				}
				else {
					cout << "������� 2 �����, ��� ������ ��������� ��������?\n";
					cin >> num;
					if (num == 1) { A.order_onefriend_tops(); A.order_alone_tops(); }
					else if(num == 2){ B.order_onefriend_tops(); B.order_alone_tops(); }
				}
				cout << "0 - ����� � ����: ";
				cin >> opt;
			}
			break;
		case 9:
			cout << "9  - ����������� � ������� ������� � ������\n";
			if (graph < 1) {
				cout << "������� ������� ����\n";
				opt = 28;
			}
			else {
				if (graph == 1) {
					A.stoks();
					A.isstoks();
				}
				else {
					cout << "������� 2 �����, ��� ������ ��������� ��������?\n";
					cin >> num;
					if (num == 1) {A.stoks();A.isstoks();}
					else if (num == 2) {B.stoks();B.isstoks();}
				}
				cout << "0 - ����� � ����: ";
				cin >> opt;
			}
			break;
		case 10:
			cout << "10 - ����������� ���������� ����� ����� ���������\n";
			if (graph < 1) {
				cout << "������� ������� ����\n";
				opt = 28;
			}
			else {
				int top1, top2;
				cout << "������� ������� 1: ";
				cin >> top1;
				cout << "������� ������� 2: ";
				cin >> top2;
				if (graph == 1) {
					if ((top1 > 0 && top1 < A.len_smeg) && (top2 > 0 && top2 < A.len_smeg)) cout<<"���� ����� ���������: "<<A.find_shortest_way(top1, top2)<<"\n";
					else cout << "������� �������������� �������!\n";
				}
				else {
					cout << "������� 2 �����, ��� ������ ��������� ��������?\n";
					cin >> num;
					if (num == 1) {
						if ((top1 > 0 && top1 < A.len_smeg) && (top2 > 0 && top2 < A.len_smeg)) cout << "���� ����� ���������: " << A.find_shortest_way(top1, top2) << "\n";
						else cout << "������� �������������� �������!\n";
					}
					else {
						if (num == 2&&(top1 > 0 && top1 < B.len_smeg) && (top2 > 0 && top2 < B.len_smeg)) cout << "���� ����� ���������: " << B.find_shortest_way(top1, top2) << "\n";
						else cout << "������� �������������� �������!\n";
					}
				}
				cout << "0 - ����� � ����: ";
				cin >> opt;
			}
			break;
		case 11:
			cout << "11 - ����������� ��������������� �������\n";
			if (graph < 1) {
				cout << "������� ������� ����\n";
				opt = 28;
			}
			else {
				if (graph == 1) {
					A.exc();
					for (int i = 0; i < A.len_smeg; i++) cout << "exc(" << i + 1 << ")=" << A.exce[i] << "\n";
				}
				else {
					cout << "������� 2 �����, ��� ������ ��������� ��������?\n";
					cin >> num;
					if (num == 1) { A.exc(); for (int i = 0; i < A.len_smeg; i++) cout <<"exc("<<i+1<<")="<< A.exce[i] << "\n";
					}
					else if (num == 2) { B.exc(); for (int i = 0; i < B.len_smeg; i++) cout << "exc(" << i + 1 << ")=" << B.exce[i] << "\n";
					}
				}
				cout << "0 - ����� � ����: ";
				cin >> opt;
			}
			break;
		case 12:
			cout << "12 - ����������� �������� � ������� ����� (��� ���� ��������� ���������)\n";
			if (graph < 1) {
				cout << "������� ������� ����\n";
				opt = 28;
			}
			else {
				if (graph == 1) {
					A.exc();
					A.find_diametr();
					for (int i = 0; i < A.kss; i++) cout << "��" << i + 1 << " �������: " << A.diametr[i] << "; ������: " << A.radius[i] << "\n";
				}
				else {
					cout << "������� 2 �����, ��� ������ ��������� ��������?\n";
					cin >> num;
					if (num == 1) { A.exc(); A.find_diametr(); for (int i = 0; i < A.kss; i++) cout << "��"<<i+1<<" �������: "<<A.diametr[i]<<"; ������: "<<A.radius[i]<<"\n"; }
					else if (num == 2) { B.exc(); B.find_diametr(); for (int i = 0; i < B.kss; i++) cout << "��" << i + 1 <<" �������: " << B.diametr[i] << "; ������: " << B.radius[i] << "\n";
					}
				}
				cout << "0 - ����� � ����: ";
				cin >> opt;
			}
			break;
		case 13:
			cout << "13 - ����������� ��������� � ������ ����� (��� ���� ��������� ���������)\n";
			if (graph < 1) {
				cout << "������� ������� ����\n";
				opt = 28;
			}
			else {
				if (graph == 1) {
					A.exc(); A.find_diametr(); A.find_peref();
				}
				else {
					cout << "������� 2 �����, ��� ������ ��������� ��������?\n";
					cin >> num;
					if (num == 1) { A.exc(); A.find_diametr(); A.find_peref(); }
					else if (num == 2) { B.exc(); B.find_diametr(); B.find_peref(); }
				}
				cout << "0 - ����� � ����: ";
				cin >> opt;
			}
			break;
		case 14:
			cout << "14 - ����������� ������� �����\n";
			if (graph < 1) {
				cout << "������� ������� ����\n";
				opt = 28;
			}
			else {
				if (graph == 1) {
					A.exc(); A.compass();
				}
				else {
					cout << "������� 2 �����, ��� ������ ��������� ��������?\n";
					cin >> num;
					if (num == 1) { A.exc(); A.compass(); }
					else if (num == 2) { B.exc(); B.compass(); }
				}
				cout << "0 - ����� � ����: ";
				cin >> opt;
			}
			break;
		case 15:
			cout << "15 - ���������� ������� � ����\n";
			if (graph < 1) {
				cout << "������� ������� ����\n";
				opt = 28;
			}
			else {
				if (graph == 1) {
					A.add_top(); A.print_matrix('s');
				}
				else {
					cout << "������� 2 �����, ��� ������ ��������� ��������?\n";
					cin >> num;
					if (num == 1) { A.add_top(); A.print_matrix('s'); }
					else if (num == 2) { B.add_top(); B.print_matrix('s'); }
				}
				cout << "0 - ����� � ����: ";
				cin >> opt;
			}
			break;
		case 16:
			cout << "16 - �������� ������� � ����\n";
			if (graph < 1) {
				cout << "������� ������� ����\n";
				opt = 28;
			}
			else {
				int top;
				cout << "������� �������: ";
				cin >> top;
				cout << "";
				if (graph == 1) {
					A.delete_top(top); A.print_matrix('s');
				}
				else {
					cout << "������� 2 �����, ��� ������ ��������� ��������?\n";
					cin >> num;
					if (num == 1) { A.delete_top(top); A.print_matrix('s'); }
					else if (num == 2) { B.delete_top(top); B.print_matrix('s'); }
				}
				cout << "0 - ����� � ����: ";
				cin >> opt;
			}
			break;
		case 17:
			cout << "17 - ���������� ����� � ����\n";
			if (graph < 1) {
				cout << "������� ������� ����\n";
				opt = 28;
			}
			else {
				int top1,top2;
				cout << "������� �������1: ";
				cin >> top1;
				cout << "������� �������2: ";
				cin >> top2;
				if (graph == 1) {
					A.add_way(top1,top2); A.print_matrix('i');
				}
				else {
					cout << "������� 2 �����, ��� ������ ��������� ��������?\n";
					cin >> num;
					if (num == 1) { A.add_way(top1, top2); A.print_matrix('i'); }
					else if (num == 2) { B.add_way(top1, top2); B.print_matrix('i'); }
				}
				cout << "0 - ����� � ����: ";
				cin >> opt;
			}
			break;
		case 18:
			cout << "18 - �������� ����� � ����\n";
			if (graph < 1) {
				cout << "������� ������� ����\n";
				opt = 28;
			}
			else {
				int top1, top2;
				cout << "������� �������1: ";
				cin >> top1;
				cout << "������� �������2: ";
				cin >> top2;
				if (graph == 1) {
					A.delete_way(top1, top2); A.print_matrix('i');
				}
				else {
					cout << "������� 2 �����, ��� ������ ��������� ��������?\n";
					cin >> num;
					if (num == 1) { A.delete_way(top1, top2); A.print_matrix('i');
					}
					else if (num == 2) { B.delete_way(top1, top2); B.print_matrix('i');
					}
				}
				cout << "0 - ����� � ����: ";
				cin >> opt;
			}
			break;
		case 19:
			cout << "19 - ����������� ���������� �����\n";
			if (graph < 1) {
				cout << "������� ������� ����\n";
				opt = 28;
			}
			else {
				if (graph == 1) {
					A.full(); A.print_matrix('s');
				}
				else {
					cout << "������� 2 �����, ��� ������ ��������� ��������?\n";
					cin >> num;
					if (num == 1) {
						A.full(); A.print_matrix('s');
					}
					else if (num == 2) {
						B.full(); B.print_matrix('s');
					}
				}

				cout << "0 - ����� � ����: ";
				cin >> opt;
			}
			break;
		case 20:
			cout << "20 - ������������ �����\n";
			if (graph < 1) {
				cout << "������� ������� ����\n";
				opt = 28;
			}
			else {
				int top1, top2;
				cout << "������� �������1: ";
				cin >> top1;
				cout << "������� �������2: ";
				cin >> top2;
				if (graph == 1) {
					A.broke_in_half(top1, top2); A.print_matrix('s');
				}
				else {
					cout << "������� 2 �����, ��� ������ ��������� ��������?\n";
					cin >> num;
					if (num == 1) { A.broke_in_half(top1, top2); A.print_matrix('s');
					}
					else if (num == 2) { B.broke_in_half(top1, top2); B.print_matrix('s');
					}
				}
				cout << "0 - ����� � ����: ";
				cin >> opt;
			}
			break;
		case 21:
			cout << "21 - ���������� �����\n";
			if (graph < 1) {
				cout << "������� ������� ����\n";
				opt = 28;
			}
			else {
				int len = 0;
				cout << "������� ������ ������ �������?\n";
				cin >> len;
				int *tops = new int[len];
				cout << "������� �������:\n";
				for (int i = 0; i < len; i++) cin >> tops[i];
				if (graph == 1) {
					A.full_make_it_one(tops, len); A.print_matrix('s');
				}
				else {
					cout << "������� 2 �����, ��� ������ ��������� ��������?\n";
					cin >> num;
					if (num == 1) { A.full_make_it_one(tops, len); A.print_matrix('s');
					}
					else if (num == 2) { B.full_make_it_one(tops, len); B.print_matrix('s');
					}
				}
				cout << "0 - ����� � ����: ";
				cin >> opt;
			}
			break;
		case 22:
			cout << "22 - �������������� ������\n";
			if (graph < 1) {
				cout << "������� ������� ����\n";
				opt = 28;
			}
			else {
				int top1, top2;
				cout << "������� �������1: ";
				cin >> top1;
				cout << "������� �������2: ";
				cin >> top2;
				if (graph == 1) {
					A.make_it_one(top1, top2); A.print_matrix('s');
				}
				else {
					cout << "������� 2 �����, ��� ������ ��������� ��������?\n";
					cin >> num;
					if (num == 1) { A.make_it_one(top1, top2); A.print_matrix('s');
					}
					else  if (num == 2) { B.make_it_one(top1, top2); B.print_matrix('s');
					}
				}
				cout << "0 - ����� � ����: ";
				cin >> opt;
			}
			break;
		case 23:
			cout << "23 - ������������ �������\n";
			if (graph < 1) {
				cout << "������� ������� ����\n";
				opt = 28;
			}
			else {
				int top1;
				cout << "������� �������, ������� ����� �����������: ";
				cin >> top1;
				if (graph == 1) {
					A.double_top(top1); A.print_matrix('s');
				}
				else {
					cout << "������� 2 �����, ��� ������ ��������� ��������?\n";
					cin >> num;
					if (num == 1) { A.double_top(top1); A.print_matrix('s');
					}
					else  if (num == 2) { B.double_top(top1); A.print_matrix('s'); }
				}
				cout << "0 - ����� � ����: ";
				cin >> opt;
			}
			break;
		case 24:
			cout << "24 - ����������� �������\n";
			if (graph < 1) {
				cout << "������� ������� ����\n";
				opt = 28;
			}
			else {
				int top1;
				cout << "������� �������, ������� ����� ����������: ";
				cin >> top1;
				if (graph == 1) {
					A.full_double_top(top1); A.print_matrix('s');
				}
				else {
					cout << "������� 2 �����, ��� ������ ��������� ��������?\n";
					cin >> num;
					if (num == 1) { A.full_double_top(top1); A.print_matrix('s'); }
					else if (num == 2) { B.full_double_top(top1); B.print_matrix('s'); }
				}
				cout << "0 - ����� � ����: ";
				cin >> opt;
			}
			break;
		case 25:
			cout << "25 - ����������� (�������������) ������\n";
			if (graph < 2) {
				cout << "������ ���� ������� 2 �����\n";
				opt = 28;
			}
			else {
				A.unite(B); A.print_matrix('s');
				cout << "0 - ����� � ����: ";
				cin >> opt;
			}
			break;
		case 26:
			cout << "26 - ���������� ������\n";
			if (graph < 2) {
				cout << "������� ������� ����\n";
				opt = 28;
			}
			else {
				A.unite_full(B); A.print_matrix('s');
				cout << "0 - ����� � ����: ";
				cin >> opt;
			}
			break;
		case 27:
			cout << "27 - ������������ ������\n";
			if (graph < 2) {
				cout << "������� ������� ����\n";
				opt = 28;
			}
			else {
				A.equl(B); A.print_matrix('s');
				cout << "0 - ����� � ����: ";
				cin >> opt;
			}
			break;
		case 28:
			cout << "28 - ���� ������ �� ���������� �����\n";
			cout << "������ �� ����� ������� ���������(1), ������������� (2), ������ ��������� (3)?\n";
			cin >> opt;
			if(opt>=1&&opt<=3)ad(opt);
			cout << "0 - ����� � ����: ";
			cin >> opt;
			break;
		case 29:
			cout << "29 - ����� ������ � ��������� ����\n";
			if (graph < 1) {
				cout << "������� ������� ����\n";
				opt = 28;
			}
			else {
				if (graph == 1) {
					A.in_file(A);
				}
				else {
					cout << "������� 2 �����, ��� ������ ��������� �������� (3-������� ���)?\n";
					cin >> num;
					if (num == 1) A.in_file(A); else if (num==2) B.in_file(B); else if(num==3){ A.in_file(A); B.in_file(B);}
				}
				cout << "0 - ����� � ����: ";
				cin >> opt;
			}
			break;
		case 30:
			cout << "30 - ����� ������������ ����\n";
			cout << "������ ������� �����(1) ��� ��������� ��� ��� ���������� �����(2)?\n";
			cin >> opt;
			if (opt == 1) {
				ad_weight(); C.prim();
			}
			if (opt == 2) {
				if (graph == 0) {
					opt = 28;
				}
				else {
					if (graph == 1) {
						A.prim();
					}
					else {
						int num;
						cout << "������� 2 �����, ��� ������ ��������� �������� (3-������� ���)?\n";
						cin >> num;
						if (num == 1) A.prim(); else B.prim();
					}
				}
			}
			if (opt < 1 || opt>2) cout << "���������� �������!\n";
			cout << "0 - ����� � ����: ";
			cin >> opt;
			break;
		case 31:
			cout << "31 - ����� �����\n";
			if (graph < 1) {
				cout << "������� ������� ����\n";
				opt = 28;
			}
			else {
				if (graph == 1) {
					A.wfs();
				}
				else {
					cout << "������� 2 �����, ��� ������ ��������� ��������?\n";
					cin >> num;
					if (num == 1) A.wfs();
					else if (num == 2) B.wfs();
				}
				cout << "0 - ����� � ����: ";
				cin >> opt;
			}
			break;
		default:
			cout << "�������� �����!\n0 - ����� � ����: ";
			cin >> opt;
			break;
		}
		system("cls");
	}
	return 0;
}
