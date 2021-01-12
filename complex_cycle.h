#pragma once
#include "election_cycle.h"
#include <vector>

class ComplexCycle : public ElectionCycle {
private:
    DynamicArray<County*> counties;

public:
    /* Constructors and destructors */
    ComplexCycle();
    ComplexCycle(Date& _date_of_election);
    virtual ~ComplexCycle();

    /* Getters */
    DynamicArray<County*> getCounties() const { return counties; }
    int countieslen() const { return counties.size(); }
    County* getCounty(int id) const { return counties[id]; }

    /* Adders */
    bool addCounty(County* county);

    /* Operators */
    friend ostream& operator<<(ostream& os, const ComplexCycle& election_cycle);

    /*Serialization*/
    virtual void save(ostream& out) const;
    virtual void load(istream& in);
};