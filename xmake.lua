set_project ("nvmixfs")
set_version ("1.1.1")


option ("linux-headers", {showmenu = true, description = "Set linux-headers path."})
option ("with-gtest", {showmenu = true, description = "Whether to enable unit test by GTest.", default = false})


set_configdir ("$(builddir)/config")
add_configfiles ("src/config.h.in")

add_includedirs ("$(builddir)/config/")
add_includedirs ("src/")
add_includedirs ("src/cross-space/")
add_includedirs ("src/kernel/")
add_includedirs ("src/kernel/fs/")
add_includedirs ("src/kernel/nvm/")


target ("nvmixfs")
    add_rules ("platform.linux.module")

    add_files ("src/cross-space/*.c")
    add_files ("src/kernel/*.c")
    add_files ("src/kernel/fs/*.c")
    add_files ("src/kernel/nvm/*.c")

    set_values ("linux.driver.linux-headers", "$(linux-headers)")
    set_languages ("gnu99")
    add_cflags ("-O1")

    set_targetdir ("$(builddir)/$(plat)/$(arch)/$(mode)/bin/")

target ("nvmix-cross-space")
    set_kind ("shared")
    add_files ("src/cross-space/*.c")

    set_targetdir ("$(builddir)/$(plat)/$(arch)/$(mode)/bin/")

target ("mkfs.nvmixfs")
    set_kind ("binary")
    add_files ("src/mkfs.nvmixfs/main.cpp")

    set_targetdir ("$(builddir)/$(plat)/$(arch)/$(mode)/bin/")


includes ("snippet")

if has_config ("with-gtest") then
    includes ("test")

end
