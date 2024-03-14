cmake_minimum_required(VERSION 3.20)

add_executable(HttpClientApp
	main.cpp
	http_utils.cpp
)

find_package(OpenSSL REQUIRED)
target_compile_features(HttpClientApp PRIVATE cxx_std_17) 
target_include_directories(HttpClientApp PRIVATE 
	${Boost_INCLUDE_DIRS}
)

target_link_libraries(HttpClientApp PUBLIC 
	pqxx
	OpenSSL::SSL
	${Boost_LIBRARIES}
)


