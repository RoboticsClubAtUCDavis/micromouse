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

env = Environment(CPPFLAGS = cpp_flags)
source_objs = [env.Object(i) for i in sources]
simulate_deps = ['simulate.cpp'] + source_objs
test_deps = glob.glob('Test/*.cpp') + source_objs

env.Program('simulate', simulate_deps, LIBS=sfml_libs)
test = env.Program('Test/test', test_deps)
env.AlwaysBuild(env.Alias('test', [test], test[0].path))
