#pragma once

#include <infra/Config.h>

#include <type/Forward.h>
#include <math/Forward.h>

#ifndef TWO_GEOM_EXPORT
#define TWO_GEOM_EXPORT TWO_IMPORT
#endif

namespace two
{
    enum DrawMode : unsigned int;
    enum class PrimitiveType : unsigned int;
    enum class SymbolDetail : unsigned short;
	enum class CatmullType : unsigned int;
    
    class Shape;
    class ShapeVar;
    struct Line;
    struct Rect;
    struct Quad;
    struct Grid2;
    struct Triangle;
    struct Circle;
    struct Torus;
	struct TorusKnot;
    struct Ring;
    struct Ellipsis;
    struct Arc;
    struct ArcLine;
    struct Cylinder;
    struct Capsule;
    struct Cube;
	struct Tetraedr;
    struct Sphere;
    struct SphereRing;
    struct Spheroid;
	struct Icosaedr;
    struct Aabb;
	class Curve2;
	class Curve3;
	struct CurveBezierCubic;
	struct CurveBezierCubic3;
	struct CurveBezierQuadratic;
	struct CurveBezierQuadratic3;
	struct CurveCatmullRom3;
	struct CurveLine;
	struct CurveLine3;
	struct CurveSpline;
	struct CurveSpline3;
    struct Plane;
    struct Plane3;
    struct Face3;
    struct Segment;
    struct Ray;
    struct Plane6;
    struct Point8;
    struct VertexAttribute;
    struct Vertex;
    struct Tri;
    struct ShapeVertex;
    struct ShapeTriangle;
    struct MeshAdapter;
    class Geometry;
    struct MeshPacker;
	class MarchingCubes;
    class Distribution;
    class Poisson;
    class RandomShapePoint;
    struct Polygon;
    struct Box;
    struct Points;
    struct Grid3;
    struct ConvexHull;
    struct Symbol;
    struct ShapeSize;
    struct ProcShape;
    struct CompoundShape;
    struct DispatchDrawProcShape;
    class IcoSphere;
}

#ifdef TWO_META_GENERATOR
#include <stl/vector.h>
#include <stl/span.h>
namespace stl
{
	export_ extern template struct refl_ span_ span<two::vec3>;

	export_ extern template class refl_ seque_ vector<two::vec2>;
	export_ extern template class refl_ seque_ vector<two::vec4>;
	export_ extern template class refl_ seque_ vector<two::ivec4>;

	export_ extern template class refl_ seque_ vector<two::Circle>;
}
#endif
