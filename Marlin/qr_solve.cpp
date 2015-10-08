#include "qr_solve.h"

#ifdef AUTO_BED_LEVELING_GRID

#include <stdlib.h>
#include <math.h>

//# include "r8lib.h"

int i4_min ( int i1, int i2 )

/******************************************************************************/
/*
  Purpose:

    I4_MIN returns the smaller of two I4's.

  Licensing:

    This code is distributed under the GNU LGPL license.

  Modified:

    29 August 2006

  Author:

    John Burkardt

  Parameters:

    Input, int I1, I2, two integers to be compared.

    Output, int I4_MIN, the smaller of I1 and I2.
*/
{
  int value;

  if ( i1 < i2 )
  {
    value = i1;
  }
  else
  {
    value = i2;
  }
  return value;
}

double r8_epsilon ( void )

/******************************************************************************/
/*
  Purpose:

    R8_EPSILON returns the R8 round off unit.

  Discussion:

    R8_EPSILON is a number R which is a power of 2 with the property that,
    to the precision of the computer's arithmetic,
      1 < 1 + R
    but
      1 = ( 1 + R / 2 )

  Licensing:

    This code is distributed under the GNU LGPL license.

  Modified:

    01 September 2012

  Author:

    John Burkardt

  Parameters:

    Output, double R8_EPSILON, the R8 round-off unit.
*/
{
  const double value = 2.220446049250313E-016;

  return value;
}

double r8_max ( double x, double y )

/******************************************************************************/
/*
  Purpose:

    R8_MAX returns the maximum of two R8's.

  Licensing:

    This code is distributed under the GNU LGPL license.

  Modified:

    07 May 2006

  Author:

    John Burkardt

  Parameters:

    Input, double X, Y, the quantities to compare.

    Output, double R8_MAX, the maximum of X and Y.
*/
{
  double value;

  if ( y < x )
  {
    value = x;
  }
  else
  {
    value = y;
  }
  return value;
}

double r8_abs ( double x )

/******************************************************************************/
/*
  Purpose:

    R8_ABS returns the absolute value of an R8.

  Licensing:

    This code is distributed under the GNU LGPL license.

  Modified:

    07 May 2006

  Author:

    John Burkardt

  Parameters:

    Input, double X, the quantity whose absolute value is desired.

    Output, double R8_ABS, the absolute value of X.
*/
{
  double value;

  if ( 0.0 <= x )
  {
    value = + x;
  }
  else
  {
    value = - x;
  }
  return value;
}

double r8_sign ( double x )

/******************************************************************************/
/*
  Purpose:

    R8_SIGN returns the sign of an R8.

  Licensing:

    This code is distributed under the GNU LGPL license.

  Modified:

    08 May 2006

  Author:

    John Burkardt

  Parameters:

    Input, double X, the number whose sign is desired.

    Output, double R8_SIGN, the sign of X.
*/
{
  double value;

  if ( x < 0.0 )
  {
    value = - 1.0;
  }
  else
  {
    value = + 1.0;
  }
  return value;
}

double r8mat_amax ( int m, int n, double a[] )

/******************************************************************************/
/*
  Purpose:

    R8MAT_AMAX returns the maximum absolute value entry of an R8MAT.

  Discussion:

    An R8MAT is a doubly dimensioned array of R8 values, stored as a vector
    in column-major order.

  Licensing:

    This code is distributed under the GNU LGPL license.

  Modified:

    07 September 2012

  Author:

    John Burkardt

  Parameters:

    Input, int M, the number of rows in A.

    Input, int N, the number of columns in A.

    Input, double A[M*N], the M by N matrix.

    Output, double R8MAT_AMAX, the maximum absolute value entry of A.
*/
{
  int i;
  int j;
  double value;

  value = r8_abs ( a[0+0*m] );

  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < m; i++ )
    {
      if ( value < r8_abs ( a[i+j*m] ) )
      {
        value = r8_abs ( a[i+j*m] );
      }
    }
  }
  return value;
}

double *r8mat_copy_new ( int m, int n, double a1[] )

/******************************************************************************/
/*
  Purpose:

    R8MAT_COPY_NEW copies one R8MAT to a "new" R8MAT.

  Discussion:

    An R8MAT is a doubly dimensioned array of R8 values, stored as a vector
    in column-major order.

  Licensing:

    This code is distributed under the GNU LGPL license.

  Modified:

    26 July 2008

  Author:

    John Burkardt

  Parameters:

    Input, int M, N, the number of rows and columns.

    Input, double A1[M*N], the matrix to be copied.

    Output, double R8MAT_COPY_NEW[M*N], the copy of A1.
*/
{
  double *a2;
  int i;
  int j;

  a2 = ( double * ) malloc ( m * n * sizeof ( double ) );

  for ( j = 0; j < n; j++ )
  {
    for ( i = 0; i < m; i++ )
    {
      a2[i+j*m] = a1[i+j*m];
    }
  }

  return a2;
}

/******************************************************************************/

void daxpy ( int n, double da, double dx[], int incx, double dy[], int incy )

/******************************************************************************/
/*
  Purpose:

    DAXPY computes constant times a vector plus a vector.

  Discussion:

    This routine uses unrolled loops for increments equal to one.

  Licensing:

    This code is distributed under the GNU LGPL license. 

  Modified:

    30 March 2007

  Author:

    C version by John Burkardt

  Reference:

    Jack Dongarra, Cleve Moler, Jim Bunch, Pete Stewart,
    LINPACK User's Guide,
    SIAM, 1979.

    Charles Lawson, Richard Hanson, David Kincaid, Fred Krogh,
    Basic Linear Algebra Subprograms for Fortran Usage,
    Algorithm 539, 
    ACM Transactions on Mathematical Software, 
    Volume 5, Number 3, September 1979, pages 308-323.

  Parameters:

    Input, int N, the number of elements in DX and DY.

    Input, double DA, the multiplier of DX.

    Input, double DX[*], the first vector.

    Input, int INCX, the increment between successive entries of DX.

    Input/output, double DY[*], the second vector.
    On output, DY[*] has been replaced by DY[*] + DA * DX[*].

    Input, int INCY, the increment between successive entries of DY.
*/
{
  int i;
  int ix;
  int iy;
  int m;

  if ( n <= 0 )
  {
    return;
  }

  if ( da == 0.0 )
  {
    return;
  }
/*
  Code for unequal increments or equal increments
  not equal to 1.
*/
  if ( incx != 1 || incy != 1 )
  {
    if ( 0 <= incx )
    {
      ix = 0;
    }
    else
    {
      ix = ( - n + 1 ) * incx;
    }

    if ( 0 <= incy )
    {
      iy = 0;
    }
    else
    {
      iy = ( - n + 1 ) * incy;
    }

    for ( i = 0; i < n; i++ )
    {
      dy[iy] = dy[iy] + da * dx[ix];
      ix = ix + incx;
      iy = iy + incy;
    }
  }
/*
  Code for both increments equal to 1.
*/
  else
  {
    m = n % 4;

    for ( i = 0; i < m; i++ )
    {
      dy[i] = dy[i] + da * dx[i];
    }

    for ( i = m; i < n; i = i + 4 )
    {
      dy[i  ] = dy[i  ] + da * dx[i  ];
      dy[i+1] = dy[i+1] + da * dx[i+1];
      dy[i+2] = dy[i+2] + da * dx[i+2];
      dy[i+3] = dy[i+3] + da * dx[i+3];
    }
  }
  return;
}
/******************************************************************************/

double ddot ( int n, double dx[], int incx, double dy[], int incy )

/******************************************************************************/
/*
  Purpose:

    DDOT forms the dot product of two vectors.

  Discussion:

    This routine uses unrolled loops for increments equal to one.

  Licensing:

    This code is distributed under the GNU LGPL license. 

  Modified:

    30 March 2007

  Author:

    C version by John Burkardt

  Reference:

    Jack Dongarra, Cleve Moler, Jim Bunch, Pete Stewart,
    LINPACK User's Guide,
    SIAM, 1979.

    Charles Lawson, Richard Hanson, David Kincaid, Fred Krogh,
    Basic Linear Algebra Subprograms for Fortran Usage,
    Algorithm 539, 
    ACM Transactions on Mathematical Software, 
    Volume 5, Number 3, September 1979, pages 308-323.

  Parameters:

    Input, int N, the number of entries in the vectors.

    Input, double DX[*], the first vector.

    Input, int INCX, the increment between successive entries in DX.

    Input, double DY[*], the second vector.

    Input, int INCY, the increment between successive entries in DY.

    Output, double DDOT, the sum of the product of the corresponding
    entries of DX and DY.
*/
{
  double dtemp;
  int i;
  int ix;
  int iy;
  int m;

  dtemp = 0.0;

  if ( n <= 0 )
  {
    return dtemp;
  }
/*
  Code for unequal increments or equal increments
  not equal to 1.
*/
  if ( incx != 1 || incy != 1 )
  {
    if ( 0 <= incx )
    {
      ix = 0;
    }
    else
    {
      ix = ( - n + 1 ) * incx;
    }

    if ( 0 <= incy )
    {
      iy = 0;
    }
    else
    {
      iy = ( - n + 1 ) * incy;
    }

    for ( i = 0; i < n; i++ )
    {
      dtemp = dtemp + dx[ix] * dy[iy];
      ix = ix + incx;
      iy = iy + incy;
    }
  }
/*
  Code for both increments equal to 1.
*/
  else
  {
    m = n % 5;

    for ( i = 0; i < m; i++ )
    {
      dtemp = dtemp + dx[i] * dy[i];
    }

    for ( i = m; i < n; i = i + 5 )
    {
      dtemp = dtemp + dx[i  ] * dy[i  ] 
                    + dx[i+1] * dy[i+1] 
                    + dx[i+2] * dy[i+2] 
                    + dx[i+3] * dy[i+3] 
                    + dx[i+4] * dy[i+4];
    }
  }
  return dtemp;
}
/******************************************************************************/

double dnrm2 ( int n, double x[], int incx )

/******************************************************************************/
/*
  Purpose:

    DNRM2 returns the euclidean norm of a vector.

  Discussion:

     DNRM2 ( X ) = sqrt ( X' * X )

  Licensing:

    This code is distributed under the GNU LGPL license. 

  Modified:

    30 March 2007

  Author:

    C version by John Burkardt

  Reference:

    Jack Dongarra, Cleve Moler, Jim Bunch, Pete Stewart,
    LINPACK User's Guide,
    SIAM, 1979.

    Charles Lawson, Richard Hanson, David Kincaid, Fred Krogh,
    Basic Linear Algebra Subprograms for Fortran Usage,
    Algorithm 539,
    ACM Transactions on Mathematical Software,
    Volume 5, Number 3, September 1979, pages 308-323.

  Parameters:

    Input, int N, the number of entries in the vector.

    Input, double X[*], the vector whose norm is to be computed.

    Input, int INCX, the increment between successive entries of X.

    Output, double DNRM2, the Euclidean norm of X.
*/
{
  double absxi;
  int i;
  int ix;
  double norm;
  double scale;
  double ssq;
  double value;

  if ( n < 1 || incx < 1 )
  {
    norm = 0.0;
  }
  else if ( n == 1 )
  {
    norm = r8_abs ( x[0] );
  }
  else
  {
    scale = 0.0;
    ssq = 1.0;
    ix = 0;

    for ( i = 0; i < n; i++ )
    {
      if ( x[ix] != 0.0 )
      {
        absxi = r8_abs ( x[ix] );
        if ( scale < absxi )
        {
          ssq = 1.0 + ssq * ( scale / absxi ) * ( scale / absxi );
          scale = absxi;
        }
        else
        {
          ssq = ssq + ( absxi / scale ) * ( absxi / scale );
        }
      }
      ix = ix + incx;
    }

    norm  = scale * sqrt ( ssq );
  }

  return norm;
}
/******************************************************************************/

void dqrank ( double a[], int lda, int m, int n, double tol, int *kr, 
  int jpvt[], double qraux[] )

/******************************************************************************/
/*
  Purpose:

    DQRANK computes the QR factorization of a rectangular matrix.

  Discussion:

    This routine is used in conjunction with DQRLSS to solve
    overdetermined, underdetermined and singular linear systems
    in a least squares sense.

    DQRANK uses the LINPACK subroutine DQRDC to compute the QR
    factorization, with column pivoting, of an M by N matrix A.
    The numerical rank is determined using the tolerance TOL.

    Note that on output, ABS ( A(1,1) ) / ABS ( A(KR,KR) ) is an estimate
    of the condition number of the matrix of independent columns,
    and of R.  This estimate will be <= 1/TOL.

  Licensing:

    This code is distributed under the GNU LGPL license. 

  Modified:

    21 April 2012

  Author:

    C version by John Burkardt.

  Reference:

    Jack Dongarra, Cleve Moler, Jim Bunch, Pete Stewart,
    LINPACK User's Guide,
    SIAM, 1979,
    ISBN13: 978-0-898711-72-1,
    LC: QA214.L56.

  Parameters:

    Input/output, double A[LDA*N].  On input, the matrix whose
    decomposition is to be computed.  On output, the information from DQRDC.
    The triangular matrix R of the QR factorization is contained in the
    upper triangle and information needed to recover the orthogonal
    matrix Q is stored below the diagonal in A and in the vector QRAUX.

    Input, int LDA, the leading dimension of A, which must
    be at least M.

    Input, int M, the number of rows of A.

    Input, int N, the number of columns of A.

    Input, double TOL, a relative tolerance used to determine the
    numerical rank.  The problem should be scaled so that all the elements
    of A have roughly the same absolute accuracy, EPS.  Then a reasonable
    value for TOL is roughly EPS divided by the magnitude of the largest
    element.

    Output, int *KR, the numerical rank.

    Output, int JPVT[N], the pivot information from DQRDC.
    Columns JPVT(1), ..., JPVT(KR) of the original matrix are linearly
    independent to within the tolerance TOL and the remaining columns
    are linearly dependent.

    Output, double QRAUX[N], will contain extra information defining
    the QR factorization.
*/
{
  int i;
  int j;
  int job;
  int k;
  double *work;

  for ( i = 0; i < n; i++ )
  {
    jpvt[i] = 0;
  }

  work = ( double * ) malloc ( n * sizeof ( double ) );
  job = 1;

  dqrdc ( a, lda, m, n, qraux, jpvt, work, job );

  *kr = 0;
  k = i4_min ( m, n );

  for ( j = 0; j < k; j++ )
  {
    if ( r8_abs ( a[j+j*lda] ) <= tol * r8_abs ( a[0+0*lda] ) )
    {
      return;
    }
    *kr = j + 1;
  }

  free ( work );

  return;
}
/******************************************************************************/

void dqrdc ( double a[], int lda, int n, int p, double qraux[], int jpvt[], 
  double work[], int job )

/******************************************************************************/
/*
  Purpose:

    DQRDC computes the QR factorization of a real rectangular matrix.

  Discussion:

    DQRDC uses Householder transformations.

    Column pivoting based on the 2-norms of the reduced columns may be
    performed at the user's option.

  Licensing:

    This code is distributed under the GNU LGPL license. 

  Modified:

    07 June 2005

  Author:

    C version by John Burkardt.

  Reference:

    Jack Dongarra, Cleve Moler, Jim Bunch and Pete Stewart,
    LINPACK User's Guide,
    SIAM, (Society for Industrial and Applied Mathematics),
    3600 University City Science Center,
    Philadelphia, PA, 19104-2688.
    ISBN 0-89871-172-X

  Parameters:

    Input/output, double A(LDA,P).  On input, the N by P matrix
    whose decomposition is to be computed.  On output, A contains in
    its upper triangle the upper triangular matrix R of the QR
    factorization.  Below its diagonal A contains information from
    which the orthogonal part of the decomposition can be recovered.
    Note that if pivoting has been requested, the decomposition is not that
    of the original matrix A but that of A with its columns permuted
    as described by JPVT.

    Input, int LDA, the leading dimension of the array A.  LDA must
    be at least N.

    Input, int N, the number of rows of the matrix A.

    Input, int P, the number of columns of the matrix A.

    Output, double QRAUX[P], contains further information required
    to recover the orthogonal part of the decomposition.

    Input/output, integer JPVT[P].  On input, JPVT contains integers that
    control the selection of the pivot columns.  The K-th column A(*,K) of A
    is placed in one of three classes according to the value of JPVT(K).
      > 0, then A(K) is an initial column.
      = 0, then A(K) is a free column.
      < 0, then A(K) is a final column.
    Before the decomposition is computed, initial columns are moved to
    the beginning of the array A and final columns to the end.  Both
    initial and final columns are frozen in place during the computation
    and only free columns are moved.  At the K-th stage of the
    reduction, if A(*,K) is occupied by a free column it is interchanged
    with the free column of largest reduced norm.  JPVT is not referenced
    if JOB == 0.  On output, JPVT(K) contains the index of the column of the
    original matrix that has been interchanged into the K-th column, if
    pivoting was requested.

    Workspace, double WORK[P].  WORK is not referenced if JOB == 0.

    Input, int JOB, initiates column pivoting.
    0, no pivoting is done.
    nonzero, pivoting is done.
*/
{
  int j;
  int jp;
  int l;
  int lup;
  int maxj;
  double maxnrm;
  double nrmxl;
  int pl;
  int pu;
  int swapj;
  double t;
  double tt;

  pl = 1;
  pu = 0;
/*
  If pivoting is requested, rearrange the columns.
*/
  if ( job != 0 )
  {
    for ( j = 1; j <= p; j++ )
    {
      swapj = ( 0 < jpvt[j-1] );

      if ( jpvt[j-1] < 0 )
      {
        jpvt[j-1] = -j;
      }
      else
      {
        jpvt[j-1] = j;
      }

      if ( swapj )
      {
        if ( j != pl )
        {
          dswap ( n, a+0+(pl-1)*lda, 1, a+0+(j-1), 1 );
        }
        jpvt[j-1] = jpvt[pl-1];
        jpvt[pl-1] = j;
        pl = pl + 1;
      }
    }
    pu = p;

    for ( j = p; 1 <= j; j-- )
    {
      if ( jpvt[j-1] < 0 )
      {
        jpvt[j-1] = -jpvt[j-1];

        if ( j != pu )
        {
          dswap ( n, a+0+(pu-1)*lda, 1, a+0+(j-1)*lda, 1 );
          jp = jpvt[pu-1];
          jpvt[pu-1] = jpvt[j-1];
          jpvt[j-1] = jp;
        }
        pu = pu - 1;
      }
    }
  }
/*
  Compute the norms of the free columns.
*/
  for ( j = pl; j <= pu; j++ )
  {
    qraux[j-1] = dnrm2 ( n, a+0+(j-1)*lda, 1 );
  }

  for ( j = pl; j <= pu; j++ )
  {
    work[j-1] = qraux[j-1];
  }
/*
  Perform the Householder reduction of A.
*/
  lup = i4_min ( n, p );

  for ( l = 1; l <= lup; l++ )
  {
/*
  Bring the column of largest norm into the pivot position.
*/
    if ( pl <= l && l < pu )
    {
      maxnrm = 0.0;
      maxj = l;
      for ( j = l; j <= pu; j++ )
      {
        if ( maxnrm < qraux[j-1] )
        {
          maxnrm = qraux[j-1];
          maxj = j;
        }
      }

      if ( maxj != l )
      {
        dswap ( n, a+0+(l-1)*lda, 1, a+0+(maxj-1)*lda, 1 );
        qraux[maxj-1] = qraux[l-1];
        work[maxj-1] = work[l-1];
        jp = jpvt[maxj-1];
        jpvt[maxj-1] = jpvt[l-1];
        jpvt[l-1] = jp;
      }
    }
/*
  Compute the Householder transformation for column L.
*/
    qraux[l-1] = 0.0;

    if ( l != n )
    {
      nrmxl = dnrm2 ( n-l+1, a+l-1+(l-1)*lda, 1 );

      if ( nrmxl != 0.0 )
      {
        if ( a[l-1+(l-1)*lda] != 0.0 )
        {
          nrmxl = nrmxl * r8_sign ( a[l-1+(l-1)*lda] );
        }

        dscal ( n-l+1, 1.0 / nrmxl, a+l-1+(l-1)*lda, 1 );
        a[l-1+(l-1)*lda] = 1.0 + a[l-1+(l-1)*lda];
/*
  Apply the transformation to the remaining columns, updating the norms.
*/
        for ( j = l + 1; j <= p; j++ )
        {
          t = -ddot ( n-l+1, a+l-1+(l-1)*lda, 1, a+l-1+(j-1)*lda, 1 ) 
            / a[l-1+(l-1)*lda];
          daxpy ( n-l+1, t, a+l-1+(l-1)*lda, 1, a+l-1+(j-1)*lda, 1 );

          if ( pl <= j && j <= pu )
          {
            if ( qraux[j-1] != 0.0 )
            {
              tt = 1.0 - pow ( r8_abs ( a[l-1+(j-1)*lda] ) / qraux[j-1], 2 );
              tt = r8_max ( tt, 0.0 );
              t = tt;
              tt = 1.0 + 0.05 * tt * pow ( qraux[j-1] / work[j-1], 2 );

              if ( tt != 1.0 )
              {
                qraux[j-1] = qraux[j-1] * sqrt ( t );
              }
              else
              {
                qraux[j-1] = dnrm2 ( n-l, a+l+(j-1)*lda, 1 );
                work[j-1] = qraux[j-1];
              }
            }
          }
        }
/*
  Save the transformation.
*/
        qraux[l-1] = a[l-1+(l-1)*lda];
        a[l-1+(l-1)*lda] = -nrmxl;
      }
    }
  }
  return;
}
/******************************************************************************/

int dqrls ( double a[], int lda, int m, int n, double tol, int *kr, double b[], 
  double x[], double rsd[], int jpvt[], double qraux[], int itask )

/******************************************************************************/
/*
  Purpose:

    DQRLS factors and solves a linear system in the least squares sense.

  Discussion:

    The linear system may be overdetermined, underdetermined or singular.
    The solution is obtained using a QR factorization of the
    coefficient matrix.

    DQRLS can be efficiently used to solve several least squares
    problems with the same matrix A.  The first system is solved
    with ITASK = 1.  The subsequent systems are solved with
    ITASK = 2, to avoid the recomputation of the matrix factors.
    The parameters KR, JPVT, and QRAUX must not be modified
    between calls to DQRLS.

    DQRLS is used to solve in a least squares sense
    overdetermined, underdetermined and singular linear systems.
    The system is A*X approximates B where A is M by N.
    B is a given M-vector, and X is the N-vector to be computed.
    A solution X is found which minimimzes the sum of squares (2-norm)
    of the residual,  A*X - B.

    The numerical rank of A is determined using the tolerance TOL.

    DQRLS uses the LINPACK subroutine DQRDC to compute the QR
    factorization, with column pivoting, of an M by N matrix A.

  Licensing:

    This code is distributed under the GNU LGPL license. 

  Modified:

    10 September 2012

  Author:

    C version by John Burkardt.

  Reference:

    David Kahaner, Cleve Moler, Steven Nash,
    Numerical Methods and Software,
    Prentice Hall, 1989,
    ISBN: 0-13-627258-4,
    LC: TA345.K34.

  Parameters:

    Input/output, double A[LDA*N], an M by N matrix.
    On input, the matrix whose decomposition is to be computed.
    In a least squares data fitting problem, A(I,J) is the
    value of the J-th basis (model) function at the I-th data point.
    On output, A contains the output from DQRDC.  The triangular matrix R
    of the QR factorization is contained in the upper triangle and
    information needed to recover the orthogonal matrix Q is stored
    below the diagonal in A and in the vector QRAUX.

    Input, int LDA, the leading dimension of A.

    Input, int M, the number of rows of A.

    Input, int N, the number of columns of A.

    Input, double TOL, a relative tolerance used to determine the
    numerical rank.  The problem should be scaled so that all the elements
    of A have roughly the same absolute accuracy EPS.  Then a reasonable
    value for TOL is roughly EPS divided by the magnitude of the largest
    element.

    Output, int *KR, the numerical rank.

    Input, double B[M], the right hand side of the linear system.

    Output, double X[N], a least squares solution to the linear
    system.

    Output, double RSD[M], the residual, B - A*X.  RSD may
    overwrite B.

    Workspace, int JPVT[N], required if ITASK = 1.
    Columns JPVT(1), ..., JPVT(KR) of the original matrix are linearly
    independent to within the tolerance TOL and the remaining columns
    are linearly dependent.  ABS ( A(1,1) ) / ABS ( A(KR,KR) ) is an estimate
    of the condition number of the matrix of independent columns,
    and of R.  This estimate will be <= 1/TOL.

    Workspace, double QRAUX[N], required if ITASK = 1.

    Input, int ITASK.
    1, DQRLS factors the matrix A and solves the least squares problem.
    2, DQRLS assumes that the matrix A was factored with an earlier
       call to DQRLS, and only solves the least squares problem.

    Output, int DQRLS, error code.
    0:  no error
    -1: LDA < M   (fatal error)
    -2: N < 1     (fatal error)
    -3: ITASK < 1 (fatal error)
*/
{
  int ind;

  if ( lda < m )
  {
    /*fprintf ( stderr, "\n" );
    fprintf ( stderr, "DQRLS - Fatal error!\n" );
    fprintf ( stderr, "  LDA < M.\n" );*/
    ind = -1;
    return ind;
  }

  if ( n <= 0 )
  {
    /*fprintf ( stderr, "\n" );
    fprintf ( stderr, "DQRLS - Fatal error!\n" );
    fprintf ( stderr, "  N <= 0.\n" );*/
    ind = -2;
    return ind;
  }

  if ( itask < 1 )
  {
    /*fprintf ( stderr, "\n" );
    fprintf ( stderr, "DQRLS - Fatal error!\n" );
    fprintf ( stderr, "  ITASK < 1.\n" );*/
    ind = -3;
    return ind;
  }

  ind = 0;
/*
  Factor the matrix.
*/
  if ( itask == 1 )
  {
    dqrank ( a, lda, m, n, tol, kr, jpvt, qraux );
  }
/*
  Solve the least-squares problem.
*/
  dqrlss ( a, lda, m, n, *kr, b, x, rsd, jpvt, qraux );

  return ind;
}
/******************************************************************************/

void dqrlss ( double a[], int lda, int m, int n, int kr, double b[], double x[], 
  double rsd[], int jpvt[], double qraux[] )

/******************************************************************************/
/*
  Purpose:

    DQRLSS solves a linear system in a least squares sense.

  Discussion:

    DQRLSS must be preceded by a call to DQRANK.

    The system is to be solved is
      A * X = B
    where
      A is an M by N matrix with rank KR, as determined by DQRANK,
      B is a given M-vector,
      X is the N-vector to be computed.

    A solution X, with at most KR nonzero components, is found which
    minimizes the 2-norm of the residual (A*X-B).

    Once the matrix A has been formed, DQRANK should be
    called once to decompose it.  Then, for each right hand
    side B, DQRLSS should be called once to obtain the
    solution and residual.

  Licensing:

    This code is distributed under the GNU LGPL license. 

  Modified:

    10 September 2012

  Author:

    C version by John Burkardt

  Parameters:

    Input, double A[LDA*N], the QR factorization information
    from DQRANK.  The triangular matrix R of the QR factorization is
    contained in the upper triangle and information needed to recover
    the orthogonal matrix Q is stored below the diagonal in A and in
    the vector QRAUX.

    Input, int LDA, the leading dimension of A, which must
    be at least M.

    Input, int M, the number of rows of A.

    Input, int N, the number of columns of A.

    Input, int KR, the rank of the matrix, as estimated by DQRANK.

    Input, double B[M], the right hand side of the linear system.

    Output, double X[N], a least squares solution to the
    linear system.

    Output, double RSD[M], the residual, B - A*X.  RSD may
    overwrite B.

    Input, int JPVT[N], the pivot information from DQRANK.
    Columns JPVT[0], ..., JPVT[KR-1] of the original matrix are linearly
    independent to within the tolerance TOL and the remaining columns
    are linearly dependent.

    Input, double QRAUX[N], auxiliary information from DQRANK
    defining the QR factorization.
*/
{
  int i;
  int info;
  int j;
  int job;
  int k;
  double t;

  if ( kr != 0 )
  {
    job = 110;
    info = dqrsl ( a, lda, m, kr, qraux, b, rsd, rsd, x, rsd, rsd, job );
  }

  for ( i = 0; i < n; i++ )
  {
    jpvt[i] = - jpvt[i];
  }

  for ( i = kr; i < n; i++ )
  {
    x[i] = 0.0;
  }

  for ( j = 1; j <= n; j++ )
  {
    if ( jpvt[j-1] <= 0 )
    {
      k = - jpvt[j-1];
      jpvt[j-1] = k;

      while ( k != j )
      {
        t = x[j-1];
        x[j-1] = x[k-1];
        x[k-1] = t;
        jpvt[k-1] = -jpvt[k-1];
        k = jpvt[k-1];
      }
    }
  }
  return;
}
/******************************************************************************/

int dqrsl ( double a[], int lda, int n, int k, double qraux[], double y[], 
  double qy[], double qty[], double b[], double rsd[], double ab[], int job )

/******************************************************************************/
/*
  Purpose:

    DQRSL computes transformations, projections, and least squares solutions.

  Discussion:

    DQRSL requires the output of DQRDC.

    For K <= min(N,P), let AK be the matrix

      AK = ( A(JPVT[0]), A(JPVT(2)), ..., A(JPVT(K)) )

    formed from columns JPVT[0], ..., JPVT(K) of the original
    N by P matrix A that was input to DQRDC.  If no pivoting was
    done, AK consists of the first K columns of A in their
    original order.  DQRDC produces a factored orthogonal matrix Q
    and an upper triangular matrix R such that

      AK = Q * (R)
               (0)

    This information is contained in coded form in the arrays
    A and QRAUX.

    The parameters QY, QTY, B, RSD, and AB are not referenced
    if their computation is not requested and in this case
    can be replaced by dummy variables in the calling program.
    To save storage, the user may in some cases use the same
    array for different parameters in the calling sequence.  A
    frequently occurring example is when one wishes to compute
    any of B, RSD, or AB and does not need Y or QTY.  In this
    case one may identify Y, QTY, and one of B, RSD, or AB, while
    providing separate arrays for anything else that is to be
    computed.

    Thus the calling sequence

      dqrsl ( a, lda, n, k, qraux, y, dum, y, b, y, dum, 110, info )

    will result in the computation of B and RSD, with RSD
    overwriting Y.  More generally, each item in the following
    list contains groups of permissible identifications for
    a single calling sequence.

      1. (Y,QTY,B) (RSD) (AB) (QY)

      2. (Y,QTY,RSD) (B) (AB) (QY)

      3. (Y,QTY,AB) (B) (RSD) (QY)

      4. (Y,QY) (QTY,B) (RSD) (AB)

      5. (Y,QY) (QTY,RSD) (B) (AB)

      6. (Y,QY) (QTY,AB) (B) (RSD)

    In any group the value returned in the array allocated to
    the group corresponds to the last member of the group.

  Licensing:

    This code is distributed under the GNU LGPL license. 

  Modified:

    07 June 2005

  Author:

    C version by John Burkardt.

  Reference:

    Jack Dongarra, Cleve Moler, Jim Bunch and Pete Stewart,
    LINPACK User's Guide,
    SIAM, (Society for Industrial and Applied Mathematics),
    3600 University City Science Center,
    Philadelphia, PA, 19104-2688.
    ISBN 0-89871-172-X

  Parameters:

    Input, double A[LDA*P], contains the output of DQRDC.

    Input, int LDA, the leading dimension of the array A.

    Input, int N, the number of rows of the matrix AK.  It must
    have the same value as N in DQRDC.

    Input, int K, the number of columns of the matrix AK.  K
    must not be greater than min(N,P), where P is the same as in the
    calling sequence to DQRDC.

    Input, double QRAUX[P], the auxiliary output from DQRDC.

    Input, double Y[N], a vector to be manipulated by DQRSL.

    Output, double QY[N], contains Q * Y, if requested.

    Output, double QTY[N], contains Q' * Y, if requested.

    Output, double B[K], the solution of the least squares problem
      minimize norm2 ( Y - AK * B),
    if its computation has been requested.  Note that if pivoting was
    requested in DQRDC, the J-th component of B will be associated with
    column JPVT(J) of the original matrix A that was input into DQRDC.

    Output, double RSD[N], the least squares residual Y - AK * B,
    if its computation has been requested.  RSD is also the orthogonal
    projection of Y onto the orthogonal complement of the column space
    of AK.

    Output, double AB[N], the least squares approximation Ak * B,
    if its computation has been requested.  AB is also the orthogonal
    projection of Y onto the column space of A.

    Input, integer JOB, specifies what is to be computed.  JOB has
    the decimal expansion ABCDE, with the following meaning:

      if A != 0, compute QY.
      if B != 0, compute QTY.
      if C != 0, compute QTY and B.
      if D != 0, compute QTY and RSD.
      if E != 0, compute QTY and AB.

    Note that a request to compute B, RSD, or AB automatically triggers
    the computation of QTY, for which an array must be provided in the
    calling sequence.

    Output, int DQRSL, is zero unless the computation of B has
    been requested and R is exactly singular.  In this case, INFO is the
    index of the first zero diagonal element of R, and B is left unaltered.
*/
{
  int cab;
  int cb;
  int cqty;
  int cqy;
  int cr;
  int i;
  int info;
  int j;
  int jj;
  int ju;
  double t;
  double temp;
/*
  Set INFO flag.
*/
  info = 0;
/*
  Determine what is to be computed.
*/
  cqy =  (   job / 10000          != 0 );
  cqty = ( ( job %  10000 )       != 0 );
  cb =   ( ( job %   1000 ) / 100 != 0 );
  cr =   ( ( job %    100 ) /  10 != 0 );
  cab =  ( ( job %     10 )       != 0 );

  ju = i4_min ( k, n-1 );
/*
  Special action when N = 1.
*/
  if ( ju == 0 )
  {
    if ( cqy )
    {
      qy[0] = y[0];
    }

    if ( cqty )
    {
      qty[0] = y[0];
    }

    if ( cab )
    {
      ab[0] = y[0];
    }

    if ( cb )
    {
      if ( a[0+0*lda] == 0.0 )
      {
        info = 1;
      }
      else
      {
        b[0] = y[0] / a[0+0*lda];
      }
    }

    if ( cr )
    {
      rsd[0] = 0.0;
    }
    return info;
  }
/*
  Set up to compute QY or QTY.
*/
  if ( cqy )
  {
    for ( i = 1; i <= n; i++ )
    {
      qy[i-1] = y[i-1];
    }
  }

  if ( cqty )
  {
    for ( i = 1; i <= n; i++ )
    {
      qty[i-1] = y[i-1];
    }
  }
/*
  Compute QY.
*/
  if ( cqy )
  {
    for ( jj = 1; jj <= ju; jj++ )
    {
      j = ju - jj + 1;

      if ( qraux[j-1] != 0.0 )
      {
        temp = a[j-1+(j-1)*lda];
        a[j-1+(j-1)*lda] = qraux[j-1];
        t = -ddot ( n-j+1, a+j-1+(j-1)*lda, 1, qy+j-1, 1 ) / a[j-1+(j-1)*lda];
        daxpy ( n-j+1, t, a+j-1+(j-1)*lda, 1, qy+j-1, 1 );
        a[j-1+(j-1)*lda] = temp;
      }
    }
  }
/*
  Compute Q'*Y.
*/
  if ( cqty )
  {
    for ( j = 1; j <= ju; j++ )
    {
      if ( qraux[j-1] != 0.0 )
      {
        temp = a[j-1+(j-1)*lda];
        a[j-1+(j-1)*lda] = qraux[j-1];
        t = -ddot ( n-j+1, a+j-1+(j-1)*lda, 1, qty+j-1, 1 ) / a[j-1+(j-1)*lda];
        daxpy ( n-j+1, t, a+j-1+(j-1)*lda, 1, qty+j-1, 1 );
        a[j-1+(j-1)*lda] = temp;
      }
    }
  }
/*
  Set up to compute B, RSD, or AB.
*/
  if ( cb )
  {
    for ( i = 1; i <= k; i++ )
    {
      b[i-1] = qty[i-1];
    }
  }

  if ( cab )
  {
    for ( i = 1; i <= k; i++ )
    {
      ab[i-1] = qty[i-1];
    }
  }

  if ( cr && k < n )
  {
    for ( i = k+1; i <= n; i++ )
    {
      rsd[i-1] = qty[i-1];
    }
  }

  if ( cab && k+1 <= n )
  {
    for ( i = k+1; i <= n; i++ )
    {
      ab[i-1] = 0.0;
    }
  }

  if ( cr )
  {
    for ( i = 1; i <= k; i++ )
    {
      rsd[i-1] = 0.0;
    }
  }
/*
  Compute B.
*/
  if ( cb )
  {
    for ( jj = 1; jj <= k; jj++ )
    {
      j = k - jj + 1;

      if ( a[j-1+(j-1)*lda] == 0.0 )
      {
        info = j;
        break;
      }

      b[j-1] = b[j-1] / a[j-1+(j-1)*lda];

      if ( j != 1 )
      {
        t = -b[j-1];
        daxpy ( j-1, t, a+0+(j-1)*lda, 1, b, 1 );
      }
    }
  }
/*
  Compute RSD or AB as required.
*/
  if ( cr || cab )
  {
    for ( jj = 1; jj <= ju; jj++ )
    {
      j = ju - jj + 1;

      if ( qraux[j-1] != 0.0 )
      {
        temp = a[j-1+(j-1)*lda];
        a[j-1+(j-1)*lda] = qraux[j-1];

        if ( cr )
        {
          t = -ddot ( n-j+1, a+j-1+(j-1)*lda, 1, rsd+j-1, 1 ) 
            / a[j-1+(j-1)*lda];
          daxpy ( n-j+1, t, a+j-1+(j-1)*lda, 1, rsd+j-1, 1 );
        }

        if ( cab )
        {
          t = -ddot ( n-j+1, a+j-1+(j-1)*lda, 1, ab+j-1, 1 ) 
            / a[j-1+(j-1)*lda];
          daxpy ( n-j+1, t, a+j-1+(j-1)*lda, 1, ab+j-1, 1 );
        }
        a[j-1+(j-1)*lda] = temp;
      }
    }
  }

  return info;
}
/******************************************************************************/

/******************************************************************************/

void dscal ( int n, double sa, double x[], int incx )

/******************************************************************************/
/*
  Purpose:

    DSCAL scales a vector by a constant.

  Licensing:

    This code is distributed under the GNU LGPL license. 

  Modified:

    30 March 2007

  Author:

    C version by John Burkardt

  Reference:

    Jack Dongarra, Cleve Moler, Jim Bunch, Pete Stewart,
    LINPACK User's Guide,
    SIAM, 1979.

    Charles Lawson, Richard Hanson, David Kincaid, Fred Krogh,
    Basic Linear Algebra Subprograms for Fortran Usage,
    Algorithm 539,
    ACM Transactions on Mathematical Software,
    Volume 5, Number 3, September 1979, pages 308-323.

  Parameters:

    Input, int N, the number of entries in the vector.

    Input, double SA, the multiplier.

    Input/output, double X[*], the vector to be scaled.

    Input, int INCX, the increment between successive entries of X.
*/
{
  int i;
  int ix;
  int m;

  if ( n <= 0 )
  {
  }
  else if ( incx == 1 )
  {
    m = n % 5;

    for ( i = 0; i < m; i++ )
    {
      x[i] = sa * x[i];
    }

    for ( i = m; i < n; i = i + 5 )
    {
      x[i]   = sa * x[i];
      x[i+1] = sa * x[i+1];
      x[i+2] = sa * x[i+2];
      x[i+3] = sa * x[i+3];
      x[i+4] = sa * x[i+4];
    }
  }
  else
  {
    if ( 0 <= incx )
    {
      ix = 0;
    }
    else
    {
      ix = ( - n + 1 ) * incx;
    }

    for ( i = 0; i < n; i++ )
    {
      x[ix] = sa * x[ix];
      ix = ix + incx;
    }
  }
  return;
}
/******************************************************************************/


void dswap ( int n, double x[], int incx, double y[], int incy )

/******************************************************************************/
/*
  Purpose:

    DSWAP interchanges two vectors.

  Licensing:

    This code is distributed under the GNU LGPL license. 

  Modified:

    30 March 2007

  Author:

    C version by John Burkardt

  Reference:

    Jack Dongarra, Cleve Moler, Jim Bunch, Pete Stewart,
    LINPACK User's Guide,
    SIAM, 1979.

    Charles Lawson, Richard Hanson, David Kincaid, Fred Krogh,
    Basic Linear Algebra Subprograms for Fortran Usage,
    Algorithm 539, 
    ACM Transactions on Mathematical Software, 
    Volume 5, Number 3, September 1979, pages 308-323.

  Parameters:

    Input, int N, the number of entries in the vectors.

    Input/output, double X[*], one of the vectors to swap.

    Input, int INCX, the increment between successive entries of X.

    Input/output, double Y[*], one of the vectors to swap.

    Input, int INCY, the increment between successive elements of Y.
*/
{
  int i;
  int ix;
  int iy;
  int m;
  double temp;

  if ( n <= 0 )
  {
  }
  else if ( incx == 1 && incy == 1 )
  {
    m = n % 3;

    for ( i = 0; i < m; i++ )
    {
      temp = x[i];
      x[i] = y[i];
      y[i] = temp;
    }

    for ( i = m; i < n; i = i + 3 )
    {
      temp = x[i];
      x[i] = y[i];
      y[i] = temp;

      temp = x[i+1];
      x[i+1] = y[i+1];
      y[i+1] = temp;

      temp = x[i+2];
      x[i+2] = y[i+2];
      y[i+2] = temp;
    }
  }
  else
  {
    if ( 0 <= incx )
    {
      ix = 0;
    }
    else
    {
      ix = ( - n + 1 ) * incx;
    }

    if ( 0 <= incy )
    {
      iy = 0;
    }
    else
    {
      iy = ( - n + 1 ) * incy;
    }

    for ( i = 0; i < n; i++ )
    {
      temp = x[ix];
      x[ix] = y[iy];
      y[iy] = temp;
      ix = ix + incx;
      iy = iy + incy;
    }

  }

  return;
}
/******************************************************************************/

/******************************************************************************/

double *qr_solve ( int m, int n, double a[], double b[] )

/******************************************************************************/
/*
  Purpose:

    QR_SOLVE solves a linear system in the least squares sense.

  Discussion:

    If the matrix A has full column rank, then the solution X should be the
    unique vector that minimizes the Euclidean norm of the residual.

    If the matrix A does not have full column rank, then the solution is
    not unique; the vector X will minimize the residual norm, but so will
    various other vectors.

  Licensing:

    This code is distributed under the GNU LGPL license.

  Modified:

    11 September 2012

  Author:

    John Burkardt

  Reference:

    David Kahaner, Cleve Moler, Steven Nash,
    Numerical Methods and Software,
    Prentice Hall, 1989,
    ISBN: 0-13-627258-4,
    LC: TA345.K34.

  Parameters:

    Input, int M, the number of rows of A.

    Input, int N, the number of columns of A.

    Input, double A[M*N], the matrix.

    Input, double B[M], the right hand side.

    Output, double QR_SOLVE[N], the least squares solution.
*/
{
  double *a_qr;
  int ind;
  int itask;
  int *jpvt;
  int kr;
  int lda;
  double *qraux;
  double *r;
  double tol;
  double *x;

  a_qr = r8mat_copy_new ( m, n, a );
  lda = m;
  tol = r8_epsilon ( ) / r8mat_amax ( m, n, a_qr );
  x = ( double * ) malloc ( n * sizeof ( double ) );
  jpvt = ( int * ) malloc ( n * sizeof ( int ) );
  qraux = ( double * ) malloc ( n * sizeof ( double ) );
  r = ( double * ) malloc ( m * sizeof ( double ) );
  itask = 1;

  ind = dqrls ( a_qr, lda, m, n, tol, &kr, b, x, r, jpvt, qraux, itask );

  free ( a_qr );
  free ( jpvt );
  free ( qraux ); 
  free ( r );

  return x;
}
/******************************************************************************/

#endif
