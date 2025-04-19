#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <initializer_list>
#include <algorithm>

using namespace std;

enum class Category { NewRelease, Drama, Comedy, Romance, Horror };

string categoryToString(Category c) {
    switch (c) {
        case Category::NewRelease: return "New Release";
        case Category::Drama: return "Drama";
        case Category::Comedy: return "Comedy";
        case Category::Romance: return "Romance";
        case Category::Horror: return "Horror";
    }
    return "Unknown";
}

class Video {
    string name;
    Category category;
    double pricePerDay;
public:
    Video(string n, Category c, double p) : name(n), category(c), pricePerDay(p) {}
    string getName() const { return name; }
    Category getCategory() const { return category; }
    double getPrice() const { return pricePerDay; }
};

class Rental;

class Customer : public enable_shared_from_this<Customer> {
protected:
    string name;
    vector<shared_ptr<Rental>> rentals;
public:
    Customer(string n) : name(n) {}
    virtual ~Customer() = default;
    string getName() const { return name; }
    bool canRent(int videoCount);
    void addRental(shared_ptr<Rental> r) { rentals.push_back(r); }
    virtual shared_ptr<Rental> createRental(const vector<shared_ptr<Video>>& inventory, int currentDay) = 0;
};

class Rental {
    shared_ptr<Customer> customer;
    vector<shared_ptr<Video>> videos;
    int nights;
    double totalPrice;
    int startDay;
public:
    Rental(shared_ptr<Customer> c, vector<shared_ptr<Video>> vids, int n, int day)
        : customer(c), videos(vids), nights(n), startDay(day) {
        totalPrice = 0;
        for (auto& v : videos) totalPrice += v->getPrice() * n;
    }
    int getReturnDay() const { return startDay + nights; }
    double getTotalPrice() const { return totalPrice; }
    bool isReturned(int day) const { return day >= getReturnDay(); }
    const vector<shared_ptr<Video>>& getVideos() const { return videos; }
    shared_ptr<Customer> getCustomer() const { return customer; }
    int getNights() const { return nights; }
};

class BreezyCustomer : public Customer {
public:
    BreezyCustomer(string n) : Customer(n) {}
    shared_ptr<Rental> createRental(const vector<shared_ptr<Video>>& inventory, int currentDay) override;
};

class HoarderCustomer : public Customer {
public:
    HoarderCustomer(string n) : Customer(n) {}
    shared_ptr<Rental> createRental(const vector<shared_ptr<Video>>& inventory, int currentDay) override;
};

class RegularCustomer : public Customer {
public:
    RegularCustomer(string n) : Customer(n) {}
    shared_ptr<Rental> createRental(const vector<shared_ptr<Video>>& inventory, int currentDay) override;
};

class VideoStore {
    vector<shared_ptr<Video>> allVideos;
    vector<shared_ptr<Video>> inventory;
    vector<shared_ptr<Customer>> customers;
    vector<shared_ptr<Rental>> allRentals;
    double totalRevenue = 0;
    int currentDay = 0;

public:
    void initialize();
    void simulateDay();
    void runSimulation(int days);
    void printReport() const;
};

// stub implementations for customer types
template <typename T>
shared_ptr<Rental> makeRental(shared_ptr<Customer> cust, const vector<shared_ptr<Video>>& inventory, int minV, int maxV, int minN, int maxN, int currentDay) {
    int count = rand() % (maxV - minV + 1) + minV;
    if ((int)inventory.size() < count) return nullptr;
    vector<shared_ptr<Video>> selected(inventory.begin(), inventory.begin() + count);
    int nights = rand() % (maxN - minN + 1) + minN;
    return make_shared<Rental>(cust, selected, nights, currentDay);
}

shared_ptr<Rental> BreezyCustomer::createRental(const vector<shared_ptr<Video>>& inventory, int currentDay) {
    return makeRental<BreezyCustomer>(shared_from_this(), inventory, 1, 2, 1, 2, currentDay);
}

shared_ptr<Rental> HoarderCustomer::createRental(const vector<shared_ptr<Video>>& inventory, int currentDay) {
    if ((int)inventory.size() < 3) return nullptr;
    vector<shared_ptr<Video>> selected = { inventory[0], inventory[1], inventory[2] };
    return make_shared<Rental>(shared_from_this(), selected, 7, currentDay);
}

shared_ptr<Rental> RegularCustomer::createRental(const vector<shared_ptr<Video>>& inventory, int currentDay) {
    return makeRental<RegularCustomer>(shared_from_this(), inventory, 1, 3, 3, 5, currentDay);
}

void VideoStore::initialize() {
    for (int i = 0; i < 4; ++i) {
        allVideos.push_back(make_shared<Video>("NewRelease_" + to_string(i), Category::NewRelease, 5.0));
        allVideos.push_back(make_shared<Video>("Drama_" + to_string(i), Category::Drama, 3.0));
        allVideos.push_back(make_shared<Video>("Comedy_" + to_string(i), Category::Comedy, 2.0));
        allVideos.push_back(make_shared<Video>("Romance_" + to_string(i), Category::Romance, 3.0));
        allVideos.push_back(make_shared<Video>("Horror_" + to_string(i), Category::Horror, 4.0));
    }
    inventory = allVideos;

    customers.push_back(make_shared<BreezyCustomer>("Alice"));
    customers.push_back(make_shared<HoarderCustomer>("Bob"));
    customers.push_back(make_shared<RegularCustomer>("Carol"));
    customers.push_back(make_shared<BreezyCustomer>("Dave"));
    customers.push_back(make_shared<RegularCustomer>("Eve"));
    customers.push_back(make_shared<HoarderCustomer>("Frank"));
    customers.push_back(make_shared<RegularCustomer>("Grace"));
    customers.push_back(make_shared<BreezyCustomer>("Heidi"));
    customers.push_back(make_shared<HoarderCustomer>("Ivan"));
    customers.push_back(make_shared<RegularCustomer>("Judy"));
}

void VideoStore::simulateDay() {
    // 處理歸還
    vector<shared_ptr<Video>> returnedToday;
    for (auto it = allRentals.begin(); it != allRentals.end(); ) {
        if ((*it)->isReturned(currentDay)) {
            for (auto& v : (*it)->getVideos()) {
                returnedToday.push_back(v);
            }
            it = allRentals.erase(it); // 移除已完成租借
        } else {
            ++it;
        }
    }
    inventory.insert(inventory.end(), returnedToday.begin(), returnedToday.end());

    // 模擬最多 5 位顧客來店
    int customerCount = rand() % 5 + 1;
    random_shuffle(customers.begin(), customers.end());

    for (int i = 0; i < customerCount && !inventory.empty(); ++i) {
        auto& cust = customers[i];
        auto rental = cust->createRental(inventory, currentDay);
        if (rental) {
            // 從庫存中移除租出去的影片
            for (auto& v : rental->getVideos()) {
                auto it = find(inventory.begin(), inventory.end(), v);
                if (it != inventory.end()) inventory.erase(it);
            }
            totalRevenue += rental->getTotalPrice();
            allRentals.push_back(rental);
        }
    }
}

void VideoStore::runSimulation(int days) {
    for (currentDay = 1; currentDay <= days; ++currentDay) {
        simulateDay();
    }
}

void VideoStore::printReport() const {
    cout << "\n===== Final Report =====\n";
    cout << "Videos remaining in store: " << inventory.size() << endl;
    for (auto& v : inventory) {
        cout << " - " << v->getName() << " (" << categoryToString(v->getCategory()) << ")\n";
    }
    cout << "Total revenue: $" << totalRevenue << endl;
    cout << "(Note: rental logs not implemented yet)\n";
}

int main() {
    srand(time(0));
    VideoStore store;
    store.initialize();
    store.runSimulation(35);
    store.printReport();
    return 0;
}