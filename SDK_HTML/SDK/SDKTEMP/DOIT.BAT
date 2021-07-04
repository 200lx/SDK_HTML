@ECHO OFF
rem *************************************************
rem CHANGE PATH BELOW ACCORDING TO YOUR INSTALLATION:
rem *************************************************
A:
CD\
CD A:\SDK
rem *************************************************
if exist grepit.bat  del grepit.bat
if exist grepall.htm del grepall.htm
if exist grepall.lst del grepall.lst
PICKIT.EXE
if errorlevel 128 goto posreslt
:negreslt
ESC.COM
goto end
:posreslt
rem *************************************************
rem IF YOU USE EXKEY AND IT IS IN YOUR PATH, USE: 
rem *************************************************
rem EXKEY -X{ESC}{WAIT}{F9}GREPALL.HTM{ENTER}
rem INSTEAD OF:
ESC.COM
rem *************************************************
:end


