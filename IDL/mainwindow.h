#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QDebug>
#include <QFile>
#include <QKeyEvent>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionNew_File_triggered();
    void addTextTab(QString name);
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSave_as_new_triggered();
    void deleteTab(int index);
    void unsaved();

private:
    Ui::MainWindow *ui;

protected:
    bool eventFilter(QObject* widget, QEvent* event);

};

#endif // MAINWINDOW_H
