#pragma once

#include "citizen.h"
#include "county.h"
#include "dynamic_array.h"
#include <fstream>
#include <string>

#define rcastcc reinterpret_cast<const char*>
#define rcastc reinterpret_cast<char*>

using namespace std;
class Citizen;

class Party {
    
private:
    string name;                                //name of the party
    int id;                                     //ID of the party 
    Citizen* party_leader;                      //Pointer to the party leader
    DynamicArray<Citizen*> party_reps;          //array of party rep

public:
    static int number_of_parties;

    /* Ctors & Dtors */
    Party();
    Party(string _name, Citizen* _party_leader);
    ~Party();

    /* Getters */
    string getName() { return name; }
    int getId() { return id; }
    Citizen* getLeader() { return party_leader; }
    DynamicArray<Citizen*> getPartyReps() { return party_reps; }
    int partyRepsLen() const { return party_reps.size(); }

    /* Setters */
    bool setLeader(Citizen* leader) { party_leader = leader; return true; }

    /* Methods */
    bool addPartyRep(Citizen* citizen);         //Add the citizen as party rep

    /* Operators */
    friend ostream& operator<<(ostream& os, const Party& party);

    /* Serialization */
    void save(ostream& out) const;
    void load(istream& in);
};