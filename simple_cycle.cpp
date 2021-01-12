#include "simple_cycle.h"

SimpleCycle::SimpleCycle() :
	ElectionCycle(),
	number_of_electors(0),
	chosen_electors(DynamicArray<Citizen*>())
{}

SimpleCycle::SimpleCycle(Date& _date_of_election, int number_of_electors) :
	ElectionCycle(_date_of_election), number_of_electors(number_of_electors),
	chosen_electors(DynamicArray<Citizen*>())
{}

SimpleCycle::~SimpleCycle() {}

bool SimpleCycle::addChosenElector(Citizen* chosen_elector) {
	chosen_electors.push_back(chosen_elector);
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

	/* Saving the logical number of residents */
	int residents_size = residents.size();
	out.write(rcastcc(&residents_size), sizeof(residents_size));

	/* Saving the residents */
	for (int i = 0; i < residents_size; i++) {
		residents[i]->save(out);
	}

	/* Saving the logical number of parties */
	int parties_size = parties.size();
	out.write(rcastcc(&parties_size), sizeof(parties_size));

	/* Saving the parties */
	for (int i = 0; i < parties_size; i++) {
		parties[i]->save(out);
	}

	/* Saving the number of electors */
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

void SimpleCycle::load(istream& in) {

	/* Loading the date */
	date_of_election.load(in);

	/* Loading the current vote amount */
	in.read(rcastc(&current_vote_amount), sizeof(current_vote_amount));

	/* Loading the numbers of residents */
	int residents_size;
	in.read(rcastc(&residents_size), sizeof(residents_size));
	residents.set_size(residents_size);

	/* Loading the residents */
	int cur_home_county = -1;
	int* voted_parties = new int[residents_size];
	for (int i = 0; i < residents_size; i++) {
		residents[i] = new Citizen();

		residents[i]->load(in);
		in.read(rcastc(&cur_home_county), sizeof(cur_home_county));
		in.read(rcastc(&voted_parties[i]), sizeof(voted_parties[i]));
	}

	/* Loading the numbers of the parties */
	int parties_size;
	in.read(rcastc(&parties_size), sizeof(parties_size));
	parties.set_size(parties_size);

	Party::number_of_parties = parties_size;

	/* Loading the parties */
	int cur_leader_id = 0, cur_size = 0, cur_logi_size = 0;
	for (int i = 0; i < parties_size; i++) {
		parties[i] = new Party();

		parties[i]->load(in);

		/* Loading the leader id */
		in.read(rcastc(&cur_leader_id), sizeof(cur_leader_id));
		parties[i]->setLeader(getResident(cur_leader_id));
		getResident(cur_leader_id)->makeRepresentative(parties[i]);

		/* Loading the party reps numbers */
		in.read(rcastc(&cur_logi_size), sizeof(cur_logi_size));

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
	int chosen_electors_size;
	in.read(rcastc(&chosen_electors_size), sizeof(chosen_electors_size));
	chosen_electors.set_size(chosen_electors_size);

	/* Loading the chosen electors */
	int cur_chosen_elector_id = 0;
	for (int i = 0; i < chosen_electors_size; i++) {
		in.read(rcastc(&cur_chosen_elector_id), sizeof(cur_chosen_elector_id));
		addChosenElector(getResident(cur_chosen_elector_id));
	}

	/* Setting the votes */
	for (int i = 0; i < residents_size; i++)
	{
		if (voted_parties[i] != -1)
			residents[i]->setVoted(parties[voted_parties[i]]);
	}

	/* Freeing the memory */
	delete[] voted_parties;
}