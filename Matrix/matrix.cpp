#include "matrix.h"
#include<cmath>
#include<cstdlib>
#include<iostream>

void Matrix::SetAllElem(void){
	double input=0;
	for(int i=0;i<row;i++){
		for(int j=0;j<col;j++){
			std::cin>>input;
			A[i][j]=input;
		}
	}
}

void Matrix::FillAll(double val){
	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++)
			A[i][j]=val;
}

void Matrix::ShowElem(void){
	for(int i=0;i<row;i++){
		for(int j=0;j<col;j++){
			std::cout<<A[i][j]<<" ";
		}
		std::cout<<std::endl;
	}
}

void Matrix::SetElem(unsigned int row,unsigned int col,double val){
	if(!(1<= row && row<=this->row)) fprintf(stderr,"idx error\n");
	if(!(1<= col && col<=this->col)) fprintf(stderr,"idx error\n");
	
	A[row-1][col-1]=val;
}

double Matrix::GetElem(unsigned int row,unsigned int col){
	if(!(1<= row && row<=this->row)) fprintf(stderr,"idx error\n");
	if(!(1<= col && col<=this->col)) fprintf(stderr,"idx error\n");
	return A[row-1][col-1];
}

Matrix::Matrix(const Matrix& rhs){
	this->row=rhs.row;
	this->col=rhs.col;
	A.resize(row);
	for(int i=0;i<col;i++)
		A[i].resize(col);
	A=rhs.A;
}

Matrix& Matrix::operator=(const Matrix& rhs){
	if(!isSameSize(rhs))
		fprintf(stderr,"Match matrix size.\n");
	A=rhs.A;
		
	return (*this);
}

Matrix Matrix::operator+(const Matrix& rhs)const{
	if(!isSameSize(rhs))
		fprintf(stderr,"Match matrix size.\n");
	Matrix sum(row,col);
	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++)
			sum.A[i][j]=A[i][j]+rhs.A[i][j];
	return sum;
}

Matrix Matrix::operator-(const Matrix& rhs)const{
	if(!isSameSize(rhs))
		fprintf(stderr,"Match matrix size.\n");
	Matrix diff(row,col);
	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++)
			diff.A[i][j]=A[i][j]-rhs.A[i][j];
	return diff;
}

Matrix Matrix::operator*(const Matrix& rhs)const{
	if(!CanMultiply(rhs))
		fprintf(stderr,"Make matrix size proper.\n");
	Matrix multi(row,rhs.col);
	for(int i=0;i<row;i++){
		for(int j=0;j<rhs.col;j++){
			double sum=0;
			for(int k=0;k<col;k++){
				sum+=A[i][k]*rhs.A[k][j];
			}
			multi.A[i][j]=sum;
		}
	}
	return multi;
}

Matrix Matrix::operator*(const double c)const{
	Matrix multi(row,col);
	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++)
			multi.A[i][j]=c*A[i][j];

	return multi;
}

Matrix Matrix::operator/(const double c)const{
	Matrix multi(row,col);
	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++)
			multi.A[i][j]=A[i][j]/c;
	return multi;
}


Matrix& Matrix::operator+=(const Matrix& rhs){
	if(!isSameSize(rhs))
		fprintf(stderr,"Match matrix size.\n");
	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++)
			A[i][j]+=rhs.A[i][j];
	return *this;
}

Matrix& Matrix::operator-=(const Matrix& rhs){
	if(!isSameSize(rhs))
		fprintf(stderr,"Match matrix size.\n");
	for(int i=0;i<row;i++)
		for(int j=0;j<col;j++)
			A[i][j]-=rhs.A[i][j];
	return *this;
}

Matrix operator*(const double c,const Matrix& X){
	Matrix multi(X.row,X.col);
	for(int i=0;i<X.row;i++)
		for(int j=0;j<X.col;j++)
			multi.A[i][j]=c*X.A[i][j];

	return multi;
}

Matrix operator/(const double c,const Matrix& X){
	Matrix multi(X.row,X.col);
	for(int i=0;i<X.row;i++)
		for(int j=0;j<X.col;j++)
			multi.A[i][j]=X.A[i][j]/c;
	return multi;
}

double InnerProd(const Matrix& a,const Matrix& b){
	double sum=0;
	for(int i=0;i<a.row;i++)
		sum+=a.A[i][0]*b.A[i][0];
	return sum;
}

double det(const Matrix& X){
	if(!X.isSquare()) 
		fprintf(stderr,"Argument matrix is not square.\n");
	int *pivot=(int*)malloc(sizeof(int)*X.row);
	Matrix LU_X=LU(X,pivot);
	double prod=1;
	for(int i=0;i<LU_X.row;i++)
		prod*=LU_X.A[pivot[i]][pivot[i]];
	free(pivot);
	return prod;
}

double tr(const Matrix& X){
	if(!X.isSquare()) 
		fprintf(stderr,"Argument matrix is not square.\n");
	double sum=0;
	for(int i=0;i<X.row;i++)
		sum+=X.A[i][i];
	return sum;
}

Matrix diag(const Matrix& X){
	Matrix D(X.row,X.col);
	for(int i=0;i<X.row;i++)
		D.A[i][i]=X.A[i][i];
	return D;
}

Matrix transpose(const Matrix& X){
	Matrix tp(X.col,X.row);
	for(int i=0;i<X.col;i++)
		for(int j=0;j<X.row;j++)
			tp.A[i][j]=X.A[j][i];
	return tp;
}

Matrix expm(const Matrix& X){
	if(!X.isSquare())
		fprintf(stderr,"Argument matrix is not square.\n");
	const int order=10;
	double factorial=1;
	Matrix expX=identity(X.row);
	Matrix Y=identity(X.row);
	for(int i=1;i<order;i++){
		factorial*=i;
		Y=Y*X;
		expX+=Y/factorial;
	}
	return expX;
}

double l1_norm(const Matrix& X){
	double max=sum_col(X,1);
	for(int i=2;i<=X.col;i++)
		max=max>sum_col(X,i)?max:sum_col(X,i);
	return max;
}

double sum_col(const Matrix& X, unsigned int col){
	double sum=0;
	for(int i=0;i<X.row;i++)
		sum+=abs(X.A[i][col-1]);
	return sum;
}

double sup_norm(const Matrix& X){
	double max=sum_row(X,1);
	for(int i=2;i<=X.row;i++)
		max=max>sum_row(X,i)?max:sum_row(X,i);
	return max;
}

double sum_row(const Matrix& X,unsigned int row){
	double sum=0;
	for(int i=0;i<X.col;i++)
		sum+=abs(X.A[row-1][i]);
	return sum;
}

double F_norm(const Matrix& X){
	double sum=0;
	for(int i=0;i<X.row;i++)
		for(int j=0;j<X.col;j++)
			sum+=X.A[i][j]*X.A[i][j];
	return sqrt(sum);
}

Matrix GaussianElim(Matrix A, Matrix b){
	if(!A.isSquare())
		fprintf(stderr,"Argument matrix is not square.\n");
	int *pivot=(int*)malloc(sizeof(int)*b.row);
	for(int i=0;i<b.row;i++)
		pivot[i]=i;
	forward_elim(A,b,pivot);
	Matrix x=back_subs(A,b,pivot);
	free(pivot);
	return x;
	
}

void forward_elim(Matrix& A,Matrix& b,int *pivot){
	for(int i=0;i<b.row-1;i++){
		pivotting(A,pivot,i);
		for(int j=i+1;j<b.row;j++){
			double m=A.A[pivot[j]][i]/A.A[pivot[i]][i];
			for(int k=i;k<b.row;k++){
				A.A[pivot[j]][k]-=m*A.A[pivot[i]][k];
			}
			b.A[pivot[j]][0]-=m*b.A[pivot[i]][0];
		}
	}
}

Matrix back_subs(const Matrix& A,const Matrix& b,int *pivot){
	Matrix x(b.row,b.col);
	const int n=b.row;
	x.A[n-1][0]=b.A[pivot[n-1]][0]/A.A[pivot[n-1]][n-1];
	for(int i=n-2;i>=0;i--){
		x.A[i][0]=b.A[pivot[i]][0];
		for(int j=n-1;j>i;j--)
			x.A[i][0]-=A.A[pivot[i]][j]*x.A[j][0];
		x.A[i][0]/=A.A[pivot[i]][i];
	}
	return x;
}

void pivotting(const Matrix& A,int *pivot,int i){
	int max_i=i;
	for(int j=i+1;j<A.row;j++){
		max_i=abs(A.A[pivot[j]][i])>abs(A.A[pivot[max_i]][i])?j:max_i;
	}
	int tmp=pivot[i];
	pivot[i]=pivot[max_i];
	pivot[max_i]=tmp;
}

Matrix LU(Matrix X,int *pivot){
	for(int i=0;i<X.row;i++) pivot[i]=i;
	for(int i=0;i<X.row-1;i++){
	//	pivotting(X,pivot,i);
		double w=X.A[pivot[i]][pivot[i]];
		for(int j=i+1;j<X.row;j++){
			X.A[pivot[j]][i]/=w;
			for(int k=i+1;k<X.col;k++){
				X.A[pivot[j]][k]-=X.A[pivot[j]][i]*X.A[pivot[i]][k];
			}
		}
	}
	return X;
}

double PowerEigen(const Matrix& X,const Matrix& init){
	Matrix x=init;
	double eigen=0;
	const double eps=10e-10;
	const int loop_max=100;
	for(int i=0;i<loop_max;i++){
		Matrix y=X*x;
		double new_eigen=y.A[0][0]/x.A[0][0];
		if(abs(new_eigen-eigen)<abs(eigen)*eps) break;
		x=y/y.A[0][0];
		eigen=new_eigen;
	}
	return eigen;
}

Matrix JacobiEigen(const Matrix& X,Matrix& U){
	if(!X.isSquare())
		fprintf(stderr,"Argument matrix is not square.\n");
	Matrix D=X;
	int loop_max=10;
	for(int loop=0;loop<loop_max;loop++){
		Matrix rotate=identity(X.row);
		int max_i=0;
		int max_j=1;
		for(int i=0;i<D.row;i++){
			for(int j=0;j<D.col;j++){
				if(i==j) continue;
				if(abs(D.A[i][j])>abs(D.A[max_i][max_j])){
					max_i=i;
					max_j=j;
				}
			}
		}
		double cos2=abs(D.A[max_i][max_i]-D.A[max_j][max_j])/sqrt(4*D.A[max_i][max_j]*D.A[max_i][max_j]+(D.A[max_i][max_i]-D.A[max_j][max_j])*(D.A[max_i][max_i]-D.A[max_j][max_j]));
		double cos=sqrt((1+cos2)/2);
		double sin=sgn(-D.A[max_i][max_j]*(D.A[max_i][max_i]-D.A[max_j][max_j]))*sqrt((1-cos2)/2);
		rotate.A[max_i][max_i]=cos;
		rotate.A[max_i][max_j]=sin;
		rotate.A[max_j][max_i]=-sin;
		rotate.A[max_j][max_j]=cos;
		D=transpose(rotate)*D*rotate;
		U=U*rotate;

	}
	return D;
}


Matrix identity(unsigned int n){
	Matrix I(n,n);
	for(int i=1;i<=n;i++)
		I.SetElem(i,i,1);
	return I;
}

