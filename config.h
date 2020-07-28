#include <string>
#include <QDir>

#ifndef CONFIG_H
#define CONFIG_H

const std::string conf_file_path = QDir::home().filePath("custom_tester.cfg").toStdString();

std::string workspace_path;
std::string workspace_data_path;
std::string build_flags;
std::string open_editor_command;
std::string open_diff_editor_command;

const std::string solution_name = "solution";
const std::string brutalsol_name = "brutalsol";
const std::string generator_name = "generator";
const std::string input_fullname = "input.in";
const std::string solution_output_fullname = "solution.out";
const std::string brutalsol_output_fullname = "brutalsol.out";

int tests = 1;
bool run_alive = false;
int time_out = 5000;

#endif // CONFIG_H
