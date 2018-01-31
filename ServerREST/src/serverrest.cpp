#include "serverrest.h"

#include "apiv1.h"

using namespace Cutelyst;

ServerREST::ServerREST(QObject *parent) : Application(parent)
{
}

ServerREST::~ServerREST()
{
}

bool ServerREST::init()
{
    new ApiV1(this);

    return true;
}

