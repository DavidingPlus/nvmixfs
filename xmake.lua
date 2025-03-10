option ("linux-headers", {showmenu = true, description = "Set linux-headers path."})


target ("nvmixfs")
    set_languages ("gnu99")
    add_rules ("platform.linux.module")
    add_files ("src/*.c")
    set_values ("linux.driver.linux-headers", "$(linux-headers)")

    add_cflags ("-O1")


includes ("snippet")
