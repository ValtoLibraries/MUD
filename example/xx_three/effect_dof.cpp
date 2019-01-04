#include <mud/mud.h>

using namespace mud;

Texture* load_envmap(const string& path, const string& format)
{
	string paths[6] = {
		path + "px" + format, path + "nx" + format,
		path + "py" + format, path + "ny" + format,
		path + "pz" + format, path + "nz" + format
	};
	return nullptr; //THREE.CubeTextureLoader().load(urls);
}

/*
function onWindowResize() {
	windowHalfX = window.innerWidth / 2;
	windowHalfY = window.innerHeight / 2;
	width = window.innerWidth;
	height = window.innerHeight;
	camera.aspect = width / height;
	camera.updateProjectionMatrix();
	renderer.setSize(width, height);
	postprocessing.composer.setSize(width, height);
}
*/

struct DoFRenderer : public Renderer
{
	DoFRenderer(GfxSystem& gfx_system, Pipeline& pipeline)
		: Renderer(gfx_system, pipeline, Shading::Shaded)
	{
		this->add_pass<PassClear>(gfx_system);
		this->add_pass<PassOpaque>(gfx_system);
		//this->add_pass<PassBokeh>(gfx_system);
		this->add_pass<PassEffects>(gfx_system);
		this->add_pass<PassPostProcess>(gfx_system, *pipeline.block<BlockCopy>());
		this->init();

		// var renderPass = new THREE.RenderPass(scene, camera);
		// var bokehPass = new THREE.BokehPass(scene, camera, {
		//     focus: 1.0,
		//     aperture : 0.025,
		//     maxblur : 1.0,
		//     width : width,
		//     height : height
		// });
		// bokehPass.renderToScreen = true;
		// var composer = new THREE.EffectComposer(renderer);
		// composer.addPass(renderPass);
		// composer.addPass(bokehPass);
		// postprocessing.composer = composer;
		// postprocessing.bokeh = bokehPass;
		// 
		// renderer.autoClear = false;
	}
};

void ex_xx_three(Shell& app, Widget& parent, Dockbar& dockbar)
{
	SceneViewer& viewer = ui::scene_viewer(parent);
	ui::orbit_controller(viewer);

	//camera = new THREE.PerspectiveCamera(70, width / height, 1, 3000);
	//camera.position.z = 200;
	//renderer = new THREE.WebGLRenderer();
	//renderer.setPixelRatio(window.devicePixelRatio);
	//renderer.setSize(width, height);
	//container.appendChild(renderer.domElement);

	Gnode& scene = viewer.m_scene->begin();

	static int xgrid = 14;
	static int ygrid = 9;
	static int zgrid = 14;
	static bool single_material = false;

	static vec2 mouse;
	if(MouseEvent event = viewer.mouse_event(DeviceType::Mouse, EventType::Moved))
	{
		mouse.x = event.m_relative.x - viewer.m_frame.m_size.x / 2.f;
		mouse.y = event.m_relative.y - viewer.m_frame.m_size.y / 2.f;
	}

	if(MouseEvent event = viewer.mouse_event(DeviceType::Touch, EventType::Pressed))
		//or(MouseEvent event = viewer.mouse_event(DeviceType::Touch, EventType::Moved))
	{
		//if(event.touches.length == 1) {
		//	event.preventDefault();
		//	mouseX = event.touches[0].pageX - windowHalfX;
		//	mouseY = event.touches[0].pageY - windowHalfY;
		//}
	}

	int nobjects = xgrid * ygrid * zgrid;

	//var geo = new THREE.SphereBufferGeometry(1, 20, 10);
	static Sphere sphere = {};
	static Model& geo = app.m_gfx_system.fetch_symbol(Symbol(), sphere, PLAIN);

	static std::vector<Material*> materials;

	Texture* texcube = load_envmap("textures/cube/SwedishRoyalCastle/", ".jpg");
	for(int i = 0; i < nobjects; ++i)
	{
		string name = "object" + to_string(i);
		materials.push_back(&app.m_gfx_system.fetch_material(name.c_str(), "pbr/pbr"));
		//materials.push_back(&app.m_gfx_system.materials().create(name.c_str()));
		//materials.push_back(new THREE.MeshBasicMaterial({ color: 0xff1100, envMap : texcube }));
	}

	int count = 0;
	for(int i = 0; i < xgrid; i++)
		for(int j = 0; j < ygrid; j++)
			for(int k = 0; k < zgrid; k++)
			{
				int x = 200 * (i - xgrid / 2);
				int y = 200 * (j - ygrid / 2);
				int z = 200 * (k - zgrid / 2);
				Gnode& n = gfx::node(scene, {}, vec3(x, y, z), ZeroQuat, vec3(60));
				gfx::item(n, geo, 0U, single_material ? materials[0] : materials[count]);
				count++;
			}

	struct Parameters { float focus = 500.f; float aperture = 5.f; float maxblur = 1.f; };
	Parameters params;

	Widget& controls = ui::stack(viewer);
	ui::slider_field<float>(controls, "focus",    { params.focus,    { 10.0, 3000.0, 10 } });
	ui::slider_field<float>(controls, "aperture", { params.aperture, { 0, 10, 0.1 } });
	ui::slider_field<float>(controls, "maxblur",  { params.maxblur,  { 0.0, 3.0, 0.025 } });

	auto update = []()
	{
		//bokeh.uniforms["focus"].value = params.focus;
		//bokeh.uniforms["aperture"].value = params.aperture * 0.00001;
		//bokeh.uniforms["maxblur"].value = params.maxblur;
	};

	//var postprocessing = {};

	double time = 0.0;//Date.now() * 0.00005;
	Camera& camera = viewer.m_camera;
	camera.m_eye.x += (mouse.x - camera.m_eye.x) * 0.036;
	camera.m_eye.y += (-(mouse.y)-camera.m_eye.y) * 0.036;
	//camera.m_target = scene.position;
	if(!single_material)
		for(int i = 0; i < nobjects; i++)
		{
			float h = (360 * (i / nobjects + int(time)) % 360) / 360;
			materials[i]->m_unshaded_block.m_colour = hsl_to_rgb(h, 1, 0.5);
		}
	
	//postprocessing.composer.render(0.1);
}


#ifdef _XX_THREE_EXE
void pump(Shell& app)
{
	edit_context(app.m_ui->begin(), app.m_editor, true);
	ex_xx_three(app, *app.m_editor.m_screen, *app.m_editor.m_dockbar);
}

int main(int argc, char *argv[])
{
	cstring example_path = MUD_RESOURCE_PATH "examples/17_wfc/";
	Shell app(carray<cstring, 2>{ MUD_RESOURCE_PATH, example_path }, argc, argv);
	app.run(pump);
}
#endif
