for /d /r %%i in (*) do (
    pushd %%i
    ren *.cpp *.c
    popd
)