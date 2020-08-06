# CMake generated Testfile for 
# Source directory: D:/LBM_CPP/tests
# Build directory: D:/LBM_CPP/cmake-build-debug/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(run_tests "run_tests")
set_tests_properties(run_tests PROPERTIES  _BACKTRACE_TRIPLES "D:/LBM_CPP/tests/CMakeLists.txt;17;add_test;D:/LBM_CPP/tests/CMakeLists.txt;0;")
subdirs("../googletest-build")
