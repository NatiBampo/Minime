#include "mainwindow.h"
#include "lib104/serverTableModel.h"
#include "lib104/API/Server104.h"
#include "lib104/startButtonDelegate.h"
#include <QLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();


    QVBoxLayout* verticalLayout = new QVBoxLayout();

    //QWidget *widget = new QWidget(this);

    ServerTableModel model;
    QTableView *view = new QTableView();

    Server104 first_server;
    QThread first_thread;
    model.appendServer(first_server);
    //first_server.moveToThread(&first_thread);
    //first_thread.start();

    view->setModel(&model);
    view->setVisible(true);
    /*StartButtonDelegate *itemDelegate = new StartButtonDelegate();
    view->setItemDelegateForColumn(3, itemDelegate);
    connect(itemDelegate,SIGNAL(buttonClicked(QModelIndex)),model,SLOT(startServer(QModelIndex)));*/

    //widget->setLayout(verticalLayout);


    QPushButton *addServerButton = new QPushButton("Add server");
    QPushButton *startServerButton = new QPushButton("Start server");
    verticalLayout->addWidget(view);
    verticalLayout->addWidget(addServerButton);
    verticalLayout->addWidget(startServerButton);
    QWidget widget;
    widget.setLayout(verticalLayout);

    widget.show();

    return a.exec();
}
