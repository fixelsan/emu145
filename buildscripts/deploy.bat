set DEPLOYDIR=pmkemu
set QT4SDK=C:\QtSDK
set QT4SDKM=c:/QtSDK
set MINGW=c:\MinGW


set REALBUILDDISK=s:
set BDISK=m:
set BDIR=emu145-build
set MINGWDIR=%BDISK%/%BDIR%/pmkemu


PATH=%PATH%;c:\bin

pushd

%REALBUILDDISK%
mkdir emu145-build-dir
subst %BDISK% emu145-build-dir

%BDISK%
cd \
del /q/s %BDIR%
svn checkout http://emu145.googlecode.com/svn/trunk/ %BDIR%
cd %BDIR%\pmkemu


echo cd %MINGWDIR% >build.sh
echo %QT4SDKM%/Desktop/Qt/4.8.0/mingw/bin/qmake.exe >>build.sh
echo make -f Makefile.Release >>build.sh


%MINGW%\msys\1.0\bin\sh --login -i %MINGWDIR%/build.sh

del /q /s %DEPLOYDIR%
mkdir %DEPLOYDIR%
copy /b release\pmkemu.exe %DEPLOYDIR%
copy /b "%MINGW%\bin\libstdc++-6.dll" %DEPLOYDIR%
copy /b "%QT4SDK%\Desktop\Qt\4.8.0\mingw\bin\libgcc_s_dw2-1.dll" %DEPLOYDIR%
copy /b "%QT4SDK%\Desktop\Qt\4.8.0\mingw\bin\QtCore4.dll" %DEPLOYDIR%
copy /b "%QT4SDK%\Desktop\Qt\4.8.0\mingw\bin\mingwm10.dll" %DEPLOYDIR%
copy /b "%QT4SDK%\Desktop\Qt\4.8.0\mingw\bin\QtGui4.dll" %DEPLOYDIR%


for /f "tokens=1,2,3,4 delims=/ " %%a in ("%date%") do set wday=%%a&set month=%%b&set day=%%c&set year=%%d
for /f "tokens=1,2,3,4 delims=:. " %%a in ("%time%") do set t1=%%a&set t2=%%b&set t3=%%c&set t4=%%d

set ZIPNAME=pmkemu_snapshot_win32_%year%%month%%day%_%t1%%t2%.zip

zip %ZIPNAME% -r %DEPLOYDIR%

..\buildscripts\googlecode_upload.py -s "win32 snapshot build" -p emu145 %ZIPNAME%


popd
subst /d %BDISK%