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

    ServerVariant first_server;
    QThread first_thread;
    first_server.moveToThread(&first_thread);
    first_thread.start();
    model.appendServer(first_server);

    view->setModel(&model);
    view->setVisible(true);


    //QPushButton *addServerButton = new QPushButton("Add server");
    QPushButton *startServerButton = new QPushButton("Start server");
    QPushButton *startServerButton2 = new QPushButton("Start server2");
    QPushButton *doShitButton = new QPushButton("Do shit");
    verticalLayout->addWidget(view);
    //verticalLayout->addWidget(addServerButton);
    verticalLayout->addWidget(startServerButton);
    verticalLayout->addWidget(startServerButton2);
    verticalLayout->addWidget(doShitButton);
    bool flag = true;
    bool flag2 = true;

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
//            sigint_handler(2);
            someshit_handler();
            //running = false;
        }
    });

    QObject::connect(startServerButton, &QPushButton::clicked,
            &first_server, &ServerVariant::startServer);




    QObject::connect(startServerButton2, &QPushButton::clicked,
            [&flag2, &startServerButton2, &first_server]
    {
        if (flag2)
        {
            qDebug() << "11";
            first_server.state = true;
            startServerButton2->setText("Stop2");
            flag2 = false;
        }
        else
        {
            qDebug() << "21";
            flag2 = true;
            startServerButton2->setText("Start2");
            first_server.state = false;
        }
    });

    QObject::connect(startServerButton2, &QPushButton::clicked,
            &first_server, &ServerVariant::startServer2);


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
