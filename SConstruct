#!/usr/bin/env python
# coding=utf8

env = Environment(tools = ['default', 'avrtools'])
env.Append(CFLAGS = '-Iinclude')

env.StaticLibrary('libu',env.Glob('src/*.c'))

# compile tests
testenv = Environment()
testenv.Append(CFLAGS = Split('-Iinclude -std=c99'))

testenv.BuildDir('build/test-x64','src')
libx64 = testenv.StaticLibrary('build/test-x64',env.Glob('build/test-x64/*.c'))

testenv.Append(LIBS = ['check',libx64])

testenv.Program('run_tests',['tests/main.c'] + testenv.Glob('tests/test*.c'))
