-- mud toolchain
-- actions

function mud_amalgamate(modules)
    for _, m in ipairs(modules) do
        local dir = os.getcwd()
        os.chdir(m.root)
        os.execute(path.join(MUD_DIR, "3rdparty/amalgamate/amalgamate.py") .. " -c " .. m.path .. "/Refl/amalgam.h.json" .. " -s " .. MUD_SRC_DIR)
        os.execute(path.join(MUD_DIR, "3rdparty/amalgamate/amalgamate.py") .. " -c " .. m.path .. "/Refl/amalgam.cpp.json" .. " -s " .. MUD_SRC_DIR)
        os.chdir(dir)
    end
end

function mud_write_api(m)
    headers = os.matchfiles(path.join(m.path, "**.h"))
    --table.insert(headers, os.matchfiles(path.join(m.path, "**.hpp")))
    
    local f, err = io.open(path.join(m.path, "Api.h"), "wb")
    io.output(f)
    for _, h in ipairs(headers) do
        if not string.find(h, "Api.h") then
            io.printf("#include <" .. path.getrelative(m.root, h) .. ">")
        end
    end
    io.printf("")
    f:close()
end

function mud_write_mxx(m)
	local f, err = io.open(path.join(m.path, m.dotname .. ".mxx"), "wb")
    io.output(f)
    
    local p = io.printf
    p("#include <cpp/preimport.h>")
    p("")
    p("#include <infra/Config.h>")
    p("")
    p("export module " .. m.dotname .. ";")
    p("export import std.core;")
    p("export import std.io;")
    p("export import std.threading;")
    p("export import std.regex;")
    p("")
    for _, dep in ipairs(m.deps or {}) do
        if dep.cppmodule then
            p("export import " .. dep.dotname .. ";")
        end
    end
    p("")
    p("#include <" .. m.subdir .. "/Api.h>")
    if m.reflect then
        p("#include <meta/" .. m.subdir .. "/Module.h>")
        p("#include <meta/" .. m.subdir .. "/Convert.h>")
    end
    p("")
    
    f:close()
end

function mud_bootstrap(modules)
    for _, m in ipairs(modules) do
        mud_write_api(m)
        mud_write_mxx(m)
    end
end

function mud_reflect(modules)
    local current = {}
    includedirs = function(dirs)
        for _, dir in ipairs(dirs) do
            if not table.contains(current.includedirs, dir) then
                --print("includedir " .. dir)
                table.insert(current.includedirs, dir)
            end
        end
    end
    
    local temp_refl_path = path.join(BUILD_DIR, "refl")
    local jsons = {}
    for _, m in ipairs(modules) do
        if m.refl then
            print('mud reflect ' .. m.idname)
            current = {
                namespace = iif(m.namespace, m.namespace, ''),
                name = m.name,
                dotname = m.dotname,
                idname = m.idname,
                root = m.root,
                subdir = m.subdir,
                path = m.path,
                basetypes = m.basetypes,
                aliases = m.aliases,
                dependencies = {},
                includedirs = {},
            }
            
            for i, dep in ipairs(m.deps or {}) do
                if dep.refl then
                    table.insert(current.dependencies, dep.idname)
                end
            end
            
            -- trick to collect the includes
            if m.lib then
                for i, dep in ipairs(m.lib.deps or {}) do
                    if dep.usage_decl then
                        dep.usage_decl()
                    end
                end
            end
            
            local json_path = path.join(temp_refl_path, m.idname .. "_refl.json")
            local f, err = io.open(json_path, "wb")
            io.output(f)
            io.printf(json.encode(current))
            f:close()
            
            table.insert(jsons, json_path)
        end
    end
    
    print(path.join(MUD_DIR, "src/refl/Metagen", "generator.py") .. " " .. table.concat(jsons, " "))
    os.execute(path.join(MUD_DIR, "src/refl/Metagen", "generator.py") .. " " .. table.concat(jsons, " "))
end

newaction {
    trigger     = "bootstrap",
    description = "Bootstrap c++ modules",
    execute     = function()
        mud_bootstrap(MODULES)
    end
}

newaction {
    trigger     = "reflect",
    description = "Generate reflection",
    execute     = function()
        mud_reflect(MODULES)
    end
}

newaction {
    trigger     = "amalgamate",
    description = "Generate amalgamation files",
    execute     = function()
        mud_amalgamate(MODULES)
    end
}

