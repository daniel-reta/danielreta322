#include <iostream>
#include <vector>
#include <random>
#include <map>

using namespace std;

enum class HealthStatus {
    SUSCEPTIBLE,
    SICK,
    RECOVERED,
    VACCINATED
};

class Disease {
private:
    int duration; // Duration of the disease in days
    double transmissionProbability; // Probability of disease transmission

public:
    Disease(int days, double transmissionProb) : duration(days), transmissionProbability(transmissionProb) {}

    int getDuration() const {
        return duration;
    }

    double getTransmissionProbability() const {
        return transmissionProbability;
    }
};

class Population;

class Person {
private:
    string name;
    HealthStatus status;
    int daysLeftSick;
    int initialDuration; // Initial duration of the disease when infected
    int x, y; // coordinates of the person in a grid

public:
    Person(const string& personName) : name(personName), status(HealthStatus::SUSCEPTIBLE), daysLeftSick(0), initialDuration(0), x(0), y(0) {}

    HealthStatus getStatus() const {
        return status;
    }

    void infect(const Disease& disease) {
        if (status == HealthStatus::SUSCEPTIBLE) {
            status = HealthStatus::SICK;
            daysLeftSick = disease.getDuration(); // Set initial daysLeftSick to full duration
            initialDuration = disease.getDuration(); // Store initial duration
        }
    }

    void one_more_day() {
        if (status == HealthStatus::SICK) {
            if (daysLeftSick > 0) {
                --daysLeftSick;
                if (daysLeftSick == 0) {
                    status = HealthStatus::RECOVERED;
                }
            }
        }
    }

    void vaccinate() {
        if (status == HealthStatus::SUSCEPTIBLE) {
            status = HealthStatus::VACCINATED;
        }
    }

    void setCoordinates(int xPos, int yPos) {
        x = xPos;
        y = yPos;
    }

    pair<int, int> getCoordinates() const {
        return {x, y};
    }

    void interactWithRandomPeople(Population& population, const Disease& disease, int numInteractionsPerDay);

    friend class Population; // Grant access to Population class
};

class Population {
private:
    vector<Person> people;
    map<pair<int, int>, Person*> grid;
    int width, height; // Width and height of the population grid
    int totalInfected; // Track total infected people

public:
    Population(const vector<string>& names) : totalInfected(0) {
        for (const auto& name : names) {
            Person person(name);
            people.push_back(person);
        }
        width = names.size();
        height = 1; // For a 1D representation
    }

    void setupGrid() {
        int currentX = 0, currentY = 0;
        for (auto& person : people) {
            person.setCoordinates(currentX, currentY);
            currentX++;
        }

        for (auto& person : people) {
            auto coords = person.getCoordinates();
            grid[coords] = &person;
        }
    }

    void one_more_day(const Disease& disease, int numInteractionsPerDay);

    auto begin() {
        return people.begin();
    }

    auto end() {
        return people.end();
    }

    int count_infected() const {
        int infectedCount = 0;
        for (const auto& person : people) {
            if (person.getStatus() == HealthStatus::SICK) {
                infectedCount++;
            }
        }
        return infectedCount;
    }

    Person* getRandomPerson() {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dis(0, people.size() - 1);
        return &people[dis(gen)];
    }

    void vaccinateRandomPeople(int numToVaccinate) {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dis(0, people.size() - 1);

        int count = 0;
        while (count < numToVaccinate) {
            int index = dis(gen);
            if (people[index].getStatus() == HealthStatus::SUSCEPTIBLE) {
                people[index].vaccinate();
                count++;
            }
        }
    }

    Person* getPersonAtCoordinates(const pair<int, int>& coordinates) {
        if (grid.count(coordinates) > 0) {
            return grid[coordinates];
        }
        return nullptr;
    }

    int getWidth() const {
        return width;
    }

    int getHeight() const {
        return height;
    }

    int getTotalInfected() const {
        return totalInfected;
    }

    void increaseTotalInfected() {
        totalInfected++;
    }
};

void Person::interactWithRandomPeople(Population& population, const Disease& disease, int numInteractionsPerDay) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(0, population.getWidth() - 1);

    for (int i = 0; i < numInteractionsPerDay; ++i) {
        int randomIndex = dis(gen);
        Person* randomPerson = population.getPersonAtCoordinates({randomIndex, 0}); // Assuming a 1D grid

        if (randomPerson && randomPerson->getStatus() == HealthStatus::SUSCEPTIBLE) {
            double transmissionChance = dis(gen) / static_cast<double>(numeric_limits<int>::max());
            if (transmissionChance <= disease.getTransmissionProbability()) {
                randomPerson->infect(disease);
                population.increaseTotalInfected();
            }
        }
    }
}

void Population::one_more_day(const Disease& disease, int numInteractionsPerDay) {
    for (auto& person : people) {
        person.one_more_day();
    }

    for (auto& person : people) {
        if (person.getStatus() == HealthStatus::SICK) {
            person.interactWithRandomPeople(*this, disease, numInteractionsPerDay);
        }
    }
}

int main() {
    int populationSize = 10; // Set population size
    int numVaccinated = 0.4*populationSize; // Set the number of vaccinated people
    double transmissionProbability = 0.3; // Probability of disease transmission
    int numInteractionsPerDay = 2; // Set the number of interactions per day

    vector<string> names;
    for (int i = 1; i <= populationSize; ++i) {
        names.push_back("Person" + to_string(i));
    }

    Population population(names);

    Disease disease(5, transmissionProbability); // Disease lasts for 5 days with a specified transmission probability

    population.setupGrid();

    population.getRandomPerson()->infect(disease); // Infect a random person

    population.vaccinateRandomPeople(numVaccinated); // Vaccinate a specified number of random people

    int totalDays = 30; // Total days for simulation

    for (int day = 1; day <= totalDays; ++day) {
        cout << "In day " << day << " #sick: " << population.count_infected() << " : ";

        for (const auto& person : population) {
            if (person.getStatus() == HealthStatus::SICK) {
                cout << "+ ";
            } else if (person.getStatus() == HealthStatus::SUSCEPTIBLE) {
                cout << "? ";
            } else if (person.getStatus() == HealthStatus::RECOVERED) {
                cout << "- ";
            } else if (person.getStatus() == HealthStatus::VACCINATED) {
                cout << "* ";
            }
        }

        cout << endl;

        population.one_more_day(disease, numInteractionsPerDay);

        int infectedCount = population.count_infected(); // Count infected after one_more_day operation
        if (infectedCount == 0) {
            cout << "Disease ran its course by day " << day << endl;
            break;
        }
    }

    cout << "Total infected: " << population.getTotalInfected() + 1 << endl; // Print total infected count

    return 0;
}
