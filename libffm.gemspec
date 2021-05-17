require_relative "lib/libffm/version"

Gem::Specification.new do |spec|
  spec.name          = "libffm"
  spec.version       = Libffm::VERSION
  spec.summary       = "Field-aware factorization machines for Ruby"
  spec.homepage      = "https://github.com/ankane/libffm"
  spec.license       = "MIT"

  spec.author        = "Andrew Kane"
  spec.email         = "andrew@ankane.org"

  spec.files         = Dir["*.{md,txt}", "{ext,lib,vendor}/**/*"]
  spec.require_path  = "lib"
  spec.extensions    = ["ext/libffm/extconf.rb"]

  spec.required_ruby_version = ">= 2.6"

  spec.add_dependency "rice", ">= 4.0.2"
end
