#include "lib104/serverTableModel.h"
#include "lib104/API/ServerVariant.h"
#include "lib104/startButtonDelegate.h"
#include <QLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QApplication>
#include <QTableView>
#include <QObject>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QVBoxLayout* verticalLayout = new QVBoxLayout();

    ServerTableModel model;
    QTableView *view = new QTableView();

    Server104 first_server;
    QThread first_thread;
    first_server.moveToThread(&first_thread);
    first_thread.start();
    model.appendServer(first_server);

    view->setModel(&model);
    view->setVisible(true);

    /*StartButtonDelegate *itemDelegate = new StartButtonDelegate();
    view->setItemDelegateForColumn(3, itemDelegate);
    connect(itemDelegate,SIGNAL(buttonClicked(QModelIndex)),model,SLOT(startServer(QModelIndex)));*/

    //widget->setLayout(verticalLayout);


    //QPushButton *addServerButton = new QPushButton("Add server");
    QPushButton *startServerButton = new QPushButton("Start server");
    QPushButton *doShitButton = new QPushButton("Do shit");
    verticalLayout->addWidget(view);
    //verticalLayout->addWidget(addServerButton);
    verticalLayout->addWidget(startServerButton);
    verticalLayout->addWidget(doShitButton);
    bool flag = true;


    QObject::connect(startServerButton, &QPushButton::clicked,
            [&flag, &startServerButton]
    {
        if (flag)
        {
//            emit first_server.startServer();
            qDebug() << "1";
            startServerButton->setText("Stop");
            flag = false;
        }
        else
        {
            qDebug() << "2";
            flag = true;
            startServerButton->setText("Start");
            //running = false;
            sigint_handler(2);
        }
    });

    QObject::connect(startServerButton, &QPushButton::clicked,
            &first_server, &Server104::startServer);

    int counter = 0;

    QObject::connect(doShitButton, &QPushButton::released,
                    doShitButton, [doShitButton, &counter]{
        doShitButton->setText("Eat shit " + QString::number(counter));
        counter++;
    });

    QWidget widget;
    widget.setLayout(verticalLayout);

    widget.show();

    return a.exec();
}
