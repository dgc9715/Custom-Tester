#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "config.h"
#include <chrono>
#include <QDir>
#include <QFile>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    myprocess(new QProcess())
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
    ui->test_number_comboBox->setCurrentIndex(2);

    if (!QDir(QString(workspace_data_path.data())).exists())
        QDir().mkdir(QString(workspace_data_path.data()));

    ui->timelimit_spinBox->setValue(time_out);
}

MainWindow::~MainWindow()
{
    delete ui;
}

inline std::string get_path(std::string full_name, bool data_path = false, bool add_quote = true)
{
    return (add_quote ? "'" : "") + (data_path ? workspace_data_path : workspace_path) + full_name + (add_quote ? "'" : "");
}

inline std::string get_compiled_name(std::string name)
{
    return "custom_tester_" + name + ".out";
}

inline std::string get_build_command(std::string name)
{
    return "g++ " + build_flags + " " + get_path(name + ".cpp") + " -o " + get_path(get_compiled_name(name), true);
}

int MainWindow::_system(std::string programpath, std::string inputpath = "", std::string outputpath = "")
{
    int x;
    if (!inputpath.empty() || !outputpath.empty())
    {
        std::string command = programpath;
        if (!inputpath.empty()) command += " < " + inputpath;
        if (!outputpath.empty()) command += " > " + outputpath;
        command += " 2> " + get_path("log.txt", true);
        myprocess->start("bash", QStringList() << "-c" << QString(command.data()));
        myprocess->waitForFinished(time_out);
        QString errorString = myprocess->errorString();
        if (myprocess->exitStatus() == QProcess::CrashExit) errorString = "The process crashed", x = -1;
        else x = myprocess->exitCode();
        if (errorString.toStdString() == "Process operation timed out") x = -1;
        if (x)
        {
            QFile log(QString(get_path("log.txt", true, false).data()));
            log.open(QIODevice::ReadOnly | QIODevice::Text);
            QByteArray log_text = log.readAll();
            log.close();
            log.open(QIODevice::WriteOnly | QIODevice::Text);
            log.write(myprocess->readAllStandardError());
            log.write("\n\n");
            log.write(log_text);
            log.write("\n");
            log.write(errorString.toStdString().data());
            log.write("\n");
            log.write(("Exit Code: " + std::to_string(x)).data());
            log.write("\n");
            log.close();
        }
        myprocess->terminate();
    }
    else
    {
        programpath += " 2> " + get_path("log.txt", true);
        x = system(programpath.data());
    }

    if (x/* && (s.substr(0, 3) == "g++" || run_alive)*/)
    {
        if (programpath.substr(0, 3) == "cmp") on_view_diff_pushButton_clicked();
        else system(("subl " + get_path("log.txt", true)).data());
    }
    return x;
}

void MainWindow::on_generator_pushButton_clicked()
{
    system(("subl " + get_path(generator_name + ".cpp")).data());
}

void MainWindow::on_solution_pushButton_clicked()
{
    system(("subl " + get_path(solution_name + ".cpp")).data());
}

void MainWindow::on_brutalsol_pushButton_clicked()
{
    system(("subl " + get_path(brutalsol_name + ".cpp")).data());
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
        if (ok && _system(get_build_command(generator_name))) ok = false;
        ui->progressBar->setValue(ui->progressBar->value()+1);
    }

    if (ui->solution_checkBox->isChecked())
    {
        if (ok && _system(get_build_command(solution_name))) ok = false;
        ui->progressBar->setValue(ui->progressBar->value()+1);
    }

    if (ui->brutalsol_checkBox->isChecked())
    {
        if (ok && _system(get_build_command(brutalsol_name))) ok = false;
        ui->progressBar->setValue(ui->progressBar->value()+1);
    }

    ui->progressBar->hide();
}

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
        ui->curtest_label->repaint();

        if (_system(get_path(get_compiled_name(generator_name), true), "", get_path(input_fullname, true))) break;

        timepoint curtime = get_time();
        if (_system(get_path(get_compiled_name(solution_name), true), get_path(input_fullname, true), get_path(solution_output_fullname, true))) break;
        if (ui->show_time_checkBox->isChecked()) ui->solution_time_label->setText(get_elapsed(curtime));

        curtime = get_time();
        if (_system(get_path(get_compiled_name(brutalsol_name), true), get_path(input_fullname, true), get_path(brutalsol_output_fullname, true))) break;
        if (ui->show_time_checkBox->isChecked()) ui->brutalsol_time_label->setText(get_elapsed(curtime));

        if (_system("cmp " + get_path(solution_output_fullname, true) + " " + get_path(brutalsol_output_fullname, true))) break;

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
    ui->timelimit_spinBox->setEnabled(true);
}

void MainWindow::on_run_pushButton_clicked()
{
    if (!run_alive)
    {
        ui->timelimit_spinBox->setEnabled(false);
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
    system(("subl " + get_path(input_fullname, true)).data());
    system(("subl -n " + get_path(solution_output_fullname, true) + " " + get_path(brutalsol_output_fullname, true)
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

void MainWindow::on_timelimit_spinBox_valueChanged(int arg1)
{
    time_out = arg1;
}
