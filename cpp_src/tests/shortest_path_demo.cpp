#include "../include/Graph.h"
#include "../include/ShortestPath.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <set>
#include <cstdlib>
#include <sstream>

static std::string trim(const std::string& s) {
    size_t b = 0, e = s.size();
    while (b < e && std::isspace(static_cast<unsigned char>(s[b]))) ++b;
    while (e > b && std::isspace(static_cast<unsigned char>(s[e - 1]))) --e;
    return s.substr(b, e - b);
}

static std::vector<std::string> splitCSV(const std::string& s) {
    std::vector<std::string> out;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, ',')) {
        item = trim(item);
        if (!item.empty()) out.push_back(item);
    }
    return out;
}

static bool buildCombinedPath(Graph& g,
                              const std::vector<std::string>& points,
                              std::vector<std::string>& combinedPath,
                              int& totalDistance,
                              std::string& err) {
    combinedPath.clear();
    totalDistance = 0;
    if (points.size() < 2) { err = "Need at least start and end."; return false; }
    for (size_t i = 0; i + 1 < points.size(); ++i) {
        auto seg = ShortestPath::find(g, points[i], points[i+1]);
        if (!seg.found) { err = seg.message; return false; }
        totalDistance += seg.distance;
        if (combinedPath.empty()) {
            combinedPath.insert(combinedPath.end(), seg.path.begin(), seg.path.end());
        } else {
            combinedPath.insert(combinedPath.end(), seg.path.begin() + 1, seg.path.end());
        }
    }
    return true;
}

static void exportDot(Graph& g,
                      const std::vector<std::string>& nodes,
                      const std::vector<std::string>& path,
                      const std::string& start,
                      const std::string& end,
                      const std::string& outDot,
                      const std::string& outPng) {
    std::ofstream ofs(outDot);
    if (!ofs) {
        std::cout << "Failed to write DOT file: " << outDot << "\n";
        return;
    }
    ofs << "graph G {\n";
    ofs << "  rankdir=LR;\n";
    ofs << "  bgcolor=\"#ffffff\";\n";
    ofs << "  node [shape=circle, style=filled, fillcolor=\"#f9f9f9\", color=\"#222222\", fontcolor=\"#222222\"];\n";
    ofs << "  edge [color=\"#555555\", fontcolor=\"#222222\"];\n";

    for (const auto& n : nodes) {
        if (n == start) ofs << "  \"" << n << "\" [style=filled, fillcolor=lightgreen];\n";
        else if (n == end) ofs << "  \"" << n << "\" [style=filled, fillcolor=lightblue];\n";
        else ofs << "  \"" << n << "\";\n";
    }

    std::set<std::pair<std::string,std::string>> pathEdges;
    for (size_t i = 1; i < path.size(); ++i) {
        std::string a = path[i-1], b = path[i];
        if (a > b) std::swap(a, b);
        pathEdges.emplace(a, b);
    }

    for (const auto& t : g.getAllEdges()) {
        int w; std::string u, v; std::tie(w, u, v) = t;
        std::string a = u, b = v; if (a > b) std::swap(a, b);
        bool isOnPath = pathEdges.count({a, b}) > 0;
        ofs << "  \"" << u << "\" -- \"" << v << "\" [label=\"" << w << "\"";
        if (isOnPath) ofs << ", color=\"#d62728\", penwidth=3";
        ofs << "];\n";
    }

    ofs << "}\n";
    ofs.close();
    std::cout << "Wrote Graphviz DOT: " << outDot << "\n";

    if (!outPng.empty()) {
        std::string cmd = std::string("dot -Tpng \"") + outDot + "\" -o \"" + outPng + "\"";
        int rc = std::system(cmd.c_str());
        if (rc == 0) {
            std::cout << "Rendered PNG: " << outPng << "\n";
        } else {
            std::cout << "Could not render PNG. Ensure Graphviz (dot) is installed and in PATH.\n";
        }
    }
}

int main(int argc, char** argv) {
    Graph g;
    // Simple undirected weighted graph (sample network)
    g.addEdge("A", "B", 4);
    g.addEdge("A", "C", 2);
    g.addEdge("B", "C", 5);
    g.addEdge("B", "D", 10);
    g.addEdge("C", "E", 3);
    g.addEdge("E", "D", 4);
    g.addEdge("D", "F", 11);
    // Additional nodes/edges for complexity
    g.addEdge("C", "G", 9);
    g.addEdge("E", "H", 8);
    g.addEdge("G", "H", 2);
    g.addEdge("H", "I", 1);
    g.addEdge("I", "J", 7);
    g.addEdge("F", "J", 6);
    g.addEdge("D", "G", 2);
    g.addEdge("B", "I", 12);
    g.addEdge("A", "H", 15);
    g.addEdge("J", "K", 3);
    g.addEdge("K", "L", 5);
    g.addEdge("I", "L", 4);
    g.addEdge("E", "K", 9);
    g.addEdge("G", "L", 6);

    std::cout << "Available cities: ";
    auto nodes = g.getNodes();
    for (size_t i = 0; i < nodes.size(); ++i) {
        std::cout << nodes[i] << (i + 1 < nodes.size() ? ", " : "\n");
    }

    // Non-interactive mode: support CLI args (start end [via1 via2 ...])
    if (argc >= 3) {
        std::string start = argv[1];
        std::string end = argv[argc-1];
        std::vector<std::string> points;
        points.push_back(start);
        for (int i = 2; i < argc-1; ++i) points.push_back(argv[i]);
        points.push_back(end);

        std::vector<std::string> combinedPath; int totalDist = 0; std::string err;
        bool ok = buildCombinedPath(g, points, combinedPath, totalDist, err);
        if (!ok) { std::cout << err << "\n"; return 1; }

        std::cout << "Distance: " << totalDist << "\n";
        std::cout << "Path: ";
        for (size_t i = 0; i < combinedPath.size(); ++i) {
            std::cout << combinedPath[i];
            if (i + 1 < combinedPath.size()) std::cout << " -> ";
        }
        std::cout << "\n";

        std::string outDot = "cpp_src/tests/shortest_path_demo.dot";
        std::string outPng = "cpp_src/tests/shortest_path_demo.png";
        exportDot(g, nodes, combinedPath, start, end, outDot, outPng);
        return 0;
    }

    std::cout << "Type 'q' at any prompt to exit." << std::endl;

    while (true) {
        std::string start, end;
        std::cout << "Enter start city: " << std::flush;
        if (!std::getline(std::cin, start)) break;
        start = trim(start);
        if (start == "q" || start == "Q") break;
        if (start.empty()) { std::cout << "Please enter a city name.\n"; continue; }

        std::cout << "Enter end city: " << std::flush;
        if (!std::getline(std::cin, end)) break;
        end = trim(end);
        if (end == "q" || end == "Q") break;
        if (end.empty()) { std::cout << "Please enter a city name.\n"; continue; }

        std::cout << "Intermediate stops (comma-separated, optional): " << std::flush;
        std::string viaLine; std::getline(std::cin, viaLine);
        auto vias = splitCSV(viaLine);

        std::vector<std::string> points; points.push_back(start);
        points.insert(points.end(), vias.begin(), vias.end()); points.push_back(end);
        std::vector<std::string> combinedPath; int totalDist = 0; std::string err;
        bool ok = buildCombinedPath(g, points, combinedPath, totalDist, err);
        if (!ok) { std::cout << err << "\n"; continue; }

        std::cout << "Distance: " << totalDist << "\n";
        std::cout << "Path: ";
        for (size_t i = 0; i < combinedPath.size(); ++i) {
            std::cout << combinedPath[i];
            if (i + 1 < combinedPath.size()) std::cout << " -> ";
        }
        std::cout << "\n";

        // Export Graphviz DOT with shortest path highlighted
        exportDot(g, nodes, combinedPath, start, end,
                  "cpp_src/tests/shortest_path_demo.dot",
                  "cpp_src/tests/shortest_path_demo.png");
    }

    return 0;
}
