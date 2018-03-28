@echo off
title batch run queries

..\AutoTester.exe source_nested.txt queries_nested_Follows_Star.txt out_nested_Follows_Star.xml
..\AutoTester.exe source_nested.txt queries_nested_Follows.txt out_nested_Follows.xml

..\AutoTester.exe source_nested.txt queries_nested_Modifies.txt out_nested_Modifies.xml

..\AutoTester.exe source_nested.txt queries_nested_Parent_Star.txt out_nested_Parent_Star.xml
..\AutoTester.exe source_nested.txt queries_nested_Parent.txt out_nested_Parent.xml

..\AutoTester.exe source_nested.txt queries_nested_Pattern.txt out_nested_Pattern.xml

..\AutoTester.exe source_nested.txt queries_nested_Select_All.txt out_nested_Select_All.xml

..\AutoTester.exe source_nested.txt queries_nested_Uses.txt out_nested_Uses.xml

..\AutoTester.exe source_nested.txt queries_nested_Common_Zero.txt out_nested_Common_Zero.xml
..\AutoTester.exe source_nested.txt queries_nested_Common_One.txt out_nested_Common_One.xml
..\AutoTester.exe source_nested.txt queries_nested_Common_Two.txt out_nested_Common_Two.xml

echo.
echo done
pause