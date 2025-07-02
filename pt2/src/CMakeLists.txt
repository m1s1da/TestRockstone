add_library(toptracker STATIC
        TopTracker.cpp
)

target_include_directories(toptracker PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_features(toptracker PUBLIC cxx_std_23)

find_package(Boost REQUIRED COMPONENTS system)

target_link_libraries(toptracker PUBLIC Boost::system)
