#ifndef IECSERVERVI\EW_H
#define IECSERVERVI\EW_H

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
