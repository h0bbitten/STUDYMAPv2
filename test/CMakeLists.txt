
add_executable(Knn_test KNN_test.c)
target_link_libraries(Knn_test KNN m)

add_test(NAME Knn_test COMMAND Knn_test)