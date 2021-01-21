#include <iostream>
#include <string.h>
#include "county.h"
#include <exception>

using namespace std;

int County::num_of_counties = 0;

County::County() :
    name(""),
    id(0),
    number_of_electors(0),
    current_vote_amount(0),
    residents(DynamicArray<Citizen*>()),
    chosen_electors(DynamicArray<Citizen*>())
{}

County::County(string _name, int _number_of_electors) :
    name(_name),
    id(County::num_of_counties),
    number_of_electors(_number_of_electors),
    current_vote_amount(0),
    residents(DynamicArray<Citizen*>()),
    chosen_electors(DynamicArray<Citizen*>())
{
    if (_number_of_electors < 1) {
        throw invalid_argument("Simple cycle: Number of electors is not valid.");
    }

    County::num_of_counties++;
}

County::County(const County& other) {
    *this = other;
}

bool County::addVote() {
    current_vote_amount++;

    return true;
}

bool County::addResident(Citizen* new_resident) {
    residents.push_back(new_resident);
    return true;
}

bool County::addChosenElector(Citizen* chosen_elector) {
    chosen_electors.push_back(chosen_elector);
    return true;
}

ostream& operator<<(ostream& os, const County& county) {
    os << "ID: " << county.id << " Name: " << county.name << ", Number of electors: " << county.number_of_electors << endl
       << "The county is not relative." << endl;

    return os;
}

ostream& operator<<(ostream& os, const RelativeCounty& county) {
    os << "ID: " << county.id << " Name: " << county.name << ", Number of electors: " << county.number_of_electors << endl
        << "The county is relative." << endl;

    return os;
}

void County::operator=(const County& other) {
    if (this != &other) {
        name = other.name;
        id = other.id;
        number_of_electors = other.number_of_electors;
        current_vote_amount = other.current_vote_amount;

        residents = other.residents;
        chosen_electors = other.chosen_electors;
    }
}

County::~County() {}

void County::save(ostream& out) const
{
    /*Saving the type of the cycle*/
    int county_type = -1;
    if (typeid(*this).name() == typeid(County).name()) {
        county_type = 0;
    }
    else {
        county_type = 1;
    }

    out.write(rcastcc(&county_type), sizeof(county_type));

    /*Saving the name*/
    int len = name.size();
    out.write(rcastcc(&len), sizeof(len));  
    for (int i = 0; i < len; i++)
    {
        out.write(rcastcc(&name[i]), sizeof(char));
    }

    /*Saving the id*/
    out.write(rcastcc(&id), sizeof(id));

    /*Saving if the county is relative*/
    // out.write(rcastcc(&is_relative), sizeof(is_relative));

    /*Saving the current vote amount*/
    out.write(rcastcc(&current_vote_amount), sizeof(current_vote_amount));

    /*Saving number of the electors*/
    out.write(rcastcc(&number_of_electors), sizeof(number_of_electors));

    /* Saving the logical number of electors */
    int chosen_electors_size = chosen_electors.size();
    out.write(rcastcc(&chosen_electors_size), sizeof(chosen_electors_size));

    /* Saving the chosen elector's ids */
    int cur_id = 0;
    for (int i = 0; i < chosen_electors_size; i++) {
        cur_id = chosen_electors[i]->getId();
        out.write(rcastcc(&cur_id), sizeof(cur_id));
    }
}

void County::load(istream& in)
{
    int len;
    in.read(rcastc(&len), sizeof(len));
    name.resize(len);
    for (int i = 0; i < len; i++)
    {
        in.read(rcastc(&name[i]), sizeof(char));
    }
    name[len] = '\0';

    in.read(rcastc(&id), sizeof(id));
    in.read(rcastc(&current_vote_amount), sizeof(current_vote_amount));
    in.read(rcastc(&number_of_electors), sizeof(number_of_electors));

    int chosen_electors_capacity, chosen_electors_size;
    in.read(rcastc(&chosen_electors_size), sizeof(chosen_electors_size));
    chosen_electors.set_size(chosen_electors_size);
}