#include "../include/FewestStops.h"
#include <map>

FewestStopsResult FewestStops::find(Graph& g, string start, string end) {
    FewestStopsResult res;
    res.found = false;
    res.stops = 0;

    // Check if cities exist
    auto nodes = g.getNodes();
    bool startExists = false, endExists = false;
    for (const auto& n : nodes) {
        if (n == start) startExists = true;
        if (n == end) endExists = true;
    }

    if (!startExists || !endExists) {
        res.message = "One or both cities not found in the network.";
        return res;
    }

    map<string, bool> visited;
    map<string, string> parent;
    CustomQueue<string> q;

    q.enqueue(start);
    visited[start] = true;

    while (!q.empty()) {
        string u = q.front(); 
        q.dequeue();
        
        if (u == end) {
            res.found = true;
            res.message = "Path found with fewest stops.";
            
            // Reconstruct path using CustomStack
            CustomStack<string> pathStack;
            string curr = end;
            while (curr != start) {
                pathStack.push(curr);
                curr = parent[curr];
            }
            pathStack.push(start);

            while (!pathStack.empty()) {
                res.path.push_back(pathStack.top());
                pathStack.pop();
            }
            
            res.stops = res.path.size() - 1;
            return res;
        }

        for (const auto& e : g.getNeighbors(u)) {
            if (!visited[e.dest]) {
                visited[e.dest] = true;
                parent[e.dest] = u;
                q.enqueue(e.dest);
            }
        }
    }

    res.message = "No path exists between these cities.";
    return res;
}
