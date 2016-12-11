// findMinSaftyPath.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

static int INF = INT_MAX / 2;

int n; //���������� ������ � �������
int m; //���������� ��� � �������
vector<int> *adj; //������ ���������
vector<int> *weight; //��� ����� � �������
vector<bool> used; //������ ��� �������� ���������� � ���������� � �� ���������� ��������
int *dist; //������ ��� �������� ���������� �� ��������� �������
int *pred; //������ �������, ����������� ��� �������������� ����������� ���� �� ��������� ������� 
int start; //��������� �������, �� ������� ������ ���������� �� ���� ������
int a; //�������� �� ������� ������ ���������

		   //��������� ������� ��������� �������� �� ��������� �������
void dejkstra(int s) {
	dist[s] = 0; //���������� ���������� �� ��������� ������� ����� 0
	for (int iter = 0; iter < n; ++iter) {
		int v = -1;
		int distV = INF;
		//�������� �������, ���������� ���������� �� �������� ��� �� �������
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
		//������������� ��� ����, ��������� �� ��������� �������
		for (int i = 0; i < adj[v].size(); ++i) {
			int u = adj[v][i];
			int weightU = weight[v][i];
			//���������� �������
			if (dist[v] + weightU < dist[u]) {
				dist[u] = dist[v] + weightU;
				pred[u] = v;
			}
		}
		//�������� ������� v �������������, �� ��� ������� ���������� ����������
		used[v] = true;
	}
}

//��������� ���������� ������� ������ � �������
void readData() {
	scanf("%d %d %d", &n, &m, &start); //��������� ���������� ������, ���������� ��� ����� � ��������� �������
	start--;

	//�������������� ������ ��������� ����� ����������� n
	adj = new vector<int>[n];
	//������������� ������, � ������� �������� ���� �����
	weight = new vector<int>[n];

	//��������� ����, �������� ������� �����
	for (int i = 0; i < m; ++i) {
		int u, v, w;
		scanf("%d %d %d", &u, &v, &w);
		u--;
		v--;
		adj[u].push_back(v);
		weight[u].push_back(w);
	}

	used.resize(n, false);

	pred = new int[n];
	dist = new int[n];
	for (int i = 0; i < n; ++i) {
		pred[i] = -1;
		dist[i] = INF;
	}

}

//��������� �������������� ����������� ���� �� ������� �������
void printWay(int v) {
	if (v == -1) {
		return;
	}
	printWay(pred[v]);
	printf("%d ", (v + 1));
}

//��������� ������ ������ � �������
void printData() {
	for (int v = 0; v < n; ++v) {
		if (dist[v] != INF) {
			printf("%d ", dist[v]);
		}
		else {
			printf("-1 ");
		}
	}
	printf("\n");
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
	run();
	return 0;
}