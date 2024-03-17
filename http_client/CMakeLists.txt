cmake_minimum_required(VERSION 3.20)
	
add_executable(http_client
	main.cpp
)

add_library(html_parser 
	html_parser/html_parser.cpp
)

add_library(http_utils
	http_utils/http_utils.cpp
)

add_library(links_getter 
	links_getter/links_getter.cpp
)

target_include_directories(http_client PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

find_package(OpenSSL REQUIRED)
target_compile_features(http_client PRIVATE cxx_std_17) 
target_include_directories(http_client PRIVATE 
	${Boost_INCLUDE_DIRS}
)

target_link_libraries(http_client PUBLIC 
	http_utils
	html_parser
	links_getter
	pqxx
	OpenSSL::SSL
	${Boost_LIBRARIES}
)
