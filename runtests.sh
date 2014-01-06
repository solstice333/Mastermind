#!/bin/bash

./a.out < test1.in > test1_kn.out
./a.out < test2.in > test2_kn.out
./a.out < test3.in > test3_kn.out
./a.out < test4.in > test4_kn.out
./a.out < test5.in > test5_kn.out
./a.out < test6.in > test6_kn.out
./a.out < test7.in > test7_kn.out

echo -e "\n< your output"
echo -e "\n> expected output\n"

echo -e "\nTEST1\n"
diff -bw test1_kn.out test1.out
echo -e "\nTEST2\n"
diff -bw test2_kn.out test2.out
echo -e "\nTEST3\n"
diff -bw test3_kn.out test3.out
echo -e "\nTEST4\n"
diff -bw test4_kn.out test4.out
echo -e "\nTEST5\n"
diff -bw test5_kn.out test5.out
echo -e "\nTEST6\n"
diff -bw test6_kn.out test6.out
echo -e "\nTEST7\n"
diff -bw test7_kn.out test7.out
