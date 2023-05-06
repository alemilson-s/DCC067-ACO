#include <iostream>
#include "Graph.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    Graph a(0, false, true, false);
    a.insertNode(1);
    a.insertEdge(2, 1);
    a.insertEdge(1, 3);
    a.insertEdge(1, 5);
    a.insertEdge(5, 4);

    a.print();
}
