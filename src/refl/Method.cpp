//  Copyright (c) 2019 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <infra/Cpp20.h>

#ifdef MUD_MODULES
module mud.refl;
#else
#include <refl/Method.h>
#include <refl/Meta.h>
#include <refl/Class.h>
#include <infra/Vector.h>
#endif

namespace mud
{
	template <> Type& type<VirtualMethod>() { static Type ty("VirtualMethod"); return ty; }

	Param::Param(cstring name, Var value, Flags flags)
		: m_index(0)
		, m_name(name)
		, m_value(value)
		, m_flags(flags)
	{
		if(value == Ref())
			m_flags = static_cast<Flags>(m_flags | Nullable);
	}

	Signature::Signature(const std::vector<Param>& paramvec, const Var& returnval)
		: m_params(paramvec)
		, m_returnval(returnval)
	{}

	static uint32_t s_callable_index = 0;

	Callable::Callable(cstring name, const std::vector<Param>& paramvec, Var returnval)
		: m_index(++s_callable_index)
		, m_name(name)
		, m_returnval(returnval)
		, m_params(paramvec)
		, m_num_defaults(0)
	{
		this->setup();
	}

	void Callable::setup()
	{
		for(size_t i = 0; i < m_params.size(); ++i)
		{
			m_params[i].m_index = i;
			m_arguments.emplace_back(m_params[i].m_value);

			if(m_num_defaults == 0 && m_params[i].defaulted())
				m_num_defaults = m_params.size() - i;
		}
		
		m_num_required = m_params.size() - m_num_defaults;
	}

	bool Callable::validate(array<Var> args, size_t offset) const
	{
		if(args.m_count < m_arguments.size() - m_num_defaults)
			return false;

		bool valid = true;
		for(size_t i = offset; i < m_params.size(); ++i)
		{
			valid &= type(m_params[i].m_value).is(type(args[i]));
			valid &= m_params[i].nullable() || !args[i].null();
		}
		return valid;
	}

	Function::Function(Namespace* location, cstring name, FunctionPointer identity, FunctionFunc trigger, const std::vector<Param>& paramvec, Var returnval)
		: Callable(name, paramvec, returnval)
		, m_namespace(location)
		, m_identity(identity)
		, m_call(trigger)
	{
		Operator op = as_operator(*this);
		if(op && is_class(*op.m_type))
		{
			cls(*op.m_type).m_operators.push_back(op);
		}
	}

	Operator as_operator(Function& function)
	{
		if(function.m_name == string("add"))
		{
			return { &function, &type(function.m_params[0].m_value), function.m_name, "+" };
		}
		else if(function.m_name == string("subtract"))
		{
			return { &function, &type(function.m_params[0].m_value), function.m_name, "-" };
		}
		else if(function.m_name == string("multiply"))
		{
			return { &function, &type(function.m_params[0].m_value), function.m_name, "*" };
		}
		else if(function.m_name == string("divide"))
		{
			return { &function, &type(function.m_params[0].m_value), function.m_name,  "/" };
		}
		else return {};
	}

	Method::Method(Type& object_type, cstring name, Address address, MethodFunc trigger, const std::vector<Param>& paramvec, Var returnval)
		: Callable(name, vector_union({ { "self", Ref(object_type) } }, paramvec), returnval)
		, m_object_type(&object_type)
		, m_address(address)
		, m_call(trigger)
	{}

	Constructor::Constructor(Type& object_type, ConstructorFunc constructor, const std::vector<Param>& paramvec)
		: Callable(object_type.m_name, vector_union({ { "self", Ref(object_type) } }, paramvec))
		, m_object_type(&object_type)
		, m_call(constructor)
	{}

	Constructor::Constructor(Type& object_type, cstring name, ConstructorFunc constructor, const std::vector<Param>& paramvec)
		: Callable(name, vector_union({ { "self", Ref(object_type) } }, paramvec))
		, m_object_type(&object_type)
		, m_call(constructor)
	{}

	CopyConstructor::CopyConstructor(Type& object_type, CopyConstructorFunc constructor)
		: Callable(object_type.m_name, { { "self", Ref(object_type) } })
		, m_object_type(&object_type)
		, m_call(constructor)
	{}

	Destructor::Destructor(Type& object_type, DestructorFunc destructor)
		: Callable(object_type.m_name, { { "self", Ref(object_type) } })
		, m_object_type(&object_type)
		, m_call(destructor)
	{}

	Call::Call()
	{}

	Call::Call(const Callable& callable, std::vector<Var> arguments)
		: m_callable(&callable)
		, m_arguments(arguments)
		, m_result(callable.m_returnval)
	{}

	Call::Call(const Callable& callable)
		: Call(callable, callable.m_arguments)
	{}

	Call::Call(const Callable& callable, Ref object)
		: Call(callable)
	{
		m_arguments[0] =  object;
	}

	bool Call::validate() { return m_callable && m_callable->validate(to_array(m_arguments)); }

	const Var& Call::operator()() { (*m_callable)(to_array(m_arguments), m_result); return m_result; }
	const Var& Call::operator()(Ref object) { m_arguments[0] = object; (*m_callable)(to_array(m_arguments), m_result); return m_result; }
}
