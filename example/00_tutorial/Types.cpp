

#include <infra/Cpp20.h>

#ifdef MUD_MODULES
module ._00_tutorial;
#else
#include <00_tutorial/Types.h>
#include <00_tutorial/Api.h>
#include <type/Vector.h>
//#include <ecs/Proto.h>
#endif

namespace mud
{
    // Exported types
    template <> _00_TUTORIAL_EXPORT Type& type<ShapeType>() { static Type ty("ShapeType"); return ty; }
    
    template <> _00_TUTORIAL_EXPORT Type& type<MyObject>() { static Type ty("MyObject"); return ty; }
}
