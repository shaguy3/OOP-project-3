#pragma once
#include "election_cycle.h"

class ComplexCycle : public ElectionCycle {
private:
    County** counties;
    int counties_num_size;
    int counties_num_logi;
    void resizeCounties();

public:
    /* Constructors and destructors */
    ComplexCycle();
    ComplexCycle(Date& _date_of_election);
    virtual ~ComplexCycle();

    /* Getters */
    County** getCounties() const { return counties; }
    int countieslen() const { return counties_num_logi; }
    County* getCounty(int id) const { return counties[id]; }

    /* Adders */
    bool addCounty(County* county);

    /* Operators */
    friend ostream& operator<<(ostream& os, const ComplexCycle& election_cycle);

    /*Serialization*/
    virtual void save(ostream& out) const;
    virtual void load(istream& in);
};