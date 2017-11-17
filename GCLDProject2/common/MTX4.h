
#ifndef __MATRIX_H_INCLUDED__
#define __MATRIX_H_INCLUDED__


#include "AABB.h"
#include "VEC3.h"


//! 4x4 matrix. Mostly used as transformation matrix for 3d calculations.
/** The matrix is a D3D style matrix, row major with translations in the 4th row. */
class MTX4
{
public:

	//! Constructor Flags
	enum eConstructor
	{
		EM4CONST_NOTHING = 0,
		EM4CONST_COPY,
		EM4CONST_IDENTITY,
		EM4CONST_TRANSPOSED,
		EM4CONST_INVERSE,
		EM4CONST_INVERSE_TRANSPOSED
	};

	//! Default constructor
	/** \param constructor Choose the initialization style */
	MTX4( eConstructor constructor = EM4CONST_IDENTITY );
	//! Copy constructor
	/** \param other Other matrix to copy from
		\param constructor Choose the initialization style */
#if 1
	MTX4( const MTX4& other,eConstructor constructor = EM4CONST_COPY);
#else
	MTX4( const MTX4& other,eConstructor constructor);
#endif
	//! Simple operator for directly accessing every element of the matrix.
	f32& operator()(const s32 row, const s32 col) { 
		definitelyIdentityMatrix=false;
		return M[ row * 4 + col ]; 
	}

	//! Simple operator for directly accessing every element of the matrix.
	const f32& operator()(const s32 row, const s32 col) const { 
		return M[row * 4 + col];
	}

	//! Simple operator for linearly accessing every element of the matrix.
	f32& operator[](u32 index) { 
		definitelyIdentityMatrix=false; 
		return M[index]; 
	}

	//! Simple operator for linearly accessing every element of the matrix.
	const f32& operator[](u32 index) const { 
		return M[index]; 
	}
#if 0
	//! Sets this matrix equal to the other matrix.
	inline MTX4& operator=(const MTX4 &other);
#endif
	//! Sets all elements of this matrix to the value.
	inline MTX4& operator=(const f32& scalar);

	//! Returns pointer to internal array
	const f32* pointer() const { return M; }
	f32* pointer() { 
		definitelyIdentityMatrix=false; return M;
	}

	//! Returns true if other matrix is equal to this matrix.
	bool operator==(const MTX4 &other) const;

	//! Returns true if other matrix is not equal to this matrix.
	bool operator!=(const MTX4 &other) const;

	//! Add another matrix.
	MTX4 operator+(const MTX4& other) const;

	//! Add another matrix.
	MTX4& operator+=(const MTX4& other);

	//! Subtract another matrix.
	MTX4 operator-(const MTX4& other) const;

	//! Subtract another matrix.
	MTX4& operator-=(const MTX4& other);

	//! set this matrix to the product of two matrices
	inline MTX4& setbyproduct(const MTX4& other_a,const MTX4& other_b );

	//! Set this matrix to the product of two matrices
	/** no optimization used,
		use it if you know you never have a identity matrix */
	MTX4& setbyproduct_nocheck(const MTX4& other_a,const MTX4& other_b );

	//! Multiply by another matrix.
	MTX4 operator*(const MTX4& other) const;

	//! Multiply by another matrix.
	MTX4& operator*=(const MTX4& other);

	//! Multiply by another matrix as if both matrices where 3x4.
	MTX4 mult34(const MTX4& m2) const;

	//! Multiply by another matrix as if both matrices where 3x4.
	MTX4& mult34(const MTX4& m2, MTX4& out) const;

	//! Multiply by scalar.
	MTX4 operator*(const f32& scalar) const;

	//! Multiply by scalar.
	MTX4& operator*=(const f32& scalar);

	//! Set matrix to identity.
	inline MTX4& makeIdentity();

	//! Returns true if the matrix is the identity matrix
	inline bool isIdentity() const;

	//! Returns true if the matrix is the identity matrix
	bool isIdentity_integer_base () const;

	//! Returns the c'th column of the matrix, without the lowest row.
	VEC3 getColumn(u32 c) const;

	//! Returns the c'th column of the matrix.
//	vector4d getFullColumn(u32 c) const;

	//! Sets the c'th column of the matrix, without the lowest row.
	MTX4& setColumn(u32 c, const VEC3& v);

	//! Sets the c'th column of the matrix.
	//MTX4& setFullColumn(u32 c, const vector4d& v);

	//! Gets the current translation
	VEC3 getTranslation() const;

	//! Set the translation of the current matrix. Will erase any previous values.
	MTX4& setTranslation( const VEC3& translation );

	//! Set the inverse translation of the current matrix. Will erase any previous values.
	MTX4& setInverseTranslation( const VEC3& translation );

	//! Make a rotation matrix from Euler angles. The 4th row and column are unmodified.
	inline MTX4& setRotationRadians( const VEC3& rotation );

	//! Make a rotation matrix from Euler angles. The 4th row and column are unmodified.
	MTX4& setRotationDegrees( const VEC3& rotation );

	//! Returns the rotation, as set by setRotation().
	/** This code was orginally written by by Chev. */
	 VEC3 getRotationDegrees() const;

	//! Make an inverted rotation matrix from Euler angles.
	/** The 4th row and column are unmodified. */
	inline MTX4& setInverseRotationRadians( const VEC3& rotation );

	//! Make an inverted rotation matrix from Euler angles.
	/** The 4th row and column are unmodified. */
	MTX4& setInverseRotationDegrees( const VEC3& rotation );

	//! Set Scale
	MTX4& setScale( const VEC3& scale );

	//! Set Scale
	MTX4& setScale( const f32 scale ) {
		return setScale( VEC3(scale,scale,scale));
	}

	//! Apply scale to this matrix as if multiplication was on the left.
	MTX4& preScale( const VEC3& scale );

	//! Apply scale to this matrix as if multiplication was on the right.
	MTX4& postScale( const VEC3& scale );

	//! Get Scale
	 VEC3 getScale() const;

	//! Translate a vector by the inverse of the translation part of this matrix.
	void inverseTranslateVect( VEC3& vect ) const;

	//! Rotate a vector by the inverse of the rotation part of this matrix.
	void inverseRotateVect( VEC3& vect ) const;

	//! Rotate a vector by the rotation part of this matrix.
	void rotateVect( VEC3& vect ) const;

	//! An alternate transform vector method, writing into a second vector
	void rotateVect(VEC3& out, const VEC3& in) const;

	//! An alternate transform vector method, writing into an array of 3 floats
	void rotateVect(f32 *out,const VEC3 &in) const;

	//! Transforms the vector by this matrix
	void transformVect( VEC3& vect) const;

	//! Transforms input vector by this matrix and stores result in output vector
	void transformVect( VEC3& out, const VEC3& in ) const;

	//! Transforms the vector by this matrix as though it was in 2D (Z ignored).
	void transformVect2D( VEC3& vect) const;

	//! Transforms input vector by this matrix and stores result in output vector as though it was in 2D (Z ignored).
	void transformVect2D( VEC3& out, const VEC3& in ) const;

	//! Transforms the vector by this matrix
//	void transformVect( vector2df& vect) const;

	//! Transforms input vector by this matrix and stores result in output vector
//	void transformVect( vector2df& out, const vector2df& in ) const;

//	//! Transforms the vector representing texture coordinates by this matrix
//	void transformTexCoord( vector2df& vect) const;

	//! Transforms input vector representing texture coordinates by this matrix and stores result in output vector
//	void transformTexCoord( vector2df& out, const vector2df& in ) const;

	//! An alternate transform vector method, writing into an array of 4 floats
	void transformVect(f32 *out,const VEC3 &in) const;

	//! Translate a vector by the translation part of this matrix.
	void translateVect( VEC3& vect ) const;

	//! Transforms a plane by this matrix
//	void transformPlane(  plane3d<f32> &plane) const;

	//! Transforms a plane by this matrix ( some problems to solve..)
//	void transformPlane_new(  plane3d<f32> &plane) const;

	//! Transforms a plane by this matrix
//	void transformPlane( const  plane3d<f32> &in,  plane3d<f32> &out) const;

	//! Transforms a axis aligned bounding box
	/** The result box of this operation may not be accurate at all. For
		correct results, use transformBoxEx() */
	void transformBox( AABB& box) const;

	//! Transforms a axis aligned bounding box
	/** The result box of this operation should by accurate, but this operation
		is slower than transformBox(). */
	void transformBoxEx( AABB& box) const;

	//! Multiplies this matrix by a 1x4 matrix
	void multiplyWith1x4Matrix(f32* matrix) const;

	//! Calculates inverse of matrix. Slow.
	/** \return Returns false if there is no inverse matrix.*/
	bool makeInverse();


	//! Inverts a primitive matrix which only contains a translation and a rotation
	/** \param out: where result matrix is written to. */
	bool getInversePrimitive ( MTX4& out ) const;

	//! Gets the inversed matrix of this one
	/** \param out: where result matrix is written to.
		\return Returns false if there is no inverse matrix. */
	bool getInverse(MTX4& out) const;

	//! Builds a right-handed perspective projection matrix based on a field of view
	//MTX4& buildProjectionMatrixPerspectiveFovRH(f32 fieldOfViewRadians, f32 aspectRatio, f32 zNear, f32 zFar);

	//! Builds a right-handed perspective projection matrix based on a field of view
	MTX4& buildProjectionMatrixPerspectiveFov(f32 fieldOfViewRadians, f32 aspectRatio, f32 zNear, f32 zFar);

	//! Builds a right-handed perspective projection matrix.
	//MTX4& buildProjectionMatrixPerspectiveRH(f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar);

	//! Builds a left-handed perspective projection matrix.
	MTX4& buildProjectionMatrixPerspective(f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar);

	//! Builds a centered right-handed orthogonal projection matrix.
	MTX4& buildProjectionMatrixOrtho(f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar);

	//! Builds a right-handed orthogonal projection matrix.
	MTX4& buildProjectionMatrixOrtho(f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar);

	//! Builds a right-handed orthogonal projection matrix.
	//MTX4& buildProjectionMatrixOrthoRH(f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar);

	//! Builds a right-handed look-at matrix.
	MTX4& buildCameraLookAtMatrix(
		const VEC3& position,
		const VEC3& target,
		const VEC3& upVector);

	//! Builds a right-handed look-at matrix.
	//MTX4& buildCameraLookAtMatrixRH(
	//		const VEC3& position,
	//		const VEC3& target,
	//		const VEC3& upVector);

	//! Builds a matrix that flattens geometry into a plane.
	/** \param light: light source
		\param plane: plane into which the geometry if flattened into
		\param point: value between 0 and 1, describing the light source.
		If this is 1, it is a point light, if it is 0, it is a directional light. */
//	MTX4& buildShadowMatrix(const VEC3& light,  plane3df plane, f32 point=1.0f);

	//! Builds a matrix which transforms a normalized Device Coordinate to Device Coordinates.
	/** Used to scale <-1,-1><1,1> to viewport, for example from von <-1,-1> <1,1> to the viewport <0,0><0,640> */
	//MTX4& buildNDCToDCMatrix( const  rect<s32>& area, f32 zScale);

	//! Creates a new matrix as interpolated matrix from two other ones.
	/** \param b: other matrix to interpolate with
		\param time: Must be a value between 0 and 1. */
	MTX4 interpolate(const  MTX4& b, f32 time) const;

	//! Gets transposed matrix
	MTX4 getTransposed() const;

	//! Gets transposed matrix
	inline void getTransposed( MTX4& dest ) const;

	/*
	  construct 2D Texture transformations
	  rotate about center, scale, and transform.
	*/
	//! Set to a texture transformation matrix with the given parameters.
	//MTX4& buildTextureTransform( f32 rotateRad,
	//									const  vector2df &rotatecenter,
	//									const  vector2df &translate,
	//									const  vector2df &scale);

	//! Set texture transformation rotation
	/** Rotate about z axis, recenter at (0.5,0.5).
		Doesn't clear other elements than those affected
		\param radAngle Angle in radians
		\return Altered matrix */
	MTX4& setTextureRotationCenter( f32 radAngle );

	//! Set texture transformation translation
	/** Doesn't clear other elements than those affected.
		\param x Offset on x axis
		\param y Offset on y axis
		\return Altered matrix */
	MTX4& setTextureTranslate( f32 x, f32 y );

	//! Set texture transformation translation, using a transposed representation
	/** Doesn't clear other elements than those affected.
		\param x Offset on x axis
		\param y Offset on y axis
		\return Altered matrix */
	MTX4& setTextureTranslateTransposed( f32 x, f32 y );

	//! Set texture transformation scale
	/** Doesn't clear other elements than those affected.
		\param sx Scale factor on x axis
		\param sy Scale factor on y axis
		\return Altered matrix. */
	MTX4& setTextureScale( f32 sx, f32 sy );

	//! Set texture transformation scale, and recenter at (0.5,0.5)
	/** Doesn't clear other elements than those affected.
		\param sx Scale factor on x axis
		\param sy Scale factor on y axis
		\return Altered matrix. */
	MTX4& setTextureScaleCenter( f32 sx, f32 sy );

	//! Applies a texture post scale.
	/**	\param sx Scale factor on x axis
		\param sy Scale factor on y axis
		\return Altered matrix. */
	MTX4& postTextureScale ( f32 sx, f32 sy );

	//! Sets all matrix data members at once
	MTX4& setM(const f32* data);

	//! Gets all matrix data members at once
	/** \returns data */
	f32* getM(f32* data) const;

	//! Sets if the matrix is definitely identity matrix
	void setDefinitelyIdentityMatrix( bool isDefinitelyIdentityMatrix);

	//! Gets if the matrix is definitely identity matrix
	bool getDefinitelyIdentityMatrix() const;

	static inline void rowMatrixProduct(f32* M, const f32* m1, const f32* m2 )
	{
		GLITCH_DEBUG_BREAK_IF(M == m1 || M == m2);
		M[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2] + m1[12]*m2[3];
		M[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2] + m1[13]*m2[3];
		M[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2] + m1[14]*m2[3];
		M[3] = m1[3]*m2[0] + m1[7]*m2[1] + m1[11]*m2[2] + m1[15]*m2[3];

		M[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6] + m1[12]*m2[7];
		M[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6] + m1[13]*m2[7];
		M[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6] + m1[14]*m2[7];
		M[7] = m1[3]*m2[4] + m1[7]*m2[5] + m1[11]*m2[6] + m1[15]*m2[7];

		M[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10] + m1[12]*m2[11];
		M[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10] + m1[13]*m2[11];
		M[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10] + m1[14]*m2[11];
		M[11] = m1[3]*m2[8] + m1[7]*m2[9] + m1[11]*m2[10] + m1[15]*m2[11];

		M[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12]*m2[15];
		M[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13]*m2[15];
		M[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15];
		M[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];
	}

	static inline void rowMatrixProduct34(f32* M, const f32* m1, const f32* m2 )
	{
		GLITCH_DEBUG_BREAK_IF(M == m1 || M == m2);
		M[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2];
		M[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2];
		M[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2];
		M[3] = f32(0);

		M[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6];
		M[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6];
		M[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6];
		M[7] = f32(0);

		M[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10];
		M[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10];
		M[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10];
		M[11] = f32(0);

		M[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12];
		M[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13];
		M[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14];
		M[15] = f32(1);
	}

private:
	//! Matrix data, stored in row-major order
	f32 M[16];
	//! Flag is this matrix is identity matrix
	mutable bool definitelyIdentityMatrix;
};

// Default constructor
//template <class f32>
inline MTX4::MTX4( eConstructor constructor ) : definitelyIdentityMatrix(false)
{
	switch ( constructor )
	{
		case EM4CONST_NOTHING:
		case EM4CONST_COPY:
			break;
		case EM4CONST_IDENTITY:
		case EM4CONST_INVERSE:
		default:
			makeIdentity();
			break;
	}
}

// Copy constructor
//template <class f32>
inline MTX4::MTX4( const MTX4& other, eConstructor constructor) : definitelyIdentityMatrix(false)
{
	switch ( constructor )
	{
		case EM4CONST_IDENTITY:
			makeIdentity();
			break;
		case EM4CONST_NOTHING:
			break;
		case EM4CONST_COPY:
			*this = other;
			break;
		case EM4CONST_TRANSPOSED:
			other.getTransposed(*this);
			break;
		case EM4CONST_INVERSE:
			if (!other.getInverse(*this))
				memset(M, 0, 16*sizeof(f32));
			break;
		case EM4CONST_INVERSE_TRANSPOSED:
			if (!other.getInverse(*this))
				memset(M, 0, 16*sizeof(f32));
			else
				*this=getTransposed();
			break;
	}
}

//! Add another matrix.
//template <class f32>
inline MTX4 MTX4::operator+(const MTX4& other) const
{
	MTX4 temp ( EM4CONST_NOTHING );

	temp[0] = M[0]+other[0];
	temp[1] = M[1]+other[1];
	temp[2] = M[2]+other[2];
	temp[3] = M[3]+other[3];
	temp[4] = M[4]+other[4];
	temp[5] = M[5]+other[5];
	temp[6] = M[6]+other[6];
	temp[7] = M[7]+other[7];
	temp[8] = M[8]+other[8];
	temp[9] = M[9]+other[9];
	temp[10] = M[10]+other[10];
	temp[11] = M[11]+other[11];
	temp[12] = M[12]+other[12];
	temp[13] = M[13]+other[13];
	temp[14] = M[14]+other[14];
	temp[15] = M[15]+other[15];

	return temp;
}

//! Add another matrix.
//template <class f32>
inline MTX4& MTX4::operator+=(const MTX4& other)
{
	M[0]+=other[0];
	M[1]+=other[1];
	M[2]+=other[2];
	M[3]+=other[3];
	M[4]+=other[4];
	M[5]+=other[5];
	M[6]+=other[6];
	M[7]+=other[7];
	M[8]+=other[8];
	M[9]+=other[9];
	M[10]+=other[10];
	M[11]+=other[11];
	M[12]+=other[12];
	M[13]+=other[13];
	M[14]+=other[14];
	M[15]+=other[15];

	return *this;
}

//! Subtract another matrix.
//template <class f32>
inline MTX4 MTX4::operator-(const MTX4& other) const
{
	MTX4 temp ( EM4CONST_NOTHING );

	temp[0] = M[0]-other[0];
	temp[1] = M[1]-other[1];
	temp[2] = M[2]-other[2];
	temp[3] = M[3]-other[3];
	temp[4] = M[4]-other[4];
	temp[5] = M[5]-other[5];
	temp[6] = M[6]-other[6];
	temp[7] = M[7]-other[7];
	temp[8] = M[8]-other[8];
	temp[9] = M[9]-other[9];
	temp[10] = M[10]-other[10];
	temp[11] = M[11]-other[11];
	temp[12] = M[12]-other[12];
	temp[13] = M[13]-other[13];
	temp[14] = M[14]-other[14];
	temp[15] = M[15]-other[15];

	return temp;
}

//! Subtract another matrix.
//template <class f32>
inline MTX4& MTX4::operator-=(const MTX4& other)
{
	M[0]-=other[0];
	M[1]-=other[1];
	M[2]-=other[2];
	M[3]-=other[3];
	M[4]-=other[4];
	M[5]-=other[5];
	M[6]-=other[6];
	M[7]-=other[7];
	M[8]-=other[8];
	M[9]-=other[9];
	M[10]-=other[10];
	M[11]-=other[11];
	M[12]-=other[12];
	M[13]-=other[13];
	M[14]-=other[14];
	M[15]-=other[15];

	return *this;
}

//! Multiply by scalar.
//template <class f32>
inline MTX4 MTX4::operator*(const f32& scalar) const
{
	MTX4 temp ( EM4CONST_NOTHING );

	temp[0] = M[0]*scalar;
	temp[1] = M[1]*scalar;
	temp[2] = M[2]*scalar;
	temp[3] = M[3]*scalar;
	temp[4] = M[4]*scalar;
	temp[5] = M[5]*scalar;
	temp[6] = M[6]*scalar;
	temp[7] = M[7]*scalar;
	temp[8] = M[8]*scalar;
	temp[9] = M[9]*scalar;
	temp[10] = M[10]*scalar;
	temp[11] = M[11]*scalar;
	temp[12] = M[12]*scalar;
	temp[13] = M[13]*scalar;
	temp[14] = M[14]*scalar;
	temp[15] = M[15]*scalar;

	return temp;
}

//! Multiply by scalar.
//template <class f32>
inline MTX4& MTX4::operator*=(const f32& scalar)
{
	M[0]*=scalar;
	M[1]*=scalar;
	M[2]*=scalar;
	M[3]*=scalar;
	M[4]*=scalar;
	M[5]*=scalar;
	M[6]*=scalar;
	M[7]*=scalar;
	M[8]*=scalar;
	M[9]*=scalar;
	M[10]*=scalar;
	M[11]*=scalar;
	M[12]*=scalar;
	M[13]*=scalar;
	M[14]*=scalar;
	M[15]*=scalar;

	return *this;
}

//! Multiply by another matrix.
//template <class f32>
inline MTX4& MTX4::operator*=(const MTX4& other)
{
	// do checks on your own in order to avoid copy creation
	if ( !other.getDefinitelyIdentityMatrix() )
	{
		if ( this->getDefinitelyIdentityMatrix() )
		{
			return (*this = other);
		}
		else
		{
			MTX4 temp ( *this );
			return setbyproduct_nocheck( temp, other );
		}
	}
	return *this;
}

//! multiply by another matrix
// set this matrix to the product of two other matrices
// goal is to reduce stack use and copy
//template <class f32>
inline MTX4& MTX4::setbyproduct_nocheck(const MTX4& other_a,const MTX4& other_b )
{
	const f32 *m1 = other_a.M;
	const f32 *m2 = other_b.M;

	rowMatrixProduct(M, m1, m2);
	/*M[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2] + m1[12]*m2[3];
	  M[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2] + m1[13]*m2[3];
	  M[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2] + m1[14]*m2[3];
	  M[3] = m1[3]*m2[0] + m1[7]*m2[1] + m1[11]*m2[2] + m1[15]*m2[3];

	  M[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6] + m1[12]*m2[7];
	  M[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6] + m1[13]*m2[7];
	  M[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6] + m1[14]*m2[7];
	  M[7] = m1[3]*m2[4] + m1[7]*m2[5] + m1[11]*m2[6] + m1[15]*m2[7];

	  M[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10] + m1[12]*m2[11];
	  M[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10] + m1[13]*m2[11];
	  M[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10] + m1[14]*m2[11];
	  M[11] = m1[3]*m2[8] + m1[7]*m2[9] + m1[11]*m2[10] + m1[15]*m2[11];

	  M[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12]*m2[15];
	  M[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13]*m2[15];
	  M[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15];
	  M[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];*/
	definitelyIdentityMatrix=false;
	return *this;
}


//! multiply by another matrix
// set this matrix to the product of two other matrices
// goal is to reduce stack use and copy
//template <class f32>
inline MTX4& MTX4::setbyproduct(const MTX4& other_a, const MTX4& other_b )
{
	if (other_a.getDefinitelyIdentityMatrix())
		return (*this = other_b);
	else if (other_b.getDefinitelyIdentityMatrix())
		return (*this = other_a);
	else
		return setbyproduct_nocheck(other_a,other_b);
}

//! multiply by another matrix
//template <class f32>
inline MTX4 MTX4::operator*(const MTX4& m2) const
{
	// Testing purpose..
	if (this->getDefinitelyIdentityMatrix())
		return m2;
	if (m2.getDefinitelyIdentityMatrix())
		return *this;

	MTX4 m3 ( EM4CONST_NOTHING );

	const f32 *m1 = M;

	m3[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2] + m1[12]*m2[3];
	m3[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2] + m1[13]*m2[3];
	m3[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2] + m1[14]*m2[3];
	m3[3] = m1[3]*m2[0] + m1[7]*m2[1] + m1[11]*m2[2] + m1[15]*m2[3];

	m3[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6] + m1[12]*m2[7];
	m3[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6] + m1[13]*m2[7];
	m3[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6] + m1[14]*m2[7];
	m3[7] = m1[3]*m2[4] + m1[7]*m2[5] + m1[11]*m2[6] + m1[15]*m2[7];

	m3[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10] + m1[12]*m2[11];
	m3[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10] + m1[13]*m2[11];
	m3[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10] + m1[14]*m2[11];
	m3[11] = m1[3]*m2[8] + m1[7]*m2[9] + m1[11]*m2[10] + m1[15]*m2[11];

	m3[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12]*m2[15];
	m3[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13]*m2[15];
	m3[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14]*m2[15];
	m3[15] = m1[3]*m2[12] + m1[7]*m2[13] + m1[11]*m2[14] + m1[15]*m2[15];
	return m3;
}


//! multiply by another matrix
//template <class f32>
inline MTX4 MTX4::mult34(const MTX4& m2) const
{
	MTX4 out;
	this->mult34(m2, out);
	return out;
}


//! multiply by another matrix
//template <class f32>
inline MTX4& MTX4::mult34(const MTX4& m2, MTX4& out) const
{
	// Testing purpose..
	if ( getDefinitelyIdentityMatrix() )
	{
		out = m2;
		return out;
	}

	if ( m2.getDefinitelyIdentityMatrix() )
	{
		out = *this;
		return out;
	}

	const f32* m1 = M;

	out.M[0] = m1[0]*m2[0] + m1[4]*m2[1] + m1[8]*m2[2];
	out.M[1] = m1[1]*m2[0] + m1[5]*m2[1] + m1[9]*m2[2];
	out.M[2] = m1[2]*m2[0] + m1[6]*m2[1] + m1[10]*m2[2];
	out.M[3] = 0.0f;

	out.M[4] = m1[0]*m2[4] + m1[4]*m2[5] + m1[8]*m2[6];
	out.M[5] = m1[1]*m2[4] + m1[5]*m2[5] + m1[9]*m2[6];
	out.M[6] = m1[2]*m2[4] + m1[6]*m2[5] + m1[10]*m2[6];
	out.M[7] = 0.0f;

	out.M[8] = m1[0]*m2[8] + m1[4]*m2[9] + m1[8]*m2[10];
	out.M[9] = m1[1]*m2[8] + m1[5]*m2[9] + m1[9]*m2[10];
	out.M[10] = m1[2]*m2[8] + m1[6]*m2[9] + m1[10]*m2[10];
	out.M[11] = 0.0f;

	out.M[12] = m1[0]*m2[12] + m1[4]*m2[13] + m1[8]*m2[14] + m1[12];
	out.M[13] = m1[1]*m2[12] + m1[5]*m2[13] + m1[9]*m2[14] + m1[13];
	out.M[14] = m1[2]*m2[12] + m1[6]*m2[13] + m1[10]*m2[14] + m1[14];
	out.M[15] = 1.0f;

	out.definitelyIdentityMatrix = false;//definitelyIdentityMatrix && m2.definitelyIdentityMatrix;
		
	return out;
}

//template <class f32>
inline VEC3 MTX4::getColumn(u32 c) const
{
	const f32* v = &M[c * 4];
	return VEC3(v[0], v[1], v[2]);
}

//template <class f32>
//inline vector4d MTX4::getFullColumn(u32 c) const
//{
//	const f32* v = &M[c * 4];
//	return vector4d(v[0], v[1], v[2], v[3]);
//}

//template <class f32>
inline MTX4& MTX4::setColumn(u32 c, const VEC3& v)
{
	f32* dst = &M[c * 4];
	dst[0] = v.X;
	dst[1] = v.Y;
	dst[2] = v.Z;
	return *this;
}

//template <class f32>
//inline MTX4& MTX4::setFullColumn(u32 c, const vector4d& v)
//{
//	f32* dst = &M[c * 4];
//	dst[0] = v.X;
//	dst[1] = v.Y;
//	dst[2] = v.Z;
//	dst[3] = v.W;
//	return *this;
//}

//template <class f32>
inline VEC3 MTX4::getTranslation() const
{
	return VEC3(M[12], M[13], M[14]);
}


//template <class f32>
inline MTX4& MTX4::setTranslation( const VEC3& translation )
{
	M[12] = translation.X;
	M[13] = translation.Y;
	M[14] = translation.Z;
	definitelyIdentityMatrix=false;
	return *this;
}

//template <class f32>
inline MTX4& MTX4::setInverseTranslation( const VEC3& translation )
{
	M[12] = -translation.X;
	M[13] = -translation.Y;
	M[14] = -translation.Z;
	definitelyIdentityMatrix=false;
	return *this;
}

//template <class f32>
inline MTX4& MTX4::setScale( const VEC3& scale )
{
	M[0] = scale.X;
	M[5] = scale.Y;
	M[10] = scale.Z;
	definitelyIdentityMatrix=false;
	return *this;
}

//template <class f32>
inline MTX4& MTX4::preScale( const VEC3& scale )
{
	if (definitelyIdentityMatrix)
	{
		setScale(scale);
	}
	else
	{
		M[0] *= scale.X;
		M[1] *= scale.Y;
		M[2] *= scale.Z;

		M[4] *= scale.X;
		M[5] *= scale.Y;
		M[6] *= scale.Z;

		M[8] *= scale.X;
		M[9] *= scale.Y;
		M[10] *= scale.Z;

		M[12] *= scale.X;
		M[13] *= scale.Y;
		M[14] *= scale.Z;

		definitelyIdentityMatrix=false;
	}
	return *this;
}

//template <class f32>
inline MTX4& MTX4::postScale( const VEC3& scale )
{
	if (definitelyIdentityMatrix)
	{
		setScale(scale);
	}
	else
	{
		M[0] *= scale.X;
		M[1] *= scale.X;
		M[2] *= scale.X;

		M[4] *= scale.Y;
		M[5] *= scale.Y;
		M[6] *= scale.Y;

		M[8] *= scale.Z;
		M[9] *= scale.Z;
		M[10] *= scale.Z;

		definitelyIdentityMatrix=false;
	}
	return *this;
}

//template <class f32>
inline VEC3 MTX4::getScale() const
{
	VEC3 vScale;
	vScale.X = VEC3(M[0],M[1],M[2]).getLength();
	vScale.Y = VEC3(M[4],M[5],M[6]).getLength();
	vScale.Z = VEC3(M[8],M[9],M[10]).getLength();
	return vScale;
}

//template <class f32>
inline MTX4& MTX4::setRotationDegrees( const VEC3& rotation )
{
	return setRotationRadians( rotation *  DEGTORAD );
}

//template <class f32>
inline MTX4& MTX4::setInverseRotationDegrees( const VEC3& rotation )
{
	return setInverseRotationRadians( rotation *  DEGTORAD );
}

//template <class f32>
inline MTX4& MTX4::setRotationRadians( const VEC3& rotation )
{
	const f64 cr = cos( rotation.X );
	const f64 sr = sin( rotation.X );
	const f64 cp = cos( rotation.Y );
	const f64 sp = sin( rotation.Y );
	const f64 cy = cos( rotation.Z );
	const f64 sy = sin( rotation.Z );

	M[0] = (f32)( cp*cy );
	M[1] = (f32)( cp*sy );
	M[2] = (f32)( -sp );

	const f64 srsp = sr*sp;
	const f64 crsp = cr*sp;

	M[4] = (f32)( srsp*cy-cr*sy );
	M[5] = (f32)( srsp*sy+cr*cy );
	M[6] = (f32)( sr*cp );

	M[8] = (f32)( crsp*cy+sr*sy );
	M[9] = (f32)( crsp*sy-sr*cy );
	M[10] = (f32)( cr*cp );
	definitelyIdentityMatrix=false;
	return *this;
}


//! Returns the rotation, as set by setRotation(). This code was sent
//! in by Chev.
//template <class f32>
inline  VEC3 MTX4::getRotationDegrees() const
{
	const MTX4 &mat = *this;

	f64 Y = -asin(mat(0,2));
	const f64 C = cos(Y);
	Y *= RADTODEG64;

	f64 rotx, roty, X, Z;

	if (fabs(C)>ROUNDING_ERROR_64)
	{
		const f32 invC = (f32)(1.0/C);
		rotx = mat(2,2) * invC;
		roty = mat(1,2) * invC;
		X = atan2( roty, rotx ) * RADTODEG64;
		rotx = mat(0,0) * invC;
		roty = mat(0,1) * invC;
		Z = atan2( roty, rotx ) * RADTODEG64;
	}
	else
	{
		X = 0.0;
		rotx = mat(1,1);
		roty = -mat(1,0);
		Z = atan2( roty, rotx ) * RADTODEG64;
	}

	// fix values that get below zero
	// before it would set (!) values to 360
	// that where above 360:
	if (X < 0.0) X += 360.0;
	if (Y < 0.0) Y += 360.0;
	if (Z < 0.0) Z += 360.0;

	return VEC3((f32)X,(f32)Y,(f32)Z);
}


//template <class f32>
inline MTX4& MTX4::setInverseRotationRadians( const VEC3& rotation )
{
	f64 cr = cos( rotation.X );
	f64 sr = sin( rotation.X );
	f64 cp = cos( rotation.Y );
	f64 sp = sin( rotation.Y );
	f64 cy = cos( rotation.Z );
	f64 sy = sin( rotation.Z );

	M[0] = (f32)( cp*cy );
	M[4] = (f32)( cp*sy );
	M[8] = (f32)( -sp );

	f64 srsp = sr*sp;
	f64 crsp = cr*sp;

	M[1] = (f32)( srsp*cy-cr*sy );
	M[5] = (f32)( srsp*sy+cr*cy );
	M[9] = (f32)( sr*cp );

	M[2] = (f32)( crsp*cy+sr*sy );
	M[6] = (f32)( crsp*sy-sr*cy );
	M[10] = (f32)( cr*cp );
	definitelyIdentityMatrix=false;
	return *this;
}


/*!
 */
//template <class f32>
inline MTX4& MTX4::makeIdentity()
{
	memset(M, 0, 16*sizeof(f32));
	M[0] = M[5] = M[10] = M[15] = (f32)1;
	definitelyIdentityMatrix=true;
	return *this;
}


/*
  check identity with epsilon
  solve floating range problems..
*/
//template <class f32>
inline bool MTX4::isIdentity() const
{
	if (definitelyIdentityMatrix)
		return true;
	if (!equals_f32( M[ 0], (f32)1 ) ||
		!equals_f32( M[ 5], (f32)1 ) ||
		!equals_f32( M[10], (f32)1 ) ||
		!equals_f32( M[15], (f32)1 ))
		return false;

	for (s32 i=0; i<4; ++i)
		for (s32 j=0; j<4; ++j)
			if ((j != i) && (!iszero_f32((*this)(i,j))))
				return false;

	definitelyIdentityMatrix=true;
	return true;
}

/*
  doesn't solve floating range problems..
  but takes care on +/- 0 on translation because we are changing it..
  reducing floating point branches
  but it needs the floats in memory..
*/
//template <class f32>
inline bool MTX4::isIdentity_integer_base() const
{
	if (definitelyIdentityMatrix)
		return true;
	if(IR(M[0])!=F32_VALUE_1)	return false;
	if(IR(M[1])!=0)			return false;
	if(IR(M[2])!=0)			return false;
	if(IR(M[3])!=0)			return false;

	if(IR(M[4])!=0)			return false;
	if(IR(M[5])!=F32_VALUE_1)	return false;
	if(IR(M[6])!=0)			return false;
	if(IR(M[7])!=0)			return false;

	if(IR(M[8])!=0)			return false;
	if(IR(M[9])!=0)			return false;
	if(IR(M[10])!=F32_VALUE_1)	return false;
	if(IR(M[11])!=0)		return false;

	if(IR(M[12])!=0)		return false;
	if(IR(M[13])!=0)		return false;
	if(IR(M[13])!=0)		return false;
	if(IR(M[15])!=F32_VALUE_1)	return false;
	definitelyIdentityMatrix=true;
	return true;
}


//template <class f32>
inline void MTX4::rotateVect( VEC3& vect ) const
{
	VEC3 tmp = vect;
	vect.X = tmp.X*M[0] + tmp.Y*M[4] + tmp.Z*M[8];
	vect.Y = tmp.X*M[1] + tmp.Y*M[5] + tmp.Z*M[9];
	vect.Z = tmp.X*M[2] + tmp.Y*M[6] + tmp.Z*M[10];
}

//! An alternate transform vector method, writing into a second vector
//template <class f32>
inline void MTX4::rotateVect(VEC3& out, const VEC3& in) const
{
	out.X = in.X*M[0] + in.Y*M[4] + in.Z*M[8];
	out.Y = in.X*M[1] + in.Y*M[5] + in.Z*M[9];
	out.Z = in.X*M[2] + in.Y*M[6] + in.Z*M[10];
}

//! An alternate transform vector method, writing into an array of 3 floats
//template <class f32>
inline void MTX4::rotateVect(f32 *out, const VEC3& in) const
{
	out[0] = in.X*M[0] + in.Y*M[4] + in.Z*M[8];
	out[1] = in.X*M[1] + in.Y*M[5] + in.Z*M[9];
	out[2] = in.X*M[2] + in.Y*M[6] + in.Z*M[10];
}

//template <class f32>
inline void MTX4::inverseRotateVect( VEC3& vect ) const
{
	VEC3 tmp = vect;
	vect.X = tmp.X*M[0] + tmp.Y*M[1] + tmp.Z*M[2];
	vect.Y = tmp.X*M[4] + tmp.Y*M[5] + tmp.Z*M[6];
	vect.Z = tmp.X*M[8] + tmp.Y*M[9] + tmp.Z*M[10];
}

//template <class f32>
inline void MTX4::transformVect( VEC3& vect) const
{
	f32 vector[3];

	vector[0] = vect.X*M[0] + vect.Y*M[4] + vect.Z*M[8] + M[12];
	vector[1] = vect.X*M[1] + vect.Y*M[5] + vect.Z*M[9] + M[13];
	vector[2] = vect.X*M[2] + vect.Y*M[6] + vect.Z*M[10] + M[14];

	vect.X = vector[0];
	vect.Y = vector[1];
	vect.Z = vector[2];
}

//template <class f32>
inline void MTX4::transformVect2D( VEC3& vect) const
{
	f32 vector[2];

	vector[0] = vect.X*M[0] + vect.Y*M[4] + M[12];
	vector[1] = vect.X*M[1] + vect.Y*M[5] + M[13];

	vect.X = vector[0];
	vect.Y = vector[1];
}

//template <class f32>
inline void MTX4::transformVect2D( VEC3& out, const VEC3& in) const
{
	out.X = in.X*M[0] + in.Y*M[4] + M[12];
	out.Y = in.X*M[1] + in.Y*M[5] + M[13];
}

//template <class f32>
inline void MTX4::transformVect( VEC3& out, const VEC3& in) const
{
	out.X = in.X*M[0] + in.Y*M[4] + in.Z*M[8] + M[12];
	out.Y = in.X*M[1] + in.Y*M[5] + in.Z*M[9] + M[13];
	out.Z = in.X*M[2] + in.Y*M[6] + in.Z*M[10] + M[14];
}

//template <class f32>
//inline void MTX4::transformVect( vector2df& vect) const
//{
//	f32 vector[2];
//
//	vector[0] = vect.X*M[0] + vect.Y*M[4] + M[12];
//	vector[1] = vect.X*M[1] + vect.Y*M[5] + M[13];
//
//	vect.X = vector[0];
//	vect.Y = vector[1];
//}

//template <class f32>
//inline void MTX4::transformVect( vector2df& out, const vector2df& in) const
//{
//	out.X = in.X*M[0] + in.Y*M[4] + M[12];
//	out.Y = in.X*M[1] + in.Y*M[5] + M[13];
//}

//template <class f32>
//inline void MTX4::transformTexCoord( vector2df& vect) const
//{
//	f32 vector[2];
//
//	// following buildTextureTransform, translation part is not in the
//	// fourth but third column
//	vector[0] = vect.X*M[0] + vect.Y*M[4] + M[8/*12*/];
//	vector[1] = vect.X*M[1] + vect.Y*M[5] + M[9/*13*/];
//
//	vect.X = vector[0];
//	vect.Y = vector[1];
//}
//
//template <class f32>
//inline void MTX4::transformTexCoord( vector2df& out, const vector2df& in) const
//{
//	// following buildTextureTransform, translation part is not in the
//	// fourth but third column
//	out.X = in.X*M[0] + in.Y*M[4] + M[8/*12*/];
//	out.Y = in.X*M[1] + in.Y*M[5] + M[9/*13*/];
//}


//template <class f32>
inline void MTX4::transformVect(f32 *out, const VEC3 &in) const
{
	out[0] = in.X*M[0] + in.Y*M[4] + in.Z*M[8] + M[12];
	out[1] = in.X*M[1] + in.Y*M[5] + in.Z*M[9] + M[13];
	out[2] = in.X*M[2] + in.Y*M[6] + in.Z*M[10] + M[14];
	out[3] = in.X*M[3] + in.Y*M[7] + in.Z*M[11] + M[15];
}


//! Transforms a plane by this matrix
//template <class f32>
//inline void MTX4::transformPlane(  plane3d<f32> &plane) const
//{
//	VEC3 member;
//	transformVect(member, plane.getMemberPoint());
//
//	VEC3 origin(0,0,0);
//	transformVect(plane.Normal);
//	transformVect(origin);
//
//	plane.Normal -= origin;
//	plane.D = - member.dotProduct(plane.Normal);
//}

//! Transforms a plane by this matrix
//template <class f32>
//inline void MTX4::transformPlane_new(  plane3d<f32> &plane) const
//{
//	// rotate normal -> rotateVect ( plane.n );
//	VEC3 n;
//	n.X = plane.Normal.X*M[0] + plane.Normal.Y*M[4] + plane.Normal.Z*M[8];
//	n.Y = plane.Normal.X*M[1] + plane.Normal.Y*M[5] + plane.Normal.Z*M[9];
//	n.Z = plane.Normal.X*M[2] + plane.Normal.Y*M[6] + plane.Normal.Z*M[10];
//
//	// compute new d. -> getTranslation(). dotproduct ( plane.n )
//	plane.D -= M[12] * n.X + M[13] * n.Y + M[14] * n.Z;
//	plane.Normal.X = n.X;
//	plane.Normal.Y = n.Y;
//	plane.Normal.Z = n.Z;
//}

//! Transforms a plane by this matrix
//template <class f32>
//inline void MTX4::transformPlane( const  plane3d<f32> &in,  plane3d<f32> &out) const
//{
//	out = in;
//	transformPlane( out );
//}

//! Transforms a axis aligned bounding box
//template <class f32>
inline void MTX4::transformBox( AABB& box) const
{
	if (getDefinitelyIdentityMatrix())
		return;

	transformVect(box.MinEdge);
	transformVect(box.MaxEdge);
	box.repair();
}

//! Transforms a axis aligned bounding box more accurately than transformBox()
//template <class f32>
inline void MTX4::transformBoxEx( AABB& box) const
{
	const f32 Amin[3] = {box.MinEdge.X, box.MinEdge.Y, box.MinEdge.Z};
	const f32 Amax[3] = {box.MaxEdge.X, box.MaxEdge.Y, box.MaxEdge.Z};

	f32 Bmin[3];
	f32 Bmax[3];

	Bmin[0] = Bmax[0] = M[12];
	Bmin[1] = Bmax[1] = M[13];
	Bmin[2] = Bmax[2] = M[14];

	const MTX4 &m = *this;

	for (u32 i = 0; i < 3; ++i)
	{
		for (u32 j = 0; j < 3; ++j)
		{
			const f32 a = m(j,i) * Amin[j];
			const f32 b = m(j,i) * Amax[j];

			if (a < b)
			{
				Bmin[i] += a;
				Bmax[i] += b;
			}
			else
			{
				Bmin[i] += b;
				Bmax[i] += a;
			}
		}
	}

	box.MinEdge.X = Bmin[0];
	box.MinEdge.Y = Bmin[1];
	box.MinEdge.Z = Bmin[2];

	box.MaxEdge.X = Bmax[0];
	box.MaxEdge.Y = Bmax[1];
	box.MaxEdge.Z = Bmax[2];
}


//! Multiplies this matrix by a 1x4 matrix
//template <class f32>
inline void MTX4::multiplyWith1x4Matrix(f32* matrix) const
{
	/*
	  0  1  2  3
	  4  5  6  7
	  8  9  10 11
	  12 13 14 15
	*/

	f32 mat[4];
	mat[0] = matrix[0];
	mat[1] = matrix[1];
	mat[2] = matrix[2];
	mat[3] = matrix[3];

	matrix[0] = M[0]*mat[0] + M[4]*mat[1] + M[8]*mat[2] + M[12]*mat[3];
	matrix[1] = M[1]*mat[0] + M[5]*mat[1] + M[9]*mat[2] + M[13]*mat[3];
	matrix[2] = M[2]*mat[0] + M[6]*mat[1] + M[10]*mat[2] + M[14]*mat[3];
	matrix[3] = M[3]*mat[0] + M[7]*mat[1] + M[11]*mat[2] + M[15]*mat[3];
}

//template <class f32>
inline void MTX4::inverseTranslateVect( VEC3& vect ) const
{
	vect.X = vect.X-M[12];
	vect.Y = vect.Y-M[13];
	vect.Z = vect.Z-M[14];
}

//template <class f32>
inline void MTX4::translateVect( VEC3& vect ) const
{
	vect.X = vect.X+M[12];
	vect.Y = vect.Y+M[13];
	vect.Z = vect.Z+M[14];
}


//template <class f32>
inline bool MTX4::getInverse(MTX4& out) const
{
#if 1
	if (this->getDefinitelyIdentityMatrix())
	{
		out = *this;
		return true;
	}

	// Cramer's rule.

	f32 t0 = M[10] * M[15] - M[11] * M[14];
	f32 t1 = M[6] * M[15] - M[7] * M[14];
	f32 t2 = M[6] * M[11] - M[7] * M[10];
	f32 t3 = M[2] * M[15] - M[3] * M[14];
	f32 t4 = M[2] * M[11] - M[3] * M[10];
	f32 t5 = M[2] * M[7] - M[3] * M[6];

	f32 t6 = M[8] * M[13] - M[9] * M[12];
	f32 t7 = M[4] * M[13] - M[5] * M[12];
	f32 t8 = M[4] * M[9] - M[5] * M[8];
	f32 t9 = M[0] * M[13] - M[1] * M[12];
	f32 t10 = M[0] * M[9] - M[1] * M[8];
	f32 t11 = M[0] * M[5] - M[1] * M[4];

	f32 det = t0 * t11 - t1 * t10 + t2 * t9 + t3 * t8 - t4 * t7 + t5 * t6;
	if ( iszero_f32( det ))
	{
		return false;
	}

	out.M[0] = M[5] * t0 - M[9] * t1 + M[13] * t2;
	out.M[1] = M[9] * t3 - M[1] * t0 - M[13] * t4;
	out.M[2] = M[1] * t1 - M[5] * t3 + M[13] * t5;
	out.M[3] = M[5] * t4 - M[1] * t2 - M[9] * t5;

	out.M[4] = M[8] * t1 - M[4] * t0 - M[12] * t2;
	out.M[5] = M[0] * t0 - M[8] * t3 + M[12] * t4;
	out.M[6] = M[4] * t3 - M[0] * t1 - M[12] * t5;
	out.M[7] = M[0] * t2 - M[4] * t4 + M[8] * t5;

	out.M[8] = M[7] * t6 - M[11] * t7 + M[15] * t8;
	out.M[9] = M[11] * t9 - M[3] * t6 - M[15] * t10;
	out.M[10] = M[3] * t7 - M[7] * t9 + M[15] * t11;
	out.M[11] = M[7] * t10 - M[3] * t8 - M[11] * t11;

	out.M[12] = M[10] * t7 - M[6] * t6 - M[14] * t8;
	out.M[13] = M[2] * t6 - M[10] * t9 + M[14] * t10;
	out.M[14] = M[6] * t9 - M[2] * t7 - M[14] * t11;
	out.M[15] = M[2] * t8 - M[6] * t10 + M[10] * t11;

	//det =  reciprocal(det);
	det =  1.0f / det;//sqrtf(det);
	for ( int i = 0; i < 16; ++i )
	{
		out.M[i] *= det;
	}
	out.definitelyIdentityMatrix = definitelyIdentityMatrix;
	return true;
#else
	/// Calculates the inverse of this Matrix
	/// The inverse is calculated using Cramers rule.
	/// If no inverse exists then 'false' is returned.

	if ( this->getDefinitelyIdentityMatrix() )
	{
		out=*this;
		return true;
	}

	const MTX4 &m = *this;

	f32 d = (m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0)) * (m(2, 2) * m(3, 3) - m(2, 3) * m(3, 2)) -
		(m(0, 0) * m(1, 2) - m(0, 2) * m(1, 0)) * (m(2, 1) * m(3, 3) - m(2, 3) * m(3, 1)) +
		(m(0, 0) * m(1, 3) - m(0, 3) * m(1, 0)) * (m(2, 1) * m(3, 2) - m(2, 2) * m(3, 1)) +
		(m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1)) * (m(2, 0) * m(3, 3) - m(2, 3) * m(3, 0)) -
		(m(0, 1) * m(1, 3) - m(0, 3) * m(1, 1)) * (m(2, 0) * m(3, 2) - m(2, 2) * m(3, 0)) +
		(m(0, 2) * m(1, 3) - m(0, 3) * m(1, 2)) * (m(2, 0) * m(3, 1) - m(2, 1) * m(3, 0));

	if(  iszero ( d ) )
		return false;

	d =  reciprocal ( d );

	out(0, 0) = d * (m(1, 1) * (m(2, 2) * m(3, 3) - m(2, 3) * m(3, 2)) +
					 m(1, 2) * (m(2, 3) * m(3, 1) - m(2, 1) * m(3, 3)) +
					 m(1, 3) * (m(2, 1) * m(3, 2) - m(2, 2) * m(3, 1)));
	out(0, 1) = d * (m(2, 1) * (m(0, 2) * m(3, 3) - m(0, 3) * m(3, 2)) +
					 m(2, 2) * (m(0, 3) * m(3, 1) - m(0, 1) * m(3, 3)) +
					 m(2, 3) * (m(0, 1) * m(3, 2) - m(0, 2) * m(3, 1)));
	out(0, 2) = d * (m(3, 1) * (m(0, 2) * m(1, 3) - m(0, 3) * m(1, 2)) +
					 m(3, 2) * (m(0, 3) * m(1, 1) - m(0, 1) * m(1, 3)) +
					 m(3, 3) * (m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1)));
	out(0, 3) = d * (m(0, 1) * (m(1, 3) * m(2, 2) - m(1, 2) * m(2, 3)) +
					 m(0, 2) * (m(1, 1) * m(2, 3) - m(1, 3) * m(2, 1)) +
					 m(0, 3) * (m(1, 2) * m(2, 1) - m(1, 1) * m(2, 2)));
	out(1, 0) = d * (m(1, 2) * (m(2, 0) * m(3, 3) - m(2, 3) * m(3, 0)) +
					 m(1, 3) * (m(2, 2) * m(3, 0) - m(2, 0) * m(3, 2)) +
					 m(1, 0) * (m(2, 3) * m(3, 2) - m(2, 2) * m(3, 3)));
	out(1, 1) = d * (m(2, 2) * (m(0, 0) * m(3, 3) - m(0, 3) * m(3, 0)) +
					 m(2, 3) * (m(0, 2) * m(3, 0) - m(0, 0) * m(3, 2)) +
					 m(2, 0) * (m(0, 3) * m(3, 2) - m(0, 2) * m(3, 3)));
	out(1, 2) = d * (m(3, 2) * (m(0, 0) * m(1, 3) - m(0, 3) * m(1, 0)) +
					 m(3, 3) * (m(0, 2) * m(1, 0) - m(0, 0) * m(1, 2)) +
					 m(3, 0) * (m(0, 3) * m(1, 2) - m(0, 2) * m(1, 3)));
	out(1, 3) = d * (m(0, 2) * (m(1, 3) * m(2, 0) - m(1, 0) * m(2, 3)) +
					 m(0, 3) * (m(1, 0) * m(2, 2) - m(1, 2) * m(2, 0)) +
					 m(0, 0) * (m(1, 2) * m(2, 3) - m(1, 3) * m(2, 2)));
	out(2, 0) = d * (m(1, 3) * (m(2, 0) * m(3, 1) - m(2, 1) * m(3, 0)) +
					 m(1, 0) * (m(2, 1) * m(3, 3) - m(2, 3) * m(3, 1)) +
					 m(1, 1) * (m(2, 3) * m(3, 0) - m(2, 0) * m(3, 3)));
	out(2, 1) = d * (m(2, 3) * (m(0, 0) * m(3, 1) - m(0, 1) * m(3, 0)) +
					 m(2, 0) * (m(0, 1) * m(3, 3) - m(0, 3) * m(3, 1)) +
					 m(2, 1) * (m(0, 3) * m(3, 0) - m(0, 0) * m(3, 3)));
	out(2, 2) = d * (m(3, 3) * (m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0)) +
					 m(3, 0) * (m(0, 1) * m(1, 3) - m(0, 3) * m(1, 1)) +
					 m(3, 1) * (m(0, 3) * m(1, 0) - m(0, 0) * m(1, 3)));
	out(2, 3) = d * (m(0, 3) * (m(1, 1) * m(2, 0) - m(1, 0) * m(2, 1)) +
					 m(0, 0) * (m(1, 3) * m(2, 1) - m(1, 1) * m(2, 3)) +
					 m(0, 1) * (m(1, 0) * m(2, 3) - m(1, 3) * m(2, 0)));
	out(3, 0) = d * (m(1, 0) * (m(2, 2) * m(3, 1) - m(2, 1) * m(3, 2)) +
					 m(1, 1) * (m(2, 0) * m(3, 2) - m(2, 2) * m(3, 0)) +
					 m(1, 2) * (m(2, 1) * m(3, 0) - m(2, 0) * m(3, 1)));
	out(3, 1) = d * (m(2, 0) * (m(0, 2) * m(3, 1) - m(0, 1) * m(3, 2)) +
					 m(2, 1) * (m(0, 0) * m(3, 2) - m(0, 2) * m(3, 0)) +
					 m(2, 2) * (m(0, 1) * m(3, 0) - m(0, 0) * m(3, 1)));
	out(3, 2) = d * (m(3, 0) * (m(0, 2) * m(1, 1) - m(0, 1) * m(1, 2)) +
					 m(3, 1) * (m(0, 0) * m(1, 2) - m(0, 2) * m(1, 0)) +
					 m(3, 2) * (m(0, 1) * m(1, 0) - m(0, 0) * m(1, 1)));
	out(3, 3) = d * (m(0, 0) * (m(1, 1) * m(2, 2) - m(1, 2) * m(2, 1)) +
					 m(0, 1) * (m(1, 2) * m(2, 0) - m(1, 0) * m(2, 2)) +
					 m(0, 2) * (m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0)));
	out.definitelyIdentityMatrix = definitelyIdentityMatrix;
	return true;
#endif
}

//! Inverts a primitive matrix which only contains a translation and a rotation
//! \param out: where result matrix is written to.
//template <class f32>
inline bool MTX4::getInversePrimitive ( MTX4& out ) const
{
	out.M[0 ] = M[0];
	out.M[1 ] = M[4];
	out.M[2 ] = M[8];
	out.M[3 ] = 0;

	out.M[4 ] = M[1];
	out.M[5 ] = M[5];
	out.M[6 ] = M[9];
	out.M[7 ] = 0;

	out.M[8 ] = M[2];
	out.M[9 ] = M[6];
	out.M[10] = M[10];
	out.M[11] = 0;

	out.M[12] = (f32)-(M[12]*M[0] + M[13]*M[1] + M[14]*M[2]);
	out.M[13] = (f32)-(M[12]*M[4] + M[13]*M[5] + M[14]*M[6]);
	out.M[14] = (f32)-(M[12]*M[8] + M[13]*M[9] + M[14]*M[10]);
	out.M[15] = 1;
	out.definitelyIdentityMatrix = definitelyIdentityMatrix;
	return true;
}

/*!
 */
//template <class f32>
inline bool MTX4::makeInverse()
{
	if (definitelyIdentityMatrix)
		return true;

	MTX4 temp ( EM4CONST_NOTHING );

	if (getInverse(temp))
	{
		*this = temp;
		return true;
	}

	return false;
}

#if 0
//template <class f32>
inline MTX4& MTX4::operator=(const MTX4 &other)
{
	const f32* M2 = other.M;
	M[0] = M2[0]; M[1] = M2[1]; M[2] = M2[2]; M[3] = M2[3];
	M[4] = M2[4]; M[5] = M2[5]; M[6] = M2[6]; M[7] = M2[7];
	M[8] = M2[8]; M[9] = M2[9]; M[10] = M2[10]; M[11] = M2[11];
	M[12] = M2[12]; M[13] = M2[13]; M[14] = M2[14]; M[15] = M2[15];
	definitelyIdentityMatrix=other.definitelyIdentityMatrix;
	return *this;
}
#endif

//template <class f32>
inline MTX4& MTX4::operator=(const f32& scalar)
{
	for (s32 i = 0; i < 16; ++i)
		M[i]=scalar;
	definitelyIdentityMatrix=false;
	return *this;
}


//template <class f32>
inline bool MTX4::operator==(const MTX4 &other) const
{
	if (definitelyIdentityMatrix && other.definitelyIdentityMatrix)
		return true;
	for (s32 i = 0; i < 16; ++i)
		if (M[i] != other.M[i])
			return false;

	return true;
}


//template <class f32>
inline bool MTX4::operator!=(const MTX4 &other) const
{
	return !(*this == other);
}


// Builds a right-handed perspective projection matrix based on a field of view
//	//template <class f32>
//	inline MTX4& MTX4::buildProjectionMatrixPerspectiveFovRH(
//			f32 fieldOfViewRadians, f32 aspectRatio, f32 zNear, f32 zFar)
//	{
//#ifdef GLITCH_USE_WII_DEVICE
//		//MTXPerspective( (float (*)[4])&M[0], MTXRadToDeg(fieldOfViewRadians), aspectRatio, zNear, zFar);
//		MTXPerspective( (float (*)[4])&M[0], MTXRadToDeg(fieldOfViewRadians), -aspectRatio, zNear, zFar);
//		*this = getTransposed();
//		definitelyIdentityMatrix=false;
//		return *this;
//#else
//		const f64 h = 1.0/tan(fieldOfViewRadians/2.0);
//		const f32 w = ( f32 )( h / aspectRatio );
//
//		M[0] = w;
//		M[1] = 0;
//		M[2] = 0;
//		M[3] = 0;
//
//		M[4] = 0;
//		M[5] = (f32)h;
//		M[6] = 0;
//		M[7] = 0;
//
//		M[8] = 0;
//		M[9] = 0;
////		M[10] = (f32)(-zNear/(zFar-zNear)); // RVL version
//		M[10] = (f32)(zFar/(zNear-zFar)); // DirectX version
////		M[10] = (f32)(zFar+zNear/(zNear-zFar)); // OpenGL version
////		M[11] = -1;
//
//		M[12] = 0;
//		M[13] = 0;
////		M[14] = (f32)(-zNear*zFar/(zFar-zNear)); // RVL version
//		M[14] = (f32)(zNear*zFar/(zNear-zFar)); // DirectX version
////		M[14] = (f32)(2.0f*zNear*zFar/(zNear-zFar)); // OpenGL version
//		M[15] = 0;
//		definitelyIdentityMatrix=false;
//		return *this;
//#endif
//	}


// Builds a left-handed perspective projection matrix based on a field of view
//template <class f32>
inline MTX4& MTX4::buildProjectionMatrixPerspectiveFov(
	f32 fieldOfViewRadians, f32 aspectRatio, f32 zNear, f32 zFar)
{
#if 0//def GLITCH_USE_WII_DEVICE
	//MTXPerspective( (float (*)[4])&M[0], MTXRadToDeg(fieldOfViewRadians), aspectRatio, zNear, zFar);
	MTXPerspective( (float (*)[4])&M[0], MTXRadToDeg(fieldOfViewRadians), aspectRatio, zNear, zFar);
	*this = getTransposed();
	definitelyIdentityMatrix=false;
	return *this;
#else

	const f64 h = 1.0/tan(fieldOfViewRadians/2.0);
	const f32 w = (f32)(h / aspectRatio);

	M[0] = w;
	M[1] = 0;
	M[2] = 0;
	M[3] = 0;

	M[4] = 0;
	M[5] = (f32)h;
	M[6] = 0;
	M[7] = 0;

	M[8] = 0;
	M[9] = 0;
	M[10] = (f32)(zFar/(zFar-zNear));
	M[11] = 1;

	M[12] = 0;
	M[13] = 0;
	M[14] = (f32)(-zNear*zFar/(zFar-zNear));
	M[15] = 0;
	definitelyIdentityMatrix=false;
	return *this;
#endif
}


// Builds centered a left-handed orthogonal projection matrix.
//template <class f32>
inline MTX4& MTX4::buildProjectionMatrixOrtho(
	f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar)
{
	M[0] = (f32)(2/widthOfViewVolume);
	M[1] = 0;
	M[2] = 0;
	M[3] = 0;

	M[4] = 0;
	M[5] = (f32)(2/heightOfViewVolume);
	M[6] = 0;
	M[7] = 0;

	M[8] = 0;
	M[9] = 0;
	M[10] = (f32)(1/(zFar-zNear));
	M[11] = 0;

	M[12] = 0;
	M[13] = 0;
	M[14] = (f32)(zNear/(zNear-zFar));
	M[15] = 1;
	definitelyIdentityMatrix=false;
	return *this;
}


// Builds a left-handed orthogonal projection matrix.
//template <class f32>
inline MTX4& MTX4::buildProjectionMatrixOrtho(
	f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar)
{
	f32 w = right - left;
	f32 h = top - bottom;
	M[0] = (f32)(2/w);
	M[1] = 0;
	M[2] = 0;
	M[3] = 0;

	M[4] = 0;
	M[5] = (f32)(2/h);
	M[6] = 0;
	M[7] = 0;

	M[8] = 0;
	M[9] = 0;
	M[10] = (f32)(1/(zFar-zNear));
	M[11] = 0;

	M[12] = - (left + right) / w;;
	M[13] = - (bottom + top) / h;
	M[14] = (f32)(zNear/(zNear-zFar));
	M[15] = 1;
	definitelyIdentityMatrix=false;
	return *this;
}


// Builds a right-handed orthogonal projection matrix.
//template <class f32>
//inline MTX4& MTX4::buildProjectionMatrixOrthoRH(
//		f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar)
//{
//	M[0] = (f32)(2/widthOfViewVolume);
//	M[1] = 0;
//	M[2] = 0;
//	M[3] = 0;

//	M[4] = 0;
//	M[5] = (f32)(2/heightOfViewVolume);
//	M[6] = 0;
//	M[7] = 0;

//	M[8] = 0;
//	M[9] = 0;
//	M[10] = (f32)(1/(zNear-zFar));
//	M[11] = 0;

//	M[12] = 0;
//	M[13] = 0;
//	M[14] = (f32)(zNear/(zNear-zFar));
//	M[15] = -1;
//	definitelyIdentityMatrix=false;
//	return *this;
//}


// Builds a right-handed perspective projection matrix.
//template <class f32>
//inline MTX4& MTX4::buildProjectionMatrixPerspectiveRH(
//		f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar)
//{
//	M[0] = (f32)(2*zNear/widthOfViewVolume);
//	M[1] = 0;
//	M[2] = 0;
//	M[3] = 0;

//	M[4] = 0;
//	M[5] = (f32)(2*zNear/heightOfViewVolume);
//	M[6] = 0;
//	M[7] = 0;

//	M[8] = 0;
//	M[9] = 0;
//	M[10] = (f32)(zFar/(zNear-zFar));
//	M[11] = -1;

//	M[12] = 0;
//	M[13] = 0;
//	M[14] = (f32)(zNear*zFar/(zNear-zFar));
//	M[15] = 0;
//	definitelyIdentityMatrix=false;
//	return *this;
//}


// Builds a left-handed perspective projection matrix.
//template <class f32>
inline MTX4& MTX4::buildProjectionMatrixPerspective(
	f32 widthOfViewVolume, f32 heightOfViewVolume, f32 zNear, f32 zFar)
{
	M[0] = (f32)(2*zNear/widthOfViewVolume);
	M[1] = 0;
	M[2] = 0;
	M[3] = 0;

	M[4] = 0;
	M[5] = (f32)(2*zNear/heightOfViewVolume);
	M[6] = 0;
	M[7] = 0;

	M[8] = 0;
	M[9] = 0;
	M[10] = (f32)(zFar/(zFar-zNear));
	M[11] = 1;

	M[12] = 0;
	M[13] = 0;
	M[14] = (f32)(zNear*zFar/(zNear-zFar));
	M[15] = 0;
	definitelyIdentityMatrix=false;
	return *this;
}


// Builds a matrix that flattens geometry into a plane.
//template <class f32>
//inline MTX4& MTX4::buildShadowMatrix(const VEC3& light,  plane3df plane, f32 point)
//{
//	plane.Normal.normalize();
//	const f32 d = plane.Normal.dotProduct(light);
//
//	M[ 0] = (f32)(-plane.Normal.X * light.X + d);
//	M[ 1] = (f32)(-plane.Normal.X * light.Y);
//	M[ 2] = (f32)(-plane.Normal.X * light.Z);
//	M[ 3] = (f32)(-plane.Normal.X * point);
//
//	M[ 4] = (f32)(-plane.Normal.Y * light.X);
//	M[ 5] = (f32)(-plane.Normal.Y * light.Y + d);
//	M[ 6] = (f32)(-plane.Normal.Y * light.Z);
//	M[ 7] = (f32)(-plane.Normal.Y * point);
//
//	M[ 8] = (f32)(-plane.Normal.Z * light.X);
//	M[ 9] = (f32)(-plane.Normal.Z * light.Y);
//	M[10] = (f32)(-plane.Normal.Z * light.Z + d);
//	M[11] = (f32)(-plane.Normal.Z * point);
//
//	M[12] = (f32)(-plane.D * light.X);
//	M[13] = (f32)(-plane.D * light.Y);
//	M[14] = (f32)(-plane.D * light.Z);
//	M[15] = (f32)(-plane.D * point + d);
//	definitelyIdentityMatrix=false;
//	return *this;
//}

// Builds a left-handed look-at matrix.
//template <class f32>
inline MTX4& MTX4::buildCameraLookAtMatrix(
	const VEC3& position,
	const VEC3& target,
	const VEC3& upVector)
{

#if 0//def GLITCH_USE_WII_DEVICE

	VEC3 vup = upVector; //.normalize();
	vup.normalize();
	MTXLookAt( (float (*)[4])&M[0], (const Vec *)&position, (const Vec *)&upVector, (const Vec *)&target);
	M[12] = M[13] = M[14] = 0;
	M[15] = 1;
	*this = getTransposed();
	definitelyIdentityMatrix=false;
	return *this;

#else

	VEC3 zaxis = target - position;
	zaxis.normalize();

	VEC3 xaxis = upVector.crossProduct(zaxis);
	xaxis.normalize();

	VEC3 yaxis = zaxis.crossProduct(xaxis);

	M[0] = (f32)xaxis.X;
	M[1] = (f32)yaxis.X;
	M[2] = (f32)zaxis.X;
	M[3] = 0;

	M[4] = (f32)xaxis.Y;
	M[5] = (f32)yaxis.Y;
	M[6] = (f32)zaxis.Y;
	M[7] = 0;

	M[8] = (f32)xaxis.Z;
	M[9] = (f32)yaxis.Z;
	M[10] = (f32)zaxis.Z;
	M[11] = 0;

	M[12] = (f32)-xaxis.dotProduct(position);
	M[13] = (f32)-yaxis.dotProduct(position);
	M[14] = (f32)-zaxis.dotProduct(position);
	M[15] = 1;
	definitelyIdentityMatrix=false;
	return *this;

#endif
}


// Builds a right-handed look-at matrix.
//template <class f32>
//inline MTX4& MTX4::buildCameraLookAtMatrixRH(
//			const VEC3& position,
//			const VEC3& target,
//			const VEC3& upVector)
//{
///*#ifdef GLITCH_USE_WII_DEVICE
//	VEC3 vup = upVector; //.normalize();
//	vup.normalize();
//	MTXLookAt( (float (*)[4])&M[0], (const Vec *)&position, (const Vec *)&upVector, (const Vec *)&target);
//	M[12] = M[13] = M[14] = 0;
//	M[15] = 1;
//	*this = getTransposed();
//	definitelyIdentityMatrix=false;
//	return *this;
//#else*/
//	VEC3 zaxis = position - target;
//	zaxis.normalize();

//	VEC3 xaxis = upVector.crossProduct(zaxis);
//	xaxis.normalize();

//	VEC3 yaxis = zaxis.crossProduct(xaxis);

//	M[0] = (f32)xaxis.X;
//	M[1] = (f32)yaxis.X;
//	M[2] = (f32)zaxis.X;
//	M[3] = 0;

//	M[4] = (f32)xaxis.Y;
//	M[5] = (f32)yaxis.Y;
//	M[6] = (f32)zaxis.Y;
//	M[7] = 0;

//	M[8] = (f32)xaxis.Z;
//	M[9] = (f32)yaxis.Z;
//	M[10] = (f32)zaxis.Z;
//	M[11] = 0;

//	M[12] = (f32)-xaxis.dotProduct(position);
//	M[13] = (f32)-yaxis.dotProduct(position);
//	M[14] = (f32)-zaxis.dotProduct(position);
//	M[15] = 1;
//	definitelyIdentityMatrix=false;
//	return *this;
////#endif
//}


// creates a new matrix as interpolated matrix from this and the passed one.
//template <class f32>
inline MTX4 MTX4::interpolate(const  MTX4& b, f32 time) const
{
	MTX4 mat ( EM4CONST_NOTHING );

	for (u32 i=0; i < 16; i += 4)
	{
		mat.M[i+0] = (f32)(M[i+0] + ( b.M[i+0] - M[i+0] ) * time);
		mat.M[i+1] = (f32)(M[i+1] + ( b.M[i+1] - M[i+1] ) * time);
		mat.M[i+2] = (f32)(M[i+2] + ( b.M[i+2] - M[i+2] ) * time);
		mat.M[i+3] = (f32)(M[i+3] + ( b.M[i+3] - M[i+3] ) * time);
	}
	return mat;
}


// returns transposed matrix
//template <class f32>
inline MTX4 MTX4::getTransposed() const
{
	MTX4 t ( EM4CONST_NOTHING );
	getTransposed ( t );
	return t;
}


// returns transposed matrix
//template <class f32>
inline void MTX4::getTransposed( MTX4& o ) const
{
	o.M[ 0] = M[ 0];
	o.M[ 1] = M[ 4];
	o.M[ 2] = M[ 8];
	o.M[ 3] = M[12];

	o.M[ 4] = M[ 1];
	o.M[ 5] = M[ 5];
	o.M[ 6] = M[ 9];
	o.M[ 7] = M[13];

	o.M[ 8] = M[ 2];
	o.M[ 9] = M[ 6];
	o.M[10] = M[10];
	o.M[11] = M[14];

	o.M[12] = M[ 3];
	o.M[13] = M[ 7];
	o.M[14] = M[11];
	o.M[15] = M[15];
	o.definitelyIdentityMatrix=definitelyIdentityMatrix;
}


// used to scale <-1,-1><1,1> to viewport
////template <class f32>
//inline MTX4& MTX4::buildNDCToDCMatrix( const  rect<s32>& viewport, f32 zScale)
//{
//	const f32 scaleX = (viewport.getWidth() - 0.75f ) / 2.0f;
//	const f32 scaleY = -(viewport.getHeight() - 0.75f ) / 2.0f;
//
//	const f32 dx = -0.5f + ( (viewport.UpperLeftCorner.X + viewport.LowerRightCorner.X ) / 2.0f );
//	const f32 dy = -0.5f + ( (viewport.UpperLeftCorner.Y + viewport.LowerRightCorner.Y ) / 2.0f );
//
//	makeIdentity();
//	M[12] = (f32)dx;
//	M[13] = (f32)dy;
//	return setScale( VEC3((f32)scaleX, (f32)scaleY, (f32)zScale));
//}

/*!
  Generate texture coordinates as linear functions so that:
  u = Ux*x + Uy*y + Uz*z + Uw
  v = Vx*x + Vy*y + Vz*z + Vw
  The matrix M for this case is:
  Ux  Vx  0  0
  Uy  Vy  0  0
  Uz  Vz  0  0
  Uw  Vw  0  0
*/


//template <class f32>
//inline MTX4& MTX4::buildTextureTransform( f32 rotateRad,
//														const  vector2df &rotatecenter,
//														const  vector2df &translate,
//														const  vector2df &scale)
//{
//	const f32 c = cosf(rotateRad);
//	const f32 s = sinf(rotateRad);
//
//	M[0] = (f32)(c * scale.X);
//	M[1] = (f32)(s * scale.Y);
//	M[2] = 0;
//	M[3] = 0;
//
//	M[4] = (f32)(-s * scale.X);
//	M[5] = (f32)(c * scale.Y);
//	M[6] = 0;
//	M[7] = 0;
//
//	M[8] = (f32)(scale.X * (rotatecenter.X - c * rotatecenter.X + s * rotatecenter.Y) + translate.X);
//	M[9] = (f32)(scale.Y * (rotatecenter.Y - s * rotatecenter.X - c * rotatecenter.Y) + translate.Y);
//	M[10] = 1;
//	M[11] = 0;
//
//	M[12] = 0;
//	M[13] = 0;
//	M[14] = 0;
//	M[15] = 1;
//	definitelyIdentityMatrix=false;
//	return *this;
//}
//

// rotate about z axis, center ( 0.5, 0.5 )
//template <class f32>
inline MTX4& MTX4::setTextureRotationCenter( f32 rotateRad )
{
	const f32 c = cosf(rotateRad);
	const f32 s = sinf(rotateRad);
	M[0] = (f32)c;
	M[1] = (f32)s;

	M[4] = (f32)-s;
	M[5] = (f32)c;

	M[8] = (f32)(0.5f * ( s - c) + 0.5f);
	M[9] = (f32)(-0.5f * ( s + c) + 0.5f);
	definitelyIdentityMatrix = definitelyIdentityMatrix && (rotateRad==0.0f);
	return *this;
}


//template <class f32>
inline MTX4& MTX4::setTextureTranslate ( f32 x, f32 y )
{
	M[8] = (f32)x;
	M[9] = (f32)y;
	definitelyIdentityMatrix = definitelyIdentityMatrix && (x==0.0f) && (y==0.0f);
	return *this;
}


//template <class f32>
inline MTX4& MTX4::setTextureTranslateTransposed ( f32 x, f32 y )
{
	M[2] = (f32)x;
	M[6] = (f32)y;
	definitelyIdentityMatrix = definitelyIdentityMatrix && (x==0.0f) && (y==0.0f) ;
	return *this;
}

//template <class f32>
inline MTX4& MTX4::setTextureScale ( f32 sx, f32 sy )
{
	M[0] = (f32)sx;
	M[5] = (f32)sy;
	definitelyIdentityMatrix = definitelyIdentityMatrix && (sx==1.0f) && (sy==1.0f);
	return *this;
}

//template <class f32>
inline MTX4& MTX4::postTextureScale ( f32 sx, f32 sy )
{
	M[0] *= (f32)sx;
	M[1] *= (f32)sx;
	M[4] *= (f32)sy;
	M[5] *= (f32)sy;
	definitelyIdentityMatrix = definitelyIdentityMatrix && (sx==1.0f) && (sy==1.0f);
	return *this;
}


//template <class f32>
inline MTX4& MTX4::setTextureScaleCenter( f32 sx, f32 sy )
{
	M[0] = (f32)sx;
	M[5] = (f32)sy;
	M[8] = (f32)(0.5f - 0.5f * sx);
	M[9] = (f32)(0.5f - 0.5f * sy);
	definitelyIdentityMatrix = definitelyIdentityMatrix && (sx==1.0f) && (sy==1.0f);
	return *this;
}


// sets all matrix data members at once
//template <class f32>
inline MTX4& MTX4::setM(const f32* data)
{
	memcpy(M,data, 16*sizeof(f32));

	definitelyIdentityMatrix = false;
	return *this;
}

// gets all matrix data members at once
//template <class f32>
inline f32* MTX4::getM(f32* data) const
{
	memcpy(data, M, 16 * sizeof(f32));
	return data;
}


// sets if the matrix is definitely identity matrix
//template <class f32>
inline void MTX4::setDefinitelyIdentityMatrix( bool isDefinitelyIdentityMatrix)
{
	definitelyIdentityMatrix = isDefinitelyIdentityMatrix;
}


// gets if the matrix is definitely identity matrix
//template <class f32>
inline bool MTX4::getDefinitelyIdentityMatrix() const
{
	return definitelyIdentityMatrix;
}


// Multiply by scalar.
//template <class f32>
inline MTX4 operator*(const f32 scalar, const MTX4& mat)
{
	return mat*scalar;
}


//! Typedef for f32 matrix
//typedef MTX4<f32> MTX4;
//! global const identity matrix
extern const MTX4 IdentityMatrix;

//} // end namespace core

//namespace os
//{
//
//
// MTX4 byteswap(const  MTX4& v)
//{
//	 MTX4 ret;
//
//	for(u32 i = 0; i < 16; ++i){
//		ret[i] = byteswap(v[i]);
//	}
//
//	return ret;
//}
//
//} // end namespace os
//
//} // end namespace glitch

#endif

