#!python

cpp_flags     = '-std=c++11 -Wall -Wextra'
sfml_libs     = Split('sfml-graphics sfml-window sfml-system')
simulate_deps = Split('''simulate.cpp
                         Maze.cpp
                         Coordinate.cpp
                         Node.cpp
                         Direction.cpp
                         Path.cpp''')
test_deps     = Split('''Test/Test-Direction.cpp
                         Test/Test-Main.cpp
                         Direction.cpp''')

env = Environment(CPPFLAGS = cpp_flags)
env.Program('simulate', map(env.Object, simulate_deps), LIBS=sfml_libs)
test = env.Program('Test/test', map(env.Object, test_deps))
env.AlwaysBuild(env.Alias('test', [test], test[0].path))
