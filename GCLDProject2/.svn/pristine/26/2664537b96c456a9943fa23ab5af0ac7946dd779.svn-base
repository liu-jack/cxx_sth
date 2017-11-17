#ifndef __VECTOR_3D_H_INCLUDED__
#define __VECTOR_3D_H_INCLUDED__

//! returns if a equals b, taking possible rounding errors into account
inline bool equals_f32(const f32 a, const f32 b, const f32 tolerance = ROUNDING_ERROR_32)
{
	return (a + tolerance >= b) && (a - tolerance <= b);
}

//! returns if a equals zero, taking rounding errors into account
inline bool iszero_f32(const f32 a, const f32 tolerance = ROUNDING_ERROR_32)
{
	return fabsf( a ) <= tolerance;
}

//! 3d vector class with lots of operators and methods.

class VEC3
{
public:

	//!
	typedef f32 SValueType;

	//! Default constructor (null vector).
	VEC3() : X(0), Y(0), Z(0) {}
	//! Constructor with three different values
	VEC3(f32 nx, f32 ny, f32 nz) : X(nx), Y(ny), Z(nz) {}
	//! Constructor with the same value for all elements
	explicit VEC3(f32 n) : X(n), Y(n), Z(n) {}
	//! Copy constructor
	VEC3(const VEC3& other) : X(other.X), Y(other.Y), Z(other.Z) {}

	//! Copy conversion constructor
	//template <typename T2>
	//explicit VEC3(const VEC3<T2>& other) : X(f32 (other.X)), Y(f32 (other.Y)), Z(f32 (other.Z)) {}

	//! Copy conversion constructor
	//template <typename T2>
	//explicit VEC3(const vector2d<T2>& xy, f32  z = f32 (0))
	//	: X(f32 (xy.X)),
	//	  Y(f32 (xy.Y)),
	//	  Z(z)
	//{
	//}

	//!
	//template <typename T2>
	//explicit VEC3(const vector4d<T2>& other);

	// operators

	//void setX(f32 x) { X = x; }
	//void setX(f32 y) { Y = y; }
	//void setX(f32 z) { Z = z; }

	//!
	f32 & operator [] (u32 i)
	{
//		GLITCH_DEBUG_BREAK_IF(i >= 3);
		return getDataPtr()[i];
	}

	//!
	const f32 & operator [] (u32 i) const
	{
//		GLITCH_DEBUG_BREAK_IF(i >= 3);
		return getDataPtr()[i];
	}

	VEC3 operator-() const { return VEC3(-X, -Y, -Z); }

	VEC3& operator=(const VEC3& other) { X = other.X; Y = other.Y; Z = other.Z; return *this; }

	VEC3 operator+(const VEC3& other) const { return VEC3(X + other.X, Y + other.Y, Z + other.Z); }
	VEC3& operator+=(const VEC3& other) { X+=other.X; Y+=other.Y; Z+=other.Z; return *this; }
	VEC3 operator+(const f32 val) const { return VEC3(X + val, Y + val, Z + val); }
	VEC3& operator+=(const f32 val) { X+=val; Y+=val; Z+=val; return *this; }

	VEC3 operator-(const VEC3& other) const { return VEC3(X - other.X, Y - other.Y, Z - other.Z); }
	VEC3& operator-=(const VEC3& other) { X-=other.X; Y-=other.Y; Z-=other.Z; return *this; }
	VEC3 operator-(const f32 val) const { return VEC3(X - val, Y - val, Z - val); }
	VEC3& operator-=(const f32 val) { X-=val; Y-=val; Z-=val; return *this; }

	VEC3 operator*(const VEC3& other) const { return VEC3(X * other.X, Y * other.Y, Z * other.Z); }
	VEC3& operator*=(const VEC3& other) { X*=other.X; Y*=other.Y; Z*=other.Z; return *this; }
	VEC3 operator*(const f32 v) const { return VEC3(X * v, Y * v, Z * v); }
	VEC3& operator*=(const f32 v) { X*=v; Y*=v; Z*=v; return *this; }

	VEC3 operator/(const VEC3& other) const { return VEC3(X / other.X, Y / other.Y, Z / other.Z); }
	VEC3& operator/=(const VEC3& other) { X/=other.X; Y/=other.Y; Z/=other.Z; return *this; }
	VEC3 operator/(const f32 v) const { f32  i=(f32)1.0/v; return VEC3(X * i, Y * i, Z * i); }
	VEC3& operator/=(const f32 v) { f32  i=(f32)1.0/v; X*=i; Y*=i; Z*=i; return *this; }

	bool operator<=(const VEC3&other) const { return X<=other.X && Y<=other.Y && Z<=other.Z;}
	bool operator>=(const VEC3&other) const { return X>=other.X && Y>=other.Y && Z>=other.Z;}
	bool operator<(const VEC3&other) const { return X<other.X && Y<other.Y && Z<other.Z;}
	bool operator>(const VEC3&other) const { return X>other.X && Y>other.Y && Z>other.Z;}

	//! use weak float compare
	bool operator==(const VEC3& other) const
	{
		return this->equals(other);
	}

	bool operator!=(const VEC3& other) const
	{
		return !this->equals(other);
	}

	// functions

	//! returns if this vector equals the other one, taking floating point rounding errors into account
	bool equals(const VEC3& other, const f32 tolerance = (f32)ROUNDING_ERROR_32 ) const
	{
		return equals_f32(X, other.X, tolerance) &&
			equals_f32(Y, other.Y, tolerance) &&
			equals_f32(Z, other.Z, tolerance);
	}

	VEC3& set(const f32 nx, const f32 ny, const f32 nz)
	{
		X = nx;
		Y = ny;
		Z = nz;
		return *this;
	}

	VEC3& set(const VEC3& p)
	{
		X = p.X;
		Y = p.Y;
		Z = p.Z;
		return *this;
	}

	//VEC3& set(const vector2d& p)
	//{
	//	X = p.X;
	//	Y = p.Y;
	//	return *this;
	//}

	//VEC3& set(const vector2d& p, f32  z)
	//{
	//	X = p.X;
	//	Y = p.Y;
	//	Z = z;
	//	return *this;
	//}

	//! Get length of the vector.
	f32 getLength() const { return (f32) sqrt((f64)(X*X + Y*Y + Z*Z)); }

	//! Get squared length of the vector.
	/** This is useful because it is much faster than getLength().
		\return Squared length of the vector. */
	f32 getLengthSQ() const { return X*X + Y*Y + Z*Z; }

	//! Get the dot product with another vector.
	f32 dotProduct(const VEC3& other) const
	{
		return X*other.X + Y*other.Y + Z*other.Z;
	}

	//! Get distance from another point.
	/** Here, the vector is interpreted as point in 3 dimensional space. */
	f32 getDistanceFrom(const VEC3& other) const
	{
		return VEC3(X - other.X, Y - other.Y, Z - other.Z).getLength();
	}

	//! Returns squared distance from another point.
	/** Here, the vector is interpreted as point in 3 dimensional space. */
	f32 getDistanceFromSQ(const VEC3& other) const
	{
		return VEC3(X - other.X, Y - other.Y, Z - other.Z).getLengthSQ();
	}

	f32 getXZDistanceFromSQ(const VEC3& other) const
	{
		float dx = X - other.X;
		float dz = Z - other.Z;
		return dx * dx + dz * dz;
	}

	//! Calculates the cross product with another vector.
	/** \param p Vector to multiply with.
		\return Crossproduct of this vector with p. */
	VEC3 crossProduct(const VEC3& p) const
	{
#ifdef GLITCH_USE_RIGHT_HAND_CONVENTION
		return VEC3(-Y * p.Z + Z * p.Y, -Z * p.X + X * p.Z, -X * p.Y + Y * p.X);
#else
		return VEC3(Y * p.Z - Z * p.Y, Z * p.X - X * p.Z, X * p.Y - Y * p.X);
#endif
	}

	//! Returns if this vector interpreted as a point is on a line between two other points.
	/** It is assumed that the point is on the line.
		\param begin Beginning vector to compare between.
		\param end Ending vector to compare between.
		\return True if this vector is between begin and end, false if not. */
	bool isBetweenPoints(const VEC3& begin, const VEC3& end) const
	{
		const f32 f = (end - begin).getLengthSQ();
		return getDistanceFromSQ(begin) <= f &&
			getDistanceFromSQ(end) <= f;
	}

	//! Normalizes the vector.
	/** In case of the 0 vector the result is still 0, otherwise
		the length of the vector will be 1.
		TODO: 64 Bit template doesnt work.. need specialized template
		\return Reference to this vector after normalization. */
	VEC3& normalize()
	{
		f32  l = X*X + Y*Y + Z*Z;
		if (l == 0)
			return *this;
	//	l = (f32) reciprocal_squareroot ( (f32)l );
		l = (f32) 1.0f / sqrtf ( (f32)l );
		X *= l;
		Y *= l;
		Z *= l;
		return *this;
	}

	//! Sets the length of the vector to a new value
	VEC3& setLength(f32  newlength)
	{
		normalize();
		return (*this *= newlength);
	}

	//! Inverts the vector.
	VEC3& invert()
	{
		X *= -1.0f;
		Y *= -1.0f;
		Z *= -1.0f;
		return *this;
	}

	//! Rotates the vector by a specified number of degrees around the Y axis and the specified center.
	/** \param degrees Number of degrees to rotate around the Y axis.
		\param center The center of the rotation. */
	void rotateXZBy(f64 degrees, const VEC3& center=VEC3())
	{
		degrees *= DEGTORAD64;
		f32  cs = (f32)cos(degrees);
		f32  sn = (f32)sin(degrees);
		X -= center.X;
		Z -= center.Z;
		set(X*cs - Z*sn, Y, X*sn + Z*cs);
		X += center.X;
		Z += center.Z;
	}

	void rotateXZByRad(f64 rad)
	{
		f32  cs = (f32)cos(rad);
		f32  sn = (f32)sin(rad);
		set(Z*sn - X*cs, Y, X*sn + Z*cs);
	}

	f32 getXZRotateRad() const
	{
		return (f32)atan2(X, Z) +PI;
	}

	//! Rotates the vector by a specified number of degrees around the Z axis and the specified center.
	/** \param degrees: Number of degrees to rotate around the Z axis.
		\param center: The center of the rotation. */
	void rotateXYBy(f64 degrees, const VEC3& center=VEC3())
	{
		degrees *= DEGTORAD64;
		f32  cs = (f32)cos(degrees);
		f32  sn = (f32)sin(degrees);
		X -= center.X;
		Y -= center.Y;
		set(X*cs - Y*sn, X*sn + Y*cs, Z);
		X += center.X;
		Y += center.Y;
	}

	//! Rotates the vector by a specified number of degrees around the X axis and the specified center.
	/** \param degrees: Number of degrees to rotate around the X axis.
		\param center: The center of the rotation. */
	void rotateYZBy(f64 degrees, const VEC3& center=VEC3())
	{
		degrees *= DEGTORAD64;
		f32  cs = (f32)cos(degrees);
		f32  sn = (f32)sin(degrees);
		Z -= center.Z;
		Y -= center.Y;
		set(X, Y*cs - Z*sn, Y*sn + Z*cs);
		Z += center.Z;
		Y += center.Y;
	}

	//! Returns interpolated vector.
	/** \param other Other vector to interpolate between
		\param d Value between 0.0f and 1.0f. */
	VEC3 getInterpolated(const VEC3& other, const f32 d) const
	{
		const f32 inv = (f32) 1.0 - d;
		return VEC3(other.X*inv + X*d, other.Y*inv + Y*d, other.Z*inv + Z*d);
	}

	//! Returns interpolated vector. ( quadratic )
	/** \param v2 Second vector to interpolate with
		\param v3 Third vector to interpolate with
		\param d Value between 0.0f and 1.0f. */
	VEC3 getInterpolated_quadratic(const VEC3& v2, const VEC3& v3, const f32 d) const
	{
		// this*(1-d)*(1-d) + 2 * v2 * (1-d) + v3 * d * d;
		const f32 inv = (f32) 1.0 - d;
		const f32 mul0 = inv * inv;
		const f32 mul1 = (f32) 2.0 * d * inv;
		const f32 mul2 = d * d;

		return VEC3 ( X * mul0 + v2.X * mul1 + v3.X * mul2,
							 Y * mul0 + v2.Y * mul1 + v3.Y * mul2,
							 Z * mul0 + v2.Z * mul1 + v3.Z * mul2);
	}

	//! Gets the Y and Z rotations of a vector.
	/** Thanks to Arras on the Irrlicht forums for this method.
		\return A vector representing the rotation in degrees of
		this vector. The Z component of the vector will always be 0. */
	VEC3 getHorizontalAngle() const
	{
		VEC3 angle;

		angle.Y = (f32)(atan2(X, Z) * RADTODEG64);

		if (angle.Y < 0.0f)
			angle.Y += 360.0f;
		if (angle.Y >= 360.0f)
			angle.Y -= 360.0f;

		const f64 z1 = sqrt(X*X + Z*Z);

		angle.X = (f32)(atan2(z1, (f64)Y) * RADTODEG64 - 90.0);

		if (angle.X < 0.0f)
			angle.X += 360.0f;
		if (angle.X >= 360.0f)
			angle.X -= 360.0f;

		return angle;
	}

	//! Builds a direction vector from (this) rotation vector.
	/** This vector is assumed to hold 3 Euler angle rotations, in degrees.
		The implementation performs the same calculations as using a matrix to
		do the rotation.
		\param[in] forwards  The direction representing "forwards" which will be
		rotated by this vector.  If you do not provide a
		direction, then the positive Z axis (0, 0, 1) will
		be assumed to be fowards.
		\return A vector calculated by rotating the forwards direction by
		the 3 Euler angles that this vector is assumed to represent. */
	VEC3 rotationToDirection(const VEC3 & forwards = VEC3(0, 0, 1)) const
	{
		const f64 cr = cos( DEGTORAD64 * X );
		const f64 sr = sin( DEGTORAD64 * X );
		const f64 cp = cos( DEGTORAD64 * Y );
		const f64 sp = sin( DEGTORAD64 * Y );
		const f64 cy = cos( DEGTORAD64 * Z );
		const f64 sy = sin( DEGTORAD64 * Z );

		const f64 srsp = sr*sp;
		const f64 crsp = cr*sp;

		const f64 pseudoMatrix[] = {
			( cp*cy ), ( cp*sy ), ( -sp ),
			( srsp*cy-cr*sy ), ( srsp*sy+cr*cy ), ( sr*cp ),
			( crsp*cy+sr*sy ), ( crsp*sy-sr*cy ), ( cr*cp )};

		return VEC3(
			(f32)(forwards.X * pseudoMatrix[0] +
				forwards.Y * pseudoMatrix[3] +
				forwards.Z * pseudoMatrix[6]),
			(f32)(forwards.X * pseudoMatrix[1] +
				forwards.Y * pseudoMatrix[4] +
				forwards.Z * pseudoMatrix[7]),
			(f32)(forwards.X * pseudoMatrix[2] +
				forwards.Y * pseudoMatrix[5] +
				forwards.Z * pseudoMatrix[8]));
	}

	//! Fills an array of 4 values with the vector data (usually floats).
	/** Useful for setting in shader constants for example. The fourth value
		will always be 0. */
	void getAs4Values(f32* array) const
	{
		array[0] = X;
		array[1] = Y;
		array[2] = Z;
		array[3] = 0;
	}

	//!
	const f32* getDataPtr() const
	{
		return reinterpret_cast<const f32*>(this);
	}

	//!
	f32* getDataPtr()
	{
		return reinterpret_cast<f32*>(this);
	}

	//! X coordinate of the vector
	f32 X;
	//! Y coordinate of the vector
	f32 Y;
	//! Z coordinate of the vector
	f32 Z;
};


//! Function multiplying a scalar and a vector component-wise.
inline
VEC3 operator*(const f32 scalar, const VEC3& vector)
{
	return vector * scalar;
}

////------------------------------------------------------------------------------
//// vector2d definitions depending on VEC3 defs
////------------------------------------------------------------------------------
//
//template <typename f32 >
//template <typename T2>
//inline
//vector2d::vector2d(const VEC3<T2>& other)
//	: X(other.X)
//	, Y(other.Y)
//{
//}
//
//} // end namespace core

//namespace os
//{
//
//
//VEC3 byteswap(const VEC3& v)
//{
//	return VEC3(
//			os::byteswap(v.X),
//			os::byteswap(v.Y),
//			os::byteswap(v.Z)
//	);
//}
//
//} // end namespace os
//} // end namespace glitch
//
#endif

