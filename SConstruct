#!python

import glob

cpp_flags = '-std=c++11 -Wall -Wextra -Werror'
sources = [
        'Maze.cpp',
        'Coordinate.cpp',
        'Node.cpp',
        'Direction.cpp',
        'Path.cpp',
]

sfml_libs = Split('sfml-graphics sfml-window sfml-system')
simulate_deps = ['simulate.cpp'] + sources
test_deps = glob.glob('Test/*.cpp') + sources

env = Environment(CPPFLAGS = cpp_flags)
env.Program('simulate', map(env.Object, simulate_deps), LIBS=sfml_libs)
test = env.Program('Test/test', map(env.Object, test_deps))
env.AlwaysBuild(env.Alias('test', [test], test[0].path))
