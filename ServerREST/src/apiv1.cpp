#include "apiv1.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSettings>
#include <QUuid>
#include <QDebug>

using namespace Cutelyst;

ApiV1::ApiV1(QObject *parent) : Controller(parent)
{
}

ApiV1::~ApiV1()
{
}

void ApiV1::usuarios(Context *c)
{
    qDebug() << Q_FUNC_INFO;
}

void ApiV1::usuarios_GET(Context *c)
{
    qDebug() << Q_FUNC_INFO;
    QSettings s;
    const QStringList uuids = s.childGroups();

    QJsonArray array;
    for (const QString &uuid : uuids) {
        array.append(uuid);
    }

    c->response()->setJsonBody(array);
}

void ApiV1::usuarios_POST(Context *c)
{
    qDebug() << Q_FUNC_INFO;
    const QString uuid = QUuid::createUuid().toString()
            .remove(QLatin1Char('{'))
            .remove(QLatin1Char('}'));
    usuarios_uuid_PUT(c, uuid);
}

void ApiV1::usuarios_uuid(Context *c, const QString &uuid)
{
    qDebug() << Q_FUNC_INFO << uuid;
}

void ApiV1::usuarios_uuid_GET(Context *c, const QString &uuid)
{
    qDebug() << Q_FUNC_INFO << uuid;
    QSettings s;
    if (!s.childGroups().contains(uuid)) {
        c->response()->setJsonBody(QJsonObject{
                                       {QStringLiteral("error"), QStringLiteral("not found")}
                                   });
        c->response()->setStatus(Response::NotFound);
        return;
    }

    s.beginGroup(uuid);
    c->response()->setJsonBody(QJsonObject{
                                   {QStringLiteral("nome"), s.value(QStringLiteral("nome")).toString()},
                                   {QStringLiteral("idade"), s.value(QStringLiteral("idade")).toInt()}
                               });
}

void ApiV1::usuarios_uuid_PUT(Context *c, const QString &uuid)
{
    qDebug() << Q_FUNC_INFO << uuid;
    const QJsonDocument doc = c->request()->bodyData().toJsonDocument();
    const QJsonObject obj = doc.object();

    QSettings s;
    s.beginGroup(uuid);
    s.setValue(QStringLiteral("nome"), obj.value(QStringLiteral("nome")).toString());
    s.setValue(QStringLiteral("idade"), obj.value(QStringLiteral("idade")).toDouble());
    s.endGroup();
    s.sync();

    if (s.status()) {
        c->response()->setJsonBody(QJsonObject{
                                       {QStringLiteral("status"), QStringLiteral("error")},
                                       {QStringLiteral("error"), QStringLiteral("falhou")}
                                   });
    } else {
        c->response()->setJsonBody(QJsonObject{
                                       {QStringLiteral("status"), QStringLiteral("ok")},
                                       {QStringLiteral("uuid"), uuid}
                                   });
    }
}

void ApiV1::usuarios_uuid_DELETE(Context *c, const QString &uuid)
{
    qDebug() << Q_FUNC_INFO << uuid;
    QSettings s;
    bool removed = s.childGroups().contains(uuid);
    if (removed) {
        s.remove(uuid);
        s.sync();
    }

    if (!removed || s.status()) {
        c->response()->setJsonBody(QJsonObject{
                                       {QStringLiteral("status"), QStringLiteral("error")},
                                       {QStringLiteral("error"), QStringLiteral("falhou")}
                                   });
    } else {
        c->response()->setJsonBody(QJsonObject{
                                       {QStringLiteral("status"), QStringLiteral("ok")},
                                   });
    }
}
