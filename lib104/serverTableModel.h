#ifndef SERVERTABLEMODEL_H
#define SERVERTABLEMODEL_H

#include <QAbstractItemModel>
#include <QThread>
#include <QDebug>
#include "Server104.h"

class ServerTableModel : public QAbstractTableModel
{
    Q_OBJECT
    enum Column { IP = 0, Port = 1, ASDU = 2 };
    //, IOT = 3, TAGTYPE = 4, VALUE = 5, TIME = 6, STATE = 7 };

    Server104 defaultServer;
    Server104 second_server;
    QList<Server104> m_servers;
    QList<QThread> m_server_threads;

public:
    explicit ServerTableModel(QObject *parent = nullptr);
    QStringList headers;

private:

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    void appendServer();

public slots:
    void startServer(const QModelIndex &index);
    void appendServer(const unsigned int *address);//, const float *tag, const char* time, const bool state );
    void deleteServer(const QModelIndex &index);
};


#endif // SERVERTABLEMODEL_H
