cmake_minimum_required(VERSION 3.10)

project(DirectoryFounder)

set(CMAKE_CXX_STANDARD 17)

add_executable(server Server/Server.cpp mainServer.cpp)
add_executable(client PathFounder/PathFounder.cpp Client/Client.cpp mainClient.cpp)

target_link_options(server PUBLIC -pthread)
target_link_options(client PUBLIC -pthread)

add_custom_target(server_run COMMAND server)
add_custom_target(client_run COMMAND client)
add_custom_target(run COMMAND chmod +x ../run.sh && bash ../run.sh)

add_custom_target(clear COMMAND rm -rf client server )