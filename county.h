#pragma once
#include "citizen.h"
#include "dynamic_array.h"
#include <fstream>
#include <string>

#define rcastcc reinterpret_cast<const char*>
#define rcastc reinterpret_cast<char*>

using namespace std;
class Citizen;


class County {

protected:
    string name;
    int id;
    int number_of_electors;
    int current_vote_amount;
    DynamicArray<Citizen*> residents;
    DynamicArray<Citizen*> chosen_electors;

public:
    static int num_of_counties;
    /* Counstructors and destructors */
    County();
    County(string _name, int _number_of_electors);
    County(const County& other);
    virtual ~County();

    /* Getters */
    string getName() const { return name; }
    int getId() const { return id; }
    int getVoteAmount() const { return current_vote_amount; }
    int getNumberOfElectors() const { return number_of_electors; }
    DynamicArray<Citizen*> getResidents() const { return residents; }
    int residentsLen() const { return residents.size(); }
    DynamicArray<Citizen*> getChosenElectors() const { return chosen_electors; }
    int chosenElectorsLen() const { return chosen_electors.size(); }

    /* Setters */
    bool setChosenElectorsLen(int new_len) { chosen_electors.set_size(new_len); return true; }
    bool setResidentsLen(int new_len) { residents.set_size(new_len); return true; }

    /* Adders */
    bool addVote();
    bool addResident(Citizen* new_resident);
    bool addChosenElector(Citizen* chosen_elector);

    /* Operator overloads */
    friend ostream& operator<<(ostream& os, const County& county);
    void operator=(const County& other);

    /*Serialization*/
    void save(ostream& out) const;
    void load(istream& in);
};

class RelativeCounty : public County {
public:
    RelativeCounty() : County() {}
    RelativeCounty(string _name, int _number_of_electors) : 
        County (_name, _number_of_electors) {}

    virtual ~RelativeCounty() {}

    friend ostream& operator<<(ostream& os, const RelativeCounty& county);
};