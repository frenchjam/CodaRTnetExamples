void CodaRTnetTracker::GetUnitPlacement( int unit, Vector3 &pos, Quaternion &ori ) {

	Vector3		offset;
	Matrix3x3	rotation;
	Matrix3x3	ortho;

	GetUnitTransform( unit, offset, rotation );

	// I have to check: Is the position equal to its offset, 
	//  or the negative of the offset?
	// ScaleVector( pos, offset, -1.0 );
	CopyVector( pos, offset );
	// I want to be sure that the rotation matrix is really a rotation matrix.
	// This means that it must be orthonormal.
	// In fact, I am not convinced that CODA guarantees an orthonormal matrix.
	OrthonormalizeMatrix( ortho, rotation );
	// Express the orientation of the CODA unit as a quaternion.
	MatrixToQuaternion( ori, ortho );

}

// On a CodaRTnet system there is a procedure that is used to the 
int CodaRTnetTracker::PerformAlignment( int origin, int x_negative, int x_positive, int xy_negative, int xy_positive ) {
	return( NORMAL_EXIT );
}
