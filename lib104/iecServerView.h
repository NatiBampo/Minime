#ifndef IECSERVERVIEW_H
#define IECSERVERVIEW_H

#include <QTableView>

class IECServerView : public QTableView
{
    Q_OBJECT
public:
    explicit IECServerView(QObject *parent = nullptr);
    ~IECServerView();
    void addServer();

signals:

};

#endif // IECSERVERVI\EW_H
