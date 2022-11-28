#include <iostream>
#include <string>
#include <set>
#include <map>
#include <fstream>
#include "Windows.h"
using namespace std;

ofstream fout("cppstudio.txt");
string color[] = {
    "BLUE", "GREEN", "RED", "YELLOW", "ORANGE", "PINK",
    "BLACK", "BROWN", "WHITE", "PURPLE", "VOILET", "MAGENTA", 
    "CYAN", "GREY", "PLUM", "GOLDEN", "SILVER"
};

// Для построения матрицы смежности
class Graph {
private:

    bool** adjMatrix;
    int numVertices;

public:
    Graph(int numVertices)
    {
        this->numVertices = numVertices;
        adjMatrix = new bool* [numVertices];

        for (int i = 0; i < numVertices; i++) {
            adjMatrix[i] = new bool[numVertices];
            for (int j = 0; j < numVertices; j++)
                adjMatrix[i][j] = false;
        }
    }
    ~Graph() {
        for (int i = 0; i < numVertices; i++)
            delete[] adjMatrix[i];
        delete[] adjMatrix;
    }

    void addEdgeMatrix(int i, int j) {
        adjMatrix[i][j] = true;
        adjMatrix[j][i] = true;
    }

    void toString() {
        for (int i = 0; i < numVertices; i++) {
            cout << i << " : ";
            for (int j = 0; j < numVertices; j++) {
                cout << adjMatrix[i][j] << " ";
                fout << adjMatrix[i][j] << " ";
            }
            cout << endl;
            fout << endl;
        }
    }

    void Memset() {
        for (int i = 0; i < numVertices; i++) {
            for (int j = 0; j < numVertices; j++)
                adjMatrix[i][j] = false;
        }
    }
};

void addEdge(set<int>* adj, int w, int v)
{
    if (w != v) {
        adj[w].insert(v);
        adj[v].insert(w);
    }
}

void GraphColoring(set<int>* adj, int V) {
    multimap<int, int> result;
    int* resilt = new int[V];

    // Присвоить первый цвет первой вершине
    resilt[0] = 0;

    // Оставшиеся вершины V-1 как неназначенные
    for (int i = 1; i < V; i++)
        resilt[i] = -1;

    // Временный массив для хранения доступных цветов. Правда значение available[cr] будет означать
    // что цвет cr равен присвоен одной из его смежных вершин

    bool* available = new bool[V];
    for (int cr = 0; cr < V; cr++)
        available[cr] = false;

    int n=0;
    // Назначить цвета оставшимся вершинам V-1
    for (int i = 1; i < V; i++)
    {
        // обработал все смежные вершины и отметил их цвета как отсуствующие
        for (auto j = adj[i].begin(); j != adj[i].end(); ++j)
            if (resilt[*j] != -1)
                available[resilt[*j]] = true;

        // Нашел первый доступный цвет
        int cr;
        for (cr = 0; cr < V; cr++)
            if (available[cr] == false)
                break;
        n = cr;
        resilt[i] = cr; // присвоение нужного цвета

        // сбросил до false, для возврата
        for (auto j = adj[i].begin(); j != adj[i].end(); ++j)
            if (resilt[*j] != -1)
                available[resilt[*j]] = false;
    }

    // Вывод рузультата
    for (int i = 0; i < V; i++) {
        result.emplace(resilt[i], i);
        cout << "Вершина: " << i << " --->  Цвет:" << color[resilt[i]] << endl;
        fout << "Вершина: " << i << " --->  Цвет:" << color[resilt[i]] << endl;
    }
    cout << "-----------------";
    for (int i = 0; i < n+1; i++)
    {
        cout << endl << "Вершины цвета " << color[i] << ": ";
        fout << endl << "Вершины цвета " << color[i] << ": ";
        for (auto it = result.begin(); it!= result.end(); ++it)
        {
            if (it->first == i) {
                cout << it->second<<", ";
                fout << it->second << ", ";
            }
        }
    }
    cout << endl;

    delete[] available;
    delete[] resilt;
}

int main() {
    system("color F0");
    srand(time(NULL));
    setlocale(0, "");

    int z;
l:
    system("cls");
    cout << "Введите кол-во вершин графа: "; cin >> z;
    Graph c(z);
    /////////////////////////////////////////////////////////////////////////////////
    set<int>* adj = new set<int>[z];
    string s;
    while (true) {
        system("cls");
        cout << "Количество вершин: " << z
             << "\n Выберете способ построения графа: \n 1) Случайное заполнение \n" 
             << " 2) Заполнить самому \n 3) Изменить кол-во вершин \n 4) Выход\n Введите: ";
        cin >> s;
        switch (atoi(s.c_str()))
        {
        case 1:
            system("cls");

            int r;
            cout << "Введите вероятность присутствия ребра(чем больше, тем выше вероятность): ";
            cin >> r;
            for (int i = 0; i < z; i++)
            {
                for (int j = i+1; j < z; j++)
                {
                    if (1<=(rand() % r)) {
                        c.addEdgeMatrix(i, j);
                        addEdge(adj, i, j);
                    }
                }
            }
            cout << "-----------------" << endl << "Матрица смежности: \n";
            fout << endl << "-----------------" << endl << "Матрица смежности: \n";
            c.toString();
            cout << endl << "-----------------" << endl << "Ответ: \n";
            fout << endl << "-----------------" << endl << "Ответ: \n";
            GraphColoring(adj, z);
            c.Memset();

            system("pause");
            break;
        case 2:
            system("cls");

            cout << "Вводите по две вершины через пробел"<<endl
                 << "Для выхода введите *"<<endl
                 <<"Пример: \n 0 1\n 0 2\n ....\nВведите:\n";
            while(true) {
                string a, b;
                cin >> a >> b;
                if (a == "*" || b == "*")
                    break;
                if (atoi(a.c_str()) < z && atoi(b.c_str()) < z) {
                    addEdge(adj, atoi(a.c_str()), atoi(b.c_str()));
                    c.addEdgeMatrix(atoi(a.c_str()), atoi(b.c_str()));
                }
                else {
                    cout << "Такой вершины не существует"<<endl;
                }
            }
            cout << "-----------------" << endl << "Матрица смежности: \n";
            fout << endl << "-----------------" << endl << "Матрица смежности: \n";
            c.toString();
            cout << endl << "-----------------" << endl << "Ответ: \n";
            fout << endl << "-----------------" << endl << "Ответ: \n";
            GraphColoring(adj, z);
            c.Memset();

            system("pause");
            break;
        case 3:
            goto l;
            break;
        case 4:
            exit(0);
            break;
        }
        system("cls");
    }
    /////////////////////////////////////////////////////////////////////////////////
    fout.close();
    c.~Graph();
    delete[] adj;
    return 0;
}
