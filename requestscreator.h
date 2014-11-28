#ifndef REQUESTSCREATOR_H
#define REQUESTSCREATOR_H

#include "listofvars.h"
#include "modbusvar.h"
#include "listofrequests.h"

class RequestsCreator
{
public:
    RequestsCreator();
    static void makeRequests(ListOfVars* vars,ListOfRequests* reqs);
};

#endif // REQUESTSCREATOR_H
