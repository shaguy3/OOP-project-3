#include "complex_cycle.h"

ComplexCycle::ComplexCycle() : 
    ElectionCycle(),
    counties_num_size(0),
    counties_num_logi(0),
    counties(nullptr)
{

}


ComplexCycle::ComplexCycle(Date& _date_of_election) :
	ElectionCycle(_date_of_election),
	counties_num_size(5),
	counties_num_logi(0)
{
	counties = new County * [counties_num_size];
}

ComplexCycle::~ComplexCycle() {
	delete[] counties;
}

void ComplexCycle::resizeCounties() {
    counties_num_size *= 2;
    County** new_arr = new County * [counties_num_size];

    for (int i = 0; i < counties_num_logi; i++) {
        new_arr[i] = counties[i];
    }

    delete[] counties;

    counties = new_arr;
}

bool ComplexCycle::addCounty(County* county) {
    if (counties_num_logi == counties_num_size) { resizeCounties(); }

    counties[counties_num_logi] = county;
    counties_num_logi++;

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
    out.write(rcastcc(&counties_num_size), sizeof(counties_num_size));
    out.write(rcastcc(&counties_num_logi), sizeof(counties_num_logi));

    for (int i = 0; i < counties_num_logi; i++)
        counties[i]->save(out);

    /* Saving the physical and logical numbers of residents */
    out.write(rcastcc(&residents_num_size), sizeof(residents_num_size));
    out.write(rcastcc(&residents_num_logi), sizeof(residents_num_logi));

    /* Saving the residents */
    for (int i = 0; i < residents_num_logi; i++) {
        residents[i]->save(out);
    }

    /* Saving the physical and logical numbers of parties */
    out.write(rcastcc(&parties_num_size), sizeof(parties_num_size));
    out.write(rcastcc(&parties_num_logi), sizeof(parties_num_logi));

    /* Saving the parties */
    for (int i = 0; i < parties_num_logi; i++) {
        parties[i]->save(out);
    }
}

void ComplexCycle::load(istream& in) {
    /* Loading the date */
    date_of_election.load(in);

    /* Loading the current vote amount */
    in.read(rcastc(&current_vote_amount), sizeof(current_vote_amount));

    /* Loading the counties */

    in.read(rcastc(&counties_num_size), sizeof(counties_num_size));
    in.read(rcastc(&counties_num_logi), sizeof(counties_num_logi));

    County::num_of_counties = counties_num_logi;                         // Static amount of counties

    counties = new County * [counties_num_size];

    int** chosen_electors_arr = new int* [counties_num_logi];

    /* Loading the counties */
    for (int i = 0; i < counties_num_logi; i++)
    {
        counties[i] = new County();
        counties[i]->load(in);
        counties[i]->init_chosen_electors();
        counties[i]->init_residents();
        
        /* Reading the ids of the electors */
        chosen_electors_arr[i] = new int[counties[i]->chosenElectorsLen()];
        for (int j = 0; j < counties[i]->chosenElectorsLen(); j++)
        {
            in.read(rcastc(&chosen_electors_arr[i][j]), sizeof(chosen_electors_arr[i][j]));
        }

    }


    /* Loading the numbers of residents */
    in.read(rcastc(&residents_num_size), sizeof(residents_num_size));
    in.read(rcastc(&residents_num_logi), sizeof(residents_num_logi));

    residents = new Citizen * [residents_num_size];

    /* Loading the residents */
    int cur_home_county = -1;
    int* voted_parties = new int[residents_num_logi];
    for (int i = 0; i < residents_num_logi; i++) {
        residents[i] = new Citizen();

        residents[i]->load(in);
        in.read(rcastc(&cur_home_county), sizeof(cur_home_county));
        residents[i]->setHomeCounty(counties[cur_home_county]);
        counties[cur_home_county]->addResident(residents[i]);
        in.read(rcastc(&voted_parties[i]), sizeof(voted_parties[i]));
    }

    /* Loading the numbers of the parties */
    in.read(rcastc(&parties_num_size), sizeof(parties_num_size));
    in.read(rcastc(&parties_num_logi), sizeof(parties_num_logi));

    Party::number_of_parties = parties_num_logi;
    parties = new Party * [parties_num_size];

    /* Loading the parties */
    int cur_leader_id = 0, cur_size = 0, cur_logi_size = 0;
    for (int i = 0; i < parties_num_logi; i++) {
        parties[i] = new Party();

        parties[i]->load(in);

        /* Loading the leader id */
        in.read(rcastc(&cur_leader_id), sizeof(cur_leader_id));
        parties[i]->setLeader(getResident(cur_leader_id));
        getResident(cur_leader_id)->makeRepresentative(parties[i]);

        /* Loading the party reps numbers */
        in.read(rcastc(&cur_size), sizeof(cur_size));
        in.read(rcastc(&cur_logi_size), sizeof(cur_logi_size));

        parties[i]->setPartySize(cur_size);
        parties[i]->setPartyLogi(cur_logi_size);
        parties[i]->initReps(cur_size);

        /* Adding the party reps */
        int cur_party_rep_id = 0;
        for (int j = 0; j < cur_logi_size; j++) {
            in.read(rcastc(&cur_party_rep_id), sizeof(cur_party_rep_id));
            parties[i]->getPartyReps()[j] = getResident(cur_party_rep_id);
            getResident(cur_party_rep_id)->makeRepresentative(parties[i]);
        }
    }

    /* Setting the votes */
    for (int i = 0; i < residents_num_logi; i++)
    {
        if (voted_parties[i] != -1)
            residents[i]->setVoted(parties[voted_parties[i]]);
    }

    /* Freeing the memory */
    for (int i = 0; i < counties_num_logi; i++)
    {
        delete[] chosen_electors_arr[i];
    }

    delete[] chosen_electors_arr;
    delete[] voted_parties;

}