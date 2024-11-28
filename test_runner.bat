@echo off

:: Set variables
set EXE_PATH=x64/Debug/3P71-A2.exe
set DATA_PATH=3P71-A2/data/t1
set OUTPUT_DIR=results
set OUTPUT_FILE=combined_results.txt


set TRIAL=1

start %EXE_PATH% -a %DATA_PATH% 1.00 0.00 %TRIAL%
pause

:: Loop for 5 trials
for /L %%i in (1,1,5) do (
    echo Starting trial %%i...

    start %EXE_PATH% -a %DATA_PATH% 1.00 0.00 %TRIAL%
    set /a TRIAL=TRIAL+1

    start %EXE_PATH% -a %DATA_PATH% 1.00 0.10 %TRIAL%
    set /a TRIAL=TRIAL+1

    start %EXE_PATH% -a %DATA_PATH% 0.90 0.00 %TRIAL%
    set /a TRIAL=TRIAL+1

    start /wait %EXE_PATH% -a %DATA_PATH% 0.90 0.10 %TRIAL%
    set /a TRIAL=TRIAL+1
)

:: Combine all result files into one file
echo Combining result files into %OUTPUT_DIR%/%OUTPUT_FILE%...

for %%f in (%OUTPUT_DIR%/results*.txt) do (
    type "%%f" >> %OUTPUT_DIR%/%OUTPUT_FILE%
)

echo All results combined into %OUTPUT_DIR%/%OUTPUT_FILE%.
pause
