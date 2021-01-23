#include <string.h>
#include "party.h"

int Party::number_of_parties = 0;

Party::Party() :
    name(""),
    id(0),
    party_leader(nullptr),
    party_reps(DynamicArray<Citizen*>())
{}

Party::Party(string _name, Citizen* _party_leader)
    : name(_name), party_leader(_party_leader), id(number_of_parties),
    party_reps(DynamicArray<Citizen*>())
{
    number_of_parties++;
}

Party::~Party() {}


bool Party::addPartyRep(Citizen* new_rep)
{
    party_reps.push_back(new_rep);
    return true;
}

ostream& operator<<(ostream& os, const Party& party)
{
    os << "Party name: " << party.name << ". Leader " << party.party_leader->getName() << '.' << endl;
    os << "County representatives:" << endl << endl;

    if (County::num_of_counties > 0) {
        for (int i = 0; i < County::num_of_counties; i++) {
            os << "County num " << i << ":" << endl;
            for (int j = 0; j < party.party_reps.size(); j++) {
                if (party.party_reps[j]->getRepCounty()->getId() == i) {
                    os << *party.party_reps[j] << endl << endl;
                }
            }
        }
    }
    else {
        for (int i = 0; i < party.party_reps.size(); i++) {
            os << *party.party_reps[i] << endl << endl;
        }
    }

    return os;
}

const Party& Party::operator=(const Party& other)
{
    if (this != &other)
    {
        name = other.name;
        id = other.id;
        party_leader = other.party_leader;
        party_reps = other.party_reps;

    }
    return *this;
}

void Party::save(ostream& out) const {
    /*Saving the name*/
    int len = name.size();
    out.write(rcastcc(&len), sizeof(len));
    for (int i = 0; i < len; i++)
    {
        out.write(rcastcc(&name[i]), sizeof(char));
    }

    /*Saving the id*/
    out.write(rcastcc(&id), sizeof(id));

    /*Saving the leader id*/
    int leader_id = party_leader->getId();
    out.write(rcastcc(&leader_id), sizeof(leader_id));

    /*Saving the logical party size*/
    int party_reps_size = party_reps.size();
    out.write(rcastcc(&party_reps_size), sizeof(party_reps_size));

    /*Saving the party reps*/
    int cur_rep_id = 0;
    for (int i = 0; i < party_reps_size; i++) {
        cur_rep_id = party_reps[i]->getId();
        out.write(rcastcc(&cur_rep_id), sizeof(cur_rep_id));
    }
}

void Party::load(istream& in) {
    /*Loading the name*/
    int len;
    in.read(rcastc(&len), sizeof(len));
    name.resize(len);
    for (int i = 0; i < len; i++)
    {
        in.read(rcastc(&name[i]), sizeof(char));
    }
    name[len] = '\0';

    /*Loading the id*/
    in.read(rcastc(&id), sizeof(id));
}