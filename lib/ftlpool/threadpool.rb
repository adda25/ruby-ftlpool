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
# Authors::    Amedeo Setti (info@amedeosetti.com), Matteo Ragni (info@ragni.me)
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
    HARDWARE_THREADS = FtlPool.hardware_threads
    ##
    # Initialize a new `ThreadPool` object. Requires as input
    # the number of thread that handles. Default to the number
    # of hardware thread available.
    def ThreadPool.new(n = FtlPool::HARDWARE_THREADS)
      raise ArgumentError, "Pool size must be a Fixnum" unless n.is_a? Fixnum
      raise ArgumentError, "Pool size must be positive" if n <= 0
      super(n)
    end
    ##
    # Resizes current `ThreadPool` thread number. Requires as input
    # the number of thread to handle. Default to the number of hardware
    # thread available
    def size=(n = FtlPool::HARDWARE_THREADS)
      raise ArgumentError, "Pool size must be a Fixnum" unless n.is_a? Fixnum
      raise ArgumentError, "Pool size must be positive" if n <= 0
      self._resize n
    end

    alias :synchronize :sync
    alias :end_synchronize :unsynchronize
    alias :end_synchronize :unsync
    alias :end_synchronize :desync

    ##
    # Set sleep timout. It must be a positive integer.
    # A scale as :s, :sec, :msec, :ms or :ns or :nsec
    def sleep=(s)
      scale = :ns
      time  = 0
      if s.is_a? Fixnum
        time, scale = s, :ns
      elsif s.is_a? Array
        if s[0].is_a? Fixnum and s[1].is_a? Symbol
          time, scale = s
        elsif s[1].is_a? Fixnum and s[0].is_a? Symbol
          scale, time = s
        else
          raise ArgumentError, "Sleep value povided in a bad format"
        end
      else
        raise ArgumentError, "Sleep value povided in a bad format"
      end
      raise ArgumentError, "Timer must be positive" if time <= 0
      case scale
      when :nsec, :nano, :ns, :nsec, :nanosec, :nanoseconds
        self._set_sleep_time_ns(time)
      when :msec, :millisec, :ms, :msec, :milli, :milliseconds
        self._set_sleep_time_ms(time)
      when :sec, :s, :seconds
        self._set_sleep_time_s(time)
      else
        raise ArgumentError, "Bad Scale for timer given"
      end
    end
    ##
    # Set sleep timout. It must be a positive integer.
    # in nanoseconds
    def sleep_ns=(s = 1)
      raise ArgumentError, "Timer must be a Fixnum" unless time.is_a? Fixnum
      raise ArgumentError, "Timer must be positive" if time <= 0
      self._set_sleep_time_ns(time)
    end
    ##
    # Set sleep timout. It must be a positive integer.
    # in milliseconds
    def sleep_ms=(s = 1)
      raise ArgumentError, "Timer must be a Fixnum" unless time.is_a? Fixnum
      raise ArgumentError, "Timer must be positive" if time <= 0
      self._set_sleep_time_ms(time)
    end
    ##
    # Set sleep timout. It must be a positive integer.
    # in seconds
    def sleep_s=(s = 1)
      raise ArgumentError, "Timer must be a Fixnum" unless time.is_a? Fixnum
      raise ArgumentError, "Timer must be positive" if time <= 0
      self._set_sleep_time_s(time)
    end

    ##
    # Get sleep timout.
    # A scale as :s, :sec, :msec, :ms or :ns or :nsec
    def sleep(s = :ns)
      raise ArgumentError, "Scale must be a Symbol" unless s.is_a? Symbol
      time = self.sleep_ns
      case s
      when :nsec, :nano, :ns, :nsec, :nanosec, :nanoseconds
        time
      when :msec, :millisec, :ms, :msec, :milli, :milliseconds
        time * 1000000
      when :sec, :s, :seconds
        time * 1000000 * 1000
      else
        raise ArgumentError, "Bad Scale for timer given"
      end
    end
    ##
    # Get sleep timout. In milliseconds
    def sleep_ms
      return self.sleep_ns * 1000000
    end
    ##
    # Get sleep timout. In seconds
    def sleep_s
      return self.sleep_ns * 1000000 * 1000
    end

    # TODO to be completed below here!

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
