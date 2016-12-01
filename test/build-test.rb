#!/usr/bin/env ruby

Dir.chdir ".." do
  system "touch ftlpool-0.0.0.gem &&" +
         "gem uninstall ftlpool && " +
         "rm ftlpool-0.0.0.gem && " +
         "rake clobber && sleep 1 && rake compile && sleep 1 && " +
         "gem build ftlpool.gemspec && " +
         "gem install ftlpool-0.0.0.gem "
end

require 'ftlpool'
require 'pry'

puts FtlPool.hardware_threads
tp = FtlPool::ThreadPool.new 2
tp.size = 5

tp.push do
  puts "Go to sleep"
  sleep 2
  puts "I'm 1"
end.wait

tp.push do
  puts "I'm 2"
end.wait

tp.push do
  puts "I'm 3"
end.wait

tp.push do
  puts "I'm 4"
end.wait

binding.pry


a = 0
