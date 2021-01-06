#include "election_cycle.h"

ElectionCycle::ElectionCycle() :
    date_of_election({ 0,0,0 }),
    current_vote_amount(0),
    residents_num_size(5),
    residents_num_logi(0),
    parties_num_size(5),
    parties_num_logi(0),
    residents(nullptr),
    parties(nullptr)
{}

ElectionCycle::ElectionCycle(Date& _date_of_election) :
    date_of_election(_date_of_election),
    current_vote_amount(0),
    residents_num_size(5),
    residents_num_logi(0),
    parties_num_size(5),
    parties_num_logi(0)
{
    residents = new Citizen * [residents_num_size];
    parties = new Party * [parties_num_size];

}

ElectionCycle::~ElectionCycle() {
    delete[] residents;
    delete[] parties;

}

void ElectionCycle::resizeResidents() {
    residents_num_size *= 2;
    Citizen** new_arr = new Citizen * [residents_num_size];

    for (int i = 0; i < residents_num_logi; i++) {
        new_arr[i] = residents[i];
    }

    delete[] residents;

    residents = new_arr;
}

void ElectionCycle::resizeParties() {
    parties_num_size *= 2;
    Party** new_arr = new Party * [parties_num_size];

    for (int i = 0; i < parties_num_logi; i++) {
        new_arr[i] = parties[i];
    }

    delete[] parties;

    parties = new_arr;
}

Citizen* ElectionCycle::getResident(int id) const {
    int cur = 0;
    do {
        if (residents[cur]->getId() == id) { return residents[cur]; }
        else { cur++; }
    } while (cur < residents_num_logi);

    return nullptr;
}

Party* ElectionCycle::getParty(char* party_name) const {
    for (int i = 0; i < parties_num_logi; i++) {
        if (!strcmp(parties[i]->getName(), party_name)) {
            return parties[i];
        }
    }

    return nullptr;
}

bool ElectionCycle::addResident(Citizen* resident) {
    if (residents_num_logi == residents_num_size) { resizeResidents(); }

    residents[residents_num_logi] = resident;
    residents_num_logi++;

    return true;
}

bool ElectionCycle::addVote() {
    current_vote_amount++;

    return true;
}

bool ElectionCycle::addParty(Party* party) {
    if (parties_num_logi == parties_num_size) { resizeParties(); }

    parties[parties_num_logi] = party;
    parties_num_logi++;

    return true;
}