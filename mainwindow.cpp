#include "mainwindow.h"
#include "ui_mainwindow.h"

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

#include <string>
const std::string workspace_path = "/media/dgc/DATOS/Programing/custom tester/linux/";
const std::string workspace_data_path = workspace_path + "data/";
const std::string build_flags = "-std=c++14 -O2";

std::string get_path(std::string full_name, bool data_path = false)
{
    return "'" + (data_path ? workspace_data_path : workspace_path) + full_name + "'";
}

std::string get_build_command(std::string name)
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

//    g++ -std=c++14 -O2 test.cpp -o ./data/test.out
//    g++ -std=c++14 -O2 gen.cpp -o ./data/gen.out
//    g++ -std=c++14 -O2 my.cpp -o ./data/my.out
//    g++ -std=c++14 -O2 t.cpp -o ./data/t.out
//    ./data/test.out
}

void MainWindow::on_run_pushButton_clicked()
{

}
