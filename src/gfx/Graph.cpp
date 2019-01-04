//  Copyright (c) 2019 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <gfx/Cpp20.h>

#ifdef MUD_MODULES
module mud.gfx;
#else
#include <infra/Vector.h>
#include <pool/ObjectPool.h>
#include <math/Math.h>
#include <geom/Intersect.h>
#include <geom/Symbol.h>
#include <gfx/Types.h>
#include <gfx/Graph.h>
#include <gfx/Gfx.h>
#include <gfx/Draw.h>
#include <gfx/Prefab.h>
#include <gfx/Item.h>
#include <gfx/Animated.h>
#include <gfx/Particles.h>
#include <gfx/Scene.h>
#include <gfx/Asset.h>
#include <gfx/Model.h>
#include <gfx/Texture.h> // @kludge : make all this logic private and export_ asset stores
#include <gfx/Blocks/Sky.h>
#include <gfx/GfxSystem.h>
#include <gfx/Pipeline.h>
#endif

namespace mud
{
	Gnode::Gnode() : Graph() {}
	Gnode::Gnode(Scene& scene, SoundManager* sound_manager) : Graph(), m_scene(&scene), m_attach(&scene.m_root_node), m_sound_manager(sound_manager) {}
	Gnode::Gnode(Gnode* parent, void* identity) : Graph(parent, identity), m_scene(parent->m_scene), m_attach(parent->m_attach), m_sound_manager(parent->m_sound_manager) {}

	Gnode::~Gnode()
	{
		this->clear();
	}

	void Gnode::clear()
	{
		m_nodes.clear();
		if(m_node)
		{
			m_scene->m_pool->pool<Node3>().tdestroy(*m_node);
			m_node = nullptr;
		}
		if(m_item)
		{
			m_scene->m_pool->pool<Item>().tdestroy(*m_item);
			m_item = nullptr;
		}
		if(m_animated)
		{
			m_scene->m_pool->pool<Animated>().tdestroy(*m_animated);
			m_animated = nullptr;
		}
		if(m_particles)
		{
			m_scene->m_pool->pool<Particles>().tdestroy(*m_particles);
			m_particles = nullptr;
		}
		if(m_light)
		{
			m_scene->m_pool->pool<Light>().tdestroy(*m_light);
			m_light = nullptr;
		}

		if(m_sound)
		{
			m_scene->m_orphan_sounds.push_back(m_sound);
			m_sound = nullptr;
			printf("ERROR: sound goes out of graph but wasn't destroyed\n");
		}
	}

	void debug_tree(Gnode& node, size_t index, size_t depth)
	{
		auto print_depth = [](size_t depth) { for(size_t i = 0; i < depth; ++i) printf("    "); };
		print_depth(depth);
		printf("node %i\n", int(index));
		if(node.m_item)
		{
			print_depth(depth + 1);
			printf("item %s\n", node.m_item->m_model->m_name.c_str());
		}
		for(size_t i = 0; i < node.m_nodes.size(); ++i)
			debug_tree(*node.m_nodes[i], i, depth + 1);
	}

	template <class T_Element, class... T_Args>
	inline T_Element& create(Scene& scene, T_Args&&... args)
	{
		return scene.m_pool->pool<T_Element>().construct(std::forward<T_Args>(args)...);
	}

namespace gfx
{
	Gnode& node(Gnode& parent, Ref object, const mat4& transform)
	{
		Gnode& self = parent.subi(object.m_value);
		if(!self.m_node)
		{
			self.m_node = &create<Node3>(*parent.m_scene, parent.m_scene, object);
			self.m_attach = self.m_node;
		}
		self.m_node->m_transform = transform;
		return self;
	}

	Gnode& node(Gnode& parent, Ref object, const vec3& position, const quat& rotation, const vec3& scale)
	{
		return node(parent, object, bxTRS(scale, rotation, position));
	}

	Gnode& node(Gnode& parent, Ref object, const Transform& transform)
	{
		return node(parent, object, transform.m_position, transform.m_rotation, transform.m_scale);
	}

	Gnode& transform(Gnode& parent, Ref object, const vec3& position, const quat& rotation, const vec3& scale)
	{
		return node(parent, object, parent.m_attach->m_transform * bxTRS(scale, rotation, position));
	}

	Gnode& transform(Gnode& parent, Ref object, const vec3& position, const quat& rotation)
	{
		return node(parent, object, parent.m_attach->m_transform * bxTRS(Unit3, rotation, position));
	}

	void update_item_aabb(Item& item)
	{
		if(item.m_instances.size() == 0)
		{
			item.m_aabb = transform_aabb(item.m_model->m_aabb, item.m_node->m_transform);
		}
		else
		{
			item.m_aabb = {};
			for(const mat4& transform : item.m_instances)
				item.m_aabb.mergeSafe(transform_aabb(item.m_model->m_aabb, transform));
		}
	}

	void update_item_lights(Item& item)
	{
		item.m_lights.clear();

		item.m_node->m_scene->m_pool->pool<Light>().iterate([&](Light& light)
		{
			if(light.m_type == LightType::Direct || sphere_aabb_intersection(light.m_node.position(), light.m_range, item.m_aabb))
				item.m_lights.push_back(&light);
		});
	}

	Item& item(Gnode& parent, const Model& model, uint32_t flags, Material* material, size_t instances, array<mat4> transforms)
	{
		Gnode& self = parent.suba<Gnode>();
		bool update = (flags & ItemFlag::NoUpdate) == 0;
		if(!self.m_item)
		{
			self.m_item = &create<Item>(*self.m_scene, *self.m_attach, model, flags, material, instances);
			update = true;
		}
		self.m_item->m_model = const_cast<Model*>(&model);
		self.m_item->m_material = material;
		if(transforms.size() > 0)
		{
			self.m_item->m_instances.resize(instances);
			transforms.copy(self.m_item->m_instances);
		}
		if(instances > 0)
		{
			self.m_item->m_instances.resize(instances);
			self.m_item->update_instances();
		}
		if(update)
		{
			update_item_aabb(*self.m_item);
			update_item_lights(*self.m_item);
		}
		return *self.m_item;
	}

	void prefab(Gnode& parent, const Prefab& prefab, bool transform, uint32_t flags, Material* material, size_t instances, array<mat4> transforms)
	{
		Gnode& self = parent.suba<Gnode>();
		
		for(size_t i = 0; i < prefab.m_items.size(); ++i)
		{
			mat4 tr = transform ? parent.m_attach->m_transform * prefab.m_nodes[i].m_transform
								: prefab.m_nodes[i].m_transform;
			Gnode& no = node(self, {}, tr);
			Item& it = item(no, *prefab.m_items[i].m_model, prefab.m_items[i].m_flags | flags, material, instances, transforms);
			//it = prefab.m_items[i];
			//shape(self, Cube(i.m_aabb.m_center, vec3(0.1f)), Symbol::wire(Colour::Red, true));
			//shape(self, submodel->m_aabb, Symbol::wire(Colour::White));
			UNUSED(it);
		}
	}

	Item& shape_item(Gnode& parent, Model& model, const Symbol& symbol, uint32_t flags, Material* material, size_t instances, DrawMode draw_mode)
	{
		Item& self = item(parent, model, flags, material, instances);
		self.m_material = material ? material : &parent.m_scene->m_gfx_system.fetch_symbol_material(symbol, draw_mode);
		return self;
	}

	Item& shape(Gnode& parent, const Shape& shape, const Symbol& symbol, uint32_t flags, Material* material, size_t instances)
	{
		Item* item = nullptr;
		Symbol white = { Colour::White, Colour::White };
		if(symbol.fill())
			item = &shape_item(parent, parent.m_scene->m_gfx_system.fetch_symbol(white, shape, PLAIN), symbol, flags, material, instances, PLAIN);
		if(symbol.outline())
			item = &shape_item(parent, parent.m_scene->m_gfx_system.fetch_symbol(white, shape, OUTLINE), symbol, flags, material, instances, OUTLINE);
		return *item;
	}

	void draw(Scene& scene, const mat4& transform, const Shape& shape, const Symbol& symbol, uint32_t flags)
	{
		UNUSED(flags);
		if(symbol.fill())
			scene.m_immediate->draw(transform, { symbol, &shape, PLAIN });
		if(symbol.outline())
			scene.m_immediate->draw(transform, { symbol, &shape, OUTLINE });
	}

	void draw(Gnode& parent, const Shape& shape, const Symbol& symbol, uint32_t flags)
	{
		draw(*parent.m_scene, parent.m_attach->m_transform, shape, symbol, flags);
	}

	Item& sprite(Gnode& parent, const Image256& image, const vec2& size, uint32_t flags, Material* material, size_t instances)
	{
		return shape(parent, Quad(size), { image }, flags, material, instances);
	}

	Item* model(Gnode& parent, const string& name, uint32_t flags, Material* material, size_t instances)
	{
		Model* model = parent.m_scene->m_gfx_system.models().file(name.c_str());
		if(model)
			return &item(parent, *model, flags, material, instances);
		return nullptr;
	}

	Animated& animated(Gnode& parent, Item& item)
	{
		Gnode& self = parent.suba();
		if(!self.m_animated)
		{
			self.m_animated = &create<Animated>(*self.m_scene, *self.m_attach);
			self.m_animated->add_item(item);
		}
		return *self.m_animated;
	}

	Particles& particles(Gnode& parent, const ParticleGenerator& emitter, uint32_t flags, size_t instances)
	{
		UNUSED(flags); UNUSED(instances);
		Gnode& self = parent.suba();
		if(!self.m_particles)
			self.m_particles = &create<Particles>(*self.m_scene, self.m_attach, Sphere(1.f), 1024);
		as<ParticleGenerator>(*self.m_particles) = emitter;
		self.m_particles->m_node = self.m_attach;
		self.m_particles->m_sprite = &parent.m_scene->m_particle_system->m_block.m_sprites.find_sprite(emitter.m_sprite_name.c_str());
		return *self.m_particles;
	}

	Light& light(Gnode& parent, LightType light_type, bool shadows, Colour colour, float range, float attenuation)
	{
		Gnode& self = parent.suba();
		if(!self.m_light)
		{
			self.m_light = &create<Light>(*self.m_scene, *self.m_attach, light_type, shadows);
		}
		self.m_light->m_type = light_type;
		self.m_light->m_colour = colour;
		self.m_light->m_range = range;
		self.m_light->m_attenuation = attenuation;
		return *self.m_light;
	}

	Light& direct_light_node(Gnode& parent, const quat& rotation)
	{
		Gnode& self = node(parent, {}, Zero3, rotation);
		Light& l = light(self, LightType::Direct, true, Colour{ 0.8f, 0.8f, 0.7f }, 1.f);
		l.m_energy = 0.6f;
		l.m_shadow_flags = CSM_Stabilize;
#ifdef MUD_PLATFORM_EMSCRIPTEN
		l.m_shadow_num_splits = 2;
#else
		l.m_shadow_num_splits = 4;
#endif
		return l;
	}

	Light& sun_light(Gnode& parent, float azimuth, float elevation)
	{
		return direct_light_node(parent, sun_rotation(azimuth, elevation));
	}

	quat facing(const vec3& direction)
	{
		float angle = atan2(direction.x, direction.z);
		return { cosf(angle / 2.f), 0.f, 1.f * sinf(angle / 2.f), 0.f };
	}

	Light& direct_light_node(Gnode& parent, const vec3& direction)
	{
		return direct_light_node(parent, facing(direction));
	}

	Light& direct_light_node(Gnode& parent)
	{
		return direct_light_node(parent, quat{ vec3{ -c_pi / 4.f, -c_pi / 4.f, 0.f } });
	}

	void radiance(Gnode& parent, const string& texture, BackgroundMode background)
	{
		parent.m_scene->m_environment.m_radiance.m_texture = parent.m_scene->m_gfx_system.textures().file(texture.c_str());
		parent.m_scene->m_environment.m_background.m_mode = background;
	}

	void custom_sky(Gnode& parent, std::function<void(Render&)> renderer)
	{
		parent.m_scene->m_environment.m_background.m_custom_function = renderer;
		parent.m_scene->m_environment.m_background.m_mode = BackgroundMode::Custom;
	}

	void manual_job(Gnode& parent, PassType pass, std::function<void(const Pass&)> job)
	{
		parent.m_scene->m_pass_jobs->m_jobs[size_t(pass)].push_back(job);
	}

	Material& pbr_material(GfxSystem& gfx_system, cstring name, const PbrMaterialBlock& pbr_block)
	{
		Program& program = *gfx_system.programs().file("pbr/pbr");
		Material& material = gfx_system.materials().fetch(name);
		material.m_program = &program;
		material.m_pbr_block = pbr_block;
		return material;
	}

	Material& pbr_material(GfxSystem& gfx_system, cstring name, const Colour& albedo, float metallic, float roughness)
	{
		return pbr_material(gfx_system, name, { albedo, metallic, roughness });
	}
}
}
