add_library(data_collection data_collection.c)
add_library(load_profile load_profile.c)
add_library(questionnaire questionnaire.c)
add_library(KNN KNN.c)
add_library(Results Results.c)


target_include_directories(data_collection PUBLIC .)
target_include_directories(load_profile PUBLIC .)
target_include_directories(questionnaire PUBLIC .)
target_include_directories(KNN PUBLIC .)
target_include_directories(Results PUBLIC .)

add_executable(STUDYMAP STUDYMAP.c)

set_target_properties(STUDYMAP PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

target_link_libraries(questionnaire data_collection load_profile KNN Results)
target_link_libraries(STUDYMAP data_collection questionnaire load_profile KNN Results m)
