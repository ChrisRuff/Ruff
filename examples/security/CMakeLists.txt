include(helpers)

make_example(rsa_runner)
target_link_libraries(rsa_runner ruff::security)

make_example(crt_timer)
target_link_libraries(crt_timer ruff::security)

make_example(rsa_aes_communication)
target_link_libraries(rsa_aes_communication ruff::security)

make_example(dh_dsa_communication)
target_link_libraries(dh_dsa_communication ruff::security)

make_example(dsa_runner)
target_link_libraries(dsa_runner ruff::security)