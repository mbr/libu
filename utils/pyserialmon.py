#!/usr/bin/env python
# coding=utf8

import serial
import optparse
import sys
import select
import collections
import os

import mu

# defaults
defaults = {
	'baudrate': 9600
}

def defhelp(field, text):
	return "%s (default: %s)" % (text, repr(defaults[field]))

parser = optparse.OptionParser()
parser.add_option('-b','--baudrate',type=int,default=9600,help=defhelp('baudrate', 'baud rate'))
parser.add_option('-C','--no-colors',help='disable colors')
(opts, args) = parser.parse_args()

if len(args) != 1:
	parser.print_usage()
	sys.exit(1)

keymap = {
	'A': 'U',
	'B': 'D',
	'C': 'R',
	'D': 'L',
}

# unbuffered stdout
sys.stdout = os.fdopen(sys.stdout.fileno(), 'w', 0)

# open connection
con = serial.Serial(args[0], baudrate = opts.baudrate)
sfd = con.fileno()

with mu.console.RawTTY(sys.stdin, cbreak = True):
	sendbuf = collections.deque()
	while True:
		wlist = ()
		if sendbuf: wlist = (sfd,)

		(rlist, wlist, elist) = select.select((sfd,sys.stdin), wlist, ())
		if sys.stdin in rlist: # inefficient, we don't care!
			c = sys.stdin.read(1)
			if '\x1b' == c: # escape sequence
				assert(sys.stdin.read(1) == '[')
				# arrow keys?
				c2 = sys.stdin.read(1)
				if c2 in keymap:
					sendbuf.append(keymap[c2])
				else: raise Exception("Unknown escape sequence: ESC-[-" + repr(c2))
			else:
				sendbuf.append(c)
		if sfd in rlist:
			if not opts.no_colors:
				sys.stdout.write(mu.console.fg_cyan)
			sys.stdout.write(con.read(1))
		if sfd in wlist:
			con.write(sendbuf[0])
			if not opts.no_colors:
				sys.stdout.write(mu.console.fg_red)
			sys.stdout.write(sendbuf.popleft())

		if not opts.no_colors:
			sys.stdout.write(mu.console.fg_white)
