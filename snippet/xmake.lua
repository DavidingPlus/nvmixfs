-- Find all directories in the current list directory
local dirs = os.dirs("*")

-- Iterate over directories and check if xmake.lua and .buildme exist
for _, dir in ipairs (dirs) do
    local xmakeFile = path.join(dir, "xmake.lua")
    local buildmeFile = path.join(dir, ".buildme")

    if os.isdir(dir) and os.isfile(xmakeFile) and os.isfile(buildmeFile) then
        -- Add subdirectory as a sub-project in xmake
        set_targetdir ("$(builddir)/$(plat)/$(arch)/$(mode)/snippet/")
        
        includes (dir)

    end

end
