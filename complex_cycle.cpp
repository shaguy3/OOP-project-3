#include "complex_cycle.h"

ComplexCycle::ComplexCycle() : 
    ElectionCycle(),
    counties(DynamicArray<County*>())
{}


ComplexCycle::ComplexCycle(Date& _date_of_election) :
	ElectionCycle(_date_of_election),
    counties(DynamicArray<County*>())
{}

ComplexCycle::~ComplexCycle() {
    for (int i = 0; i < counties.size(); i++) {
        delete counties[i];
    }
}

bool ComplexCycle::addCounty(string county_name, int number_of_electors, bool is_relative) {
    if (is_relative) {
        County* county = new RelativeCounty(county_name, number_of_electors);
        counties.push_back(county);
    }
    else {
        County* county = new County(county_name, number_of_electors);
        counties.push_back(county);
    }

    return true;
}

ostream& operator<<(ostream& os, ComplexCycle& election_cycle) {
    os << "Election cycle of date: " << election_cycle.getDate() << endl \
        << "Residents: " << endl;

    for (int i = 0; i < election_cycle.residentslen(); i++) {
        os << *election_cycle.getResidents()[i] << endl;
    }

    os << endl << "Counties: " << endl;

    for (int i = 0; i < election_cycle.countieslen(); i++) {
        os << *election_cycle.getCounties()[i] << endl;
    }

    os << endl << "Parties: " << endl;

    for (int i = 0; i < election_cycle.partieslen(); i++) {
        os << *election_cycle.getParties()[i] << endl;
    }

    return os;
}

void ComplexCycle::save(ostream& out) const {
    /* Saving the type of the cycle */
    int cycle_type = 1;
    out.write(rcastcc(&cycle_type), sizeof(cycle_type));

    /* Saving the date of the election */
    date_of_election.save(out);

    /* saving the current vote amount */
    out.write(rcastcc(&current_vote_amount), sizeof(current_vote_amount));

    /* Saving the physical and logical size of the counties */
    int counties_size = counties.size();
    out.write(rcastcc(&counties_size), sizeof(counties_size));

    for (int i = 0; i < counties_size; i++) {
        counties[i]->save(out);
    }

    /* Saving the logical number of residents */
    int residents_size = residents.size();
    out.write(rcastcc(&residents_size), sizeof(residents_size));

    /* Saving the residents */
    for (int i = 0; i < residents_size; i++) {
        residents[i]->save(out);
    }

    /* Saving the physical and logical numbers of parties */
    int parties_size = parties.size();
    out.write(rcastcc(&parties_size), sizeof(parties_size));

    /* Saving the parties */
    for (int i = 0; i < parties_size; i++) {
        parties[i]->save(out);
    }
}

void ComplexCycle::load(istream& in) {
    /* Loading the date */
    date_of_election.load(in);

    /* Loading the current vote amount */
    in.read(rcastc(&current_vote_amount), sizeof(current_vote_amount));

    /* Loading the counties */
    int counties_size;
    in.read(rcastc(&counties_size), sizeof(counties_size));
    counties.set_size(counties_size);

    County::num_of_counties = counties_size;                         // Static amount of counties

    DynamicArray<DynamicArray<int>> chosen_electors_arr(counties_size);

    /* Loading the counties */
    for (int i = 0; i < counties_size; i++)
    {
        // Checking the type of the county
        int county_type = -1;
        in.read(rcastc(&county_type), sizeof(county_type));

        if (county_type == 0) {
            counties[i] = new County();
        }
        else {
            counties[i] = new RelativeCounty();
        }
        counties[i]->load(in);
        
        /* Reading the ids of the electors */
        if (counties[i]->chosenElectorsLen() > 0) {
            chosen_electors_arr[i].set_size(counties[i]->chosenElectorsLen());
            for (int j = 0; j < counties[i]->chosenElectorsLen(); j++)
            {
                in.read(rcastc(&chosen_electors_arr[i][j]), sizeof(chosen_electors_arr[i][j]));
            }
        }
    }


    /* Loading the numbers of residents */
    int residents_size;
    in.read(rcastc(&residents_size), sizeof(residents_size));
    residents.set_size(residents_size);

    /* Loading the residents */
    int cur_home_county = -1;
    DynamicArray<int> voted_parties;
    if (residents_size > 0) {
        voted_parties.set_size(residents_size);
        for (int i = 0; i < residents_size; i++) {
            residents[i] = new Citizen();

            residents[i]->load(in);
            in.read(rcastc(&cur_home_county), sizeof(cur_home_county));
            residents[i]->setHomeCounty(counties[cur_home_county]);
            counties[cur_home_county]->addResident(residents[i]);
            in.read(rcastc(&voted_parties[i]), sizeof(voted_parties[i]));
        }
    }

    /* Loading the numbers of the parties */
    int parties_size;
    in.read(rcastc(&parties_size), sizeof(parties_size));
    parties.set_size(parties_size);

    Party::number_of_parties = parties_size;

    /* Loading the parties */
    int cur_leader_id = 0, party_reps_size = 0;
    for (int i = 0; i < parties_size; i++) {
        parties[i] = new Party();

        parties[i]->load(in);

        /* Loading the leader id */
        in.read(rcastc(&cur_leader_id), sizeof(cur_leader_id));
        parties[i]->setLeader(getResident(cur_leader_id));
        getResident(cur_leader_id)->makeRepresentative(parties[i]);

        /* Loading the party reps numbers */
        in.read(rcastc(&party_reps_size), sizeof(party_reps_size));
        parties[i]->getPartyReps().set_size(party_reps_size);

        /* Adding the party reps */
        int cur_party_rep_id = 0;
        for (int j = 0; j < party_reps_size; j++) {
            in.read(rcastc(&cur_party_rep_id), sizeof(cur_party_rep_id));
            parties[i]->getPartyReps()[j] = getResident(cur_party_rep_id);
            getResident(cur_party_rep_id)->makeRepresentative(parties[i]);
        }
    }

    /* Setting the votes */
    for (int i = 0; i < residents_size; i++)
    {
        if (voted_parties[i] != -1)
            residents[i]->setVoted(parties[voted_parties[i]]);
    }
}