-- need to install cmake(by snap recommended) and conan(by pip)
add_requires("conan::gtest 1.12.1", {alias = "gtest"})


target("gtest-run")
    set_kind ("binary")
    add_files ("*.cpp")
    add_packages ("gtest")
    set_languages ("c++11")
