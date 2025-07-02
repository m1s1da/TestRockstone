enable_testing()

find_package(GTest REQUIRED)

find_package(Boost REQUIRED COMPONENTS system)

add_executable(TopTrackerTest
        TopTrackerTest.cpp
)

target_link_libraries(TopTrackerTest
        PRIVATE
        toptracker
        GTest::gtest
        GTest::gtest_main
        Boost::system
)

if (UNIX)
    target_link_libraries(TopTrackerTest PRIVATE pthread)
endif()

add_test(NAME TopTrackerTest COMMAND TopTrackerTest)
