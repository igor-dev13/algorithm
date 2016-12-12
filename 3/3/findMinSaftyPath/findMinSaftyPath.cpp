// findMinSaftyPath.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

static int INF = INT_MAX / 2;
const int ACCIDENT_NUMBER = 10000;
const char ACCIDENT_LETTER = 'A';

int n; //количество вершин в орграфе
int m; //количество дуг в орграфе
vector<int> *adj; //список смежности
vector<int> *weight; //вес ребра в орграфе
vector<bool> used; //массив для хранения информации о пройденных и не пройденных вершинах
int *dist; //массив для хранения расстояния от стартовой вершины
int *pred; //массив предков, необходимых для восстановления кратчайшего пути из стартовой вершины 
int start; //стартовая вершина, от которой ищется расстояние до всех других

		   //процедура запуска алгоритма Дейкстры из стартовой вершины
void dejkstra(int s) {
	dist[s] = 0; //кратчайшее расстояние до стартовой вершины равно 0
	for (int iter = 0; iter < n; ++iter) {
		int v = -1;
		int distV = INF;
		//выбираем вершину, кратчайшее расстояние до которого еще не найдено
		for (int i = 0; i < n; ++i) {
			if (used[i]) {
				continue;
			}
			if (distV < dist[i]) {
				continue;
			}
			v = i;
			distV = dist[i];
		}
		//рассматриваем все дуги, исходящие из найденной вершины
		for (int i = 0; i < adj[v].size(); ++i) {
			int u = adj[v][i];
			int weightU = weight[v][i];
			//релаксация вершины
			if (dist[v] + weightU < dist[u]) {
				dist[u] = dist[v] + weightU;
				pred[u] = v;
			}
		}
		//помечаем вершину v просмотренной, до нее найдено кратчайшее расстояние
		used[v] = true;
	}
}

//процедура считывания входных данных с консоли
void readData() {
	FILE *input = 0;
	input = fopen("input.txt", "r");
	fscanf(input, "%d %d %d", &n, &m, &start); //считываем количество вершин, количество дуг графа и стартовую вершину
	start--;

	//инициализируем списка смежности графа размерности n
	adj = new vector<int>[n];
	//инициализация списка, в котором хранятся веса ребер
	weight = new vector<int>[n];

	//считываем граф, заданный списком ребер
	for (int i = 0; i < m; ++i) {
		int u, v, w;
		char accidentWay;
		fscanf(input, "%d %d %d %c", &u, &v, &w, &accidentWay);
		u--;
		v--;
		if (accidentWay == ACCIDENT_LETTER)
			w = w * ACCIDENT_NUMBER;

		adj[u].push_back(v);
		adj[v].push_back(u); // убрать если граф ориентированный
		weight[u].push_back(w);
		weight[v].push_back(w); // убрать если граф ориентированный
	}

	used.resize(n, false);

	pred = new int[n];
	dist = new int[n];
	for (int i = 0; i < n; ++i) {
		pred[i] = -1;
		dist[i] = INF;
	}

}

//процедура восстановления кратчайшего пути по массиву предком
void printWay(int v) {
	if (v == -1) {
		return;
	}
	printWay(pred[v]);
	printf("%d ", (v + 1));
}

//процедура вывода данных в консоль
void printData() {
	cout << endl << "Путь до каждой точки ";
	for (int v = 0; v < n; ++v) {
		if (dist[v] != INF) {
			if (dist[v] >= ACCIDENT_NUMBER) {
				int difference = dist[v] / ACCIDENT_NUMBER;
				dist[v] = dist[v] % ACCIDENT_NUMBER + difference;
			}
			printf("%d ", dist[v]);
		}
		else {
			printf("-1 ");
		}
	}
	cout << endl << endl << "Последовательность обхода алгоритмом" << endl;
	for (int v = 0; v < n; ++v) {
		printf("%d: ", (v + 1));
		if (dist[v] != INF) {
			printWay(v);
		}
		printf("\n");
	}
}

void run() {
	readData();
	dejkstra(start);
	printData();
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	system("CLS");

	run();
	return 0;
}