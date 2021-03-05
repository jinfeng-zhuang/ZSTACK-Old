for /r /d %%i in (*) do (
    pushd %%i
    ren *.c *.cpp
    popd
)

