#!/usr/bin/env ruby

require 'mkmf'

LIBDIR      = RbConfig::CONFIG['libdir']
INCLUDEDIR  = RbConfig::CONFIG['includedir']
HEADER_DIRS = ["../../../../ext/ThreadPool", INCLUDEDIR]
LIB_DIRS    = [LIBDIR]

puts HEADER_DIRS

have_library("c++") or have_library("stdc++")

$CXXFLAGS = ($CXXFLAGS || "") + " " +
            "-std=c++11 -O3 -pthread -Wall"
$LDFLAGS  = ($LDFLAGS || "") + " " +
            "-shared -fPIC"

dir_config('FtlPoolCPP', HEADER_DIRS, LIB_DIRS)
create_makefile('FtlPoolCPP')
