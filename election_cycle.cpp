#include "election_cycle.h"

ElectionCycle::ElectionCycle() :
    date_of_election(Date()),
    current_vote_amount(0),
    residents(DynamicArray<Citizen*>()),
    parties(DynamicArray<Party*>())
{}

ElectionCycle::ElectionCycle(Date& _date_of_election) :
    date_of_election(_date_of_election),
    current_vote_amount(0),
    residents(DynamicArray<Citizen*>()),
    parties(DynamicArray<Party*>())
{}

ElectionCycle::~ElectionCycle() {}

Citizen* ElectionCycle::getResident(int id) const {
    int cur = 0;
    do {
        if (residents[cur]->getId() == id) { return residents[cur]; }
        else { cur++; }
    } while (cur < residents.size());

    return nullptr;
}

Party* ElectionCycle::getParty(string party_name) const {
    for (int i = 0; i < parties.size(); i++) {
        if (parties[i]->getName() == party_name) {
            return parties[i];
        }
    }

    return nullptr;
}

bool ElectionCycle::addResident(Citizen* resident) {
    residents.push_back(resident);
    return true;
}

bool ElectionCycle::addVote() {
    current_vote_amount++;

    return true;
}

bool ElectionCycle::addParty(Party* party) {
    parties.push_back(party);
    return true;
}