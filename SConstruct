#!python

import glob

cpp_flags = '-std=c++11 -Wall -Wextra -Werror -g'
sources = [
        'Maze.cpp',
        'Coordinate.cpp',
        'Node.cpp',
        'Direction.cpp',
        'Path.cpp',
]

sfml_libs = Split('sfml-graphics sfml-window sfml-system')

env = Environment(CPPFLAGS = cpp_flags,
        OBJPREFIX='build/linux/', PROGPREFIX='build/linux/')
source_objs = [env.Object(i) for i in sources]
simulate_deps = ['simulate.cpp'] + source_objs
test_deps = glob.glob('Test/*.cpp') + source_objs

simulate = env.Program('simulate', simulate_deps, LIBS=sfml_libs)
test = env.Program('test', test_deps)
env.AlwaysBuild(env.Alias('test', [test], test[0].path))

teensy_cpp_flags = cpp_flags + " -mthumb -ffunction-sections -fdata-sections -nostdlib -MMD -DUSB_SERIAL -DLAYOUT_US_ENGLISH -DTEENSYDUINO=124 -DF_CPU=96000000 -Iteensy-template/teensy3 -felide-constructors -fno-rtti"
teensy_env = Environment(CPPFLAGS = teensy_cpp_flags,
        CXX="arm-none-eabi-g++" , LINK="arm-none-eabi-ld",
#        LINKFLAGS="-gc-sections -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Tteensy-template/teensy3/mk66fx1m0.ld",
        LINKFLAGS="-gc-sections -Tteensy-template/teensy3/mk66fx1m0.ld",
        OBJPREFIX='build/teensy/', PROGPREFIX='build/teensy/')
teensy_template = teensy_env.Command("teensy-template", None,
        "git clone -q https://github.com/apmorton/teensy-template")
teensy_source_objs = [teensy_env.Object(i) for i in sources]
teensy_deps = ['main.cpp'] + teensy_source_objs
teensy_env.Program('mouse.elf', teensy_deps)
teensy_env.Depends('mouse.elf', teensy_template)
