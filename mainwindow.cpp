#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "config.h"
#include <chrono>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->hide();
    ui->curtest_label->hide();
    if (!ui->show_time_checkBox->isChecked())
    {
        ui->solution_time_label->hide();
        ui->brutalsol_time_label->hide();
    }
    for (int i = 1; i <= 9; ++i)
        ui->test_number_comboBox->addItem(QString(("1e" + std::to_string(i)).data()));
    ui->test_number_comboBox->setCurrentIndex(4);
}

MainWindow::~MainWindow()
{
    delete ui;
}

inline std::string get_path(std::string full_name, bool data_path = false)
{
    return "'" + (data_path ? workspace_data_path : workspace_path) + full_name + "'";
}

inline std::string get_build_command(std::string name)
{
    return "g++ " + build_flags + " " + get_path(name + ".cpp") + " -o " + get_path(name + ".out", true);
}

int MainWindow::_system(std::string s)
{
    s += " 2> " + get_path("log.txt", true);
    int x = system(s.data());
    if (x)
    {
        if (s.substr(0, 3) == "cmp") on_view_diff_pushButton_clicked();
        else system(("subl " + get_path("log.txt", true)).data());
    }
    return x;
}

void MainWindow::on_generator_pushButton_clicked()
{
    system(("subl " + get_path("gen.cpp")).data());
}

void MainWindow::on_solution_pushButton_clicked()
{
    system(("subl " + get_path("my.cpp")).data());
}

void MainWindow::on_brutalsol_pushButton_clicked()
{
    system(("subl " + get_path("t.cpp")).data());
}

void MainWindow::on_build_pushButton_clicked()
{
    ui->progressBar->setMaximum(
               (int) ui->generator_checkBox->isChecked()
                   + ui->solution_checkBox->isChecked()
                   + ui->brutalsol_checkBox->isChecked() );
    ui->progressBar->show();
    ui->progressBar->setValue(0);
    ui->progressBar->repaint();

    bool ok = true;
    if (ui->generator_checkBox->isChecked())
    {
        if (ok && _system(get_build_command("gen"))) ok = false;
        ui->progressBar->setValue(ui->progressBar->value()+1);
    }

    if (ui->solution_checkBox->isChecked())
    {
        if (ok && _system(get_build_command("my"))) ok = false;
        ui->progressBar->setValue(ui->progressBar->value()+1);
    }

    if (ui->brutalsol_checkBox->isChecked())
    {
        if (ok && _system(get_build_command("t"))) ok = false;
        ui->progressBar->setValue(ui->progressBar->value()+1);
    }

    ui->progressBar->hide();
}

bool run_alive = false;
typedef std::chrono::_V2::system_clock::time_point timepoint;

inline timepoint get_time()
{
    return std::chrono::high_resolution_clock::now();
}

inline QString get_elapsed(timepoint t)
{
    return QString((std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(get_time() - t).count()) + " ms").data());
}

void MainWindow::_run()
{
    ui->progressBar->setMaximum(tests);
    ui->progressBar->show();
    ui->progressBar->setValue(0);
    ui->progressBar->repaint();
    ui->curtest_label->show();
    ui->curtest_label->setText("Running on test");
    ui->curtest_label->repaint();

    while (ui->progressBar->value() != tests)
    {
        ui->curtest_label->setText(QString(("Running on test " + std::to_string(ui->progressBar->value()+1)).data()));

        if (_system((get_path("gen.out", true) + " > " + get_path("a.txt", true)))) break;

        timepoint curtime = get_time();
        if (_system((get_path("my.out", true) + " < " + get_path("a.txt", true) + " > " + get_path("my.txt", true)))) break;
        if (ui->show_time_checkBox->isChecked()) ui->solution_time_label->setText(get_elapsed(curtime));

        curtime = get_time();
        if (_system((get_path("t.out", true) + " < " + get_path("a.txt", true) + " > " + get_path("t.txt", true)))) break;
        if (ui->show_time_checkBox->isChecked()) ui->brutalsol_time_label->setText(get_elapsed(curtime));

        if (_system(("cmp " + get_path("my.txt", true) + " " + get_path("t.txt", true)).data())) break;

        ui->progressBar->setValue(ui->progressBar->value()+1);
        QApplication::processEvents();
        if (!run_alive) break;
    }

    run_alive = false;
    ui->progressBar->hide();
    ui->curtest_label->hide();
    ui->run_pushButton->setText("Run");
    ui->build_pushButton->setEnabled(true);
    ui->test_number_comboBox->setEnabled(true);
}

void MainWindow::on_run_pushButton_clicked()
{
    if (!run_alive)
    {
        ui->test_number_comboBox->setEnabled(false);
        ui->build_pushButton->setEnabled(false);
        ui->run_pushButton->setText("Stop");
        run_alive = true;
        _run();
        return;
    }

    run_alive = false;
    ui->run_pushButton->setText("Run");
    ui->build_pushButton->setEnabled(true);
}

void MainWindow::on_view_diff_pushButton_clicked()
{
    system(("subl " + get_path("a.txt", true)).data());
    system(("subl -n " + get_path("my.txt", true) + " " + get_path("t.txt", true)
           + " --command 'sublimerge_diff_views {\"left_read_only\": true, \"right_read_only\": true}'").data());
}

void MainWindow::on_show_time_checkBox_stateChanged(int arg1)
{
    if (arg1)
    {
        ui->solution_time_label->setText("");
        ui->brutalsol_time_label->setText("");
        ui->solution_time_label->show();
        ui->brutalsol_time_label->show();
    }
    else
    {
        ui->solution_time_label->hide();
        ui->brutalsol_time_label->hide();
    }
}

void MainWindow::on_test_number_comboBox_currentIndexChanged(int index)
{
    tests = 1;
    for (int i = 0; i <= index; ++i)
        tests *= 10;
}
