#include <iostream>
#include <vector>
#include <string>
#include "../include/PathFinder.h"

using namespace std;

void displayMenu() {
    cout << "\n--- Optimal Path Finder Menu ---\n";
    cout << "1. Add a Route (City A <-> City B)\n";
    cout << "2. Find Shortest Path (Dijkstra)\n";
    cout << "3. Find Path with Fewest Stops (BFS)\n";
    cout << "4. Find Longest Path (DFS Backtracking)\n";
    cout << "5. Find Reachable Cities (DFS)\n";
    cout << "6. Plan Multi-City Tour (TSP Backtracking)\n";
    cout << "7. Find Cheapest Network (MST Kruskal's)\n";
    cout << "8. View All Cities and Routes\n";
    cout << "9. Clear All Data\n";
    cout << "0. Exit\n";
    cout << "Enter choice: ";
}

int main() {
    PathFinder pf;
    int choice;
    string c1, c2;
    int dist;

    // Sample Data for quick testing
    pf.addCity("Addis Ababa", "Adama", 100);
    pf.addCity("Adama", "Dire Dawa", 350);
    pf.addCity("Addis Ababa", "Bahir Dar", 500);
    pf.addCity("Bahir Dar", "Gondar", 180);
    pf.addCity("Adama", "Hawassa", 200);

    while (true) {
        displayMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        if (choice == 0) break;

        switch (choice) {
            case 1:
                cout << "Enter City 1, City 2, and Distance: ";
                cin >> c1 >> c2 >> dist;
                {
                    OperationResult res = pf.addCity(c1, c2, dist);
                    cout << res.message << endl;
                }
                break;

            case 2:
                cout << "Enter Start and End City: ";
                cin >> c1 >> c2;
                {
                    ShortestPathResult res = pf.findShortestPath(c1, c2);
                    cout << res.message << endl;
                    if (res.found) {
                        cout << "Distance: " << res.distance << " km\nPath: ";
                        for (size_t i = 0; i < res.path.size(); ++i) {
                            cout << res.path[i] << (i == res.path.size() - 1 ? "" : " -> ");
                        }
                        cout << endl;
                    }
                }
                break;

            case 3:
                cout << "Enter Start and End City: ";
                cin >> c1 >> c2;
                {
                    FewestStopsResult res = pf.findFewestStops(c1, c2);
                    cout << res.message << endl;
                    if (res.found) {
                        cout << "Stops: " << res.stops << "\nPath: ";
                        for (size_t i = 0; i < res.path.size(); ++i) {
                            cout << res.path[i] << (i == res.path.size() - 1 ? "" : " -> ");
                        }
                        cout << endl;
                    }
                }
                break;

            case 4:
                cout << "Enter Start and End City: ";
                cin >> c1 >> c2;
                {
                    LongestPathResult res = pf.findLongestPath(c1, c2);
                    cout << res.message << endl;
                    if (res.found) {
                        cout << "Distance: " << res.distance << " km\nPath: ";
                        for (size_t i = 0; i < res.path.size(); ++i) {
                            cout << res.path[i] << (i == res.path.size() - 1 ? "" : " -> ");
                        }
                        cout << endl;
                    }
                }
                break;

            case 5:
                cout << "Enter Starting City: ";
                cin >> c1;
                {
                    vector<string> reachable = pf.findReachableCities(c1);
                    if (reachable.empty()) {
                        cout << "No other cities reachable from " << c1 << endl;
                    } else {
                        cout << "Reachable cities: ";
                        for (size_t i = 0; i < reachable.size(); ++i) {
                            cout << reachable[i] << (i == reachable.size() - 1 ? "" : ", ");
                        }
                        cout << endl;
                    }
                }
                break;

            case 6:
                {
                    int n;
                    cout << "How many cities in the tour? ";
                    cin >> n;
                    vector<string> tourCities(n);
                    cout << "Enter the " << n << " cities: ";
                    for (int i = 0; i < n; ++i) cin >> tourCities[i];
                    
                    TourResult res = pf.planMultiCityTour(tourCities);
                    cout << res.message << endl;
                    if (res.found) {
                        cout << "Total Distance: " << res.totalDistance << " km\nTour: ";
                        for (size_t i = 0; i < res.path.size(); ++i) {
                            cout << res.path[i] << (i == res.path.size() - 1 ? "" : " -> ");
                        }
                        cout << endl;
                    }
                }
                break;

            case 7:
                {
                    MSTResult res = pf.findCheapestNetwork();
                    cout << res.message << endl;
                    if (res.found) {
                        cout << "Total Network Cost: " << res.totalCost << " km\nEdges:\n";
                        for (size_t i = 0; i < res.edges.size(); ++i) {
                            cout << get<0>(res.edges[i]) << " <-> " << get<1>(res.edges[i]) 
                                 << " (" << get<2>(res.edges[i]) << " km)\n";
                        }
                    }
                }
                break;

            case 8:
                {
                    vector<string> cities = pf.getAllCities();
                    cout << "Cities (" << cities.size() << "): ";
                    for (size_t i = 0; i < cities.size(); ++i) {
                        cout << cities[i] << (i == cities.size() - 1 ? "" : ", ");
                    }
                    cout << "\nRoutes:\n";
                    auto routes = pf.getAllRoutes();
                    for (size_t i = 0; i < routes.size(); ++i) {
                        cout << get<0>(routes[i]) << " <-> " << get<1>(routes[i]) 
                             << " (" << get<2>(routes[i]) << " km)\n";
                    }
                }
                break;

            case 9:
                pf.clearAll();
                cout << "All data cleared.\n";
                break;

            default:
                cout << "Invalid choice.\n";
        }
    }

    return 0;
}
