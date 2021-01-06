#include "citizen.h"

Citizen::Citizen() :
    name(nullptr),
    id(0),
    year_of_birth(0),
    home_county(nullptr),
    is_representative(nullptr),
    has_voted(nullptr)
{}

Citizen::Citizen(const char* _name, int _id, int _year_of_birth, County* _home_county)
    : id(_id), year_of_birth(_year_of_birth),
    home_county(_home_county),
    is_representative(nullptr),
    has_voted(nullptr)
{
    name = new char[strlen(_name) + 1];

    int cur_char = 0;
    while (_name[cur_char] != '\0') {
        name[cur_char] = _name[cur_char];
        cur_char++;
    }
    name[cur_char] = '\0';
}

Citizen::~Citizen()
{
    delete[] name;
}

std::ostream& operator<<(std::ostream& os, const Citizen& other)
{
    if (other.home_county) {
        os << "Name: " << other.name << endl << "Id: " << other.id << endl \
            << "Year of birth: " << other.year_of_birth << endl \
            << "Home county: " << other.home_county->getName() << "(ID: " << other.home_county->getId() << ")" << endl;
    }
    else {
        os << "Name: " << other.name << endl << "Id: " << other.id << endl \
            << "Year of birth: " << other.year_of_birth << endl;
    }

    return os;
}

void Citizen::operator=(const Citizen& other) {
    if (this != &other) {
        name = strdup(other.name);
        id = other.id;
        year_of_birth = other.year_of_birth;
        home_county = other.home_county;
        is_representative = other.is_representative;
        has_voted = other.has_voted;
    }
}

bool Citizen::makeRepresentative(Party* party_name) {
    is_representative = party_name;

    return true;
}

bool Citizen::setVoted(Party* party_name) {
    has_voted = party_name;

    return true;
}

bool Citizen::setHomeCounty(County* county) {
    home_county = county;
    
    return true;
}

void Citizen::save(ostream& out) const {
    /*Saving the name*/
    int len = strlen(name);
    out.write(rcastcc(&len), sizeof(len));
    for (int i = 0; i < len; i++)
    {
        out.write(rcastcc(&name[i]), sizeof(char));
    }

    /*Saving the id*/
    out.write(rcastcc(&id), sizeof(id));

    /*Saving the year of birth*/
    out.write(rcastcc(&year_of_birth), sizeof(year_of_birth));

    /*Saving the home county id*/
    int county_id = -1;
    if (home_county) {
        county_id = home_county->getId();
    }
    out.write(rcastcc(&county_id), sizeof(county_id));

    /*Saving the voted party*/
    int voted_party = -1;
    if (has_voted) {
        voted_party = has_voted->getId();
    }
    out.write(rcastcc(&voted_party), sizeof(voted_party));
}

void Citizen::load(istream& in) {
    
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

    /*Loading the year of birth*/
    in.read(rcastc(&year_of_birth), sizeof(year_of_birth));
}