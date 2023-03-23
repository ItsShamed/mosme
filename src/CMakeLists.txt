include_directories(${mosme_SOURCE_DIR}/include)

set(CPM_SOURCE_CACHE)
set(CPM_USE_LOCAL_PACKAGES)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets)

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(mosme 
            MANUAL_FINALIZATION
            main.cpp ui/loginwindow.cpp ui/loginwindow.h ui/loginwindow.ui MosmeApp.cpp MosmeApp.h
            ui/ModelBackedWidget.cpp ui/ModelBackedWidget.h ui/mainwindow.cpp ui/mainwindow.h ui/mainwindow.ui ConfigStorage.cpp ConfigStorage.h)
else()
    add_executable(mosme main.cpp ui/loginwindow.cpp ui/loginwindow.h ui/loginwindow.ui MosmeApp.cpp MosmeApp.h
            ui/ModelBackedWidget.cpp ui/ModelBackedWidget.h ui/mainwindow.cpp ui/mainwindow.h ui/mainwindow.ui ConfigStorage.cpp ConfigStorage.h)
endif ()
target_compile_features(mosme PRIVATE cxx_attribute_deprecated)

CPMAddPackage("gh:fmtlib/fmt#9.1.0")
CPMAddPackage("gh:nlohmann/json@3.10.5")

target_link_libraries(mosme
        PRIVATE
        fmt::fmt
        nlohmann_json::nlohmann_json
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::QWindowsIntegrationPlugin
        )

if (WIN32)
    #    target_link_libraries(mosme Qt6::QWindowsIntegrationPlugin)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/styles/qwindowsvistastyle.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/styles/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/styles/qwindowsvistastyle.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/styles/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(mosme)
endif ()
