#include "complex_cycle.h"
#include "simple_cycle.h"
#include <iostream>
#include <string.h>
#include <typeinfo>
#include <fstream>
#include <string>

using namespace std;

// Test commit


const int VOTING_AGE = 16;

void swap(int& a, int& b)
{
    int temp = a;
    a = b;
    b = temp;
}

void addCounty(ComplexCycle* election_cycle) {
    bool isRelative;
    string county_name;
    cin.ignore();
    cout << "Please enter the County's name: ";
    getline(cin, county_name);

    char is_relative;
    cout << "Is the county relative (assignes it's electors relatively to the partie's vote percent)? (y/n): ";
    cin >> is_relative;
    while (is_relative != 'y' && is_relative != 'n') {
        cout << "Not a valid input. please enter y for yes or n for no: ";
        cin >> is_relative;
    }


    if (is_relative == 'y')
        isRelative = true;
    else
        isRelative = false;

    int number_of_electors = 0;
    cout << "Please enter the number of electors in the county: ";
    cin >> number_of_electors;
    while (number_of_electors <= 0) {
        cout << "Not a valid number of electors. Please enter a positive number: " << endl;
        cin >> number_of_electors;
    }

    County* new_county = new County(county_name, number_of_electors, isRelative);
    election_cycle->addCounty(new_county);

    cout << endl;
}

void addResident(ElectionCycle* election_cycle) {

    string resident_name;
    cin.ignore();
    cout << "Please enter the Resident's name: ";
    getline(cin, resident_name);

    int id = 0;
    cout << "Please enter the resident's ID: ";
    do {
        cin >> id;
        if (id <= 0) {
            cout << "Non valid ID. Please enter a non negative number: ";
            id = 0;
        }

        for (int i = 0; i < election_cycle->residentslen(); i++) {
            if (id == election_cycle->getResidents()[i]->getId()) {
                cout << "This ID is already in use. Please enter a unique ID: ";
                id = 0;
            }
        }
    } while (id == 0);

    int year_of_birth = 0;
    cout << "please enter the resident's Year of birth: ";
    do {
        cin >> year_of_birth;
        if (year_of_birth > (election_cycle->getDate().getYear() - VOTING_AGE)) {
            cout << "Resident is too young to vote. Please enter a valid year of birth" << endl;
            year_of_birth = 0;
        }
    } while (year_of_birth == 0);

    if (typeid(*election_cycle).name() == typeid(ComplexCycle).name()) {

        ComplexCycle* complex_cycle = dynamic_cast<ComplexCycle*>(election_cycle);

        int county_id = -1;
        cout << "Please enter the resident's home county ID: ";
        do {
            cin >> county_id;
            if (county_id < 0 || county_id >(complex_cycle->countieslen() - 1)) {
                cout << "Not a valid county id. Please enter a non negative number up to " << complex_cycle->countieslen() - 1 << ": ";
                county_id = -1;
            }
        } while (county_id == -1);

        Citizen* new_resident = new Citizen(resident_name, id, year_of_birth, complex_cycle->getCounty(county_id));
        complex_cycle->addResident(new_resident);
        complex_cycle->getCounty(county_id)->addResident(new_resident);
    }
    else {
        SimpleCycle* simple_cycle = dynamic_cast<SimpleCycle*>(election_cycle);

        int county_id = 0;

        Citizen* new_resident = new Citizen(resident_name, id, year_of_birth, nullptr);
        simple_cycle->addResident(new_resident);
    }
}

void addParty(ElectionCycle* election_cycle) {
    Citizen* party_leader = nullptr;
    string party_name;
    cin.ignore();
    cout << "Please enter the party name: ";
    getline(cin, party_name);

    int party_leader_id = 0;
    cout << "Please enter the ID of the party's Leader: ";
    do {
        cin >> party_leader_id;
        party_leader = election_cycle->getResident(party_leader_id);
        if (!party_leader) {
            cout << "There is no resident with matching ID. Please enter an existing resident's ID: ";
            party_leader_id = 0;
        }

        else if (party_leader->isRepresentative()) {
            cout << "This resident is already a representative. Please select another citizen: ";
            party_leader_id = 0;
        }
    } while (party_leader_id == 0);

    Party* new_party = new Party(party_name, party_leader);
    election_cycle->addParty(new_party);
    party_leader->makeRepresentative(new_party);
}

void addPartyRep(ElectionCycle* election_cycle) {
    Citizen* relevant_citizen = nullptr; 
    Party* relevant_party = nullptr;
    int party_rep_id = -1;
    cout << "Please enter the ID of the party's representative: ";
    do {
        cin >> party_rep_id;
        relevant_citizen = election_cycle->getResident(party_rep_id);
        if (!relevant_citizen) {
            cout << "There is no resident with matching ID. Please enter an existing resident's ID: ";
            party_rep_id = -1;
        }

        else if (relevant_citizen->isRepresentative()) {
            cout << "This resident is already a representative. Please select another citizen: ";
            party_rep_id = -1;
        }
    } while (party_rep_id == -1);

    string party_name;
    cin.ignore();
    cout << "Please enter the name of the party representative's party: ";
    do {
        
        getline(cin, party_name);
        relevant_party = election_cycle->getParty(party_name);
        if (!relevant_party) {
            cin.ignore();
            cout << "There is no party with that name. Please select an existing name: ";
        }
    } while (!relevant_party);

    relevant_citizen->makeRepresentative(relevant_party);
    relevant_party->addPartyRep(election_cycle->getResident(party_rep_id));
}

void showCounties(ComplexCycle* election_cycle) {
    cout << "County list: " << endl << endl;
    for (int i = 0; i < election_cycle->countieslen(); i++) {
        cout << i << ". " << *election_cycle->getCounties()[i] << endl;
    }

    cout << endl;
}

void showResidents(ElectionCycle* election_cycle) {
    cout << "Residents list: " << endl << endl;
    for (int i = 0; i < election_cycle->residentslen(); i++) {
        cout << i << ". " << *election_cycle->getResidents()[i] << endl;
    }

    cout << endl;
}

void showParties(ElectionCycle* election_cycle) {
    cout << "Parties list: " << endl << endl;
    for (int i = 0; i < election_cycle->partieslen(); i++) {
        cout << i << ". " << *election_cycle->getParties()[i] << endl;
    }

    cout << endl;
}

void addVote(ElectionCycle* election_cycle) {
    Citizen* voter = nullptr;
    int voter_id = -1;
    cout << "Please enter the ID of the voter: ";
    do {
        cin >> voter_id;
        voter = election_cycle->getResident(voter_id);
        if (!voter) {
            cout << "There is no resident with matching ID. Please enter an existing resident's ID: ";
            voter_id = -1;
        }
        else if (voter->hasVoted()) {
            cout << "This resident has already voted. Please select another citizen: ";
            voter_id = -1;
        }
    } while (voter_id == -1);

    Party* voted_party = nullptr;
    string party_name;
    cin.ignore();
    cout << "Please enter the party name which the resident voted for: ";
    do {
        getline(cin, party_name);
        voted_party = election_cycle->getParty(party_name);
        if (!voted_party) {
            cout << "There is no party with that name. Please select an existing name: ";
        }
    } while (!voted_party);

    voter->setVoted(voted_party);
    if (voter->getHomeCounty()) { voter->getHomeCounty()->addVote(); }
    election_cycle->addVote();
}

void complexElectionResults(ComplexCycle* election_cycle) {

    cout << "*****************Printing election results!********************" << endl << endl;

    int** election_result = new int* [election_cycle->countieslen()];               //Number of votes for each party in each county
    double** percentage_table = new double* [election_cycle->countieslen()];        //Percentage of received votes for each party in each county
    int** elected_reps_nums = new int* [election_cycle->countieslen()];             //Number of electors for each party in each county 
    Party** winner_per_county = new Party * [election_cycle->countieslen()];        //Winning party in each county
    int* sorted_parties = new int[election_cycle->partieslen()];                    //Parties sorted from the most electors to less
    int* electors_per_party = new int[election_cycle->partieslen()];                //Amount of electors that each party received
    int* votes_per_party = new int[election_cycle->partieslen()];                   //Amount of votes that each party received

    for (int i = 0; i < election_cycle->countieslen(); i++)     // Initilization of two dimantional arrays
    {
        election_result[i] = new int[election_cycle->partieslen()];
        percentage_table[i] = new double[election_cycle->partieslen()];
        elected_reps_nums[i] = new int[election_cycle->partieslen()];
        for (int j = 0; j < election_cycle->partieslen(); j++) {
            election_result[i][j] = 0;
            percentage_table[i][j] = 0;
            elected_reps_nums[i][j] = 0;
        }
    }

    for (int i = 0; i < election_cycle->partieslen(); i++) {    // Initialization of one dimantional arrays
        sorted_parties[i] = i;
        electors_per_party[i] = 0;
        votes_per_party[i] = 0;
    }

    for (int i = 0; i < election_cycle->residentslen(); i++)    // Counting the votes for each county and party
    {
        if (election_cycle->getResidents()[i]->hasVoted()) {
            int countyVote = election_cycle->getResidents()[i]->getHomeCounty()->getId();
            int partyVote = election_cycle->getResidents()[i]->hasVoted()->getId();
            election_result[countyVote][partyVote]++;
        }
    }

    for (int i = 0; i < election_cycle->partieslen(); i++)      // Summing the votes
    {
        for (int j = 0; j < election_cycle->countieslen(); j++)
            votes_per_party[i] += election_result[j][i];
    }

    double double_percent = 0.0;
    int int_percent = 0;
    for (int i = 0; i < election_cycle->countieslen(); i++)     // Counting the percentage of the votes 
    {
        for (int j = 0; j < election_cycle->partieslen(); j++)
        {
            percentage_table[i][j] = (static_cast<double>(election_result[i][j]) / static_cast<double>(election_cycle->getCounty(i)->getVoteAmount())) * 100.0;
            elected_reps_nums[i][j] = static_cast<int>(((percentage_table[i][j] / 100) * election_cycle->getCounty(i)->getNumberOfElectors()) + 0.5);
        }
    }

    for (int i = 0; i < election_cycle->countieslen(); i++)     // In case of showing results more than once
    {
        for (int j = 0; j < election_cycle->getCounty(i)->chosenElectorsLen(); j++)
        {
            election_cycle->getCounty(i)->getChosenElectors()[j] = nullptr;
        }
        election_cycle->getCounty(i)->setChosenElectorsLen(0);
    }

    for (int i = 0; i < election_cycle->countieslen(); i++) {   // Adding the parties that won in each county 
        double winner_percentage = 0.0;
        int winner_index = 0;
        for (int j = 0; j < election_cycle->partieslen(); j++) {
            if (percentage_table[i][j] > winner_percentage) {
                winner_percentage = percentage_table[i][j];
                winner_index = j;
            }
        }
        winner_per_county[i] = election_cycle->getParties()[winner_index];
    }


    for (int i = 0; i < election_cycle->countieslen(); i++) {   // Calculating the sum of electors per party
        if (election_cycle->getCounty(i)->isRelative()) {
            for (int j = 0; j < election_cycle->partieslen(); j++) {
                electors_per_party[j] += elected_reps_nums[i][j];
            }
        }
        else {
            electors_per_party[winner_per_county[i]->getId()] += election_cycle->getCounty(i)->getNumberOfElectors();
        }
    }

    for (int j = 0; j < election_cycle->partieslen(); j++) {    // Adding electors to each county
        for (int k = 0; k < election_cycle->getParties()[j]->partyRepsLen(); k++) {
            Citizen* chosen_elector = election_cycle->getParties()[j]->getPartyReps()[k];
            int home_county_id = chosen_elector->getHomeCounty()->getId();
            if (elected_reps_nums[home_county_id][j] > 0) {
                election_cycle->getCounty(home_county_id)->addChosenElector(chosen_elector);
                elected_reps_nums[home_county_id][j] -= 1;
            }
        }
    }


    cout << "Chosen county electors:" << endl;
    for (int i = 0; i < election_cycle->countieslen(); i++) {
        cout << "County #" << i << ":" << endl;
        cout << *election_cycle->getCounty(i) << endl;
        for (int j = 0; j < election_cycle->getCounty(i)->chosenElectorsLen(); j++) {
            cout << *election_cycle->getCounty(i)->getChosenElectors()[j];
            cout << "Party: " << election_cycle->getCounty(i)->getChosenElectors()[j]->isRepresentative()->getName() << endl << endl;
        }
        cout << endl;

        for (int k = 0; k < election_cycle->partieslen(); k++)
        {
            cout << "Party " << election_cycle->getParties()[k]->getName() << " received " <<
                percentage_table[i][k] << "% of votes" << endl;
        }

        cout << "County voter turnout: " << (static_cast<double>(election_cycle->getCounty(i)->getVoteAmount()) /
            static_cast<double>(election_cycle->getCounty(i)->residentsLen())) * 100.0 << "%" << endl;
        cout << "Winner in county " << election_cycle->getCounty(i)->getName() << ": " << winner_per_county[i]->getLeader()->getName() << endl << endl;
    }

    for (int i = 0; i < election_cycle->partieslen() - 1; i++) {
        for (int j = 0; j < election_cycle->partieslen() - i - 1; j++) {
            if (electors_per_party[j] < electors_per_party[j + 1]) {
                swap(sorted_parties[j], sorted_parties[j + 1]);
            }
        }
    }

    cout << "Final results:" << endl << endl;
    for (int i = 0; i < election_cycle->partieslen(); i++) {
        cout << i + 1 << " place: " << election_cycle->getParties()[sorted_parties[i]]->getLeader()->getName()
            << " with " << votes_per_party[sorted_parties[i]] << " votes" \
            << " and " << electors_per_party[sorted_parties[i]] << " electors. " << endl;
    }


    /*Freeing all of the arrays that were used */

    for (int i = 0; i < election_cycle->countieslen(); i++)
        delete[] election_result[i];
    delete[] election_result;

    for (int i = 0; i < election_cycle->countieslen(); i++)
        delete[] percentage_table[i];
    delete[] percentage_table;

    for (int i = 0; i < election_cycle->countieslen(); i++)
        delete[] elected_reps_nums[i];
    delete[] elected_reps_nums;

    delete[] winner_per_county;
    delete[] electors_per_party;
    delete[] votes_per_party;
    delete[] sorted_parties;
}

void simpleElectionResults(SimpleCycle* election_cycle) {

    cout << "*****************Printing election results!********************" << endl << endl;

    int* election_results = new int[election_cycle->partieslen()];              //Number of votes for each party.
    double* percentage_table = new double[election_cycle->partieslen()];        //Percentage of received votes for each party.
    int* elected_reps_nums = new int[election_cycle->partieslen()];             //Number of electors for each party.
    int* sorted_parties = new int[election_cycle->partieslen()];                //Parties sorted from the most electors to less.

    /* Arrays initializations */
    for (int i = 0; i < election_cycle->partieslen(); i++) {
        election_results[i] = 0;
        percentage_table[i] = 0.0;
        elected_reps_nums[i] = 0;
        sorted_parties[i] = i;
    }

    for (int i = 0; i < election_cycle->residentslen(); i++) { // Counting the votes.
        if (election_cycle->getResidents()[i]->hasVoted()) {
            election_results[election_cycle->getResidents()[i]->hasVoted()->getId()]++;
        }
    }

    for (int i = 0; i < election_cycle->partieslen(); i++) { // Calculating the percentages of the votes and the electors per party.
        percentage_table[i] = (static_cast<double>(election_results[i]) / static_cast<double>(election_cycle->getVoteAmount())) * 100.0;
        elected_reps_nums[i] = static_cast<int>(((percentage_table[i] / 100) * election_cycle->getNumberOfElectors()) + 0.5);
    }

    for (int i = 0; i < election_cycle->chosenElectorsLen(); i++) { // In case of showing results more than once
        election_cycle->getChosenElectors()[i] = nullptr;
    }
    election_cycle->setChosenElectorsLen(0);

    // Searching for the winner party
    Party* winner = nullptr;
    double winner_percentage = 0.0;
    for (int i = 0; i < election_cycle->partieslen(); i++) {
        if (percentage_table[i] > winner_percentage) {
            winner_percentage = percentage_table[i];
            winner = election_cycle->getParties()[i];
        }
    }

    /* Adding the chosen electors */
    for (int i = 0; i < election_cycle->partieslen(); i++) {
        int cur_num_of_reps = elected_reps_nums[i];
        for (int j = 0; j < election_cycle->getParties()[i]->partyRepsLen(); j++) {
            Citizen* chosen_elector = election_cycle->getParties()[i]->getPartyReps()[j];
            if (cur_num_of_reps > 0) {
                election_cycle->addChosenElector(chosen_elector);
                cur_num_of_reps--;
            }
        }
    }

    cout << "Chosen electors: " << endl;
    for (int i = 0; i < election_cycle->chosenElectorsLen(); i++) {
        cout << *election_cycle->getChosenElectors()[i];
        cout << "Party: " << election_cycle->getChosenElectors()[i]->isRepresentative()->getName() << endl << endl;
    }
    cout << endl;

    for (int i = 0; i < election_cycle->partieslen(); i++) {
        cout << "Party " << election_cycle->getParties()[i]->getName() << " recieved " << \
            percentage_table[i] << "% of the votes" << endl;
    }

    cout << "The winner is: " << winner->getLeader()->getName() << endl << endl;

    for (int i = 0; i < election_cycle->partieslen() - 1; i++) {
        for (int j = 0; j < election_cycle->partieslen() - i - 1; j++) {
            if (elected_reps_nums[j] < elected_reps_nums[j + 1]) {
                swap(sorted_parties[j], sorted_parties[j + 1]);
            }
        }
    }


    cout << "The overall turnout percentage: " << (static_cast<double>(election_cycle->getVoteAmount()) /
        static_cast<double>(election_cycle->residentslen())) * 100.0 << "%" << endl << endl;

    cout << "Final results: " << endl << endl;
    for (int i = 0; i < election_cycle->partieslen(); i++) {
        cout << i + 1 << " place: " << election_cycle->getParties()[sorted_parties[i]]->getLeader()->getName() \
            << " with " << election_results[i] << " votes" \
            << " and " << elected_reps_nums[sorted_parties[i]] << " electors." << endl;
    }


    delete[] sorted_parties;
    delete[] elected_reps_nums;
    delete[] percentage_table;
    delete[] election_results;
}

void saveElectionCycle(ElectionCycle* election_cycle) {
    char filename[30];
    cout << "Saving current election cycle..." << endl;
    cout << "Please enter the name of the file you want to save (no spaces and extention required). ";
    cin >> filename;

    ofstream outfile(filename, ios::binary);
    if (!outfile) {
        cout << "Error with outfile" << endl;
        exit(-1);
    }

    election_cycle->save(outfile);

    outfile.close();
}

ElectionCycle* loadElectionCycle() {
    ElectionCycle* election_cycle;

    char filename[30];
    cout << "Please enter the name of the file you want to load (no spaces and extention required). ";
    cin >> filename;

    ifstream infile(filename, ios::binary);
    if (!infile) {
        cout << "Error with infile" << endl;
        exit(-1);
    }

    int cycle_type = -1;
    infile.read(rcastc(&cycle_type), sizeof(cycle_type));

    if (cycle_type == 1)
    {
        election_cycle = new ComplexCycle();

    }
    else
    {
        election_cycle = new SimpleCycle();
    }
    election_cycle->load(infile);


    infile.close();

    return election_cycle;
}

void mainMenu(ElectionCycle* election_cycle) {
    int choice = 0;

    enum mainMenu {None, Add_county, Add_citizen, Add_party, Add_rep, Show_counties, Show_residents, Show_parties, Voting, Results, Exit, 
    Save_Cycle, Load_Cycle};

    while (choice != 10) {
        cout << "Please select an option:" << endl;
        cout << "1.  Add a county." << endl;
        cout << "2.  Add a citizen." << endl;
        cout << "3.  Add a party." << endl;
        cout << "4.  Add a party representative." << endl;
        cout << "5.  Show all of the counties." << endl;
        cout << "6.  Show all of the Citizens." << endl;
        cout << "7.  Show all of the parties." << endl;
        cout << "8.  Vote." << endl;
        cout << "9.  Show the election results." << endl;
        cout << "10. Quit." << endl;
        cout << "11. Save current election cycle." << endl;
        cout << "12. Load existing election cycle." << endl << endl;

        cin >> choice;
        cout << endl;

        if (choice > 12 || choice < 1) {
            cout << "Not a valid choice. Please choose a number between 1 and 10." << endl << endl;
            continue;
        }

        switch (choice) {
        case Add_county:
            if (typeid(*election_cycle).name() == typeid(ComplexCycle).name()) {
                ComplexCycle* complex_cycle = dynamic_cast<ComplexCycle*>(election_cycle);
                addCounty(complex_cycle);
            }
            else {
                cout << "There are no counties in this election cycle." << endl;
            }
            break;

        case Add_citizen:
            if (typeid(*election_cycle).name() == typeid(ComplexCycle).name()) {
                ComplexCycle* complex_cycle = dynamic_cast<ComplexCycle*>(election_cycle);
                if (complex_cycle->countieslen() == 0) {
                    cout << "There are no counties! please enter a county first." << endl;
                }
                else { addResident(election_cycle); }
            }
            else {
                addResident(election_cycle);
            }
            break;

        case Add_party:
            if (election_cycle->residentslen() == 0) {
                cout << "There are no residents! Who will lead the party? (Enter a resident first)" << endl;
            }
            else {
                int non_representatives = 0;
                for (int i = 0; i < election_cycle->residentslen(); i++) {
                    if (!election_cycle->getResidents()[i]->isRepresentative()) {
                        non_representatives = 1;
                    }
                }

                if (!non_representatives) {
                    cout << "All of the existing residents are already representing existing parties. Please add more citizens." << endl;
                }
                else { addParty(election_cycle); }
            }
            break;

        case Add_rep:
            if (election_cycle->partieslen() == 0) {
                cout << "There are no parties to add representatives to. Please add a party first." << endl;
            }
            else {
                bool non_representatives = false;
                for (int i = 0; i < election_cycle->residentslen(); i++) {
                    if (!election_cycle->getResidents()[i]->isRepresentative()) {
                        non_representatives = true;
                        break;
                    }
                }

                if (!non_representatives) {
                    cout << "All of the existing residents are already representing existing parties. Please add more citizens." << endl;
                }
                else { addPartyRep(election_cycle); }
            }
            break;

        case Show_counties:
            if (typeid(*election_cycle).name() == typeid(ComplexCycle).name()) {
                ComplexCycle* complex_cycle = dynamic_cast<ComplexCycle*>(election_cycle);
                showCounties(complex_cycle);
            }
            else {
                cout << "There are no counties to speak of." << endl;
            }
            break;

        case Show_residents:
            showResidents(election_cycle);
            break;

        case Show_parties:
            showParties(election_cycle);
            break;

        case Voting:

            if (election_cycle->partieslen() == 0) {
                cout << "There are no parties to vote to. Please add a party first." << endl;
            }
            else if (election_cycle->getVoteAmount() == election_cycle->residentslen()) {
                cout << "All of the existing residents have already voted! The voting is over." << endl;
            }
            else { addVote(election_cycle); }
            cout << "Thanks for voting!" << endl;
            break;

        case Results:
            if (typeid(*election_cycle).name() == typeid(ComplexCycle).name()) {
                ComplexCycle* complex_cycle = dynamic_cast<ComplexCycle*>(election_cycle);
                if (complex_cycle->getVoteAmount() == 0)
                    cout << "Nobody voted! Please vote! You don't want another 2016!" << endl;
                else {

                    bool valid_reps_nums = true;
                    int** representatives_per_party_per_county = new int* [complex_cycle->partieslen()];
                    for (int x = 0; x < complex_cycle->partieslen(); x++)    //Initilization of the bucket arrays(representatives_per_party_per_county)
                    {
                        representatives_per_party_per_county[x] = new int[complex_cycle->countieslen()];
                        for (int z = 0; z < complex_cycle->countieslen(); z++)
                            representatives_per_party_per_county[x][z] = 0;
                    }
                    for (int i = 0; i < complex_cycle->partieslen() && valid_reps_nums; i++) {
                        Party* cur_party = complex_cycle->getParties()[i];
                        for (int j = 0; j < cur_party->partyRepsLen() && valid_reps_nums; j++) {
                            representatives_per_party_per_county[i][cur_party->getPartyReps()[j]->getHomeCounty()->getId()] += 1;
                        }

                        for (int k = 0; k < complex_cycle->countieslen() && valid_reps_nums; k++) {
                            if (representatives_per_party_per_county[i][k] < complex_cycle->getCounty(k)->getNumberOfElectors()) {
                                cout << "Party " << i << " does not have enough representatives in county " << k << "." \
                                    << " Please enter more representatives for the specified county and party." << endl;
                                valid_reps_nums = false;
                            }
                        }
                    }


                    if (valid_reps_nums) { complexElectionResults(complex_cycle); }

                    for (int i = 0; i < election_cycle->partieslen(); i++)  //Deleting the temp array
                        delete[] representatives_per_party_per_county[i];
                    delete[] representatives_per_party_per_county;
                }
            }
            else {
                SimpleCycle* simple_cycle = dynamic_cast<SimpleCycle*>(election_cycle);
                if (simple_cycle->getVoteAmount() == 0) {
                    cout << "Nobody voted! Please vote! You don't want another 2016!" << endl;
                }
                else {
                    bool valid_reps_nums = true;
                    for (int i = 0; i < simple_cycle->partieslen(); i++) {
                        if (simple_cycle->getParties()[i]->partyRepsLen() < simple_cycle->getNumberOfElectors()) {
                            cout << "Party: " << simple_cycle->getParties()[i]->getName() << " does not have enough representatives." \
                                << " Please enter more representatives for the specified party." << endl;
                            valid_reps_nums = false;
                            break;
                        }
                    }

                    if (valid_reps_nums) { simpleElectionResults(simple_cycle); }
                }
            }
            break;
        case Save_Cycle:
        {
            saveElectionCycle(election_cycle);
            break;
        }

        case Load_Cycle:
            delete election_cycle;

            election_cycle = loadElectionCycle();
            
            break;
        }

        cout << endl;
    }
}

void firstMenu() {
    int choice = 0;
    ElectionCycle* election_cycle = nullptr;

    enum firstMenu {None, New_Election_Cycle, Load_Election_Cycle, Exit};

    cout << "Please select an option: " << endl \
        << "1. Create a new election cycle" << endl \
        << "2. Load an existing election cycle" << endl \
        << "3. Quit" << endl << endl;
    cin >> choice;

    while (choice < 1 || choice > 3) {
        cout << "Not a valid choice. Please enter a number between 1 and 3." << endl;
    }

    switch (choice)
    {
    case New_Election_Cycle:
    {
        int day = 0, month = 0, year = 0;
        cout << "Please choose the day of the election: ";
        cin >> day;
        while (day <= 0 || day > 31)
        {
            cout << "Day format is wrong. Please enter the number between 1-31. ";
            cin >> day;
        }

        cout << "Please choose the month of the election: ";
        cin >> month;
        while (month <= 0 || month > 12)
        {
            cout << "Month format is wrong. Please enter the number between 1-12. ";
            cin >> month;
        }

        cout << "Please choose the year of the election: ";
        cin >> year;
        while (year < 0)
        {
            cout << "Year format is wrong. Please enter a positive number. ";
            cin >> year;
        }

        Date date_of_election(day, month, year);

        int type;
        enum electionType { None, Simple_cycle, Complex_cycle };

        cout << "Pleae enter the type of the election cycle" \
            << " (1: Simple election cycle, 2: Complex election cycle): ";
        cin >> type;

        while (type < 1 || type > 2) {
            cout << "Not a valid input. Please enter one of the following numbers: \
                         1 - Simple election cycle, 2 - Complex election cycle";
            cin >> type;
        }

        if (type == Simple_cycle) {
            int number_of_electors = 0;
            cout << "Please choose the number of electors in the election cycle: ";
            cin >> number_of_electors;

            while (number_of_electors < 0) {
                cout << "Not a valid number of electors. Please choose a positive number: ";
                cin >> number_of_electors;
            }

            election_cycle = new SimpleCycle(date_of_election, number_of_electors);
            mainMenu(election_cycle);
        }
        else if (type == Complex_cycle) {
            election_cycle = new ComplexCycle(date_of_election);
            mainMenu(election_cycle);
        }

        break;
    }
    case Load_Election_Cycle:

        if (election_cycle)
            delete election_cycle;

        election_cycle = loadElectionCycle();
        mainMenu(election_cycle);

        break;

    case Exit:
        return;
        break;

    default:
        break;
    }

    if (election_cycle)
        delete election_cycle;
}

int main() {

    /* Main program */
    cout << "Hello! and welcome to our computerized election system. Please read the software requirements below: " << endl \
        << "1. All of the string inputs are maxed at length of 30" << endl \
        << "2. Adding a citizen requires an existing county." << endl \
        << "3. Adding a party requires an existing non representative citizen." << endl \
        << "4. Adding a party representative requires an existing party, and a non representative citizen." << endl \
        << "5. The leader of the party is automatically a party representative, but isn't included in party representative list per county. " << endl \
        << "6. Citizens can only represent their home county." << endl << endl;

    firstMenu();

    return 0;
}
