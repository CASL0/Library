#include<vector>

#ifndef MAT_H
#define MAT_H

class Matrix{
public:
	Matrix(unsigned int row,unsigned int col){
		this->row=row;
		this->col=col;
		A.resize(row);
		for(int i=0;i<row;i++)
			A[i].resize(col);
	}
	~Matrix(void){}
	Matrix(const Matrix& rhs);
	
	Matrix& operator=(const Matrix& rhs);
	Matrix operator+(const Matrix& rhs)const;
	Matrix operator-(const Matrix& rhs)const;
	Matrix operator*(const Matrix& rhs)const;

	//スカラー倍
	Matrix operator*(const double c)const;
	Matrix operator/(const double c)const;

	Matrix& operator+=(const Matrix& rhs);
	Matrix& operator-=(const Matrix& rhs);

	//スカラー倍
	friend Matrix operator*(const double c,const Matrix& X);
	friend Matrix operator/(const double c,const Matrix& X);
	//ベクトルa，bの標準内積を返す
	friend double InnerProd(const Matrix& a,const Matrix& b);
	//行列Xの行列式を返す
	friend double det(const Matrix& X);
	//行列Xのトレースを返す
	friend double tr(const Matrix& X);
	//行列Xの対角成分のみを要素に持つ行列を返す
	friend Matrix diag(const Matrix& X);
	//行列Xの転置を返す
	friend Matrix transpose(const Matrix& X);
	//行列Xの指数関数を返す
	friend Matrix expm(const Matrix& X);
	//行列XのL1ノルムを返す	
	friend double l1_norm(const Matrix& X);
	//行列Xのcol列の列和を返す
	friend double sum_col(const Matrix& X, unsigned int col);
	//行列Xのsupノルムを返す	
	friend double sup_norm(const Matrix& X);
	//行列Xのrow行の行和を返す
	friend double sum_row(const Matrix& X, unsigned int row);
	//行列Xのフロベニウスノルムを返す
	friend double F_norm(const Matrix& X);
	//行列Xの絶対値最大の固有値を返す
	friend double PowerEigen(const Matrix& X,const Matrix& init);
	//行列Xを固有値分解する
	//引数: 
	//    X: 実対称行列
	//返り値:
	//    U(参照渡し): 各列が固有ベクトル
	//    対角成分が固有値である対角行列
	friend Matrix JacobiEigen(const Matrix& X,Matrix& U);

	//ガウスの消去法により連立１次方程式Ax=bを解く関数
	//引数: 
	//    A: 係数行列(正方)
	//    b: ベクトル
	//返り値:
	//    Ax=bの解x
	friend Matrix GaussianElim(Matrix A,Matrix b);
	friend void forward_elim(Matrix& A, Matrix& b,int *pivot);
	friend Matrix back_subs(const Matrix& A,const Matrix& b,int *pivot);
	friend void pivotting(const Matrix& A,int *pivot,int i);
	//枢軸選択なし
	friend Matrix LU(Matrix X,int *pivot);

	void SetAllElem(void);
	//メンバ行列のrow行col列目にvalをセットする
	void SetElem(unsigned int row,unsigned int col,double val);
	//メンバ行列のすべての要素にvalをセットする
	void FillAll(double val);
	//メンバ行列のrow行col列目の値を返す
	double GetElem(unsigned int row,unsigned int col);
	void ShowElem(void);
private:
	//メンバの行列が引数の行列と同じサイズかを判定(足し算や引き算が可能かの判定)
	//同じサイズならばtrue, そうでなければfalseを返す
	inline bool isSameSize(const Matrix& rhs) const{
		bool size_check=1;
		if(this->row!=rhs.row) size_check&=0;
		if(this->col!=rhs.col) size_check&=0;
		return size_check;
	}
	//メンバの行列との掛け算が可能かどうかを判定
	//掛け算が可能ならばtrue, そうでなければfalseを返す
	inline bool CanMultiply(const Matrix& rhs)const{
		return col==rhs.row?1:0;
	}
	//メンバの行列が正方行列かどうかを判定
	//正方ならtrue, そうでなければfalseを返す
	inline bool isSquare(void)const{
		return row==col;
	}
	
private:
	std::vector<std::vector<double> > A;
	unsigned int row;
	unsigned int col;
};

//単位行列を生成する関数
//生成した行列のサイズnを引数で受け取り，そのサイズの単位行列を返す
extern Matrix identity(unsigned int n);

template <typename T>
T abs(T x){
	return x>=0?x:-x;
}

template <typename T>
T sgn(T x){
	return x>=0?1:-1;
}

#endif	//MAT_H
