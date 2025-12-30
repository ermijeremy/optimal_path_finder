#include "../include/PathFinder.h"

OperationResult PathFinder::addCity(string city1, string city2, int distance) {
    OperationResult res;
    if (distance <= 0) {
        res.success = false;
        res.message = "Distance must be positive.";
        return res;
    }
    
    graph.addEdge(city1, city2, distance);
    res.success = true;
    res.message = "Route added: " + city1 + " <-> " + city2 + " (" + to_string(distance) + " km)";
    return res;
}

OperationResult PathFinder::updateCity(string city1, string city2, int distance) {
    OperationResult res;
    if (distance <= 0) {
        res.success = false;
        res.message = "Distance must be positive.";
        return res;
    }
    
    if (graph.updateEdge(city1, city2, distance)) {
        res.success = true;
        res.message = "Route updated: " + city1 + " <-> " + city2 + " (" + to_string(distance) + " km)";
    } else {
        res.success = false;
        res.message = "Route not found. Use 'Add Route' to create it.";
    }
    return res;
}

OperationResult PathFinder::removeCity(string city1, string city2) {
    OperationResult res;
    if (!graph.hasEdge(city1, city2)) {
        res.success = false;
        res.message = "Route not found.";
        return res;
    }
    
    graph.removeEdge(city1, city2);
    res.success = true;
    res.message = "Route removed: " + city1 + " <-> " + city2;
    return res;
}

ShortestPathResult PathFinder::findShortestPath(string start, string end) {
    return ShortestPath::find(graph, start, end);
}

LongestPathResult PathFinder::findLongestPath(string start, string end) {
    return LongestPath::find(graph, start, end);
}

FewestStopsResult PathFinder::findFewestStops(string start, string end) {
    return FewestStops::find(graph, start, end);
}

vector<string> PathFinder::findReachableCities(string start) {
    return ReachableCities::find(graph, start);
}

TourResult PathFinder::planMultiCityTour(vector<string> cities) {
    return MultiCityTour::plan(graph, cities);
}

MSTResult PathFinder::findCheapestNetwork() {
    return CheapestNetwork::find(graph);
}

vector<string> PathFinder::getAllCities() {
    return graph.getNodes();
}

vector<tuple<string, string, int>> PathFinder::getAllRoutes() {
    auto edges = graph.getAllEdges();
    vector<tuple<string, string, int>> result;
    for (size_t i = 0; i < edges.size(); ++i) {
        const auto& edge = edges[i];
        result.push_back(make_tuple(get<1>(edge), get<2>(edge), get<0>(edge)));
    }
    return result;
}

void PathFinder::clearAll() {
    graph.clear();
}
