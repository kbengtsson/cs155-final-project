//------------------------------------------------------------------------------
// GrB_BinaryOp_ztype: return the type of z for z=f(x,y)
//------------------------------------------------------------------------------

// SuiteSparse:GraphBLAS, Timothy A. Davis, (c) 2017, All Rights Reserved.
// http://suitesparse.com   See GraphBLAS/Doc/License.txt for license.

//------------------------------------------------------------------------------

#include "GB.h"

GrB_Info GrB_BinaryOp_ztype         // return the type of z
(
    GrB_Type *ztype,                // return type of output z
    const GrB_BinaryOp binaryop     // binary operator to query
)
{

    //--------------------------------------------------------------------------
    // check inputs
    //--------------------------------------------------------------------------

    WHERE ("GrB_BinaryOp_ztype (&ztype, binaryop)") ;
    RETURN_IF_NULL (ztype) ;
    RETURN_IF_NULL_OR_UNINITIALIZED (binaryop) ;
    ASSERT_OK (GB_check (binaryop, "binaryop for ztype", 0)) ;

    //--------------------------------------------------------------------------
    // return the ztype
    //--------------------------------------------------------------------------

    (*ztype) = binaryop->ztype ;
    return (REPORT_SUCCESS) ;
}

