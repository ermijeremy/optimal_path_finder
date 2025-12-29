#include "../include/ReachableCities.h"
#include <map>

vector<string> ReachableCities::find(Graph& g, string start) {
    vector<string> reachable;
    
    // Check if start city exists
    auto nodes = g.getNodes();
    bool exists = false;
    for (const auto& n : nodes) {
        if (n == start) {
            exists = true;
            break;
        }
    }
    if (!exists) return reachable;

    map<string, bool> visited;
    CustomStack<string> s; // Using Person 1's Stack for DFS
    s.push(start);

    while (!s.empty()) {
        string u = s.top(); 
        s.pop();
        
        if (!visited[u]) {
            visited[u] = true;
            
            // Add to list if it's not the starting city
            if (u != start) {
                reachable.push_back(u);
            }
            
            for (const auto& e : g.getNeighbors(u)) {
                if (!visited[e.dest]) {
                    s.push(e.dest);
                }
            }
        }
    }
    return reachable;
}
