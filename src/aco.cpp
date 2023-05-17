#include "Graph.h"
#include <queue>
#include <limits>
#include <valarray>

using namespace std;

typedef struct {
    vector<int> path;
    vector<bool> visited;
    double solution_value;
} Ant;

void initializeAnts(vector<Ant> &ants) {
    for (int i = 0; i < ants.size(); i++) {
        ants[i].visited.resize(ants.size(), false);
        ants[i].path.clear();
        ants[i].path.push_back(i);
        ants[i].visited[i] = true;
        ants[i].solution_value = 0;
    }
}

void initializeParameters(vector<Ant> &ants, Graph &g, float pheromone) {
    Node *node = g.getFirstNode();
    while (node != nullptr) {
        Edge *edge = node->getFirstEdge();
        while (edge != nullptr) {
            edge->setPheromone(pheromone);
            edge = edge->getNextEdge();
        }
        node = node->getNextNode();
    }
    initializeAnts(ants);
}

Edge *selectNextCity(Ant &ant, Graph &g, float alpha, float beta) {
    vector<Edge *> edges;
    edges.clear();
    int current_city = ant.path.back();
    Edge *edge = g.getNode(current_city)->getFirstEdge();
    double q = 0;
    while (edge != nullptr) {
        if (!ant.visited[edge->getTargetId()]) {
            q += pow(edge->getPheromone(), alpha) + pow(1 / edge->getWeight(), beta);
            edges.push_back(edge);
        }
        edge = edge->getNextEdge();
    }

    vector<double> probabilities(edges.size(), 0.0);
//    edge = g.getNode(current_city)->getFirstEdge();
//    int k = 0;
//    while (edge != nullptr) {
//        if (!ant.visited[edge->getTargetId()]) {
//            probabilities[k] = pow(edge->getPheromone(), alpha) * pow(1 / edge->getWeight(), beta) / q;
//            k++;
//        }
//        edge = edge->getNextEdge();
//    }

    for (int k = 0; k < edges.size(); k++) {
        probabilities[k] = pow(edges[k]->getPheromone(), alpha) * pow(1.0 / edges[k]->getWeight(), beta) / q;
    }

    double p[probabilities.size()];
    for (int i = 0; i < probabilities.size(); i++)
        p[i] = probabilities[i] * 1000;

    double t = 0;
    int r = rand() % 1000;
    for (int i = 0; i < probabilities.size(); i++) {
        t += p[i];
        if (t >= r) {
            return edges[i];
        }
    }
    return 0;

}


void aco(Graph &g, int cycles, float evaporation, float alpha, float beta) {
    Ant best;
    int n_ants = g.getOrder();
    best.solution_value = numeric_limits<double>::max();
    vector<Ant> ants(n_ants, Ant());
    initializeParameters(ants, g, 10);
    cycles = cycles / (n_ants * n_ants);
    int t = 0;
    while (t < cycles) {
        initializeAnts(ants);
        int j = 0;
        while (j < g.getOrder() - 1) {
            int k = 0;
            while (k < n_ants) {
                Edge *next_city = selectNextCity(ants[k], g, alpha, beta);
                ants[k].path.push_back(next_city->getTargetId());
                ants[k].visited[next_city->getTargetId()] = true;
                ants[k].solution_value += next_city->getWeight();
                k++;
            }
        }


        t++;
    }
}

