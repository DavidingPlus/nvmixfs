add_requires ("conan::gtest 1.12.1", {alias = "gtest"})

target ("MagicNumberTest")
    set_kind ("binary")
    add_files ("main.cpp")
    add_packages ("gtest")
