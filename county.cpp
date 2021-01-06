#include <iostream>
#include <string.h>
#include "county.h"

using namespace std;

int County::num_of_counties = 0;

void County::resizeResidents() {

    residents_num_size *= 2;
    Citizen** new_arr = new Citizen * [residents_num_size];

    for (int i = 0; i < residents_num_logi; i++) {
        new_arr[i] = residents[i];
    }

    delete[] residents;

    residents = new_arr;
}

void County::resizeChosenElectors() {
    chosen_electors_size *= 2;
    Citizen** new_arr = new Citizen * [chosen_electors_size];

    for (int i = 0; i < chosen_electors_logi; i++) {
        new_arr[i] = chosen_electors[i];
    }

    delete[] chosen_electors;

    chosen_electors = new_arr;
}

County::County() :
    name(nullptr),
    id(0),
    number_of_electors(0),
    is_relative(0),
    current_vote_amount(0),
    residents_num_logi(0),
    chosen_electors_logi(0),
    residents_num_size(0),
    chosen_electors_size(0),
    residents(nullptr),
    chosen_electors(nullptr)
{}

County::County(char* _name, int _number_of_electors, bool _is_relative) :
    id(County::num_of_counties),
    number_of_electors(_number_of_electors),
    is_relative(_is_relative),
    current_vote_amount(0),
    residents_num_logi(0),
    chosen_electors_logi(0),
    residents_num_size(5),
    chosen_electors_size(5) {

    name = new char[strlen(_name) + 1];

    int cur_char = 0;
    while (_name[cur_char] != '\0') {
        name[cur_char] = _name[cur_char];
        cur_char++;
    }
    name[cur_char] = '\0';

    residents = new Citizen * [residents_num_size];
    chosen_electors = new Citizen * [chosen_electors_size];

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
    if (residents_num_logi == residents_num_size) { resizeResidents(); }

    residents[residents_num_logi] = new_resident;
    residents_num_logi++;

    return true;
}

bool County::addChosenElector(Citizen* chosen_elector) {
    if (chosen_electors_logi == chosen_electors_size) { resizeChosenElectors(); }

    chosen_electors[chosen_electors_logi] = chosen_elector;
    chosen_electors_logi++;

    return true;
}

ostream& operator<<(ostream& os, const County& county) {
    os << "ID: " << county.id << " Name: " << county.name << ", Number of electors: " << county.number_of_electors << endl;
    if (county.is_relative) {
        os << "The county is relative." << endl;
    }
    else {
        os << "The county is not relative." << endl;
    }

    return os;
}

void County::operator=(const County& other) {
    if (this != &other) {
        name = strdup(other.name);
        id = other.id;
        is_relative = other.is_relative;
        number_of_electors = other.number_of_electors;
        current_vote_amount = other.current_vote_amount;
        residents_num_size = other.residents_num_size;
        residents_num_logi = other.residents_num_logi;
        chosen_electors_size = other.chosen_electors_size;
        chosen_electors_logi = other.chosen_electors_logi;

        residents = new Citizen * [residents_num_size];
        for (int i = 0; i < residents_num_logi; i++) {
            residents[i] = other.residents[i];
        }

        chosen_electors = new Citizen * [chosen_electors_size];
        for (int i = 0; i < chosen_electors_logi; i++) {
            chosen_electors[i] = other.chosen_electors[i];
        }
    }
}

County::~County() {
    delete[] name;
    delete[] residents;
    delete[] chosen_electors;
}

void County::save(ostream& out) const
{
    /*Saving the name*/
    int len = strlen(name);
    out.write(rcastcc(&len), sizeof(len));  
    for (int i = 0; i < len; i++)
    {
        out.write(rcastcc(&name[i]), sizeof(char));
    }

    /*Saving the id*/
    out.write(rcastcc(&id), sizeof(id));

    /*Saving if the county is relative*/
    out.write(rcastcc(&is_relative), sizeof(is_relative));

    /*Saving the current vote amount*/
    out.write(rcastcc(&current_vote_amount), sizeof(current_vote_amount));

    /*Saving the numbers of the residents*/
    out.write(rcastcc(&residents_num_size), sizeof(residents_num_size));

    /*Saving number of the electors*/
    out.write(rcastcc(&number_of_electors), sizeof(number_of_electors));


    /* Saving the physical and logical numbers of electors */
    out.write(rcastcc(&chosen_electors_size), sizeof(chosen_electors_size));
    out.write(rcastcc(&chosen_electors_logi), sizeof(chosen_electors_logi));

    /* Saving the chosen elector's ids */
    int cur_id = 0;
    for (int i = 0; i < chosen_electors_logi; i++) {
        cur_id = chosen_electors[i]->getId();
        out.write(rcastcc(&cur_id), sizeof(cur_id));
    }
}

void County::load(istream& in)
{
    int len;
    in.read(rcastc(&len), sizeof(len));
    name = new char[len + 1];
    for (int i = 0; i < len; i++)
    {
        in.read(rcastc(&name[i]), sizeof(char));
    }
    name[len] = '\0';

    in.read(rcastc(&id), sizeof(id));
    in.read(rcastc(&is_relative), sizeof(is_relative));
    in.read(rcastc(&current_vote_amount), sizeof(current_vote_amount));
    in.read(rcastc(&residents_num_size), sizeof(residents_num_size));
    in.read(rcastc(&number_of_electors), sizeof(number_of_electors));
    in.read(rcastc(&chosen_electors_size), sizeof(chosen_electors_size));
    in.read(rcastc(&chosen_electors_logi), sizeof(chosen_electors_logi));
    
}