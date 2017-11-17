//AABB.h
#ifndef _AABB_H_
#define _AABB_H_
#include "VEC3.h"

class AABB
{
public:
	//! Default Constructor.	
	AABB(){};
	AABB(const VEC3 &min, const VEC3& max): MinEdge(min), MaxEdge(max) {}


	//! Constructor with only one point.
	explicit AABB(const VEC3& init): MinEdge(init), MaxEdge(init) {}
	//! Constructor with min edge and max edge as single values, not vectors.
	AABB(f32 minx, f32 miny, f32 minz, f32 maxx, f32 maxy, f32 maxz): MinEdge(minx, miny, minz), MaxEdge(maxx, maxy, maxz) {}

	// operators
	//! Equality operator
	/** \param other box to compare with.
		\return True if both boxes are equal, else false. */
	inline bool operator==(const AABB& other) const { return (MinEdge == other.MinEdge && other.MaxEdge == MaxEdge);}
	//! Inequality operator
	/** \param other box to compare with.
		\return True if both boxes are different, else false. */
	inline bool operator!=(const AABB& other) const { return !(MinEdge == other.MinEdge && other.MaxEdge == MaxEdge);}

	// functions

	//! Adds a point to the bounding box
	/** The box grows bigger, if point was outside of the box.
		\param p: Point to add into the box. */
	void addInternalPoint(const VEC3& p)
	{
		addInternalPoint(p.X, p.Y, p.Z);
	}

	//! Adds another bounding box
	/** The box grows bigger, if the new box was outside of the box.
		\param b: Other bounding box to add into this box. */
	void addInternalBox(const AABB& b)
	{
		addInternalPoint(b.MaxEdge);
		addInternalPoint(b.MinEdge);
	}

	//! Resets the bounding box to a one-point box.
	/** \param x X coord of the point.
		\param y Y coord of the point.
		\param z Z coord of the point. */
	void reset(f32 x, f32 y, f32 z)
	{
		MaxEdge.set(x,y,z);
		MinEdge = MaxEdge;
	}

	//! Resets the bounding box.
	/** \param initValue New box to set this one to. */
	void reset(const AABB& initValue)
	{
		*this = initValue;
	}

	//! Resets the bounding box to a one-point box.
	/** \param initValue New point. */
	void reset(const VEC3& initValue)
	{
		MaxEdge = initValue;
		MinEdge = initValue;
	}
//#undef max
//#undef min
//	//! Invalidate bounding box
//	void invalidate()
//	{
//		MaxEdge = VEC3(std::numeric_limits<f32>::min(),
//			                  std::numeric_limits<f32>::min(),
//			                  std::numeric_limits<f32>::min());
//		MinEdge = VEC3(std::numeric_limits<f32>::max(),
//			                  std::numeric_limits<f32>::max(),
//			                  std::numeric_limits<f32>::max());
//	}

	//! Adds a point to the bounding box
	/** The box grows bigger, if point is outside of the box.
		\param x X coordinate of the point to add to this box.
		\param y Y coordinate of the point to add to this box.
		\param z Z coordinate of the point to add to this box. */
	void addInternalPoint(f32 x, f32 y, f32 z)
	{
		if (x>MaxEdge.X) MaxEdge.X = x;
		if (y>MaxEdge.Y) MaxEdge.Y = y;
		if (z>MaxEdge.Z) MaxEdge.Z = z;

		if (x<MinEdge.X) MinEdge.X = x;
		if (y<MinEdge.Y) MinEdge.Y = y;
		if (z<MinEdge.Z) MinEdge.Z = z;
	}

	//! Determines if a point is within this box.
	/** \param p: Point to check.
		\return True if the point is within the box and false if not */
	bool isPointInside(const VEC3& p) const
	{
		return (p.X >= MinEdge.X && p.X <= MaxEdge.X &&
				p.Y >= MinEdge.Y && p.Y <= MaxEdge.Y &&
				p.Z >= MinEdge.Z && p.Z <= MaxEdge.Z);
	}

	//! Determines if a point is within this box and its borders.
	/** \param p: Point to check.
		\return True if the point is within the box and false if not. */
	bool isPointTotalInside(const VEC3& p) const
	{
		return (p.X > MinEdge.X && p.X < MaxEdge.X &&
				p.Y > MinEdge.Y && p.Y < MaxEdge.Y &&
				p.Z > MinEdge.Z && p.Z < MaxEdge.Z);
	}

	//! Determines if the box intersects with another box.
	/** \param other: Other box to check a intersection with.
		\return True if there is an intersection with the other box,
		otherwise false. */
	bool intersectsWithBox(const AABB& other) const
	{
		return (MinEdge <= other.MaxEdge && MaxEdge >= other.MinEdge);
	}

	//! Check if this box is completely inside the 'other' box.
	/** \param other: Other box to check against.
		\return True if this box is completly inside the other box,
		otherwise false. */
	bool isFullInside(const AABB& other) const
	{
		return MinEdge >= other.MinEdge && MaxEdge <= other.MaxEdge;
	}

	//! Tests if the box intersects with a line
	/** \param line: Line to test intersection with.
		\return True if there is an intersection , else false. */
	//bool intersectsWithLine(const line3d<f32>& line) const
	//{
	//	return intersectsWithLine(line.getMiddle(), line.getVector().normalize(),
	//							  (f32)(line.getLength() * 0.5));
	//}

	//! Tests if the box intersects with a line
	/** \param linemiddle Center of the line.
		\param linevect Vector of the line.
		\param halflength Half length of the line.
		\return True if there is an intersection, else false. */
	//bool intersectsWithLine(const VEC3& linemiddle,
	//						const VEC3& linevect,
	//						f32 halflength) const
	//{
	//	const VEC3 e = getExtent() * (f32)0.5;
	//	const VEC3 t = getCenter() - linemiddle;

	//	if ((fabs(t.X) > e.X + halflength * fabs(linevect.X)) ||
	//		(fabs(t.Y) > e.Y + halflength * fabs(linevect.Y)) ||
	//		(fabs(t.Z) > e.Z + halflength * fabs(linevect.Z)) )
	//		return false;

	//	f32 r = e.Y * (f32)fabs(linevect.Z) + e.Z * (f32)fabs(linevect.Y);
	//	if (fabs(t.Y*linevect.Z - t.Z*linevect.Y) > r )
	//		return false;

	//	r = e.X * (f32)fabs(linevect.Z) + e.Z * (f32)fabs(linevect.X);
	//	if (fabs(t.Z*linevect.X - t.X*linevect.Z) > r )
	//		return false;

	//	r = e.X * (f32)fabs(linevect.Y) + e.Y * (f32)fabs(linevect.X);
	//	if (fabs(t.X*linevect.Y - t.Y*linevect.X) > r)
	//		return false;

	//	return true;
	//}

	//! Classifies a relation with a plane.
	/** \param plane Plane to classify relation to.
		\return Returns ISREL3D_FRONT if the box is in front of the plane,
		ISREL3D_BACK if the box is behind the plane, and
		ISREL3D_CLIPPED if it is on both sides of the plane. */
	/*EIntersectionRelation3D classifyPlaneRelation(const plane3d<f32>& plane) const
	{
		VEC3 nearPoint(MaxEdge);
		VEC3 farPoint(MinEdge);

		if (plane.Normal.X > (f32)0)
		{
			nearPoint.X = MinEdge.X;
			farPoint.X = MaxEdge.X;
		}

		if (plane.Normal.Y > (f32)0)
		{
			nearPoint.Y = MinEdge.Y;
			farPoint.Y = MaxEdge.Y;
		}

		if (plane.Normal.Z > (f32)0)
		{
			nearPoint.Z = MinEdge.Z;
			farPoint.Z = MaxEdge.Z;
		}

		if (plane.Normal.dotProduct(nearPoint) + plane.D > (f32)0)
			return ISREL3D_FRONT;

		if (plane.Normal.dotProduct(farPoint) + plane.D > (f32)0)
			return ISREL3D_CLIPPED;

		return ISREL3D_BACK;
	}*/


	//! Get center of the bounding box
	/** \return Center of the bounding box. */
	VEC3 getCenter() const
	{
		return (MinEdge + MaxEdge) / 2;
	}


	//! Get extent of the box
	/** \return Extent of the bounding box. */
	VEC3 getExtent() const
	{
		return MaxEdge - MinEdge;
	}


	//! Stores all 8 edges of the box into an array
	/** \param edges: Pointer to array of 8 edges. */
	void getEdges(VEC3 *edges) const
	{
		const VEC3 middle = getCenter();
		const VEC3 diag = middle - MaxEdge;

		/*
		  Edges are stored in this way:
		  Hey, am I an ascii artist, or what? :) niko.
		  /3--------/7
		  /  |      / |
		  /   |     /  |
		  1---------5  |
		  |   2- - -| -6
		  |  /      |  /
		  |/        | /
		  0---------4/
		*/

		edges[0].set(middle.X + diag.X, middle.Y + diag.Y, middle.Z + diag.Z);
		edges[1].set(middle.X + diag.X, middle.Y - diag.Y, middle.Z + diag.Z);
		edges[2].set(middle.X + diag.X, middle.Y + diag.Y, middle.Z - diag.Z);
		edges[3].set(middle.X + diag.X, middle.Y - diag.Y, middle.Z - diag.Z);
		edges[4].set(middle.X - diag.X, middle.Y + diag.Y, middle.Z + diag.Z);
		edges[5].set(middle.X - diag.X, middle.Y - diag.Y, middle.Z + diag.Z);
		edges[6].set(middle.X - diag.X, middle.Y + diag.Y, middle.Z - diag.Z);
		edges[7].set(middle.X - diag.X, middle.Y - diag.Y, middle.Z - diag.Z);
	}


	//! Check if the box is empty.
	/** This means that there is no space between the min and max
		edge.
		\return True if box is empty, else false. */
	bool isEmpty() const
	{
		return MinEdge.equals ( MaxEdge );
	}


	//! Repairs the box.
	/** Necessary if for example MinEdge and MaxEdge are swapped. */
	void repair()
	{
		f32 t;

		if (MinEdge.X > MaxEdge.X)
		{
			t=MinEdge.X; 
			MinEdge.X = MaxEdge.X; 
			MaxEdge.X=t; 
		}
		if (MinEdge.Y > MaxEdge.Y)
		{
			t=MinEdge.Y;
			MinEdge.Y = MaxEdge.Y;
			MaxEdge.Y=t;
		}
		if (MinEdge.Z > MaxEdge.Z)
		{ 
			t=MinEdge.Z;
			MinEdge.Z = MaxEdge.Z;
			MaxEdge.Z=t; 
		}
	}
	//! Calculates a new interpolated bounding box.
	/** \param other: other box to interpolate between
		\param d: value between 0.0f and 1.0f.
		\return Interpolated box. */
	AABB getInterpolated(const AABB& other, f32 d) const
	{
		f32 inv = 1.0f - d;
		return AABB((other.MinEdge*inv) + (MinEdge*d),
						   (other.MaxEdge*inv) + (MaxEdge*d));
	}

	//! Get the volume enclosed by the box in cubed units
	f32 getVolume() const
	{
		const VEC3 e = getExtent();
		return e.X * e.Y * e.Z;
	}

	//! Get the surface area of the box in squared units
	f32 getArea() const
	{
		const VEC3 e = getExtent();
		return 2*(e.X*e.Y + e.X*e.Z + e.Y*e.Z);
	}




	VEC3			MinEdge,MaxEdge;
};

#endif //_AABB_H_