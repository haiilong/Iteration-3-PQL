@echo off
title batch run queries

..\AutoTester.exe source_1a.txt queries_1a_Select_All.txt out_1a_Select_All.xml > out_1a_Select_All.txt

..\AutoTester.exe source_1a.txt queries_1a_Parent.txt out_1a_Parent.xml > out_1a_Parent.txt
..\AutoTester.exe source_1a.txt queries_1a_Parent_Star.txt out_1a_Parent_Star.xml > out_1a_Parent_Star.txt

..\AutoTester.exe source_1a.txt queries_1a_Follows.txt out_1a_Follows.xml > out_1a_Follows.txt
..\AutoTester.exe source_1a.txt queries_1a_Follows_Star.txt out_1a_Follows_Star.xml > out_1a_Follows_Star.txt

..\AutoTester.exe source_1a.txt queries_1a_Pattern.txt out_1a_Pattern.xml > out_1a_Pattern.txt

..\AutoTester.exe source_1a.txt queries_1a_Modifies.txt out_1a_Modifies.xml > out_1a_Modifies.txt

..\AutoTester.exe source_1a.txt queries_1a_Uses.txt out_1a_Uses.xml > out_1a_Uses.txt

..\AutoTester.exe source_1a.txt queries_1a_Common_Zero.txt out_1a_Common_Zero.xml > out_1a_Common_Zero.txt
..\AutoTester.exe source_1a.txt queries_1a_Common_One.txt out_1a_Common_One.xml > out_1a_Common_One.txt
..\AutoTester.exe source_1a.txt queries_1a_Common_Two.txt out_1a_Common_Two.xml > out_1a_Common_Two.txt

echo.
echo done
pause