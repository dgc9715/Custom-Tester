#include <string>

#ifndef CONFIG_H
#define CONFIG_H

// Change the path to workspace and build flags here

const std::string workspace_path = "/media/dgc/DATOS/Programing/custom tester/linux gui/";
const std::string build_flags = "-std=c++14 -O2 -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC";

// Don't change anything else

const std::string workspace_data_path = workspace_path + "data/";
const std::string solution_name = "solution";
const std::string brutalsol_name = "brutalsol";
const std::string generator_name = "generator";
const std::string input_fullname = "input.in";
const std::string solution_output_fullname = "solution.out";
const std::string brutalsol_output_fullname = "brutalsol.out";
int tests = 1;
bool run_alive = false;
const int time_out = 5000;

#endif // CONFIG_H
