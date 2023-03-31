include_directories(${mosme_SOURCE_DIR}/include/verdigris)

set(CPM_SOURCE_CACHE)
set(CPM_USE_LOCAL_PACKAGES)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(ENV{QT_MESSAGE_PATTERN} "[%{time process}] %{threadid} file:/%{file}:%{line}: %{message} 
%{if-critical}%{backtrace}%{endif}")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets Network Concurrent)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Network Concurrent)

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(mosme 
            MANUAL_FINALIZATION
            main.cpp ui/loginwindow.cpp ui/loginwindow.h ui/loginwindow.ui MosmeApp.cpp MosmeApp.h
            ui/ModelBackedWidget.cpp ui/ModelBackedWidget.h ui/mainwindow.cpp ui/mainwindow.h ui/mainwindow.ui ConfigStorage.cpp ConfigStorage.h network/API/APIRequest.cpp network/API/APIRequest.h network/API/ICanPerform.cpp network/API/ICanPerform.h network/API/APIAccess.cpp network/API/APIAccess.h network/API/IAPIProvider.h network/API/APIRequestCompletionState.h exceptions/NotSupportedException.cpp exceptions/NotSupportedException.h network/API/APIState.h network/API/ReplyingAPIRequest.cpp network/API/ReplyingAPIRequest.h network/API/requests/GetStatusRequest.cpp network/API/requests/GetStatusRequest.h network/User.h network/HostCustomisedProfile.h network/HostProfile.h network/API/requests/responses/GetStatusResponse.h network/API/requests/PostAuthSignInRequest.cpp network/API/requests/PostAuthSignInRequest.h json/JsonUtils.h network/API/requests/GetUserMeRequest.cpp network/API/requests/GetUserMeRequest.h json/ConversionImpl.cpp network/API/APILoopThread.cpp network/API/APILoopThread.h)
else()
    add_executable(mosme main.cpp ui/loginwindow.cpp ui/loginwindow.h ui/loginwindow.ui MosmeApp.cpp MosmeApp.h
            ui/ModelBackedWidget.cpp ui/ModelBackedWidget.h ui/mainwindow.cpp ui/mainwindow.h ui/mainwindow.ui ConfigStorage.cpp ConfigStorage.h network/API/APIRequest.cpp network/API/APIRequest.h network/API/ICanPerform.cpp network/API/ICanPerform.h network/API/APIAccess.cpp network/API/APIAccess.h network/API/IAPIProvider.h network/API/APIRequestCompletionState.h exceptions/NotSupportedException.cpp exceptions/NotSupportedException.h network/API/APIState.h network/API/ReplyingAPIRequest.cpp network/API/ReplyingAPIRequest.h network/API/requests/GetStatusRequest.cpp network/API/requests/GetStatusRequest.h network/User.h network/HostCustomisedProfile.h network/HostProfile.h network/API/requests/responses/GetStatusResponse.h network/API/requests/PostAuthSignInRequest.cpp network/API/requests/PostAuthSignInRequest.h json/JsonUtils.h network/API/requests/GetUserMeRequest.cpp network/API/requests/GetUserMeRequest.h json/ConversionImpl.cpp network/API/APILoopThread.cpp network/API/APILoopThread.h)
endif ()
target_compile_features(mosme PRIVATE cxx_attribute_deprecated)

CPMAddPackage("gh:fmtlib/fmt#9.1.0")
CPMAddPackage("gh:nlohmann/json@3.10.5")
CPMAddPackage(
        NAME verdigris
        GIT_TAG add-cmake
        GITHUB_REPOSITORY dand-oss/verdigris
)

if (verdigris_ADDED)
   include_directories(${verdigris_SOURCE_DIR}) 
endif ()

target_link_libraries(mosme
        PRIVATE
        fmt::fmt
        nlohmann_json::nlohmann_json
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt6::Network
        Qt6::QWindowsIntegrationPlugin
        Qt6::Concurrent
        )

if (WIN32)
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

    if (EXISTS "${PROJECT_SOURCE_DIR}/libs/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/libs/libcrypto-1_1-x64.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/libs/libssl-1_1-x64.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/tls/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/libs/libcrypto-1_1-x64.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/tls/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy "${PROJECT_SOURCE_DIR}/libs/libssl-1_1-x64.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/tls/")
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

    if (EXISTS "${QT_INSTALL_PATH}/plugins/tls/qcertonlybackend.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/tls/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/tls/qcertonlybackend.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/tls/")
    endif()

    if (EXISTS "${QT_INSTALL_PATH}/plugins/tls/qopensslbackend.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/tls/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/tls/qopensslbackend.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/tls/")
    endif()

    if (EXISTS "${QT_INSTALL_PATH}/plugins/tls/qschannelbackend.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/tls/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/tls/qschannelbackend.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/tls/")
    endif()
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
