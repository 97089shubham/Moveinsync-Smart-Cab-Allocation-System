#include<bits/stdc++.h>

using namespace std;

// location class
class Location {
public:
    double latitude;
    double longitude;
    string pincode;

    Location(double lat, double lon, string pin) : latitude(lat), longitude(lon), pincode(pin) {}

    double distanceTo(const Location& other) const {
        double latDiff = latitude - other.latitude;
        double lonDiff = longitude - other.longitude;
        return sqrt(latDiff * latDiff + lonDiff * lonDiff);
    }
};

// cab class having all properties
class Cab {
public:
    int id;
    Location location;
    Location source;
    Location destination;
    bool moving;
    double speed;  // Assume some constant speed for simplicity

    Cab(int id, double lat, double lon, double srcLat, double srcLon, double destLat, double destLon, bool moving, double speed)
        : id(id), location(lat, lon, ""), source(srcLat, srcLon, ""), destination(destLat, destLon, ""), moving(moving), speed(speed) {}

    double calculateETA(const Location& dest) const {
        if (moving) {
            double distToDest = location.distanceTo(destination);
            double timeToDest = distToDest / speed;
            double distFromDestToTrip = destination.distanceTo(dest);
            double timeFromDestToTrip = distFromDestToTrip / speed;
            return timeToDest + timeFromDestToTrip;
        } else {
            return location.distanceTo(dest) / speed;
        }
    }
};

// trip class storing source and destination locations
class Trip {
public:
    Location startLocation;
    Location endLocation;

    Trip(double startLat, double startLon, double endLat, double endLon, string src_pin, string end_pin)
        : startLocation(startLat, startLon, src_pin), endLocation(endLat, endLon, end_pin) {}
};

class CabSystem {
private:
    vector<Cab> cabs;
    vector<Trip> trips;
    vector<Location> pincodeLocations;

    void loadCabs(const string& filename) {
        ifstream file(filename);
        string line;
        getline(file, line); // Skip header
        while (getline(file, line)) {
            stringstream ss(line);
            string temp;
            vector<string> data;
            while (getline(ss, temp, ',')) {
                data.push_back(temp);
            }
            int id = stoi(data[0]);
            double lat = stod(data[1]);
            double lon = stod(data[2]);
            double srcLat = stod(data[3]);
            double srcLon = stod(data[4]);
            double destLat = data[5].empty() ? 0 : stod(data[5]);
            double destLon = data[6].empty() ? 0 : stod(data[6]);
            bool moving = data[7] == "True";
            double speed = 0.0005; // Assign a constant speed for simplicity
            cabs.emplace_back(id, lat, lon, srcLat, srcLon, destLat, destLon, moving, speed);
        }
    }

    void loadPincodeData(const string& filename) {
        ifstream file(filename);
        string line;
        getline(file, line); // Skip header
        while (getline(file, line)) {
            stringstream ss(line);
            string temp;
            vector<string> data;
            while (getline(ss, temp, ',')) {
                data.push_back(temp);
            }
            string pin = data[1];
            double lat = stod(data[2]);
            double lon = stod(data[3]);
            pincodeLocations.emplace_back(lat, lon, pin);
        }
    }

    Trip generateRandomTrip() {
        int startIdx = rand() % pincodeLocations.size();
        int endIdx = rand() % pincodeLocations.size();
        while (endIdx == startIdx) {
            endIdx = rand() % pincodeLocations.size();
        }
        return Trip(pincodeLocations[startIdx].latitude, pincodeLocations[startIdx].longitude,
                    pincodeLocations[endIdx].latitude, pincodeLocations[endIdx].longitude, pincodeLocations[startIdx].pincode, pincodeLocations[endIdx].pincode);
    }

    void writeAllocatedCabToCSV(const Cab& cab) {
        ofstream file("external_cab_updates.csv", ios::app);
        file << cab.id << ","
             << cab.source.latitude << ","
             << cab.source.longitude << ","
             << cab.destination.latitude << ","
             << cab.destination.longitude << "\n";
    }

public:
    CabSystem(const string& cabFile, const string& pincodeFile) {
        loadCabs(cabFile);
        loadPincodeData(pincodeFile);
        srand(time(0));

        ofstream file("external_cab_updates.csv");
        file << "id,source_latitude,source_longitude,dest_latitude,dest_longitude\n";
    }

    void newTrip() {
        Trip trip = generateRandomTrip();
        double minTime = numeric_limits<double>::max();
        Cab* bestCab = nullptr;

        for (auto& cab : cabs) {
            double eta = cab.calculateETA(trip.startLocation);
            if (eta < minTime) {
                minTime = eta;
                bestCab = &cab;
            }
        }

        if (bestCab) {
            cout << "Trip from pincode " << trip.startLocation.pincode << " (" << trip.startLocation.latitude << ", " << trip.startLocation.longitude << ") to pincode " << trip.endLocation.pincode << " ("
                << trip.endLocation.latitude << ", " << trip.endLocation.longitude << ") allocated to Cab ID "
                << bestCab->id << endl;
            if (bestCab->moving) {
                cout << "Note: The allocated cab (ID " << bestCab->id << ") is currently moving at ("<< bestCab->location.latitude << ", "<< bestCab->location.longitude <<")" << endl;
            }
            cout << "ETA for the cab: " << minTime << " units" << endl;
            bestCab->moving = true; // Set the cab as moving
            bestCab->source = trip.startLocation;
            bestCab->destination = trip.endLocation;

            // Write the allocated cab to the external cab updates CSV
            writeAllocatedCabToCSV(*bestCab);
        } else {
            cout << "No available cab for trip from (" << trip.startLocation.latitude << ", " << trip.startLocation.longitude << ") to ("
                << trip.endLocation.latitude << ", " << trip.endLocation.longitude << ")" << endl;
        }
    }
};

int main() {
    CabSystem system("cab_data.csv", "BangaloreAreaLatLongDetails.csv");
    while(true){
        cout<<"Enter no of trips to add :- "<<endl;
        int num;
        cin>>num;
        while(num--)
            system.newTrip();
        cout<<endl;
    }
    return 0;
}
