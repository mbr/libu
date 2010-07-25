#!/usr/bin/env python
# coding=utf8

import os

mmcus = ['atmega128',
         'atmega328p']

source_dir = 'src'
module_subdir = 'modules'
for mmcu in mmcus:
	env = Environment(tools = ['default', 'avrtools'], AVR_MMCU = mmcu)
	env.Append(CFLAGS = '-Iinclude')

	build_dir = os.path.join('build', mmcu)
	env.BuildDir(build_dir, source_dir)
	env.StaticLibrary(os.path.join(build_dir, 'libu'), env.Glob(os.path.join(build_dir, '*.c')))

	# build modules
	for mod in env.Glob(os.path.join(build_dir, module_subdir, '*.c')):
		name = os.path.splitext(os.path.basename(mod.abspath))[0]
		env.StaticLibrary(os.path.join(build_dir, 'libu-%s' % name), mod)

# compile tests
testenv = Environment()
testenv.Append(CFLAGS = Split('-Iinclude -std=c99'))

testenv.BuildDir('build/test-x64',source_dir)
libx64 = testenv.StaticLibrary('build/test-x64',

		# safe test files:
		'build/test-x64/ringbuf.c')

testenv.Append(LIBS = ['check',libx64])

testenv.Program('run_tests',['tests/main.c'] + testenv.Glob('tests/test*.c'))
