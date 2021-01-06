#include <string.h>
#include "party.h"

int Party::number_of_parties = 0;

Party::Party() :
    name(nullptr),
    id(0),
    party_leader(nullptr),
    party_size(0),
    party_size_logi(0),
    party_reps(nullptr)
{}

Party::Party(char* _name, Citizen* _party_leader)
    : party_leader(_party_leader), id(number_of_parties),
    party_size(5), party_size_logi(0)
{
    name = new char[strlen(_name) + 1];

    int cur_char = 0;
    while (_name[cur_char] != '\0') {
        name[cur_char] = _name[cur_char];
        cur_char++;
    }
    name[cur_char] = '\0';

    party_reps = new Citizen * [party_size];

    number_of_parties++;
}

Party::~Party()
{
    delete[] name;
    delete[] party_reps;
}


bool Party::addPartyRep(Citizen* new_rep)
{
    if (party_size_logi == party_size)
        resizeParty();

    party_reps[party_size_logi] = new_rep;
    party_size_logi++;

    return true;
}

void Party::resizeParty()
{
    int i;
    party_size *= 2;
    Citizen** new_arr = new Citizen * [party_size];
    for (i = 0; i < party_size_logi; i++) {

        new_arr[i] = party_reps[i];
    }

    delete[] party_reps;

    party_reps = new_arr;
}

ostream& operator<<(ostream& os, const Party& party)
{
    os << "Party name: " << party.name << ". Leader " << party.party_leader->getName() << '.' << endl;
    os << "County representatives:" << endl << endl;

    if (County::num_of_counties > 0) {
        for (int i = 0; i < County::num_of_counties; i++) {
            os << "County num " << i << ":" << endl;
            for (int j = 0; j < party.party_size_logi; j++) {
                if (party.party_reps[j]->getHomeCounty()->getId() == i) {
                    os << *party.party_reps[j] << endl << endl;
                }
            }
        }
    }
    else {
        for (int i = 0; i < party.party_size_logi; i++) {
            os << *party.party_reps[i] << endl << endl;
        }
    }

    return os;
}

void Party::save(ostream& out) const {
    /*Saving the name*/
    int len = strlen(name);
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

    /*Saving the party size*/
    out.write(rcastcc(&party_size), sizeof(party_size));

    /*Saving the logical party size*/
    out.write(rcastcc(&party_size_logi), sizeof(party_size_logi));

    /*Saving the party reps*/
    int cur_rep_id = 0;
    for (int i = 0; i < party_size_logi; i++) {
        cur_rep_id = party_reps[i]->getId();
        out.write(rcastcc(&cur_rep_id), sizeof(cur_rep_id));
    }
}

void Party::load(istream& in) {
    /*Loading the name*/
    int len;
    in.read(rcastc(&len), sizeof(len));
    name = new char[len + 1];
    for (int i = 0; i < len; i++)
    {
        in.read(rcastc(&name[i]), sizeof(char));
    }
    name[len] = '\0';

    /*Loading the id*/
    in.read(rcastc(&id), sizeof(id));
}