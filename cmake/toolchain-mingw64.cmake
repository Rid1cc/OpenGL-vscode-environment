# MinGW-w64 Toolchain (MSYS2)
#
# Change this path if your MSYS2 installation is elsewhere.
# Also update .vscode/settings.json -> grafika.msys2Root to match.
set(MSYS2_ROOT "C:/msys64" CACHE PATH "MSYS2 installation directory")

set(MINGW64_BIN "${MSYS2_ROOT}/mingw64/bin")

set(CMAKE_C_COMPILER   "${MINGW64_BIN}/gcc.exe"   CACHE FILEPATH "" FORCE)
set(CMAKE_CXX_COMPILER "${MINGW64_BIN}/g++.exe"   CACHE FILEPATH "" FORCE)
set(CMAKE_RC_COMPILER  "${MINGW64_BIN}/windres.exe" CACHE FILEPATH "" FORCE)
set(CMAKE_AR           "${MINGW64_BIN}/ar.exe"     CACHE FILEPATH "" FORCE)
set(CMAKE_RANLIB       "${MINGW64_BIN}/ranlib.exe" CACHE FILEPATH "" FORCE)

set(MINGW64_DLL_DIR "${MINGW64_BIN}" CACHE PATH "" FORCE)
