add_executable(${ProjectName}
    main.cpp
        LED.cpp
        PicoOsUart.cpp
        Timers.cpp
        Timers.h
        UartHandler.cpp
        UartHandler.h
)

target_include_directories(${ProjectName} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc
)

target_link_libraries(${ProjectName}
       # hardware_uart
    pico_stdlib 
    FreeRTOS-Kernel-Heap4
    )

pico_add_extra_outputs(${ProjectName})

# Disable usb output, enable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)
