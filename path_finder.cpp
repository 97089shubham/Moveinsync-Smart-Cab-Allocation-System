#include<bits/stdc++.h>

using namespace std;

class City {
public:
    int id;
    pair<int, int> coordinates;

    City(int id, pair<int, int> coordinates) : id(id), coordinates(coordinates) {}
};

class Edge {
public:
    City* start;
    City* end;
    int weight;

    Edge(City* start, City* end, int weight) : start(start), end(end), weight(weight) {}
};

class Cab {
public:
    int id;
    vector<int> stops;

    Cab(int id) : id(id) {}

    void addStop(int cityId) {
        stops.push_back(cityId);
    }

    vector<int> getStops() {
        return stops;
    }
};

vector<City> generateFixedCities() {
    vector<City> cities;
    cities.push_back(City(1, {0, 0}));
    cities.push_back(City(2, {10, 5}));
    cities.push_back(City(3, {15, 12}));
    cities.push_back(City(4, {5, 8}));
    cities.push_back(City(5, {7, 15}));
    cities.push_back(City(6, {12, 10}));
    cities.push_back(City(7, {18, 5}));
    return cities;
}

vector<Edge> generateConnectedGraph(vector<City>& cities) {
    vector<Edge> graph;
    srand(time(nullptr));

    for (size_t i = 0; i < cities.size(); ++i) {
        for (size_t j = i + 1; j < cities.size(); ++j) {
            int weight = rand() % 10 + 1; // Random weight between 1 and 10
            graph.push_back(Edge(&cities[i], &cities[j], weight));
        }
    }

    return graph;
}

int getWeight(int start, int end, const vector<Edge>& graph) {
    for (const auto& edge : graph) {
        if ((edge.start->id == start && edge.end->id == end) || (edge.start->id == end && edge.end->id == start)) {
            return edge.weight;
        }
    }
    return numeric_limits<int>::max();
}

vector<int> dijkstra(int start, int destination, const vector<City>& cities, const vector<Edge>& graph) {
    unordered_map<int, int> dist;
    unordered_map<int, int> prev;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;

    for (const auto& city : cities) {
        dist[city.id] = numeric_limits<int>::max();
        prev[city.id] = -1;
    }

    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        int currentCity = pq.top().second;
        pq.pop();

        if (currentCity == destination) break;

        for (const auto& edge : graph) {
            if (edge.start->id == currentCity || edge.end->id == currentCity) {
                int neighbor = (edge.start->id == currentCity) ? edge.end->id : edge.start->id;
                int newDist = dist[currentCity] + edge.weight;

                if (newDist < dist[neighbor]) {
                    dist[neighbor] = newDist;
                    prev[neighbor] = currentCity;
                    pq.push({newDist, neighbor});
                }
            }
        }
    }

    vector<int> path;
    for (int at = destination; at != -1; at = prev[at]) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());
    return path;
}

int calculateTotalDistance(const vector<int>& stops, const vector<Edge>& graph) {
    int totalDistance = 0;

    for (size_t i = 0; i < stops.size() - 1; ++i) {
        int startCityId = stops[i];
        int endCityId = stops[i + 1];
        totalDistance += getWeight(startCityId, endCityId, graph);
    }

    return totalDistance;
}

void printGraphMatrix(const vector<Edge>& graph, int numCities) {
    // Initialize the matrix with maximum values
    vector<vector<int>> matrix(numCities, vector<int>(numCities, numeric_limits<int>::max()));

    // Fill the matrix with actual weights from the graph
    for (const auto& edge : graph) {
        int start = edge.start->id - 1;
        int end = edge.end->id - 1;
        matrix[start][end] = edge.weight;
        matrix[end][start] = edge.weight; // Assuming undirected graph
    }

    // Print the matrix
    cout << "\nGraph Adjacency Matrix:" << endl;
    for (int i = 0; i < numCities; ++i) {
        for (int j = 0; j < numCities; ++j) {
            if(i == j){
                cout<<"0"<<" ";
            }
            else if (matrix[i][j] == numeric_limits<int>::max()) {
                cout << "INF ";
            } else {
                cout << matrix[i][j] << " ";
            }
        }
        cout << endl;
    }
}

int main() {
    vector<City> cities = generateFixedCities();
    vector<Edge> graph = generateConnectedGraph(cities);
    printGraphMatrix(graph, cities.size());

    Cab cab1(1);
    Cab cab2(2);

    City& destinationCab1 = cities[4];
    City& destinationCab2 = cities[2];

    cout << "Initial Positions: Cab1 - " << cities[0].id << ", Cab2 - " << cities[6].id << endl;

    vector<int> optimalPathCab1 = dijkstra(cities[0].id, destinationCab1.id, cities, graph);
    vector<int> optimalPathCab2 = dijkstra(cities[6].id, destinationCab2.id, cities, graph);

    cout << "\nDijkstra - Cab1 Optimal Path: ";
    for (int stop : optimalPathCab1) cout << stop << " ";
    cout << "\nDijkstra - Cab1 Total Distance: " << calculateTotalDistance(optimalPathCab1, graph) << endl;

    cout << "\nDijkstra - Cab2 Optimal Path: ";
    for (int stop : optimalPathCab2) cout << stop << " ";
    cout << "\nDijkstra - Cab2 Total Distance: " << calculateTotalDistance(optimalPathCab2, graph) << endl;

    return 0;
}