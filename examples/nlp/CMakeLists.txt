include(helpers)
make_example(music)
target_link_libraries(music ruff::nlp)

make_example(ComedyIan)
target_link_libraries(ComedyIan ruff::nlp)
