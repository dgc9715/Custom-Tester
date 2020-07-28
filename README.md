# Custom-Tester

![Image of Yaktocat](https://github.com/dgc9715/Custom-Tester/blob/master/custom-tester.png)

### Getting started

Create **custom_tester.cfg** in home directory with four lines:

1. Workspace folder direction
1. Build flags
1. Command to open text file ($FILE)
1. Command to compare text files ($FILE1 and $FILE2)

Example of my configuration:

```
/media/dgc/DATOS/Programing/custom tester/linux gui/
-std=c++14 -O2 -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC
subl $FILE
subl -n $FILE1 $FILE2 --command 'sublimerge_diff_views {"left_read_only": true, "right_read_only": true}'
```

I used www.sublimetext.com (subl) with the package https://www.sublimerge.com/sm3/ (sublimerge_diff_views)
