file(GLOB_RECURSE ServerREST_SRCS *.cpp *.h)

set(ServerREST_SRCS
    ${ServerREST_SRCS}
    ${TEMPLATES_SRC}
)

# Create the application
add_library(ServerREST SHARED ${ServerREST_SRCS})

# Link to Cutelyst
target_link_libraries(ServerREST
    Cutelyst::Core
    Qt5::Core
    Qt5::Network
)

