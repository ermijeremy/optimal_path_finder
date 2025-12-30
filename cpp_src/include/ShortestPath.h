#ifndef SHORTEST_PATH_H
#define SHORTEST_PATH_H

#include "Graph.h"
#include "DataStructures.h"
#include <string>
#include <vector>

struct ShortestPathResult {
    bool found;
    std::vector<std::string> path;
    int distance;
    std::string message;
};

class ShortestPath {
public:
    static ShortestPathResult find(Graph& g,
                                   const std::string& start,
                                   const std::string& end);
};

#endif // SHORTEST_PATH_H
