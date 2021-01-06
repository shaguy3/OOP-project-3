#pragma once
#include "citizen.h"
#include <fstream>

#define rcastcc reinterpret_cast<const char*>
#define rcastc reinterpret_cast<char*>

using namespace std;
class Citizen;


class County {

private:
    char* name;
    int id;
    bool is_relative;
    int number_of_electors;
    int current_vote_amount;
    Citizen** residents;
    int residents_num_size;
    int residents_num_logi;
    Citizen** chosen_electors;
    int chosen_electors_size;
    int chosen_electors_logi;
    void resizeResidents();
    void resizeChosenElectors();

public:
    static int num_of_counties;
    /* Counstructors and destructors */
    County();
    County(char* _name, int _number_of_electors, bool _is_relative);
    County(const County& other);
    ~County();

    /* Getters */
    char* getName() const { return name; }
    int getId() const { return id; }
    bool isRelative() const { return is_relative; }
    int getVoteAmount() const { return current_vote_amount; }
    int getNumberOfElectors() const { return number_of_electors; }
    Citizen** getResidents() const { return residents; }
    int residentsLen() const { return residents_num_logi; }
    Citizen** getChosenElectors() const { return chosen_electors; }
    int chosenElectorsLen() const { return chosen_electors_logi; }

    /* Setters */
    bool setChosenElectorsLen(int new_len) { chosen_electors_logi = new_len; return true; }
    bool setChosenElectorsSize(int new_len){ chosen_electors_size = new_len; return true; }
    bool init_chosen_electors() { chosen_electors = new Citizen * [chosen_electors_size]; return true; }
    bool setResidentsLen(int new_len) { residents_num_logi = new_len; return true; }
    bool setResidentsSize(int new_len) { residents_num_size = new_len; return true; }
    bool init_residents() { residents = new Citizen * [residents_num_size]; return true; }

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