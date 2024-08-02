#include "serverTableModel.h"
#include "API/serverController.h"

ServerTableModel::ServerTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    headers << "IP" << "Port" << "ASDU" << "State";// << "IOT"<< "TagTape" << "TagValue" << "Time" << "State";

//    defaultServer = new ServerModel();//make a defaultInstance singleton
    //QThread server_Thread;
    //defaultServer.moveToThread(&server_Thread);
    //server_Thread.start();
    defaultServer.start();
}


int ServerTableModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}

int ServerTableModel::rowCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant ServerTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
        return QVariant(headers.at(section));
    return QVariant();
}

QVariant ServerTableModel::data(const QModelIndex &index, int role) const
{

   if (role == Qt::DisplayRole)
   {
       Server104 server = m_servers.at(index.row());

       switch (index.column())
       {
       case 0:
       {
           unsigned int ip[4];
           for (int i = 0; i < 4; i++)
               ip[i] = server.getIP()[i];
           return QString("%1.%2.%3.%4").arg(ip[0]).arg(ip[1]).arg(ip[2]).arg(ip[3]);
           break;
       }
       case 1:
           return QString("%1").arg(server.getPort());
           break;
       case 2:
           return QString("%1").arg(server.getAsdu());
       case 3:
           return server.getState() ? QString("Running") : QString("Stopped");
           break;
       /*case 4:
           return servers[index.row()].tagType;
       case 5:
           return servers[index.row()].value;
       case 6:
           return servers[index.row()].time;
       case 7:
           return servers[index.row()].state;*/
       }

   }
   return QVariant();
}

bool ServerTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if( !index.isValid() || role != Qt::EditRole || m_servers.count() <= index.row() ) {
        return false;
    }
    emit dataChanged( index, index );

    return true;
}

void ServerTableModel::appendServer()
{
    /*Server104 server;
    QThread server_Thread;
    server.moveToThread(&server_Thread);
    server_Thread.start();
    m_server_threads->append(server_Thread);

    int row = m_servers->count() - 1;
    beginInsertRows( QModelIndex(), row, row);
    m_servers->append( &server );
    endInsertRows();*/
}

void ServerTableModel::appendServer(const unsigned int *address)//, const float *tag, const char* time, bool state )
{
    /*Server104 server(address);// = new Server104(address);
    QThread server_Thread;
    server.moveToThread(&server_Thread);
    //connect(&server_Thread, &QThread::finished, server, &QObject::deleteLater);
    //connect(this, &Controller::operate, worker, &Worker::doWork);
    server_Thread.start();
    m_server_threads.append(server_Thread);

    int row = m_servers.count() - 1;
    beginInsertRows( QModelIndex(), row, row);
    m_servers.append(server);
    endInsertRows();*/
}


void ServerTableModel::startServer(const QModelIndex &index)
{
    Server104 server = m_servers[index.row()];
    bool state = server.getState();
    if (!state)
    {
        qDebug() << QString("Starting %1 server").arg(index.row());
        init_iec_server();
    }
    else
    {
        qDebug() << QString("Stopping %1 server").arg(index.row());

        sigint_handler(0);
    }
    server.setState(!state);

    //m_servers.at(index.row());

}


void ServerTableModel::deleteServer(const QModelIndex &index)
{
    /*
    //QThread server_Thread = m_server_threads.takeAt(index.row());
    //m_server_threads.at(index.row()).quit();
    //m_server_threads.a
    //server_Thread.quit();
    //server_Thread.terminate();

    //Server104 server = m_servers.takeAt(index.row());
    //server.stop;
    //delete server;
    //m_server_threads.removeAt(row);
*/
}
