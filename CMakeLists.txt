# Kirin Label Assistant
# CMakeLists.txt

cmake_minimum_required(VERSION 3.1.0)

project(KirinLabelAssistant)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(OpenCV 3.4.0 REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/src)

file(GLOB KirinLabelAssistant_SRC ${CMAKE_SOURCE_DIR}/src/*)
file(GLOB KirinLabelAssistant_UI ${CMAKE_SOURCE_DIR}/ui/*)
set(KirinLabelAssistant_ICON ${CMAKE_SOURCE_DIR}/resource/icon.rc)

qt5_wrap_ui(KirinLabelAssistant_UI_N ${KirinLabelAssistant_UI})
qt5_add_resources(KirinLabelAssistant_QRC ${CMAKE_SOURCE_DIR}/resource/kirinresource.qrc)

add_executable(${PROJECT_NAME} 
	${KirinLabelAssistant_SRC} 
	${KirinLabelAssistant_UI_N}
	${KirinLabelAssistant_QRC}
	${KirinLabelAssistant_ICON})

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Widgets ${OpenCV_LIBS})

Set_Target_Properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
