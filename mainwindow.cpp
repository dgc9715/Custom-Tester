#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

const std::string workspace_path = "/media/dgc/DATOS/Programing/custom tester/linux/";
const std::string workspace_data_path = workspace_path + "data/";
const std::string build_flags = "-std=c++14 -O2 -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC";
const int tests = 200;

inline std::string get_path(std::string full_name, bool data_path = false)
{
    return "'" + (data_path ? workspace_data_path : workspace_path) + full_name + "'";
}

inline std::string get_build_command(std::string name)
{
    return "g++ " + build_flags + " " + get_path(name + ".cpp") + " -o " + get_path(name + ".out", true);
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

    if (ui->generator_checkBox->isChecked())
    {
        system(get_build_command("gen").data());
        ui->progressBar->setValue(ui->progressBar->value()+1);
    }

    if (ui->solution_checkBox->isChecked())
    {
        system(get_build_command("my").data());
        ui->progressBar->setValue(ui->progressBar->value()+1);
    }

    if (ui->brutalsol_checkBox->isChecked())
    {
        system(get_build_command("t").data());
        ui->progressBar->setValue(ui->progressBar->value()+1);
    }

    ui->progressBar->hide();
}

bool run_alive = false;

void MainWindow::_run()
{
    ui->progressBar->setMaximum(tests);
    ui->progressBar->show();
    ui->progressBar->setValue(0);
    ui->progressBar->repaint();
    while (ui->progressBar->value() != tests)
    {
        if (system((get_path("gen.out", true) + " > " + get_path("a.txt", true)).data())) break;
        if (system((get_path("my.out", true) + " < " + get_path("a.txt", true) + " > " + get_path("my.txt", true)).data())) break;
        if (system((get_path("t.out", true) + " < " + get_path("a.txt", true) + " > " + get_path("t.txt", true)).data())) break;
        if (system(("cmp " + get_path("my.txt", true) + " " + get_path("t.txt", true)).data())) break;
        ui->progressBar->setValue(ui->progressBar->value()+1);
        if (!run_alive) break;
        QApplication::processEvents();
    }
    run_alive = false;
    ui->progressBar->hide();
    ui->run_pushButton->setText("Run");
    ui->build_pushButton->setEnabled(true);
}

void MainWindow::on_run_pushButton_clicked()
{
    if (!run_alive)
    {
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
