cmake_minimum_required(VERSION 3.20)

add_executable(http_server 
	main.cpp
	http_connection.cpp
)
add_library(searcher
	searcher/searcher.cpp
)

find_package(OpenSSL REQUIRED)
target_compile_features(http_server PRIVATE cxx_std_17) 
target_include_directories(http_server PRIVATE 
	${Boost_INCLUDE_DIRS}
)

target_link_libraries(http_server PUBLIC 
	searcher
	pqxx
	OpenSSL::SSL
	${Boost_LIBRARIES}
)
