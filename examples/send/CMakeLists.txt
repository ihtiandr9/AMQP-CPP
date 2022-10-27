

###################################
# Libev_send
###################################

add_executable(amqpcpp_libev_send sendlibev.cpp)

add_dependencies(amqpcpp_libev_send amqpcpp)

target_link_libraries(amqpcpp_libev_send amqpcpp ev pthread dl ssl)

###################################
# Libevent_send
###################################

add_executable(amqpcpp_libevent_send sendlibevent.cpp)

add_dependencies(amqpcpp_libevent_send amqpcpp)

target_link_libraries(amqpcpp_libevent_send amqpcpp event pthread dl ssl)