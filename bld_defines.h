
#define STRINGIFY(name) #name

#define SOURCE_DIRS           STRINGIFY(source_dirs)
#define SOURCE_FILES          STRINGIFY(source_files)
#define INCLUDE_DIRS          STRINGIFY(include_dirs)
#define INCLUDE_FILES         STRINGIFY(include_files)
#define STACK_SIZE            STRINGIFY(stack_size)
#define OBJECT_DIRS           STRINGIFY(object_dirs)
#define LIB_DIRS              STRINGIFY(lib_dirs)
#define MACROS                STRINGIFY(macros)

// Windows
#define MSVC_C_COMPILER       STRINGIFY(msvc_cl)
#define MSVC_CPP_COMPILER     MSVC_C_COMPILER
#define MSVC_LINKER           STRINGIFY(msvc_link)

// GNU
#define CLANG_C_COMPILER      STRINGIFY(gnu_clang)
#define GCC_C_COMPILER        STRINGIFY(gnu_gcc)
#define LLVM_LINKER           STRINGIFY(gnu_ldd)

// Java
#define JDK_JAVA_COMPILER     STRINGIFY(jdk_javac)

// Oracle
#define ORACLE_C_COMPILER     STRINGIFY(oracle_cc)

// Android
#define ANDRIOD_DX            STRINGIFY(android_dx)
#define ANDROID_DEBUG_BRIDGE  STRINGIFY(android_adb)

// Project types
