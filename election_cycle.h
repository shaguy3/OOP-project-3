#pragma once
#include "date.h"
#include "citizen.h"
#include "party.h"
#include "county.h"
#include "dynamic_array.h"
#include <string>
#include <iostream>
#include <string.h>

using namespace std;

class ElectionCycle {
protected:
    int current_vote_amount;        //Increases when voting 
    Date date_of_election;
    DynamicArray<Citizen*> residents;
    DynamicArray<Party*> parties;

public:
    /* Constructors and destructors */
    ElectionCycle();
    ElectionCycle(Date& _date_of_election);
    virtual ~ElectionCycle();

    /* Getters */
    Date& getDate() { return date_of_election; }
    DynamicArray<Citizen*>& getResidents()  { return residents; }
    DynamicArray<Party*>& getParties()  { return parties; }
    int getVoteAmount() const { return current_vote_amount; }
    int residentslen() const { return residents.size(); }
    int partieslen() const { return parties.size(); }

    /* Get specific items from the arrays */
    Citizen* getResident(int id) const;
    Party* getParty(string party_name) const;

    /* Adders */
    bool addVote();
    bool addResident(string resident_name, int id, int year_of_birth, County* home_county);
    bool addParty(string party_name, Citizen* party_leader);

    /* Serialization */
    virtual void save(ostream& out) const = 0;
    virtual void load(istream& in) = 0;
};