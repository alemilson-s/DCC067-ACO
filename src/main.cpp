#include <iostream>
#include "Graph.h"
#include "Node.h"
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include "aco.h"

using namespace std;


double calculaDistancia(Node &n, Node &m) {
    return sqrt((m.getX() - n.getX()) * (m.getX() - n.getX()) + (m.getY() - n.getY()) * (m.getY() - n.getY()));
}

Graph *leituraTSP(ifstream &input_file) {
    Graph *g = new Graph(0, false, true, false);
    if (!input_file) {
        cout << "Arquivo não está aberto!" << endl;
        return nullptr;
    }
    string line;
    getline(input_file, line);
    while (line != "DISPLAY_DATA_SECTION" && line != "NODE_COORD_SECTION")
        getline(input_file, line);
    int id;
    float x, y;
    getline(input_file, line);
    while (line != "EOF") {
        istringstream iss(line);
        iss >> id;
        iss >> x;
        iss >> y;
        Node *node = g->allocateNode(id);
        node->setX(x);
        node->setY(y);
        getline(input_file, line);
    }
    Node *node = g->getFirstNode();
    while (node != nullptr) {
        Node *prox = node->getNextNode();
        while (prox != nullptr) {
            double dist = calculaDistancia(*node, *prox);
            node->insertEdge(prox->getId(), dist);
            prox->insertEdge(node->getId(), dist);
            prox = prox->getNextNode();
        }
        node = node->getNextNode();
    }
    return g;
}

Graph *leituraATSP(ifstream &input_file) {
    Graph *g = new Graph(0, true, true, false);
    if (!input_file) {
        cout << "Arquivo não está aberto!" << endl;
        return nullptr;
    }
    int order;
    string line;
    getline(input_file, line);
    while (line != "EDGE_WEIGHT_SECTION") {
        size_t pos = line.find("DIMENSION");
        if (pos != std::string::npos) {
            istringstream iss(line);
            string aux;
            iss >> aux;
            iss >> order;
        }
        getline(input_file, line);
    }
    vector<double> vet;
    int id;
    getline(input_file, line);
    while (line != "EOF") {
        istringstream iss(line);
        string str;
        while (iss >> str) {
            double value = stod(str);
            vet.push_back(value);
        }
        getline(input_file, line);
    }
    for (int i = 0; i < order; i++) {
        for (int j = 0; j < order; j++) {
            g->insertEdge(i, j, vet[i * order + j], true);
        }
    }
    return g;
}

int main(int argc, char **argv) {
    if (argc != 4 && argc != 3) {
        cout << "ERROR: Expecting: ./<program_name> <input_file> <directed>" << endl;
        return -1;
    }
    string file_name = argv[1];
    bool directed = stoi(argv[2]);

    ifstream input_file;
    input_file.open(file_name, ios::in);

    Graph *g;
    if (directed) {
        g = leituraATSP(input_file);
    } else {
        g = leituraTSP(input_file);
    }
    if (input_file.is_open())
        input_file.close();
    int cycles = 20000;
    if (g->getOrder() >= 50)
        cycles = 70000;

//    for (int i = 0; i < 500; i++)
    aco(*g, cycles, 0.7, 1, 9);

    delete g;
    return 0;

}
