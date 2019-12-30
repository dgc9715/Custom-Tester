#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_generator_pushButton_clicked();

    void on_solution_pushButton_clicked();

    void on_brutalsol_pushButton_clicked();

    void on_build_pushButton_clicked();

    void on_run_pushButton_clicked();

    void on_view_diff_pushButton_clicked();

    void _run();

    int _system(std::string);

    void on_show_time_checkBox_stateChanged(int arg1);

    void on_test_number_comboBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
