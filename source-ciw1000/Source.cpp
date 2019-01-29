#include "stdafx.h"
#include "Source.h"

CGun::CGun() : 
theMapFIndex(0),
theMapSIndex(0),
theMaxGunNum(0)
{
}

CGun::~CGun()
{

}

void CGun::Excute()
{
	//파일에서 맵을 읽자..
	if( ReadMap() == FALSE )
	{
		return;
	}

	//처음 총잡이 넣을 좌표 구한다..
	while( GetFirstGunCoordinate() != FALSE ) 
	{
		//조건이 맞는 공간이 있으면 총잡이로 바꾸어 주자..
		SetGunPosition();

		//총총잡이 개수를 구하고 그 때의 좌표 저장..
		int newguntotalnum = 0;
		vec_GunCoordinate newguncoordinate;
		newguncoordinate.reserve(20);
		SetTotalGunNum( newguntotalnum, newguncoordinate );

		//현재 구한 총총잡이 개수와 비교를 해서 작으면 삭제
		if( theMaxGunNum > newguntotalnum )
			continue;

		//크면 기존거 전부 삭제.. 새로 등록 
		if( theMaxGunNum < newguntotalnum )
		{
			theGunCoordinate_list.clear();
			theMaxGunNum = newguntotalnum;
			theGunCoordinate_list.push_back(newguncoordinate);
		}

		//같은면 등록할 때 기존의 총잡이 위치가 똑같으면 등록 안한다..
		CheckSameMaxGunNum( newguntotalnum, newguncoordinate );

		//다시 초기화 해주자..
		if( ReadMap() == FALSE )
		{
			return;
		}
	}	

	//총총잡이 구하자..
	View();
}

BOOL CGun::ReadMap()
{
	FILE *pmap = fopen( "map.txt" , "r" );
	if( pmap == NULL )
	{
		return FALSE;
	}
	
	char word;
	for( int findex = 0; findex < TOTAL_LENGTH; findex++ )
	{
		for( int sindex = 0; sindex < TOTAL_LENGTH; sindex++ )
		{
			//마지막 문자가 자동으로 읽어지기 때문에 임의로 이렇게 해준다..
			fscanf( pmap , "%c" , &word );
			if( word == 10 )
			{
				--sindex;
				continue;
			}
			theMap[findex][sindex] = word;
		}
	}
	
	theMap[TOTAL_LENGTH][TOTAL_LENGTH] = NULL;
	fclose( pmap );

	return TRUE;
}

//처음 총잡이 넣을 좌표 구하는 함수
BOOL CGun::GetFirstGunCoordinate()
{
	while(1)
	{
		//최대 가로가 넘어가면 세로 한칸을 올려주자.
		if( theMapSIndex >= TOTAL_LENGTH )
		{
			++theMapFIndex;
			theMapSIndex = 0;
		}

		//세로의 최대index가 넘을 경우 끝.. 
		if( theMapFIndex >= TOTAL_LENGTH )
		{
			return FALSE;
		}

		//처음 넣을 총 공간을 구하자..
		if( theMap[theMapFIndex][theMapSIndex] == '#' )
		{
			theMap[theMapFIndex][theMapSIndex] = '!';
			break;
		}

		++theMapSIndex;
	}

	++theMapSIndex;

	return TRUE;
}

//조건이 맞는게 공간이 있으면 넣고 총잡이로 바꾸어 주는 함수..
void CGun::SetGunPosition()
{
	for( int heghtindex = 0; heghtindex < TOTAL_LENGTH; ++heghtindex )
	{
		for( int acrossindex = 0; acrossindex < TOTAL_LENGTH; ++acrossindex )
		{
			//빈 공간이 있는게 확인 되면..
			if( theMap[heghtindex][acrossindex] == '#' )
			{
				//가로에 다른 총잡이가 있는지 확인..
				if( FALSE == CheckGunAcross( acrossindex, heghtindex ) )
					continue;

				//세로에 다른 총잡이가 있는지 확인.. 
				if( FALSE == CheckGunHeight( acrossindex, heghtindex ) )
					continue;
				
				//모든 조건이 총족되면 넣자..
				theMap[heghtindex][acrossindex] = '!';
			}
		}
	}
}

//가로에 다른 총잡이가 있는지 확인 하는 함수..
BOOL CGun::CheckGunAcross( int aAcrossIndex, int aHeightIndex )
{
	//현재 총잡이를 넣을 좌표를 중심으로 왼쪽 벽 위치를 알아내자..
	int leftblockindex = 0;
	for( int index = 0; index < aAcrossIndex; ++index )
	{
		if( theMap[aHeightIndex][index] == '@' )
			leftblockindex = index;
	}

	//현재 총잡이를 넣을 좌표를 중심으로 오른쪽 벽 위치를 알아내자..
	int rightblockindex = 0;
	for( index = aAcrossIndex; index < TOTAL_LENGTH; ++index )
	{
		if( theMap[aHeightIndex][index] == '@' )
		{
			rightblockindex = index;
			break;
		}
	}

	//벽과 벽을 기준으로 총잡이가 존재하는지 확인..
	for( index = leftblockindex; index < rightblockindex; ++index )
	{
		//같은 라인에 다른 총잡이가 발견되면..
		if( theMap[aHeightIndex][index] == '!' )
		{
			return FALSE;
		}
	}

	return TRUE;
}

//세로에 다른 총잡이가 있는지 확인 하는 함수..
BOOL CGun::CheckGunHeight( int aAcrossIndex, int aHeightIndex )
{
	//현재 총잡이를 넣을 좌표를 중심으로 왼쪽 벽 위치를 알아내자..
	int leftblockindex = 0;
	for( int index = 0; index < aHeightIndex; ++index )
	{
		if( theMap[index][aAcrossIndex] == '@' )
			leftblockindex = index;
	}

	//현재 총잡이를 넣을 좌표를 중심으로 오른쪽 벽 위치를 알아내자..
	int rightblockindex = 0;
	for( index = aHeightIndex; index < TOTAL_LENGTH; ++index )
	{
		if( theMap[index][aAcrossIndex] == '@' )
		{
			rightblockindex = index;
			break;
		}
	}

	//벽과 벽을 기준으로 총잡이가 존재하는지 확인..
	for( index = 0; index < TOTAL_LENGTH; ++index )
	{
		//같은 라인에 다른 총잡이가 발견되면..
		if( theMap[index][aAcrossIndex] == '!' )
		{
			return FALSE;
		}
	}
	
	return TRUE;
}

void CGun::SetTotalGunNum( int& aNewGunTotalNum, vector<int>& aNewGunCoordinate )
{
	//총총잡이 개수를 구하고 그 때의 좌표 저장..
	for( int findex = 0; findex < TOTAL_LENGTH; ++findex )
	{
		for( int sindex = 0; sindex < TOTAL_LENGTH; ++sindex )
		{
			if( theMap[findex][sindex] == '!' )
			{
				//총총잡이의 개수를 구한다..
				aNewGunTotalNum++;

				//총잡이가 위치해있는 좌표도 저장해 둔다..
				int Coordinate = findex * TOTAL_LENGTH + sindex;
				aNewGunCoordinate.push_back(Coordinate);
			}
		}
	}
}

void CGun::CheckSameMaxGunNum(  int aNewGunTotalNum, vector<int> aNewGunCoordinate )
{
	//같은면 등록할 때 기존의 총잡이 위치가 똑같으면 등록 안한다..
	BOOL sameposition = FALSE;
	vector<vec_GunCoordinate>::iterator iter = theGunCoordinate_list.begin();
	while( iter != theGunCoordinate_list.end() )
	{
		vec_GunCoordinate GunSaveListiter = (*iter);
		vec_GunCoordinate::iterator NewGuniter = aNewGunCoordinate.begin();
		while( NewGuniter != aNewGunCoordinate.end() )
		{
			vec_GunCoordinate::iterator GunSaveiter = GunSaveListiter.begin();
			while( GunSaveiter != GunSaveListiter.end() )
			{
				//같은 위치가 있으면 그다음 가르킴..
				if( (*GunSaveiter) == (*NewGuniter) )
				{
					sameposition = TRUE;
					break;
				}

				//같은 위치가 하나라도 없다면 false로 되겠지..
				sameposition = FALSE;
				++GunSaveiter;
			}
			
			++NewGuniter;
		}
	
		//같은 위치가 존재하면 리턴..
		if( sameposition == TRUE )
		{
			return;
		}

		//같은 위치가 없으면 다음리스트에서 알아보자..
		++iter;
	}

	//같은 위치가 존재안하면 넣자..
	if( sameposition == FALSE )
	{
		theGunCoordinate_list.push_back(aNewGunCoordinate);
	}
}

void CGun::View()
{
	cout << theMaxGunNum << endl;

	//그 때의 위치를 확인하자..
	vector<vec_GunCoordinate>::iterator iter = theGunCoordinate_list.begin();
	while( iter != theGunCoordinate_list.end() )
	{
		vec_GunCoordinate GunSaveListiter = (*iter);
		vec_GunCoordinate::iterator iiter  = GunSaveListiter.begin();
		while( iiter != GunSaveListiter.end() )
		{
			cout << *iiter << " ";
			++iiter;
		}

		cout << endl;

		++iter;
	}
}