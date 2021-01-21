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

ElectionCycle::~ElectionCycle() {
    for (int i = 0; i < residents.size(); i++) {
        delete residents[i];
    }

    for (int i = 0; i < parties.size(); i++) {
        delete parties[i];
    }
}

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

bool ElectionCycle::addResident(string resident_name, int id, int year_of_birth, County* home_county) {
    Citizen* new_resident = new Citizen(resident_name, id, year_of_birth, home_county);
    residents.push_back(new_resident);
    return true;
}

bool ElectionCycle::addVote() {
    current_vote_amount++;
    return true;
}

bool ElectionCycle::addParty(string party_name, Citizen* party_leader) {
    Party* new_party = new Party(party_name, party_leader);
    parties.push_back(new_party);
    return true;
}