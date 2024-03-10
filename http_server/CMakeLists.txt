cmake_minimum_required(VERSION 3.20)


add_executable(HttpServerApp 
	main.cpp
	http_connection.cpp
)

find_package(OpenSSL REQUIRED)
target_compile_features(HttpServerApp PRIVATE cxx_std_17) 
target_include_directories(HttpServerApp PRIVATE 
	${Boost_INCLUDE_DIRS}
)

target_link_libraries(HttpServerApp PUBLIC 
	pqxx
	OpenSSL::SSL
	${Boost_LIBRARIES}
)
