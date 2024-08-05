#ifndef SERVERWORKER_H
#define SERVERWORKER_H

#include <QObject>
#include "lib104/serverTableModel.h"
#include "lib104/API/ServerVariant.h"

class ServerWorker : public QObject
{
    Q_OBJECT
public:
    explicit ServerWorker(QObject *parent = nullptr);
    /*void setServer(ServerTableModel &server) {
        m_server = server;
    }*/

    ServerTableModel m_server;

public slots:


signals:

};

#endif // SERVERWORKER_H
