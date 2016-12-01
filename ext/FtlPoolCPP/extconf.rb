#!/usr/bin/env ruby

require 'mkmf'

LIBDIR      = RbConfig::CONFIG['libdir']
INCLUDEDIR  = RbConfig::CONFIG['includedir']
HEADER_DIRS = ["../../../../ext/ThreadPool", INCLUDEDIR]
LIB_DIRS    = [LIBDIR]

puts HEADER_DIRS

have_library("c++") or have_library("stdc++")

$CXXFLAGS = ($CXXFLAGS || "") + " " +
            "-std=c++11 -g -pthread -Wall"
$LDFLAGS  = ($LDFLAGS || "") + " " +
            " -fPIC"

dir_config('FtlPoolCPP', HEADER_DIRS, LIB_DIRS)
create_makefile('FtlPoolCPP')
