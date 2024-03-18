cmake_minimum_required(VERSION 3.20)
	
add_executable(http_client
	main.cpp
)
add_library(indexer 
	indexer/indexer.cpp
)

add_library(html_parser 
	indexer/html_parser/html_parser.cpp
)

add_library(http_utils
	utils/http_utils.cpp
)

add_library(links_getter 
	indexer/links_getter/links_getter.cpp
)

add_library(sql_database 
	indexer/sql_database/sql_database.cpp
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
	indexer
	http_utils
	html_parser
	links_getter
	sql_database
	pqxx
	OpenSSL::SSL
	${Boost_LIBRARIES}
)
