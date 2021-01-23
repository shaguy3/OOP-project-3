#include "citizen.h"

Citizen::Citizen() :
    name(""),
    id(0),
    year_of_birth(0),
    home_county(nullptr),
    rep_county(nullptr),
    is_representative(nullptr),
    has_voted(nullptr)

{}

Citizen::Citizen(const Citizen& other) : name(other.name), id(other.id), year_of_birth(other.year_of_birth), 
  home_county(other.home_county), is_representative(other.isRepresentative()), has_voted(other.hasVoted()),
  rep_county(nullptr)
{

}

Citizen::Citizen(const string _name, const int _id, const int _year_of_birth, County* _home_county)
    : name(_name), id(_id), year_of_birth(_year_of_birth),
    home_county(_home_county),
    rep_county(nullptr),
    is_representative(nullptr),
    has_voted(nullptr)
{
    if (_id < 100000000 || id > 999999999) {
        throw invalid_argument(
            "New resident: Invalid ID. The ID must be a positive 9 digit number."
        );
    }
}

Citizen::~Citizen()
{}

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

const Citizen& Citizen::operator=(const Citizen& other) {
    if (this != &other) {
        name = other.name;
        id = other.id;
        year_of_birth = other.year_of_birth;
        home_county = other.home_county;
        is_representative = other.is_representative;
        has_voted = other.has_voted;
    }
    return *this;
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

bool Citizen::setRepCounty(County* county) {
    rep_county = county;

    return true;
}

void Citizen::save(ostream& out) const {
    /*Saving the name*/
    int len = name.size();
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

    /*Saving the rep county id*/
    int rep_id = -1;
    if (rep_county) {
        rep_id = rep_county->getId();
    }
    out.write(rcastcc(&rep_id), sizeof(rep_id));

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
    name.resize(len);
    for (int i = 0; i < len; i++)
    {
        in.read(rcastc(&name[i]), sizeof(char));
    }

    /*Loading the id*/
    in.read(rcastc(&id), sizeof(id));

    /*Loading the year of birth*/
    in.read(rcastc(&year_of_birth), sizeof(year_of_birth));
}