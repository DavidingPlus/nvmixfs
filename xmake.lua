option ("linux-headers", {showmenu = true, description = "Set linux-headers path."})


add_includedirs ("src/")

target ("nvmixfs")
    set_languages ("gnu99")
    add_rules ("platform.linux.module")
    add_files ("src/*.c")
    set_values ("linux.driver.linux-headers", "$(linux-headers)")

    add_cflags ("-O1")


includes ("snippet")
