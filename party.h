#pragma once

#include "citizen.h"
#include "county.h"
#include <fstream>
#include <string>

#define rcastcc reinterpret_cast<const char*>
#define rcastc reinterpret_cast<char*>

using namespace std;
class Citizen;

class Party {
    
private:
    string name;                                 //name of the party
    int id;                                     //ID of the party 
    Citizen* party_leader;                      //Pointer to the party leader
    Citizen** party_reps;                       //array of party rep
    int party_size;
    int party_size_logi;
    void resizeParty();
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
    Citizen** getPartyReps() { return party_reps; }
    int partyRepsLen() const { return party_size_logi; }

    /* Setters */
    bool setLeader(Citizen* leader) { party_leader = leader; return true; }
    bool setPartySize(int _party_size) { party_size = _party_size; return true; }
    bool setPartyLogi(int _party_logi) { party_size_logi = _party_logi; return true; }
    bool initReps(int size) { party_reps = new Citizen * [size]; return true; }

    /* Methods */
    bool addPartyRep(Citizen* citizen);         //Add the citizen as party rep

    /* Operators */
    friend ostream& operator<<(ostream& os, const Party& party);

    /* Serialization */
    void save(ostream& out) const;
    void load(istream& in);
};