@echo off
title batch run queries

..\AutoTester.exe source_nested.txt queries_nested_Follows_Star.txt out_nested_Follows_Star.xml > out_nested_Follows_Star.txt
..\AutoTester.exe source_nested.txt queries_nested_Follows.txt out_nested_Follows.xml > out_nested_Follows.txt

..\AutoTester.exe source_nested.txt queries_nested_Modifies.txt out_nested_Modifies.xml > out_nested_Modifies.txt

..\AutoTester.exe source_nested.txt queries_nested_Parent_Star.txt out_nested_Parent_Star.xml > out_nested_Parent_Star.txt
..\AutoTester.exe source_nested.txt queries_nested_Parent.txt out_nested_Parent.xml > out_nested_Parent.txt

..\AutoTester.exe source_nested.txt queries_nested_Pattern.txt out_nested_Pattern.xml > out_nested_Pattern.txt

..\AutoTester.exe source_nested.txt queries_nested_select_all.txt out_nested_Select_All.xml > out_nested_Select_All.txt

..\AutoTester.exe source_nested.txt queries_nested_Uses.txt out_nested_Uses.xml > out_nested_Uses.txt

..\AutoTester.exe source_nested.txt queries_nested_Common_Zero.txt out_nested_Common_Zero.xml > out_nested_Common_Zero.txt
..\AutoTester.exe source_nested.txt queries_nested_Common_One.txt out_nested_Common_One.xml > out_nested_Common_One.txt
..\AutoTester.exe source_nested.txt queries_nested_Common_Two.txt out_nested_Common_Two.xml > out_nested_Common_Two.txt

echo.
echo done
pause