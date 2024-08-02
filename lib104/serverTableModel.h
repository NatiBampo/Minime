#ifndef SERVERTABLEMODEL_H
#define SERVERTABLEMODEL_H

#include <QAbstractItemModel>
#include <QThread>
#include <QDebug>
#include <QVariant>
#include "Server104.h"

class ServerTableModel : public QAbstractTableModel
{
    Q_OBJECT
    enum Column { IP = 0, Port = 1, ASDU = 2 };
    //, IOT = 3, TAGTYPE = 4, VALUE = 5, TIME = 6, STATE = 7 };
private:
    QList<QVariant> m_servers;

public:
    explicit ServerTableModel(QObject *parent = nullptr);
    QStringList headers;
    void appendServer(Server104 &server);//const unsigned int *address);//, const float *tag, const char* time, const bool state );

protected:
    virtual int rowCount(const QModelIndex& index) const override;
    virtual int columnCount(const QModelIndex& index) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
private:

    void appendServer();

public slots:
    void startServer();//const QModelIndex &index);
    void deleteServer(const QModelIndex &index);
};


#endif // SERVERTABLEMODEL_H
