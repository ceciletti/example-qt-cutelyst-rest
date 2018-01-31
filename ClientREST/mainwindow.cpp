#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QDebug>

const static QUrl server(QLatin1String("http://localhost:3000"));
const static QString usuariosApiPath(QLatin1String("/api/v1/usuarios"));
const static QString contentTypeJson(QLatin1String("application/json"));

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_nam(new QNetworkAccessManager(this))
{
    ui->setupUi(this);

    connect(ui->obterPB, &QPushButton::clicked, this, &MainWindow::getUsuarios);
    connect(ui->uuidCB, &QComboBox::currentTextChanged, this, &MainWindow::getUsuario);
    connect(ui->novoAtualizarPB, &QPushButton::clicked, this, &MainWindow::novoOuAtualizarUsuario);
    connect(ui->apagarPB, &QPushButton::clicked, this, &MainWindow::apagarUsuario);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getUsuarios()
{
    while (ui->uuidCB->count() > 1) {
        ui->uuidCB->removeItem(1);
    }

    QUrl url = server;
    url.setPath(usuariosApiPath);

    QNetworkRequest request(url);

    QNetworkReply *reply = m_nam->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply] () {
        reply->deleteLater();
        const QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        const QJsonArray array = doc.array();
        for (const QJsonValue &value : array) {
            ui->uuidCB->addItem(value.toString());
        }
    });
}

void MainWindow::getUsuario()
{
    const QString uuid = ui->uuidCB->currentText();
    if (uuid.isEmpty()) {
        return;
    }

    QUrl url = server;
    url.setPath(usuariosApiPath + QLatin1Char('/') + uuid);

    QNetworkRequest request(url);

    QNetworkReply *reply = m_nam->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply] () {
        reply->deleteLater();
        const QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        const QJsonObject obj = doc.object();
        ui->nomeLE->setText(obj.value(QLatin1String("nome")).toString());
        ui->idadeSP->setValue(obj.value(QLatin1String("idade")).toInt());
    });
}

void MainWindow::novoOuAtualizarUsuario()
{
    if (ui->uuidCB->currentIndex() == 0) {
        novoUsuario();
    } else {
        atualizaUsuario();
    }
}

void MainWindow::novoUsuario()
{
    QUrl url = server;
    url.setPath(usuariosApiPath);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, contentTypeJson);

    QJsonObject obj{
        {QStringLiteral("nome"), ui->nomeLE->text()},
        {QStringLiteral("idade"), ui->idadeSP->value()}
    };

    QNetworkReply *reply = m_nam->post(request, QJsonDocument(obj).toJson());
    connect(reply, &QNetworkReply::finished, this, [this, reply] () {
        reply->deleteLater();
        const QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        const QJsonObject obj = doc.object();

        if (obj.value(QLatin1String("status")).toString() == QLatin1String("ok")) {
            ui->uuidCB->addItem(obj.value(QLatin1String("uuid")).toString());
            ui->uuidCB->setCurrentIndex(ui->uuidCB->count() - 1);
        } else {
            qWarning() << "ERROR" << obj.value(QLatin1String("error")).toString();
        }
    });
}

void MainWindow::atualizaUsuario()
{
    const QString uuid = ui->uuidCB->currentText();
    const int currentIndex = ui->uuidCB->currentIndex();
    if (currentIndex <= 0 && uuid.isEmpty()) {
        return;
    }

    QUrl url = server;
    url.setPath(usuariosApiPath + QLatin1Char('/') + uuid);

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, contentTypeJson);

    QJsonObject obj{
        {QStringLiteral("nome"), ui->nomeLE->text()},
        {QStringLiteral("idade"), ui->idadeSP->value()}
    };

    QNetworkReply *reply = m_nam->put(request, QJsonDocument(obj).toJson());
    connect(reply, &QNetworkReply::finished, this, [this, reply] () {
        reply->deleteLater();
        const QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        const QJsonObject obj = doc.object();

        if (obj.value(QLatin1String("status")).toString() == QLatin1String("error")) {
            qWarning() << "ERROR" << obj.value(QLatin1String("error")).toString();
        }
    });
}

void MainWindow::apagarUsuario()
{
    const QString uuid = ui->uuidCB->currentText();
    const int currentIndex = ui->uuidCB->currentIndex();
    if (currentIndex <= 0 || uuid.isEmpty()) {
        return;
    }

    QUrl url = server;
    url.setPath(usuariosApiPath + QLatin1Char('/') + uuid);

    QNetworkRequest request(url);

    QNetworkReply *reply = m_nam->deleteResource(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply, currentIndex] () {
        reply->deleteLater();
        const QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        const QJsonObject obj = doc.object();
        if (obj.value(QLatin1String("status")).toString() == QLatin1String("ok")) {
            ui->uuidCB->removeItem(currentIndex);
        } else {
            qWarning() << "ERROR" << obj.value(QLatin1String("error")).toString();
        }
    });
}
