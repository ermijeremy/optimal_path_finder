#include "../include/Graph.h"
#include <algorithm>

string toLower(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

void Graph::addEdge(string u, string v, int w) {
    string lu = toLower(u);
    string lv = toLower(v);
    
    string keyU = lowerToOriginal.count(lu) ? lowerToOriginal[lu] : u;
    string keyV = lowerToOriginal.count(lv) ? lowerToOriginal[lv] : v;
    
    lowerToOriginal[lu] = keyU;
    lowerToOriginal[lv] = keyV;
    
    if (hasEdge(u, v)) removeEdge(u, v);
    
    adjList[keyU].push_back({keyV, w});
    adjList[keyV].push_back({keyU, w});
}

bool Graph::updateEdge(string u, string v, int w) {
    string lu = toLower(u);
    string lv = toLower(v);
    
    if (!lowerToOriginal.count(lu) || !lowerToOriginal.count(lv)) return false;
    
    string keyU = lowerToOriginal[lu];
    string keyV = lowerToOriginal[lv];
    
    if (!hasEdge(u, v)) return false;
    
    // Update weight in u's list
    for (auto& edge : adjList[keyU]) {
        if (edge.dest == keyV) {
            edge.weight = w;
            break;
        }
    }
    
    // Update weight in v's list
    for (auto& edge : adjList[keyV]) {
        if (edge.dest == keyU) {
            edge.weight = w;
            break;
        }
    }
    return true;
}

void Graph::removeEdge(string u, string v) {
    string lu = toLower(u);
    string lv = toLower(v);
    
    if (!lowerToOriginal.count(lu) || !lowerToOriginal.count(lv)) return;
    
    string keyU = lowerToOriginal[lu];
    string keyV = lowerToOriginal[lv];
    
    if (adjList.count(keyU)) {
        auto& edges = adjList[keyU];
        for (auto it = edges.begin(); it != edges.end(); ) {
            if (it->dest == keyV) it = edges.erase(it);
            else ++it;
        }
        if (edges.empty()) {
            adjList.erase(keyU);
        }
    }
    if (adjList.count(keyV)) {
        auto& edges = adjList[keyV];
        for (auto it = edges.begin(); it != edges.end(); ) {
            if (it->dest == keyU) it = edges.erase(it);
            else ++it;
        }
        if (edges.empty()) {
            adjList.erase(keyV);
        }
    }
}

bool Graph::hasEdge(string u, string v) {
    string lu = toLower(u);
    string lv = toLower(v);
    
    if (!lowerToOriginal.count(lu) || !lowerToOriginal.count(lv)) return false;
    
    string keyU = lowerToOriginal[lu];
    string keyV = lowerToOriginal[lv];
    
    if (adjList.find(keyU) == adjList.end()) return false;
    for (const auto& e : adjList[keyU]) {
        if (e.dest == keyV) return true;
    }
    return false;
}

vector<Edge> Graph::getNeighbors(string u) {
    string lu = toLower(u);
    if (!lowerToOriginal.count(lu)) return {};
    
    string keyU = lowerToOriginal[lu];
    if (adjList.find(keyU) != adjList.end()) return adjList[keyU];
    return {};
}

vector<string> Graph::getNodes() {
    vector<string> nodes;
    for (const auto& pair : adjList) {
        nodes.push_back(pair.first);
    }
    return nodes;
}

void Graph::clear() {
    adjList.clear();
    lowerToOriginal.clear();
}

int Graph::getCityCount() {
    return adjList.size();
}

vector<tuple<int, string, string>> Graph::getAllEdges() {
    vector<tuple<int, string, string>> edges;
    
    for (const auto& pair : adjList) {
        string u = pair.first;
        for (const auto& e : pair.second) {
            if (u < e.dest) {
                edges.push_back(make_tuple(e.weight, u, e.dest));
            }
        }
    }
    return edges;
}
