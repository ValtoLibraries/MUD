#include <mud/mud.h>
#include <14_live_gfx/14_live_gfx.h>

using namespace mud;

static TextScript& create_wren_script(WrenInterpreter& interpreter)
{
	Signature signature = { { Param{ "scene", Ref(type<Gnode>()) }, Param{ "time", var(float()) } } };
	static TextScript script = { "Example Script", Language::Wren, signature };
	script.m_interpreter = &interpreter;

	script.m_script =
		"\n"
		"//n = Gfx.node(scene)\n"
		"//Gfx.shape(n, Cube.new(), Symbol.new(Colour.White))\n"
		"\n"
		"for (x in 1..11) {\n"
		"    for (y in 1..11) {\n"
		"\n"
		"        var angles = Vec3.new(time + x * 0.21, 0, time + y * 0.37)\n"
		"        var pos = Vec3.new(-15 + x * 3, 0, -15 + y * 3)\n"
		"\n"
		"        var r = ((time + x * 0.21).cos + 1.0) / 2.0\n"
		"        var b = ((time + y * 0.37).sin + 1.0) / 2.0\n"
		"        var g = ((time).cos + 1.0) / 2.0\n"
		"        var color = Colour.new(r, g, b, 1)\n"
		"\n"
		"        var n = Gfx.node(scene, null, pos, Quat.new(angles))\n"
		"        Gfx.draw(n, Cube.new(), Symbol.new(Colour.None, color))\n"
		"\n"
		"    }\n"
		"}\n"
		"\n"
		;

	return script;
}

static TextScript& create_lua_script(LuaInterpreter& interpreter)
{
	Signature signature = { { Param{ "scene", Ref(type<Gnode>()) }, Param{ "time", var(float()) } } };
	static TextScript script = { "Example Script", Language::Lua, signature };
	script.m_interpreter = &interpreter;

	script.m_script =
		"\n"
		"--local n = Gfx.node(scene, nil, vec3(0), quat(0,0,0,1), vec3(1))\n"
		"--Gfx.shape(n, Cube(), Symbol(Colour.White, Colour.None))\n"
		"\n"
		"for x = 1,11 do\n"
		"    for y = 1,11 do\n"
		"\n"
		"        local angles = vec3(time + x * 0.21, 0, time + y * 0.37)\n"
		"        local pos = vec3(-15 + x * 3, 0, -15 + y * 3)\n"
		"\n"
		"        local r = (math.cos(time + x * 0.21) + 1.0) / 2.0\n"
		"        local b = (math.sin(time + y * 0.37) + 1.0) / 2.0\n"
		"        local g = (math.cos(time) + 1.0) / 2.0\n"
		"        local color = Colour(r, g, b)\n"
		"\n"
		"        local n = Gfx.node(scene, nil, pos, quat(angles), vec3(1))\n"
		"        Gfx.draw(n, Cube(), Symbol(Colour.None, color))\n"
		"\n"
		"    end\n"
		"end\n"
		"\n"
		;

	return script;
}

LuaInterpreter& create_lua()
{
	static LuaInterpreter lua = { true };
	return lua;
}

WrenInterpreter& create_wren()
{
	static WrenInterpreter wren = { true };
	string init =
		"import \"mud\" for Vec3, Quat, Colour, Cube, Symbol, SymbolDetail, Gfx\n"
		"var n = null\n";
	wren.call(init.c_str());
	return wren;
}

void ex_14_live_gfx(Shell& app, Widget& parent, Dockbar& dockbar)
{
	UNUSED(app);
	SceneViewer& viewer = ui::scene_viewer(parent);
	ui::orbit_controller(viewer);

	Gnode& scene = viewer.m_scene->begin();

	static LuaInterpreter& lua = create_lua();
	static WrenInterpreter& wren = create_wren();

	static TextScript& lua_script = create_lua_script(lua);
	static TextScript& wren_script = create_wren_script(wren);

	static Language language = Language::Wren;
	//static Language language = Language::Lua;

	if(Widget* dock = ui::dockitem(dockbar, "Game", carray<uint16_t, 1>{ 1U }))
	{
		if(language == Language::Lua)
			script_edit(*dock, lua_script);
		else if(language == Language::Wren)
			script_edit(*dock, wren_script);
	}

	static Clock clock;

	std::vector<Var> args = { Ref(&scene), var(clock.read()) };
	if(language == Language::Lua)
		lua_script(args);
	else if(language == Language::Wren)
		wren_script(args);
}

#ifdef _14_LIVE_GFX_EXE
void pump(Shell& app)
{
	edit_context(app.m_ui->begin(), app.m_editor, true);
	ex_14_live_gfx(app, *app.m_editor.m_screen, *app.m_editor.m_dockbar);
}

int main(int argc, char *argv[])
{
	Shell app(cstrarray(MUD_RESOURCE_PATH), argc, argv);
	app.run(pump);
}
#endif

