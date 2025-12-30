#include "../include/ShortestPath.h"
#include <algorithm>
#include <climits>

ShortestPathResult ShortestPath::find(Graph& g,
                                      const std::string& start,
                                      const std::string& end) {
    ShortestPathResult res;
    res.found = false;
    res.distance = 0;

    // Validate nodes
    auto nodes = g.getNodes();
    bool startExists = false, endExists = false;
    for (const auto& n : nodes) {
        if (n == start) startExists = true;
        if (n == end) endExists = true;
    }

    if (!startExists || !endExists) {
        res.message = "One or both cities not found.";
        return res;
    }

    if (start == end) {
        res.found = true;
        res.distance = 0;
        res.path = {start};
        res.message = "Start and destination are the same.";
        return res;
    }

    // Distance and parent maps
    std::map<std::string, int> dist;
    std::map<std::string, std::string> parent;

    for (const auto& city : nodes)
        dist[city] = INT_MAX;

    // Use YOUR MinPQ
    MinPQ pq;
    dist[start] = 0;
    pq.push(0, start);

    // Dijkstra loop
    while (!pq.empty()) {
        PQNode top = pq.pop();

        if (top.weight > dist[top.city])
            continue;

        if (top.city == end)
            break;

        for (const auto& edge : g.getNeighbors(top.city)) {
            if (dist[top.city] != INT_MAX &&
                dist[top.city] + edge.weight < dist[edge.dest]) {

                dist[edge.dest] = dist[top.city] + edge.weight;
                parent[edge.dest] = top.city;
                pq.push(dist[edge.dest], edge.dest);
            }
        }
    }

    // No path
    if (dist[end] == INT_MAX) {
        res.message = "No path found.";
        return res;
    }

    res.found = true;
    res.distance = dist[end];

    std::string curr = end;
    while (curr != start) {
        res.path.push_back(curr);
        curr = parent[curr];
    }
    res.path.push_back(start);
    std::reverse(res.path.begin(), res.path.end());

    res.message = "Path found.";
    return res;
}
