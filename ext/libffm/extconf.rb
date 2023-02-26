require "mkmf-rice"

# -DUSESSE requires sse3
# TODO test for it or update fork to use __SSE3__

# -march=native not supported with ARM Mac
default_optflags = RbConfig::CONFIG["host_os"] =~ /darwin/i && RbConfig::CONFIG["host_cpu"] =~ /arm|aarch64/i ? "" : "-march=native"
$CXXFLAGS << " -std=c++17 $(optflags) " << with_config("optflags", default_optflags)

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
