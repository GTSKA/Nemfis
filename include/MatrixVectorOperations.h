#include "CVector_.h"
#include "CMatrix_.h"

///Horizontal vector product with matrix
CVECTOR_FLOAT4& operator*(const CVECTOR_FLOAT4& v, const CMATRIX_FLOAT4& m);

///matrix product with vertical vector
CVECTOR_FLOAT4& operator*(const CMATRIX_FLOAT4& m, const CVECTOR_FLOAT4& v);

CMATRIX_FLOAT4& operator*(const CMATRIX_FLOAT4& m1, const CMATRIX_FLOAT4& m2);
