

#pragma once

#ifndef MUD_MODULES
#include <meta/wfc/Module.h>

#include <type/Any.h>
#include <type/Vector.h>
#include <refl/MetaDecl.h>
#include <refl/Module.h>
#endif

namespace mud
{
    void mud_wfc_meta(Module& m)
    {   
    // Base Types
    
    // Enums
    {
        static Meta meta = { type<mud::Result>(), &namspc({ "mud" }), "Result", sizeof(mud::Result), TypeClass::Enum };
        static Enum enu = { type<mud::Result>(),
            false,
            { "kSuccess", "kFail", "kUnfinished" },
            { 0, 1, 2 },
            { var(mud::kSuccess), var(mud::kFail), var(mud::kUnfinished) }
        };
        meta_enum<mud::Result>();
    }
    
    
    // Sequences
    {
        static Meta meta = { type<std::vector<mud::Image256>>(), &namspc({}), "std::vector<mud::Image256>", sizeof(std::vector<mud::Image256>), TypeClass::Sequence };
        static Class cls = { type<std::vector<mud::Image256>>() };
        cls.m_content = &type<mud::Image256>();
        meta_sequence<std::vector<mud::Image256>, mud::Image256>();
    }
    
    
    
    // mud::Tile
    {
        static Meta meta = { type<mud::Tile>(), &namspc({ "mud" }), "Tile", sizeof(mud::Tile), TypeClass::Struct };
        static Class cls = { type<mud::Tile>(),
            // bases
            {  },
            {  },
            // constructors
            {
            },
            // copy constructor
            {
                { type<mud::Tile>(), [](Ref ref, Ref other) { new(&val<mud::Tile>(ref)) mud::Tile(val<mud::Tile>(other)); } }
            },
            // members
            {
                { type<mud::Tile>(), member_address(&mud::Tile::m_index), type<size_t>(), "index", var(size_t()), Member::Value, nullptr },
                { type<mud::Tile>(), member_address(&mud::Tile::m_name), type<std::string>(), "name", var(std::string()), Member::Value, nullptr },
                { type<mud::Tile>(), member_address(&mud::Tile::m_symmetry), type<char>(), "symmetry", var(char()), Member::Value, nullptr },
                { type<mud::Tile>(), member_address(&mud::Tile::m_cardinality), type<int>(), "cardinality", var(int()), Member::Value, nullptr },
                { type<mud::Tile>(), member_address(&mud::Tile::m_profile), type<int>(), "profile", var(int()), Member::Value, nullptr }
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        
        
        
        meta_class<mud::Tile>();
    }
    
    
        
    // mud::Tileset
    {
        static Meta meta = { type<mud::Tileset>(), &namspc({ "mud" }), "Tileset", sizeof(mud::Tileset), TypeClass::Struct };
        static Class cls = { type<mud::Tileset>(),
            // bases
            {  },
            {  },
            // constructors
            {
                { type<mud::Tileset>(), [](Ref ref, array<Var> args) { UNUSED(args);new(&val<mud::Tileset>(ref)) mud::Tileset(  ); }, {} }
            },
            // copy constructor
            {
                { type<mud::Tileset>(), [](Ref ref, Ref other) { new(&val<mud::Tileset>(ref)) mud::Tileset(val<mud::Tileset>(other)); } }
            },
            // members
            {
                { type<mud::Tileset>(), member_address(&mud::Tileset::m_name), type<std::string>(), "name", var(std::string()), Member::Value, nullptr },
                { type<mud::Tileset>(), member_address(&mud::Tileset::m_tile_size), type<mud::vec3>(), "tile_size", var(mud::vec3()), Member::Value, nullptr },
                { type<mud::Tileset>(), member_address(&mud::Tileset::m_tile_scale), type<mud::vec3>(), "tile_scale", var(mud::vec3()), Member::Value, nullptr },
                { type<mud::Tileset>(), member_address(&mud::Tileset::m_num_tiles), type<uint16_t>(), "nutiles", var(uint16_t()), Member::Value, nullptr }
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        
        
        
        meta_class<mud::Tileset>();
    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
        
    // mud::Wave
    {
        static Meta meta = { type<mud::Wave>(), &namspc({ "mud" }), "Wave", sizeof(mud::Wave), TypeClass::Struct };
        static Class cls = { type<mud::Wave>(),
            // bases
            {  },
            {  },
            // constructors
            {
            },
            // copy constructor
            {
                { type<mud::Wave>(), [](Ref ref, Ref other) { new(&val<mud::Wave>(ref)) mud::Wave(val<mud::Wave>(other)); } }
            },
            // members
            {
            },
            // methods
            {
                { type<mud::Wave>(), "solve", member_address(&mud::Wave::solve), [](Ref object, array<Var> args, Var& result) { val<mud::Result>(result) = val<mud::Wave>(object).solve(val<size_t>(args[0])); }, { { "limit", var(size_t()) } }, var(mud::Result()) }
            },
            // static members
            {
            }
        };
        
        
        
        
        meta_class<mud::Wave>();
    }
    
    
        
    // mud::TileWave
    {
        static Meta meta = { type<mud::TileWave>(), &namspc({ "mud" }), "TileWave", sizeof(mud::TileWave), TypeClass::Struct };
        static Class cls = { type<mud::TileWave>(),
            // bases
            { &type<mud::Wave>() },
            { base_offset<mud::TileWave, mud::Wave>() },
            // constructors
            {
                { type<mud::TileWave>(), [](Ref ref, array<Var> args) { new(&val<mud::TileWave>(ref)) mud::TileWave( val<mud::WaveTileset>(args[0]), val<uint16_t>(args[1]), val<uint16_t>(args[2]), val<uint16_t>(args[3]), val<bool>(args[4]) ); }, { { "tileset", var(mud::WaveTileset()) }, { "width", var(uint16_t()) }, { "height", var(uint16_t()) }, { "depth", var(uint16_t()) }, { "periodic", var(bool()) } } }
            },
            // copy constructor
            {
                { type<mud::TileWave>(), [](Ref ref, Ref other) { new(&val<mud::TileWave>(ref)) mud::TileWave(val<mud::TileWave>(other)); } }
            },
            // members
            {
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        
        
        
        meta_class<mud::TileWave>();
    }
    
    
        
    // mud::WaveTileset
    {
        static Meta meta = { type<mud::WaveTileset>(), &namspc({ "mud" }), "WaveTileset", sizeof(mud::WaveTileset), TypeClass::Struct };
        static Class cls = { type<mud::WaveTileset>(),
            // bases
            { &type<mud::Tileset>() },
            { base_offset<mud::WaveTileset, mud::Tileset>() },
            // constructors
            {
                { type<mud::WaveTileset>(), [](Ref ref, array<Var> args) { UNUSED(args);new(&val<mud::WaveTileset>(ref)) mud::WaveTileset(  ); }, {} }
            },
            // copy constructor
            {
                { type<mud::WaveTileset>(), [](Ref ref, Ref other) { new(&val<mud::WaveTileset>(ref)) mud::WaveTileset(val<mud::WaveTileset>(other)); } }
            },
            // members
            {
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        
        
        
        meta_class<mud::WaveTileset>();
    }
    
    
        
    // mud::array_3d<float>
    {
        static Meta meta = { type<mud::array_3d<float>>(), &namspc({ "mud" }), "array_3d<float>", sizeof(mud::array_3d<float>), TypeClass::Struct };
        static Class cls = { type<mud::array_3d<float>>(),
            // bases
            { &type<std::vector<float>>() },
            { base_offset<mud::array_3d<float>, std::vector<float>>() },
            // constructors
            {
            },
            // copy constructor
            {
                { type<mud::array_3d<float>>(), [](Ref ref, Ref other) { new(&val<mud::array_3d<float>>(ref)) mud::array_3d<float>(val<mud::array_3d<float>>(other)); } }
            },
            // members
            {
            },
            // methods
            {
            },
            // static members
            {
            }
        };
        
        
        
        
        meta_class<mud::array_3d<float>>();
    }
    

    
        m.m_types.push_back(&type<mud::Result>());
        m.m_types.push_back(&type<mud::Tile>());
        m.m_types.push_back(&type<mud::Tileset>());
        m.m_types.push_back(&type<mud::Wave>());
        m.m_types.push_back(&type<std::vector<mud::Image256>>());
        m.m_types.push_back(&type<mud::TileWave>());
        m.m_types.push_back(&type<mud::WaveTileset>());
        m.m_types.push_back(&type<mud::array_3d<float>>());
    
        {
            auto func = [](array<Var> args, Var& result) { UNUSED(result);  mud::parse_json_tileset(val<std::string>(args[0]), val<std::string>(args[1]), val<mud::Tileset>(args[2])); };
            std::vector<Param> params = { { "path", var(std::string()) }, { "subset", var(std::string()) }, { "outputTileset", var(mud::Tileset()), Param::Output } };
            static Function f = { &namspc({ "mud" }), "parse_json_tileset", function_id<void(*)(const std::string&, const std::string&, mud::Tileset&)>(&mud::parse_json_tileset), func, params, Var() };
            m.m_functions.push_back(&f);
        }
        {
            auto func = [](array<Var> args, Var& result) { UNUSED(result);  mud::parse_json_wave_tileset(val<std::string>(args[0]), val<std::string>(args[1]), val<mud::WaveTileset>(args[2])); };
            std::vector<Param> params = { { "path", var(std::string()) }, { "subset", var(std::string()) }, { "outputTileset", var(mud::WaveTileset()), Param::Output } };
            static Function f = { &namspc({ "mud" }), "parse_json_wave_tileset", function_id<void(*)(const std::string&, const std::string&, mud::WaveTileset&)>(&mud::parse_json_wave_tileset), func, params, Var() };
            m.m_functions.push_back(&f);
        }
    }
}
