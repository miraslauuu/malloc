#
# DANTE :: System Automatycznych Testów Jednostkowych oraz Akceptacyjnych
# Tomasz Jaworski, 2018-2020
#
# Plik wygenerowany automatycznie
# Znacznik czasowy: 2024-12-15 20:48:02.447139
#

cmake_minimum_required(VERSION 3.17)
project(project1 C)

# Przyjmij standard C11
set(CMAKE_C_STANDARD 11)

# Ustaw opcje kompilatora (z raportu Dante)
add_compile_options(
      "-Werror"
      "-D_GNU_SOURCE"
      "-D_ANSI_OUTPUT"
      "-DINSIDE_DANTE"
      "-fdiagnostics-color"
      "-fmax-errors=5"
      "-Wall"
      "-Wno-error=implicit-fallthrough"
      "-xc"
      "-ggdb3"
      "-std=c11"
      "-Werror=vla"
      "-Wno-error=unused-parameter"
      "-Wno-error=parentheses"
      "-Wextra"
      "-pedantic"
      "-Wno-parentheses"

)

# Ustaw opcje konsolidatora (z raportu Dante)
add_link_options(
        "-ggdb3"
        "-Wl,-cref"
        "-Wl,-Map=main.map"
        "-Wl,-wrap,main"
)

# Utwórz target 'project1' i dodaj do niego pliki źródlowe:
add_executable(project1
        "heap.c"
        "main.c"
        "unit_helper_v2.c"
        "unit_test_v2.c"
        "rdebug.c"
        "memmanager.c"
)

# Dołącz niezbędne biblioteki
target_link_libraries(project1
        "pthread"
        "m"
)