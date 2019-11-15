#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gameengine.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    GameEngine *ge = new GameEngine(this);
    setCentralWidget(ge);
    ge->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

