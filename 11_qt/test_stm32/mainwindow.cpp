#include "mainwindow.h"
#include "ui_mainwindow.h"

void setup_buttons(Ui::MainWindow *ui);



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setup_buttons(ui);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void setup_buttons(Ui::MainWindow *ui)
{
    QList<QPushButton *> butts = ui.findChildren<QPushButton *>();
}
