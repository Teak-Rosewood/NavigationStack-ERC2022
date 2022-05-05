# CMake generated Testfile for 
# Source directory: /home/blank/european_rover_challenge/navigation/src/simulation/leo_erc_common/leo_erc_description
# Build directory: /home/blank/european_rover_challenge/navigation/build/leo_erc_description
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(_ctest_leo_erc_description_rostest_test_xacro.test "/home/blank/european_rover_challenge/navigation/build/leo_erc_description/catkin_generated/env_cached.sh" "/usr/bin/python3" "/opt/ros/noetic/share/catkin/cmake/test/run_tests.py" "/home/blank/european_rover_challenge/navigation/build/leo_erc_description/test_results/leo_erc_description/rostest-test_xacro.xml" "--return-code" "/usr/bin/python3 /opt/ros/noetic/share/rostest/cmake/../../../bin/rostest --pkgdir=/home/blank/european_rover_challenge/navigation/src/simulation/leo_erc_common/leo_erc_description --package=leo_erc_description --results-filename test_xacro.xml --results-base-dir \"/home/blank/european_rover_challenge/navigation/build/leo_erc_description/test_results\" /home/blank/european_rover_challenge/navigation/src/simulation/leo_erc_common/leo_erc_description/test/xacro.test ")
set_tests_properties(_ctest_leo_erc_description_rostest_test_xacro.test PROPERTIES  _BACKTRACE_TRIPLES "/opt/ros/noetic/share/catkin/cmake/test/tests.cmake;160;add_test;/opt/ros/noetic/share/rostest/cmake/rostest-extras.cmake;52;catkin_run_tests_target;/home/blank/european_rover_challenge/navigation/src/simulation/leo_erc_common/leo_erc_description/CMakeLists.txt;19;add_rostest;/home/blank/european_rover_challenge/navigation/src/simulation/leo_erc_common/leo_erc_description/CMakeLists.txt;0;")
subdirs("gtest")
