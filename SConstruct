#!python

import glob

cpp_flags = '-std=c++11 -Wall -g'
cpp_paths = ['#ArduinoLib', '#']
env = Environment(CPPFLAGS = cpp_flags, LINKFLAGS=cpp_flags, CPPPATH=cpp_paths)

Export('env')
arduino_lib = SConscript('ArduinoLib/SConscript')
micromouse_lib = SConscript('Micromouse/SConscript', 'arduino_lib')
test = SConscript('Test/SConscript', 'micromouse_lib arduino_lib')
env.AlwaysBuild(env.Alias('test', [test], test[0].path))