# Custom-Tester

Create custom_tester.cfg in home directory with four lines:

Line 1: Workspace folder direction
Line 2: Build flags
Line 3: Command to open text file ($FILE)
Line 4: Command to compare text files ($FILE1 and $FILE2)

Example of my configuration:

/media/dgc/DATOS/Programing/custom tester/linux gui/
-std=c++14 -O2 -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC
subl $FILE
subl -n $FILE1 $FILE2 --command 'sublimerge_diff_views {"left_read_only": true, "right_read_only": true}'

I used www.sublimetext.com (subl) with the package https://www.sublimerge.com/sm3/ (sublimerge_diff_views)
