@echo off
title batch run queries

..\AutoTester.exe source_basic.txt queries_basic_Select_All.txt out_basic_Select_All.xml > out_basic_Select_All.txt

..\AutoTester.exe source_basic.txt queries_basic_Follows.txt out_basic_Follows.xml > out_basic_Follows.txt
..\AutoTester.exe source_basic.txt queries_basic_Follows_Star.txt out_basic_Follows_Star.xml > out_basic_Follows_Star.txt

..\AutoTester.exe source_basic.txt queries_basic_Parent.txt out_basic_Parent.xml > out_basic_Parent.txt
..\AutoTester.exe source_basic.txt queries_basic_Parent_Star.txt out_basic_Parent_Star.xml > out_basic_Parent_Star.txt

..\AutoTester.exe source_basic.txt queries_basic_Modifies.txt out_basic_Modifies.xml > out_basic_Modifies.txt

..\AutoTester.exe source_basic.txt queries_basic_Uses.txt out_basic_Uses.xml > out_basic_Uses.txt

..\AutoTester.exe source_basic.txt queries_basic_Pattern.txt out_basic_Pattern.xml > out_basic_Pattern.txt

..\AutoTester.exe source_basic.txt queries_basic_Common_Zero.txt out_basic_Common_Zero.xml > out_basic_Common_Zero.txt
..\AutoTester.exe source_basic.txt queries_basic_Common_One.txt out_basic_Common_One.xml > out_basic_Common_One.txt
..\AutoTester.exe source_basic.txt queries_basic_Common_Two.txt out_basic_Common_Two.xml > out_basic_Common_Two.txt

echo.
echo done
pause