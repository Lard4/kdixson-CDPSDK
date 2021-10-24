set(FILES
        CDPError.h
        CDPInterface.h
        CDPPacket.h
        Duck.h
        GPS.h
        log.h
        minmea.h
)

target_sources(${PROJECT_NAME} PRIVATE ${FILES})