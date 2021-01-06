#include "simple_cycle.h"

SimpleCycle::SimpleCycle() :
	ElectionCycle(),
	number_of_electors(0),
	chosen_electors_logi(0),
	chosen_electors_size(5)
{
	chosen_electors = new Citizen * [chosen_electors_size];
}

SimpleCycle::SimpleCycle(Date& _date_of_election, int number_of_electors) :
	ElectionCycle(_date_of_election), number_of_electors(number_of_electors),
	chosen_electors_logi(0), chosen_electors_size(5)
{
	chosen_electors = new Citizen * [chosen_electors_size];
}

SimpleCycle::~SimpleCycle() {
	delete[] chosen_electors;
}

void SimpleCycle::resizeChosenElectors() {
	chosen_electors_size *= 2;
	Citizen** new_arr = new Citizen * [chosen_electors_size];

	for (int i = 0; i < chosen_electors_logi; i++) {
		new_arr[i] = chosen_electors[i];
	}

	chosen_electors = new_arr;
}

bool SimpleCycle::addChosenElector(Citizen* chosen_elector) {
	if (chosen_electors_logi == chosen_electors_size) { resizeChosenElectors(); }

	chosen_electors[chosen_electors_logi] = chosen_elector;
	chosen_electors_logi++;

	return true;
}

void SimpleCycle::save(ostream& out) const {

	/* Saving the type of the cycle */
	int cycle_type = 0;
	out.write(rcastcc(&cycle_type), sizeof(cycle_type));

	/* Saving the date of the election */
	date_of_election.save(out);

	/* saving the current vote amount */
	out.write(rcastcc(&current_vote_amount), sizeof(current_vote_amount));

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

	/* Saving the number of electors */
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

void SimpleCycle::load(istream& in) {

	/* Loading the date */
	date_of_election.load(in);

	/* Loading the current vote amount */
	in.read(rcastc(&current_vote_amount), sizeof(current_vote_amount));

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

		/* Adding the party_reps */
		int cur_party_rep_id = 0;
		for (int j = 0; j < cur_logi_size; j++) {
			in.read(rcastc(&cur_party_rep_id), sizeof(cur_party_rep_id));
			parties[i]->getPartyReps()[j] = getResident(cur_party_rep_id);
			getResident(cur_party_rep_id)->makeRepresentative(parties[i]);
		}
	}

	/* Loading the number of electors */
	in.read(rcastc(&number_of_electors), sizeof(number_of_electors));

	/* Loading the numbers of the chosen electors */
	in.read(rcastc(&chosen_electors_size), sizeof(chosen_electors_size));
	in.read(rcastc(&chosen_electors_logi), sizeof(chosen_electors_logi));

	/* Loading the chosen electors */
	int cur_chosen_elector_id = 0;
	for (int i = 0; i < chosen_electors_logi; i++) {
		in.read(rcastc(&cur_chosen_elector_id), sizeof(cur_chosen_elector_id));
		addChosenElector(getResident(cur_chosen_elector_id));
	}

	/* Setting the votes */
	for (int i = 0; i < residents_num_logi; i++)
	{
		if (voted_parties[i] != -1)
			residents[i]->setVoted(parties[voted_parties[i]]);
	}

	/* Freeing the memory */
	delete[] voted_parties;
}