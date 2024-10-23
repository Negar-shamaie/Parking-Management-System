#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <string>
#include <map>
using namespace std;

const string CMD_REQUEST_SPOT = "request_spot";
const string CMD_ASSIGN_SPOT = "assign_spot";
const string CMD_PASS_TIME = "pass_time";
const string CMD_CHECKOUT = "checkout";

struct CAR {
    string name;
    int size;
    CAR(string _name, int _size) : name(_name), size(_size) {}
};

struct ParkingSpot {
    int id;
    int size;
    string type;
    bool ISoccupied;
    ParkingSpot(int _id, int _size, string _type) : id(_id), size(_size), type(_type), ISoccupied(false) {}
};

struct Price {
    int size;
    int static_price;
    int price_per_day;
    Price(int _size, int _static_price, int _price_per_day) : size(_size), static_price(_static_price), price_per_day(_price_per_day) {}
};

bool string_to_int(const string& str, int& result) {
    stringstream ss(str);
    ss >> result;
    return !ss.fail() && ss.eof();
}

void read_data_files(const string& cars_file, const string& parking_file, const string& price_file, vector<CAR>& cars, vector<ParkingSpot>& parking_spots, vector<Price>& prices) {

    ifstream car_stream(cars_file);
    string line;
    getline(car_stream, line); 
    while (getline(car_stream, line)) {
        stringstream ss(line);
        string name, size_str;
        int size;

        if (getline(ss, name, ',') && getline(ss, size_str)) {
            if (string_to_int(size_str, size)) {
                cars.emplace_back(name, size);
            } 
        }
    }

    ifstream parking_stream(parking_file);
    getline(parking_stream, line); 
    while (getline(parking_stream, line)) {
        stringstream ss(line);
        string id_str, size_str, type;
        int id, size;

        if (getline(ss, id_str, ',') && getline(ss, size_str, ',') && getline(ss, type)) {
            if (string_to_int(id_str, id) && string_to_int(size_str, size)) {
                parking_spots.emplace_back(id, size, type);
            } 
        }
    }


    ifstream price_stream(price_file);
    getline(price_stream, line); 
    while (getline(price_stream, line)) {
        stringstream ss(line);
        string size_str, static_price_str, price_per_day_str;
        int size, static_price, price_per_day;

        if (getline(ss, size_str, ',') && getline(ss, static_price_str, ',') && getline(ss, price_per_day_str)) {
            if (string_to_int(size_str, size) && string_to_int(static_price_str, static_price) && string_to_int(price_per_day_str, price_per_day)) {
                prices.emplace_back(size, static_price, price_per_day);
            } 
        }
    }
}


void request_spot(const string& car_name, vector<CAR>& cars, vector<ParkingSpot>& parking_spots, const vector<Price>& prices) {

    auto car_it = find_if(cars.begin(), cars.end(), [&](const CAR& car) {
        return car.name == car_name;
    });

    int car_size = car_it->size;
    vector<ParkingSpot*> suitable_spots;

    for (auto& spot : parking_spots) {
        if (!spot.ISoccupied && spot.size == car_size) {
            suitable_spots.push_back(&spot); 
        }
    }

    sort(suitable_spots.begin(), suitable_spots.end(), [](const ParkingSpot* a, const ParkingSpot* b) {
        return a->id < b->id; 
    });

    for (const auto& spot : suitable_spots) {

        auto price_it = find_if(prices.begin(), prices.end(), [&](const Price& price) {
            return price.size == spot->size;
        });

        int static_price = 0;
        int price_per_day = 0;

        if (price_it != prices.end()) {
            if (spot->type == "normal") {
                static_price = price_it->static_price; 
                price_per_day = price_it->price_per_day;
            } else if (spot->type == "covered") {
                static_price = price_it->static_price + 50; 
                price_per_day = price_it->price_per_day + 30;
            } else if (spot->type == "CCTV") {
                static_price = price_it->static_price + 80; 
                price_per_day = price_it->price_per_day + 60; 
            }
        }

        cout << spot->id << ": " << spot->type << " " << static_price << " " << price_per_day<< endl;
    }
}


void assign_spot(int spot_id, vector<ParkingSpot>& parking_spots, map<int, int>& spot_days_passed) {
    auto spot_it = find_if(parking_spots.begin(), parking_spots.end(), [spot_id](const ParkingSpot& spot) {
        return spot.id == spot_id;
    });


    spot_it->ISoccupied = true;
    spot_days_passed[spot_id] = 0; 
    cout << "Spot " << spot_id << " is occupied now." << endl;
}

void pass_time(int number_of_days, vector<ParkingSpot>& parking_spots, map<int, int>& spot_days_passed) {
    for (auto& spot : parking_spots) {
        if (spot.ISoccupied) {
            spot_days_passed[spot.id] += number_of_days; 
        }
    }
}

void checkout(int spot_id, vector<ParkingSpot>& parking_spots, const vector<Price>& prices, const map<int, int>& spot_days_passed) {
    auto spot_it = find_if(parking_spots.begin(), parking_spots.end(), [spot_id](const ParkingSpot& spot) {
        return spot.id == spot_id;
    });

    spot_it->ISoccupied = false; 
    string spot_type = spot_it->type;

    auto price_it = find_if(prices.begin(), prices.end(), [&spot_it](const Price& price) {
        return price.size == spot_it->size;
    });

    int static_price = price_it->static_price;
    int day_per_price = price_it->price_per_day;
    if (spot_type == "covered") {
        static_price += 50; 
        day_per_price += 30; 
    } else if (spot_type == "CCTV") {
        static_price += 80; 
        day_per_price += 60; 
    }

    int total_days = spot_days_passed.at(spot_id); 
    int total_cost = static_price + (total_days * day_per_price);
    
    cout << "Spot " << spot_id << " is free now." << endl<< "Total cost: " << total_cost << endl;

}


void handle_commands(vector<CAR>& cars, vector<ParkingSpot>& parking_spots, vector<Price>& prices, map<int, int>& spot_days_passed) {
    string command;
    while (getline(cin, command)) { 
    
        stringstream ss(command);
        string cmd;
        ss >> cmd; 

        if (cmd == CMD_REQUEST_SPOT) {
            string car_name;
            ss >> car_name; 
            
            request_spot(car_name, cars, parking_spots, prices);
        } 
        else if (cmd == CMD_ASSIGN_SPOT) {
            int spot_id;
            ss >> spot_id; 
            assign_spot(spot_id, parking_spots, spot_days_passed);
        } 
        else if (cmd == CMD_PASS_TIME) {
            int number_of_days;
            ss >> number_of_days;
            pass_time(number_of_days, parking_spots, spot_days_passed);
        } 
        else if (cmd == CMD_CHECKOUT) {
            int spot_id;
            ss >> spot_id;
            checkout(spot_id, parking_spots, prices, spot_days_passed);
        } 

    }
}


int main(int argc, char* argv[]) {
    map<int, int> spot_days_passed; 
    vector<CAR> cars;
    vector<ParkingSpot> parking_spots;
    vector<Price> prices;

    read_data_files(argv[1], argv[2], argv[3], cars, parking_spots, prices);

    handle_commands(cars, parking_spots, prices, spot_days_passed);

    return 0;
}

