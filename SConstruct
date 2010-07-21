#!/usr/bin/env python
# coding=utf8

env = Environment(tools = ['default', 'avrtools'])
env.Append(CFLAGS = '-Iinclude')

env.StaticLibrary('libu',env.Glob('src/*.c'))
