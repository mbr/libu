#!/usr/bin/env python
# coding=utf8

import os

mmcus = ['atmega128',
         'atmega328p']

for mmcu in mmcus:
	env = Environment(tools = ['default', 'avrtools'], AVR_MMCU = mmcu)
	env.Append(CFLAGS = '-Iinclude')

	build_dir = os.path.join('build', mmcu)
	env.BuildDir(build_dir, 'src')
	env.StaticLibrary(os.path.join(build_dir, 'libu-%s' % mmcu), env.Glob(os.path.join(build_dir, '*.c')))

# compile tests
testenv = Environment()
testenv.Append(CFLAGS = Split('-Iinclude -std=c99'))

testenv.BuildDir('build/test-x64','src')
libx64 = testenv.StaticLibrary('build/test-x64',

		# safe test files:
		'build/test-x64/ringbuf.c')

testenv.Append(LIBS = ['check',libx64])

testenv.Program('run_tests',['tests/main.c'] + testenv.Glob('tests/test*.c'))
