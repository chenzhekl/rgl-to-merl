add_executable(main main.cpp brdf_loader.cpp merl.cpp merl.h)

target_include_directories(main PUBLIC ${CMAKE_SOURCE_DIR}/third_party/brdf-loader)
