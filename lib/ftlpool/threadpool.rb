#!/usr/bin/env ruby

# Copyright (c) 2016 Amedeo Setti, Matteo Ragni
#
# Permission is hereby granted, free of charge, to any person
# obtaining a copy of this software and associated documentation
# files (the "Software"), to deal in the Software without
# restriction, including without limitation the rights to use,
# copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following
# conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
# OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
# HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
# OTHER DEALINGS IN THE SOFTWARE.

##
# Faster Than Light Pool Thread implementation
#
# Author::    Amedeo Setti (amedeosetti@gmail.com), Matteo Ragni (info@ragni.me)
# Copyright:: Copyright (c) 2016 Amedeo Setti, Matteo Ragni
# License::   MIT
module FtlPool

  ##
  # = Class ThreadPool exposes a new Thread Pool manager.
  # On creation, the number of thread must be specified.
  class ThreadPool
    ##
    # Returns the hardware thread number. It is also used as
    # default for creating a new ThreadPool obect
    def self.HW_THREAD
      # TODO Move to a C++ defined constant
    end
    ##
    # Initialize a new `ThreadPool` object. Requires as input
    # the number of thread that handles. Default to the number
    # of hardware thread available.
    def initialize(n = ThreadPool.HW_THREAD)
      raise ArgumentError, "Pool size must be a Fixnum" unless n.is_a? Fixnum
    end
    ##
    # Resizes current `ThreadPool` thread number. Requires as input
    # the number of thread to handle. Default to the number of hardware
    # thread available
    def resize(n = FtlPool.HW_THREAD)
      raise ArgumentError, "Pool size must be a Fixnum" unless n.is_a? Fixnum
      self
    end
    ##
    # Awake ThreadPool
    def awake
      self
    end
    ##
    # Stop current thread's jobs
    def stop
      self
    end
    ##
    # Wait in hold until next task for a thread arrives
    def wait
      self
    end
    ##
    # Syncronize tasks in thread
    def sync
      self
    end
    ##
    # Unsyncronize tasks in thread
    def unsync
      self
    end
    ##
    # Push new closure inside the `ThreadPool`. It accept
    # * `Proc` objects (as `Array`)
    # * `block` directly
    def push(*lambdas, &blk)
      if lambdas.size == 0
        raise ArgumentError, "At least a block is required" unless block_given?
        self.push_block(&blk)
      else
        # TODO We must check everything BEFORE pushing in the ThreadPool?
        lambdas.each do |l|
          raise ArgumentError, "All arguments must be a Proc" unless l.is_a? Proc
        end
        lambdas.each do |l|
          self.push_proc l
        end
      end
      self
    end
    alias :<< :push
    ##
    # Defines an handler for an Exception raised by a job
    # in one of the thread.
    def on_exception
      raise ArgumentError, "Requires a block" unless block_given?
    end

    private
    ##
    # 
    def push_proc(p)
      raise ArgumentError, "Requires a Proc" unless p.is_a? Proc

    end

    def push_block(&blk)
      raise RuntimeError, "Not implemented yet"
    end
  end # class ThreadPool
end # module FtlPool
