-- conan needs pip(by apt) and cmake(by snap recommended)
add_requires ("conan::gtest 1.12.1", {alias = "gtest"})


target("tests")
    set_filename ("gtest-run")

    set_kind ("binary")
    add_files ("*.cpp")
    add_packages ("gtest")
    set_languages ("c++11")

    set_default (false) -- 类似 CMake 中的 EXCLUDE_FROM_ALL。默认不构建，需要显式指定才会构建。
    set_targetdir ("$(buildir)/$(plat)/$(arch)/$(mode)/test/")

    add_deps ("nvmix-cross-space")
