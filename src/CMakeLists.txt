
add_library(data_collection data_collection.c)

target_include_directories(data_collection PUBLIC .)

add_executable(STUDYMAP STUDYMAP.c)

target_link_libraries(STUDYMAP data_collection)