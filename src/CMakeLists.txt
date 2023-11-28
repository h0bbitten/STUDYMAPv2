
add_library(data_collection data_collection.c)
add_library(questionnaire questionnaire.c)

target_include_directories(data_collection PUBLIC .)
target_include_directories(questionnaire PUBLIC .)

add_executable(STUDYMAP STUDYMAP.c KNN.c
        KNN.c)


target_link_libraries(questionnaire data_collection)
target_link_libraries(STUDYMAP data_collection questionnaire m)