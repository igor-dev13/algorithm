#include "stdafx.h"

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

/*
В файле дерево в виде
Ann
.Boris
..Cat
..Dora
...Eva
...Fred
..Goga
...Helmut
....Marta
.....Bred
.....Stiv
.....Jon
......Tom
...Nata
.Nina

5. Имеется дерево, корень которого соответствует основателю
рода.  Сыновья  каждой  вершины  задают  сыновей   и   дочерей
соответствующего    человека.   Указывается   имя   некоторого
человека.  Требуется выдать имена его детей,  внуков, сестер и
братьев, одного из родителей, дедушки или бабушки (9).
*/

const int DL = 20;          // максимальная длина
const int LEVEL_FATHER = 1;
const int LEVEL_GRAND_FATHER = 2;
const int LEVEL_GRAND_CHILDREN = 2;

using namespace std;

struct Tree
{
	char name[DL];
	int urov;
	Tree *fath;         // отец в исходном дереве
	Tree *left;
	Tree *right;
};

int readFromFile(FILE *F, Tree **r);  // чтение из файла, формирование дерева
void backFromBin(Tree *p);            // выдача исходное дерева из бинарного 

void findRelatives(Tree *p, Tree *root, string name, bool *isPersonExist);
void findOlder(Tree *p, Tree *root, int levelFather, int levelGrandFather, char * seachingName);
void findEqual(Tree *p, Tree *root, int level, char * seachingName, char * seachingNameDefault, string status);
void findYoung(Tree *p, string status, int maxlevel);
void printEqual(Tree *p, string name, string status);

int main(int argc, char* argv[])
{
	Tree *root = 0;
	FILE *Fin;
	bool isPersonExist = false;
	string name;
	setlocale(LC_ALL, "rus");
	
	if (argc != 2)
	{
		printf("\nNumber parameters is wrong");
		getchar();
		return -1;
	}
	
	Fin = fopen(argv[1], "r");
	
	if (Fin == NULL)
	{
		printf("\nCan not open file %s", argv[1]);
		getchar();
		return -1;
	}

	readFromFile(Fin, &root);
	printf("Бинарое дерево построено\n");
	printf("Вывод из бинарного дерева сильно ветвящегося\n");
	backFromBin(root);
	
	cout << endl << "Введите имя человека" << endl << endl << ">";
	getline(cin, name);
	cout << endl;

	if (name == "")
	{
		cout << "Неверное имя" << endl;
		return EXIT_FAILURE;
	}

	findRelatives(root, root, name, &isPersonExist);

	if (!isPersonExist)
	{
		cout << "Имя в дерене не найдено" << endl;
		return EXIT_FAILURE;
	}

	getchar();
	return EXIT_SUCCESS;
}

int readFromFile(FILE *F, Tree **r)
{
	char buf[DL];
	int i, k, m, len;
	Tree *p, *q, *t;       // *root = 0 
	m = 0;                 // уровень вершины
	t = 0;
	while (!feof(F))
	{
		k = fscanf(F, "%s", buf);
		len = strlen(buf);
		if (len == 0) break;            // если конец файла в следующей строке
		k = 0;
		while (buf[k] == '.') k++;     // k-уровень вершины
		p = new Tree;
		strncpy(p->name, &buf[k], len - k + 1);  // 0-(k-1) - точки, (k-1)-(len-1) - имя, \0 - конец строки
		buf[0] = '\0';                  // если конец файла в следующей строке
		p->urov = k;
		p->left = 0;
		p->right = 0;
		if (k == 0)                    // корень
		{
			*r = p;
			t = *r;
			continue;
		}
		if (k > m)                  // переход на следующий уровень
		{
			t->left = p;
			p->fath = t;
		}
		else if (k == m)            // тот же уровень
		{
			t->right = p;
			p->fath = t->fath;  // отец тот же
		}
		else        // подъем по дереву на m-k уровней
		{
			q = t;
			for (i = 0; i < m - k; i++) q = q->fath;
			// q - предыдущая вершина того же уровня
			p->fath = q->fath;   // отец тот же, что у брата
			q->right = p;
		}
		m = k;      // текущий уровень
		t = p;      // текущая вершина
	}
	fclose(F);
	return 0;
}

void backFromBin(Tree *p)
{
	int i, j;
	char st[DL];
	if (p)
	{
		for (i = 0; i < p->urov; i++) st[i] = '.';
		j = 0;
		while (st[i++] = (p->name)[j++]);
		printf("%s\n", st);
		backFromBin(p->left);
		backFromBin(p->right);
	}
}

void printEqual(Tree *p, string name, string status)
{
	if (p)
	{
		if (p->name != name)
		{
			cout << status.c_str() << " - " << p->name << endl;
		}

		if (p->right != NULL)
		{
			printEqual(p->right, name, status);
		}
	}
}

void findYoung(Tree *p, string status, int maxlevel)
{
	if (p)
	{
		if (p->urov <= maxlevel)
		{
			cout << status.c_str() << " - " << p->name << endl;
		}

		if (p->left != NULL)
		{
			findYoung(p->left, "внук/внучка", maxlevel);
		}

		if (p->right != NULL)
		{
			findYoung(p->right, status, maxlevel);
		}
	}
}

void findEqual(Tree *p, Tree *root, int level, char * seachingName, char * seachingNameDefault, string status)
{
	if (p)
	{
		string name(seachingName);

		if (p->left != NULL && p->left->name == name)
		{
			printEqual(p->left, seachingNameDefault, "сестра/брат");
		}
		else if (p->right != NULL && p->right->name == name)
		{
			findEqual(root, root, level, p->name, seachingNameDefault, "сестра/брат");
		}
		else
		{
			findEqual(p->left, root, level, seachingName, seachingNameDefault, status);
			findEqual(p->right, root, level, seachingName, seachingNameDefault, status);
		}
	}
}

void findOlder(Tree *p, Tree *root, int levelFather, int levelGrandFather, char * seachingName)
{
	if (p)
	{
		string name(seachingName);

		if (p->left != NULL && p->left->name == name)
		{			
			if (p->urov == levelFather)
			{
				cout << "отец/мать - " << p->name << endl;
			}

			if (p->urov == levelGrandFather)
			{
				cout << "бабушка/дедушка - " << p->name << endl;
			}

			findOlder(root, root, levelFather, levelGrandFather, p->name);
		}
		else if (p->right != NULL && p->right->name == name)
		{
			findOlder(root, root, levelFather, levelGrandFather, p->name);
		}
		else
		{
			findOlder(p->left, root, levelFather, levelGrandFather, seachingName);
			findOlder(p->right, root, levelFather, levelGrandFather, seachingName);
		}
	}
}

void findRelatives(Tree *p, Tree *root, string name, bool *isPersonExist)
{
	if (p)
	{
		if (p->name == name)
		{
			*isPersonExist = true;
			cout << "искомый человек - " << p->name << endl;			

			int levelFather = (int)p->urov - LEVEL_FATHER;
			int levelGrandfather = (int)p->urov - LEVEL_GRAND_FATHER;
			int levelGrandChildren = (int)p->urov + LEVEL_GRAND_CHILDREN;

			findOlder(root, root, levelFather, levelGrandfather, p->name);
			findEqual(root, root, p->urov, p->name, p->name, "сестра/брат");

			if (p->left != NULL)
			{
				findYoung(p->left, "сын/дочь", levelGrandChildren);
			}
		}

		findRelatives(p->left, root, name, isPersonExist);
		findRelatives(p->right, root, name, isPersonExist);
	}
}