#include<bits/stdc++.h>

using namespace std;

class City {
public:
    int id;
    pair<int, int> coordinates;

    City(int id, pair<int, int> coordinates) : id(id), coordinates(coordinates) {}
};

class Cab {
public:
    pair<int, int> coordinates;

    Cab(pair<int, int> coordinates) : coordinates(coordinates) {}
};

double minkowskiDistance(const pair<int, int>& point1, const pair<int, int>& point2, int p) {
    double sum = 0;
    sum += pow(abs(point1.first - point2.first), p);
    sum += pow(abs(point1.second - point2.second), p);
    return pow(sum, 1.0 / p);
}

vector<Cab> findNearestCabs(const pair<int, int>& customerLocation, const vector<Cab>& cabsDataset, int k, int p) {
    vector<double> distances;
    distances.reserve(cabsDataset.size());

    // Calculate distances from customer location to each cab
    for (const auto& cab : cabsDataset) {
        distances.push_back(minkowskiDistance(customerLocation, cab.coordinates, p));
    }

    // Create an array of indices to sort the distances array
    vector<size_t> indices(cabsDataset.size());
    iota(indices.begin(), indices.end(), 0); // Fill with 0, 1, 2, ..., n-1

    // Sort the indices based on distances
    sort(indices.begin(), indices.end(), [&](size_t a, size_t b) {
        return distances[a] < distances[b];
    });

    // Retrieve the k nearest cabs
    vector<Cab> nearestCabs;
    nearestCabs.reserve(k);
    for (int i = 0; i < k; ++i) {
        nearestCabs.push_back(cabsDataset[indices[i]]);
    }

    return nearestCabs;
}

int main() {
    // Generate a random customer location
    pair<int, int> customerLocation{rand() % 10, rand() % 10};

    // Generate a random cabs dataset for demonstration purposes
    vector<Cab> cabsDataset;
    for (int i = 0; i < 100; ++i) {
        cabsDataset.emplace_back(pair<int, int>{rand() % 100, rand() % 100});
    }

    int k = 5; // Number of nearest cabs to find
    int p = 1; // Minkowski distance order, can be adjusted, using Manhattan distance

    // Find the nearest cabs
    vector<Cab> nearestCabs = findNearestCabs(customerLocation, cabsDataset, k, p);

    // Print results
    cout << "Random Customer Location: [" << customerLocation.first << ", " << customerLocation.second << "]" << endl;
    cout << "Nearest Available Cabs:" << endl;
    for (const auto& cab : nearestCabs) {
        cout << "[" << cab.coordinates.first << ", " << cab.coordinates.second << "]" << endl;
    }

    return 0;
}