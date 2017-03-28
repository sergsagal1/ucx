AC_ARG_WITH([cuda],
            [AC_HELP_STRING([--with-cuda], [Compile with CUDA support (default is no)])],
            [],
            [with_cuda=no])

AM_CONDITIONAL([HAVE_CUDA],   [test "x$with_cuda" != "xno"])
