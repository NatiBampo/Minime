#include "serverTableModel.h"


ServerTableModel::ServerTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    headers << "Server104" << "Buttons"; //"IP" << "Port" << "ASDU" << "State";// << "IOT"<< "TagTape" << "TagValue" << "Time" << "State";
}


int ServerTableModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

int ServerTableModel::rowCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant ServerTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{

    /*if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch (section) {
        case 0:
            return QString("Cервер104");
        case 1:
            qDebug() << "Управление";
            return QString("Управление");
        }
    }
    return QVariant();*/
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
        return QVariant(headers.at(section));
    return QVariant();
}

QVariant ServerTableModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();

   if (role == Qt::DisplayRole)
   {
       Server104 server = qvariant_cast<Server104>(m_servers[row]);
       switch (col)
       {
       case 0:
       {
           unsigned int ip[4];
           for (int i = 0; i < 4; i++)
               ip[i] = server.getIP()[i];
           return QString("%1.%2.%3.%4").arg(ip[0]).arg(ip[1]).arg(ip[2]).arg(ip[3]);
           break;
       }
       /*case 1:
           qDebug() << "1";
           return QString("%1").arg(server.getPort());
           break;
       case 2:
           qDebug() << "2";

           return QString("%1").arg(server.getAsdu());
       case 3:
           qDebug() << "3";

           return server.getState() ? QString("Running") : QString("Stopped");
           break;
       case 4:
           return servers[index.row()].tagType;
       case 5:
           return servers[index.row()].value;
       case 6:
           return servers[index.row()].time;
       case 7:
           return servers[index.row()].state;*/
       }
   }
   if (role == Qt::TextAlignmentRole)
       return Qt::AlignCenter;

   return QVariant();
}
/*
bool ServerTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if( !index.isValid() || role != Qt::EditRole || m_servers.count() <= index.row() ) {
        return false;
    }
    if (index.column()==0)
        m_servers[index.row()] = value;

    emit dataChanged( index, index );

    return true;
}
*/
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

void ServerTableModel::appendServer(Server104 &server)//const unsigned int *address)//, const float *tag, const char* time, bool state )
{
    m_servers.append(QVariant::fromValue(server));
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

//const QModelIndex &index)
void ServerTableModel::startServer()
{
//    StarRating starRating = qvariant_cast<StarRating>(index.data());
//    QVariant::fromValue(StarRating(staticData[row].rating)));

    Server104 server = qvariant_cast<Server104>(m_servers[0]); //index.row()]);
    server.startServer();

    //m_servers.at(index.row());

}

void ServerTableModel::stopServer()
{
//    StarRating starRating = qvariant_cast<StarRating>(index.data());
//    QVariant::fromValue(StarRating(staticData[row].rating)));
    Server104 server = qvariant_cast<Server104>(m_servers[0]); //index.row()]);

    bool state = server.getState();
    if (!state)
    {
        qDebug() << QString("Starting %1 server").arg(0); //index.row());
        init_iec_server();
    }
    else
    {
        qDebug() << QString("Stopping %1 server").arg(0); //index.row());

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


Qt::ItemFlags ServerTableModel::flags(const QModelIndex &) const
{
    return Qt::ItemIsEnabled;// | Qt::ItemIsSelectable;
}
