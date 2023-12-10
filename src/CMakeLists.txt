add_library(data_collection data_collection.c)
add_library(load_profile load_profile.c)
add_library(questionnaire questionnaire.c)

target_include_directories(data_collection PUBLIC .)
target_include_directories(load_profile PUBLIC .)
target_include_directories(questionnaire PUBLIC .)

add_executable(STUDYMAP STUDYMAP.c KNN.c
        Results.c
        Results.h)

target_link_libraries(questionnaire data_collection load_profile)
target_link_libraries(STUDYMAP data_collection questionnaire load_profile m)
