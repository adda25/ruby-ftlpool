#!/usr/bin/env gem

require 'date'
def collect_files
  Dir.glob("**/*") # TODO we will specialize it later
end

Gem::Specification.new do |spec|
  spec.name        = "ftlpool"
  spec.version     = "0.0.0"
  spec.date        = DateTime.now.strftime("%F")
  spec.summary     = "A faster than light C++ ThreadPool implementation"
  spec.description = <<-EOS
  The gem implements a written from scratch ThreadPool
  that was implemented in C++ by Amedeo Setti.
  It tends to be... Faster Than Light.
  EOS
  spec.authors     = ["Amedeo Setti", "Matteo Ragni"]
  spec.email       = ["amedeosetti@gmail.com", "info@ragni.me"]
  spec.file        = collect_files
  spec.homepage    = 'http://rubygems.com/amedeosetti/fastpool'
  spec.license     = 'MIT'
end
