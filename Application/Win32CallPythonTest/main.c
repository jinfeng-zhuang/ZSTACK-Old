#include <stdio.h>
#include <Python.h>

char *python_code =
"\
x = 5\n\
x = x + 10\n\
print('x = ', x)\
";

int main(int argc, char *argv[])
{
    Py_Initialize();

    PyRun_SimpleString(python_code);

    Py_Finalize();

    return 0;
}