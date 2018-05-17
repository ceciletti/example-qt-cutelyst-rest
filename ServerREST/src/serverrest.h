#ifndef SERVERREST_H
#define SERVERREST_H

#include <Cutelyst/Application>

using namespace Cutelyst;

class ServerREST : public Application
{
    Q_OBJECT
    CUTELYST_APPLICATION(IID "ServerREST")
public:
    Q_INVOKABLE explicit ServerREST(QObject *parent = 0);
    ~ServerREST();

    bool init() override final;
};

#endif //SERVERREST_H

