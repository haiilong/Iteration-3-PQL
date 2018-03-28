@echo off
title batch run queries

..\AutoTester.exe source_1a.txt queries_1a_Select_All.txt out_1a_Select_All.xml

..\AutoTester.exe source_1a.txt queries_1a_Parent.txt out_1a_Parent.xml
..\AutoTester.exe source_1a.txt queries_1a_Parent_Star.txt out_1a_Parent_Star.xml

..\AutoTester.exe source_1a.txt queries_1a_Follows.txt out_1a_Follows.xml
..\AutoTester.exe source_1a.txt queries_1a_Follows_Star.txt out_1a_Follows_Star.xml

..\AutoTester.exe source_1a.txt queries_1a_Pattern.txt out_1a_Pattern.xml

..\AutoTester.exe source_1a.txt queries_1a_Modifies.txt out_1a_Modifies.xml

..\AutoTester.exe source_1a.txt queries_1a_Uses.txt out_1a_Uses.xml

..\AutoTester.exe source_1a.txt queries_1a_Common_Zero.txt out_1a_Common_Zero.xml
..\AutoTester.exe source_1a.txt queries_1a_Common_One.txt out_1a_Common_One.xml
..\AutoTester.exe source_1a.txt queries_1a_Common_Two.txt out_1a_Common_Two.xml

echo.
echo done
pause