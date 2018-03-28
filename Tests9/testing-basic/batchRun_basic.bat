@echo off
title batch run queries

..\AutoTester.exe source_basic.txt queries_basic_Select_All.txt out_basic_Select_All.xml

..\AutoTester.exe source_basic.txt queries_basic_Follows.txt out_basic_Follows.xml
..\AutoTester.exe source_basic.txt queries_basic_Follows_Star.txt out_basic_Follows_Star.xml

..\AutoTester.exe source_basic.txt queries_basic_Parent.txt out_basic_Parent.xml
..\AutoTester.exe source_basic.txt queries_basic_Parent_Star.txt out_basic_Parent_Star.xml

..\AutoTester.exe source_basic.txt queries_basic_Modifies.txt out_basic_Modifies.xml

..\AutoTester.exe source_basic.txt queries_basic_Uses.txt out_basic_Uses.xml

..\AutoTester.exe source_basic.txt queries_basic_Pattern.txt out_basic_Pattern.xml

..\AutoTester.exe source_basic.txt queries_basic_Common_Zero.txt out_basic_Common_Zero.xml
..\AutoTester.exe source_basic.txt queries_basic_Common_One.txt out_basic_Common_One.xml
..\AutoTester.exe source_basic.txt queries_basic_Common_Two.txt out_basic_Common_Two.xml

echo.
echo done
pause