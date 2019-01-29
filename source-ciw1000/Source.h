#if !defined(AFX_SOURCE_H__AAFF9B97_1683_4AB0_933A_0175C5156B03__INCLUDED_)
#define AFX_SOURCE_H__AAFF9B97_1683_4AB0_933A_0175C5156B03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const int TOTAL_LENGTH = 8;
typedef vector<int> vec_GunCoordinate;

class CGun
{
public:
	void Excute();

public:
	CGun();
	~CGun();

private:
	BOOL ReadMap();
	BOOL GetFirstGunCoordinate();
	void SetGunPosition();
	void SetTotalGunNum( int& aNewGunTotalNum, vector<int>& aNewGunCoordinate );
	void CheckSameMaxGunNum( int aNewGunTotalNum, vector<int> aNewGunCoordinate );
	void View();

private:
	BOOL CheckGunAcross( int aAcrossIndex, int aHeightIndex );
	BOOL CheckGunHeight( int aAcrossIndex, int aHeightIndex );

private:
	int  theMapFIndex;
	int  theMapSIndex;
	int  theMaxGunNum;
	vector<vec_GunCoordinate> theGunCoordinate_list;			//ÃÑÀÇ ÁÂÇ¥¸¦ °¡Áö°í ÀÖ´Â ¸®½ºÆ®.
	char theMap[TOTAL_LENGTH][TOTAL_LENGTH];					//¸ÊÁÂÇ¥ 

};


#endif // !defined(AFX_SOURCE_H__AAFF9B97_1683_4AB0_933A_0175C5156B03__INCLUDED_)
