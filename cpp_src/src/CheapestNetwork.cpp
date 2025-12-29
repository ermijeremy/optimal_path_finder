#include "../include/CheapestNetwork.h"
#include <algorithm>

MSTResult CheapestNetwork::find(Graph& g) {
    MSTResult res;
    res.found = false;
    res.totalCost = 0;

    auto nodes = g.getNodes();
    if (nodes.empty()) {
        res.message = "Graph is empty.";
        return res;
    }

    // Get all edges and sort by weight (Kruskal's algorithm)
    auto edges = g.getAllEdges();
    sort(edges.begin(), edges.end());

    // Use DisjointSet for cycle detection
    DisjointSet ds;
    for (const auto& node : nodes) {
        ds.makeSet(node);
    }

    int edgeCount = 0;
    for (const auto& edge : edges) {
        int weight = get<0>(edge);
        string u = get<1>(edge);
        string v = get<2>(edge);

        // If cities are in different sets, adding this edge won't create a cycle
        if (ds.find(u) != ds.find(v)) {
            ds.unite(u, v);
            res.edges.push_back(make_tuple(u, v, weight));
            res.totalCost += weight;
            edgeCount++;
            
            if (edgeCount == (int)nodes.size() - 1) break;
        }
    }

    if (nodes.size() == 1 || edgeCount == (int)nodes.size() - 1) {
        res.found = true;
        res.message = "Cheapest network found successfully.";
    } else {
        res.message = "Graph is not fully connected.";
    }

    return res;
}
