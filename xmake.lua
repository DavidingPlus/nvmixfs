option ("linux-headers", {showmenu = true, description = "Set linux-headers path."})


add_includedirs ("src/")

target ("nvmixfs")
    add_rules ("platform.linux.module")
    add_files ("src/*.c")
    set_values ("linux.driver.linux-headers", "$(linux-headers)")
    set_languages ("gnu99")
    add_cflags ("-O1")


includes ("snippet")
