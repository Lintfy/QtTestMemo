#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

using namespace std;

int utd;
QSet<int> pressed;
const QString fileExtention=QString("All (*.*);;Text (*.txt);;C++ (*.cpp *.h)");
QFont nFont;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    utd=0;
    ui->setupUi(this);
    addTextTab(QString("Untitled"));
    installEventFilter(this);
    nFont  . setPixelSize(15);
    ui->menuBar->setFont(nFont);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::addTextTab(QString name){

    QWidget * newTab = new QWidget(ui->MainBox);

    if (name=="Untitled") {
        QString kqs;
        kqs.setNum(utd);
        name.push_back(kqs);
        utd++;
    }

    QHBoxLayout * layout = new QHBoxLayout;
    QTextEdit *   nEdit  = new QTextEdit();

    nFont  . setPixelSize(15);

    nEdit  -> setFont(nFont);
    nEdit  -> setObjectName(name+"_edit");

    layout -> addWidget(nEdit);
    layout -> setContentsMargins(0,0,0,0);
    newTab -> setLayout(layout);
    newTab -> setObjectName(name);
    newTab -> setFont(nFont);

    connect(ui->MainBox,SIGNAL(tabCloseRequested(int)),this,SLOT(deleteTab(int)));
    connect(nEdit,SIGNAL(textChanged()),this,SLOT(unsaved()));

    ui->MainBox -> addTab(newTab,name);
    ui->MainBox -> setCurrentWidget(newTab);

}

void MainWindow::on_actionNew_File_triggered(){
    addTextTab(QString("Untitled"));
}

void MainWindow::on_actionOpen_triggered(){
    QString fileST = QFileDialog::getOpenFileName(this,QString("Open"),"/",fileExtention);

    if(!fileST.isEmpty()){

        addTextTab(fileST);

        QFile qf(fileST);
        if (!qf.open(QIODevice::ReadOnly)) {
            qDebug()<<fileST<<"could not open file";
            return;
        }
        QTextStream qfTS(&qf);

        ui->MainBox->currentWidget()->findChild<QTextEdit *>(fileST+QString("_edit"))
                ->setText(qfTS.readAll());

        qf.close();
        ui->MainBox->setTabText(ui->MainBox->indexOf(ui->MainBox->currentWidget()),fileST);
    }


}

void MainWindow::on_actionSave_triggered(){
    QString currentST  = ui->MainBox->currentWidget()->objectName();
    QTextEdit * textin = ui->MainBox->currentWidget()->findChild<QTextEdit *>(currentST+QString("_edit"));

    if (currentST[0]!="/") { on_actionSave_as_new_triggered();return; }

    ui->MainBox->setTabText(ui->MainBox->indexOf(ui->MainBox->currentWidget()),currentST);

    if (!currentST.isEmpty()) {

        QFile qf(currentST);
        if (!qf.open(QIODevice::WriteOnly)) {
            qDebug()<<currentST<<"could not save file";
            return;
        }

        QTextStream qfTS(&qf);
        qfTS << textin->toPlainText();
        qfTS.flush();
        qf.close();

    }
}

void MainWindow::on_actionSave_as_new_triggered(){
    QString titleST = ui->MainBox->currentWidget()->objectName();
    QString fileST  = QFileDialog::getSaveFileName(this,QString("Save"),"/",fileExtention);

    if (!fileST.isEmpty()) {
        ui->MainBox->currentWidget()->setObjectName(fileST);
        ui->MainBox->currentWidget()->findChild<QTextEdit *>(titleST+QString("_edit"))->setObjectName(fileST+QString("_edit"));

        on_actionSave_triggered();
    }

}

void MainWindow::deleteTab(int index){
    ui->MainBox->removeTab(index);
}

bool MainWindow::eventFilter(QObject * obj, QEvent * event)
{
    QKeyEvent * keyEvent;

    if (event->type() == QEvent::KeyPress) {
        keyEvent = static_cast<QKeyEvent * >(event);
        pressed += keyEvent->key();
        if (pressed.contains(Qt::Key_Control) and pressed.contains(Qt::Key_S)){
            on_actionSave_triggered();
        }
        return true;
    }

    if (event->type() == QEvent::KeyRelease) {
        keyEvent = static_cast<QKeyEvent * >(event);
        pressed -= keyEvent->key();

    }
    return false;
}

void MainWindow::unsaved(){
    //cout<<"YAY"<<endl;
    QString unsavedST = ui->MainBox->currentWidget()->objectName() + QString(" *");
    ui->MainBox->setTabText(ui->MainBox->indexOf(ui->MainBox->currentWidget()),unsavedST);

}
