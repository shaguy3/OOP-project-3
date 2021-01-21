#pragma once
#include "election_cycle.h"
#include <fstream>
#include <exception>

using namespace std;

#define rcastcc reinterpret_cast<const char*>
#define rcastc reinterpret_cast<char*>

class SimpleCycle : public ElectionCycle {
private:
	int number_of_electors;
	DynamicArray<Citizen*> chosen_electors;

public:
	/* Constructors and destructors */
	SimpleCycle();
	SimpleCycle(Date& _date_of_election, int number_of_electors);
	virtual ~SimpleCycle();

	/* Getters */
	int getNumberOfElectors() const { return number_of_electors; }
	DynamicArray<Citizen*> getChosenElectors() const { return chosen_electors; }
	int chosenElectorsLen() const { return chosen_electors.size(); }

	/* Adders */
	bool addChosenElector(Citizen* chosen_elector);

	/* Setters */
	bool setChosenElectorsLen(int new_len) { chosen_electors.set_size(new_len); return true; }

	/* Serialization */
	virtual void save(ostream& out) const;
	virtual void load(istream& in);
};