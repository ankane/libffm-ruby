require "mkmf-rice"

$CXXFLAGS += " -std=c++11 -march=native"

apple_clang = RbConfig::CONFIG["CC_VERSION_MESSAGE"] =~ /apple clang/i

# check omp first
if have_library("omp") || have_library("gomp")
  $CXXFLAGS += " -DUSEOMP"
  $CXXFLAGS += " -Xclang" if apple_clang
  $CXXFLAGS += " -fopenmp"
end

ext = File.expand_path(".", __dir__)
libffm = File.expand_path("../../vendor/libffm", __dir__)

$srcs = Dir["#{ext}/*.cpp", "#{libffm}/{ffm,timer}.cpp"]
$INCFLAGS += " -I#{libffm}"
$VPATH << libffm

create_makefile("libffm/ext")
