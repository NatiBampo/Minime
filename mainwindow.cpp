#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "lib104/serverTableModel.h"
#include "lib104/API/Server104.h"
#include "lib104/startButtonDelegate.h"
#include <QLayout>
#include <QPushButton>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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


    QPushButton *addServerButton = new QPushButton("Add server", this);
    QPushButton *startServerButton = new QPushButton("Start server", this);
    verticalLayout->addWidget(view);
    verticalLayout->addWidget(addServerButton);
    verticalLayout->addWidget(startServerButton);
    QWidget widget;
    widget.setLayout(verticalLayout);
    //widget.show();
    //setCentralWidget(widget);

    //setCentralWidget(&widget);
    //widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //this->setLayout(verticalLayout);

    //connect(addServerButton, &QPushButton::clicked, model, &ServerTableModel::appendServer);
    //connect(startServerButton, &QPushButton::clicked, this, &MainWindow::StartServer);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addServer()
{
     QMessageBox::information(this, "Информация", "Добавил сервер");
}

/*void MainWindow::startServer(const QModelIndex &index)
{
     QMessageBox::information(this, "Информация", "Запустил сервер");
}*/
