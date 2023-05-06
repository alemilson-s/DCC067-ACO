#ifndef SRC_EDGE_H
#define SRC_EDGE_H


class Edge {
private:
    int target_id;
    Edge *next_edge;
    float weight;

public:
    Edge(int target_id);

    ~Edge();

    int getTargetId();

    Edge *getNextEdge();

    float getWeight();

    void setNextEdge(Edge *edge);

    void setWeight(float weight);
};


#endif
