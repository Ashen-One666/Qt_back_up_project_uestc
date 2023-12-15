# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

project ("CMakeProject3")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
# 启用自动UI编译（AUTOUIC）
set(CMAKE_AUTOUIC ON)
# 自动元对象编译（AUTOMOC）
set(CMAKE_AUTOMOC ON)
# 自动资源编译（AUTORCC）
set(CMAKE_AUTORCC ON)


find_package(OPENSSL REQUIRED)
#设置模块的cMake路径    PS：注意！目录的“\”符号在项目中应换成“/”否则会报错
#设置模块的cMake路径    PS：注意！目录的“\”符号在项目中应换成“/”否则会报错
#set(CMAKE_PREFIX_PATH "C:/Users/fez0618/Desktop/msvc2019_64/lib/cmake")
#message(CMAKE_PREFIX_PATH="${CMAKE_PREFIX_PATH}")
#查找指定模块

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
qt_standard_project_setup()

file(GLOB SOURCES *.cpp)
file(GLOB HEADERS *.h)
file(GLOB RESOURCES *.qrc)
file(GLOB FORMS *.ui)

set(SRCS ${SOURCES} ${HEADERS} ${RESOURCES} ${FORMS})

source_group("Header" FILES ${HEADERS})
source_group("Sources" FILES ${SOURCES})
source_group("Ui" FILES ${FORMS})
source_group("Resource" FILES ${RESOURCES})


#强制使用17标准
set(CMAKE_CXX_STANDARD 17)  

add_executable(backup ${SRCS})

add_definitions(-DUNICODE -D_UNICODE)

# OPENSSL_INCLUDE_DIRS 是预定义变量，代表OPENSSL库的头文件路径
include_directories(${OPENSSL_INCLUDE_DIR}) 

# OPENSSL_LIBS 是预定义变量，代表OPENSSL库的lib库文件
target_link_libraries(backup ${OPENSSL_LIBRARIES})

target_link_libraries(backup Qt6::Core Qt6::Gui Qt6::Widgets)


set_target_properties(backup PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)