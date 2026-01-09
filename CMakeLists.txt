cmake_minimum_required(VERSION 3.10)
project(LibrarySystem)

# 1. Cấu hình chuẩn C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. Khai báo thư mục Header
# Giúp bạn #include "Core/Book.h" thay vì "../../../Core/Book.h"
include_directories(src)
include_directories(tests) # Để tìm thấy file catch.hpp

# 3. Gom toàn bộ Source Code (Logic)
# Tìm tất cả file .cpp trong folder src, lưu vào biến SRC_FILES
file(GLOB_RECURSE SRC_FILES "src/*.cpp")

# --- MỤC TIÊU 1: ỨNG DỤNG CHÍNH (APP) ---
# File chạy: LibraryApp.exe
# Bao gồm: main.cpp + toàn bộ SRC_FILES
add_executable(LibraryApp main.cpp ${SRC_FILES})


# --- MỤC TIÊU 2: UNIT TEST (KIỂM THỬ) ---
enable_testing()

# File chạy: RunTests.exe
# Bao gồm: TestMain.cpp + Các file test case + toàn bộ SRC_FILES
# QUAN TRỌNG: KHÔNG ĐƯỢC có main.cpp ở đây
add_executable(RunTests 
    tests/TestMain.cpp 
    tests/TestCore.cpp
    ${SRC_FILES} 
)