#ifndef APIV1_H
#define APIV1_H

#include <Cutelyst/Controller>

using namespace Cutelyst;

class ApiV1 : public Controller
{
    Q_OBJECT
public:
    explicit ApiV1(QObject *parent = 0);
    ~ApiV1();

    C_ATTR(usuarios, :Local :AutoArgs :ActionClass(REST))
    void usuarios(Context *c);

    C_ATTR(usuarios_GET, :Private)
    void usuarios_GET(Context *c);

    C_ATTR(usuarios_POST, :Private)
    void usuarios_POST(Context *c);

    C_ATTR(usuarios_uuid, :Path('usuarios') :AutoArgs :ActionClass(REST))
    void usuarios_uuid(Context *c, const QString &uuid);

    C_ATTR(usuarios_uuid_GET, :Private)
    void usuarios_uuid_GET(Context *c, const QString &uuid);

    C_ATTR(usuarios_uuid_PUT, :Private)
    void usuarios_uuid_PUT(Context *c, const QString &uuid);

    C_ATTR(usuarios_uuid_DELETE, :Private)
    void usuarios_uuid_DELETE(Context *c, const QString &uuid);
};

#endif //APIV1_H

