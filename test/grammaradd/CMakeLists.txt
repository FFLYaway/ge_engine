add_executable(gtest
        StringDiffEvaluator.cpp
        StringDiffEvaluator.h
        evolution_test.cpp
        test.cpp
        )

target_include_directories(gtest PRIVATE
        ${gram_SOURCE_DIR}/include
        ${catch_INCLUDE_DIR}
        ${fakeit_INCLUDE_DIR}
        )

target_link_libraries(gtest gram)
