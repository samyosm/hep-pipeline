add_library(hepp_utils STATIC
    src/utils.cpp
    src/config.cc
)

target_include_directories(hepp_utils PUBLIC include)
