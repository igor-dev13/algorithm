#include "stdafx.h"
#include <stdlib.h>
#include <string>
#include <fstream>
#include <iostream>
#include <windows.h>

#define FILENAME "input.txt"

using namespace std;

struct Node //Структура являющаяся звеном списка
{
	string x;     
	Node *Next, *Prev; 
};

class List
{
	int size;
	Node *Head, *Tail, *CurPoint;
public:
	List() :Head(NULL), Tail(NULL), CurPoint(NULL) {}; //Инициализируем адреса как пустые
									 
	void Show(const int count); 
	void Add(string x); 

	int FindCaptain(int n, int k);
	void ClearList();	
	void DropStudent(int k);
};

void List::Add(string x)
{
	static int count = 0;
	Node *temp = new Node;
	temp->Next = NULL;
	temp->x = x;

	if (Head != NULL) //Если список не пуст
	{
		temp->Prev = Tail; //Указываем адрес на предыдущий элемент в соотв. поле
		Tail->Next = temp; //Указываем адрес следующего за хвостом элемента
		Tail = temp; //Меняем адрес хвоста
	}
	else //Если список пустой
	{
		temp->Prev = NULL; //Предыдущий элемент указывает в пустоту
		Head = Tail = temp; //Голова=Хвост=тот элемент, что сейчас добавили
	}

	count++;
	size = count;
	Tail->Next = Head; //Делаем список круговым
}

void List::Show(const int count)
{
	Node *temp = Tail;
	for (int i = 0; i < count; i++) {
		cout << temp->x << " ";
		temp = temp->Next;
	}      cout << "\n";
}

void List::ClearList() {
	Node *temp = new Node;
	temp = CurPoint;
	CurPoint = temp->Next;

	while (temp->x != CurPoint->x)
	{		
		temp->Next = CurPoint->Next;
		cout << "Drop from student's list - " << CurPoint->x << "\n";
		delete CurPoint;
		CurPoint = temp->Next;
	}

	cout << "The captain is - " << temp->x << "\n";
	delete temp;
}

int List::FindCaptain(int n, int k) {
	if (n < 1)
	{
		cout << "Minimum student number to count from is 1\n";
		return 1;
	}

	if (k < 1)
	{
		cout << "Minimum student number to count after n is 1\n";
		return 1;
	}

	CurPoint = Tail; 
	int count = 1;	

	while (count != n) {
		CurPoint = CurPoint->Next;
		count++;
	}

	if (k == 1)
	{
		ClearList();
	}
	else
	{
		DropStudent(k);
	}	

	return 0;
}

void List::DropStudent(int k) 
{
	int count = 0;
	Node *temp = new Node;

	while (temp->x != CurPoint->x) 
	{
		temp = CurPoint;
		CurPoint = CurPoint->Next;

		if (k != 0)
		{
			count++;
		}

		if (count == k) {
			count = 1;
			temp->Next = CurPoint->Next;
			cout << "Drop from student's list - " << CurPoint->x << "\n";
			delete CurPoint;
			CurPoint = temp->Next;
		}
	}

	cout << "The captain is - " << temp->x << "\n";
	delete temp;
};

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	List lst;
	int n, k;
	string student;
	ifstream file(FILENAME);

	if (!file.is_open())
	{
		cout << "cannot open file: " << FILENAME;
		return 1;
	}
	
	file >> n;
	cout << "from " << n << endl;
	file >> k;
	cout << "every " << k << endl;	

	while (!file.eof()) 
	{
		
		file >> student; 
		lst.Add(student);
	}

	system("CLS");
	
	lst.FindCaptain(n, k);	

	// lst.Show(8);
	system("PAUSE");

	return 0;
}