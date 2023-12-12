add_library(Registration Registration.c)
add_library(load_profile Main_menu.c)
add_library(questionnaire Questionnaire.c)
add_library(KNN KNN.c)
add_library(Results Results.c)

target_include_directories(Registration PUBLIC .)
target_include_directories(load_profile PUBLIC .)
target_include_directories(questionnaire PUBLIC .)
target_include_directories(KNN PUBLIC .)
target_include_directories(Results PUBLIC .)

add_executable(STUDYMAP STUDYMAP.c)

set_target_properties(STUDYMAP PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

target_link_libraries(questionnaire Registration load_profile KNN Results)
target_link_libraries(STUDYMAP Registration questionnaire load_profile KNN Results m)
