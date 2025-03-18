option ("linux-headers", {showmenu = true, description = "Set linux-headers path."})
option ("with-gtest", {showmenu = true, description = "Whether to enable unit test by GTest.", default = false})


add_includedirs ("src/")

target ("nvmixfs")
    add_rules ("platform.linux.module")
    add_files ("src/*.c")
    set_values ("linux.driver.linux-headers", "$(linux-headers)")
    set_languages ("gnu99")
    add_cflags ("-O1")

    set_targetdir ("$(buildir)/$(plat)/$(arch)/$(mode)/bin/")


includes ("snippet")

if has_config ("with-gtest") then
    includes ("test")

end
