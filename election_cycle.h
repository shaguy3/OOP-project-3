#pragma once
#include "date.h"
#include "citizen.h"
#include "party.h"
#include "county.h"
#include <iostream>
#include <string.h>

using namespace std;

class ElectionCycle {
protected:
    int current_vote_amount;        //Increases when voting 
    Date date_of_election;
    Citizen** residents;
    int residents_num_size;
    int residents_num_logi;
    Party** parties;
    int parties_num_size;
    int parties_num_logi;
    void resizeResidents();
    void resizeParties();

public:
    /* Constructors and destructors */
    ElectionCycle();
    ElectionCycle(Date& _date_of_election);
    virtual ~ElectionCycle();

    /* Getters */
    Date& getDate() { return date_of_election; }
    Citizen** getResidents() const { return residents; }
    Party** getParties() const { return parties; }
    int getVoteAmount() const { return current_vote_amount; }
    int residentslen() const { return residents_num_logi; }
    int partieslen() const { return parties_num_logi; }

    /* Get specific items from the arrays */
    Citizen* getResident(int id) const;
    Party* getParty(char* party_name) const;

    /* Adders */
    bool addVote();
    bool addResident(Citizen* resident);
    bool addParty(Party* party);

    /* Serialization */
    virtual void save(ostream& out) const = 0;
    virtual void load(istream& in) = 0;
};