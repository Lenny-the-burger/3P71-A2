:: Set the file path to the executable and data directory
set EXE_PATH=x64/Debug/3P71-A2.exe
set DATA_PATH=3P71-A2/data/t1

:: Loop for 5 iterations
for /L %%i in (1,1,5) do (
    echo Running iteration %%i...

    :: Run the tests with different settings
    start /wait %EXE_PATH% -a %DATA_PATH% 1.00 0.00 %%i
    start /wait %EXE_PATH% -a %DATA_PATH% 1.00 0.10 %%i
    start /wait %EXE_PATH% -a %DATA_PATH% 0.90 0.00 %%i
    start /wait %EXE_PATH% -a %DATA_PATH% 0.90 0.10 %%i
)

echo All tests completed.
pause
