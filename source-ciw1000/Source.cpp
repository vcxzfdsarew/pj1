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
	//���Ͽ��� ���� ����..
	if( ReadMap() == FALSE )
	{
		return;
	}

	//ó�� ������ ���� ��ǥ ���Ѵ�..
	while( GetFirstGunCoordinate() != FALSE ) 
	{
		//������ �´� ������ ������ �����̷� �ٲپ� ����..
		SetGunPosition();

		//�������� ������ ���ϰ� �� ���� ��ǥ ����..
		int newguntotalnum = 0;
		vec_GunCoordinate newguncoordinate;
		newguncoordinate.reserve(20);
		SetTotalGunNum( newguntotalnum, newguncoordinate );

		//���� ���� �������� ������ �񱳸� �ؼ� ������ ����
		if( theMaxGunNum > newguntotalnum )
			continue;

		//ũ�� ������ ���� ����.. ���� ��� 
		if( theMaxGunNum < newguntotalnum )
		{
			theGunCoordinate_list.clear();
			theMaxGunNum = newguntotalnum;
			theGunCoordinate_list.push_back(newguncoordinate);
		}

		//������ ����� �� ������ ������ ��ġ�� �Ȱ����� ��� ���Ѵ�..
		CheckSameMaxGunNum( newguntotalnum, newguncoordinate );

		//�ٽ� �ʱ�ȭ ������..
		if( ReadMap() == FALSE )
		{
			return;
		}
	}	

	//�������� ������..
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
			//������ ���ڰ� �ڵ����� �о����� ������ ���Ƿ� �̷��� ���ش�..
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

//ó�� ������ ���� ��ǥ ���ϴ� �Լ�
BOOL CGun::GetFirstGunCoordinate()
{
	while(1)
	{
		//�ִ� ���ΰ� �Ѿ�� ���� ��ĭ�� �÷�����.
		if( theMapSIndex >= TOTAL_LENGTH )
		{
			++theMapFIndex;
			theMapSIndex = 0;
		}

		//������ �ִ�index�� ���� ��� ��.. 
		if( theMapFIndex >= TOTAL_LENGTH )
		{
			return FALSE;
		}

		//ó�� ���� �� ������ ������..
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

//������ �´°� ������ ������ �ְ� �����̷� �ٲپ� �ִ� �Լ�..
void CGun::SetGunPosition()
{
	for( int heghtindex = 0; heghtindex < TOTAL_LENGTH; ++heghtindex )
	{
		for( int acrossindex = 0; acrossindex < TOTAL_LENGTH; ++acrossindex )
		{
			//�� ������ �ִ°� Ȯ�� �Ǹ�..
			if( theMap[heghtindex][acrossindex] == '#' )
			{
				//���ο� �ٸ� �����̰� �ִ��� Ȯ��..
				if( FALSE == CheckGunAcross( acrossindex, heghtindex ) )
					continue;

				//���ο� �ٸ� �����̰� �ִ��� Ȯ��.. 
				if( FALSE == CheckGunHeight( acrossindex, heghtindex ) )
					continue;
				
				//��� ������ �����Ǹ� ����..
				theMap[heghtindex][acrossindex] = '!';
			}
		}
	}
}

//���ο� �ٸ� �����̰� �ִ��� Ȯ�� �ϴ� �Լ�..
BOOL CGun::CheckGunAcross( int aAcrossIndex, int aHeightIndex )
{
	//���� �����̸� ���� ��ǥ�� �߽����� ���� �� ��ġ�� �˾Ƴ���..
	int leftblockindex = 0;
	for( int index = 0; index < aAcrossIndex; ++index )
	{
		if( theMap[aHeightIndex][index] == '@' )
			leftblockindex = index;
	}

	//���� �����̸� ���� ��ǥ�� �߽����� ������ �� ��ġ�� �˾Ƴ���..
	int rightblockindex = 0;
	for( index = aAcrossIndex; index < TOTAL_LENGTH; ++index )
	{
		if( theMap[aHeightIndex][index] == '@' )
		{
			rightblockindex = index;
			break;
		}
	}

	//���� ���� �������� �����̰� �����ϴ��� Ȯ��..
	for( index = leftblockindex; index < rightblockindex; ++index )
	{
		//���� ���ο� �ٸ� �����̰� �߰ߵǸ�..
		if( theMap[aHeightIndex][index] == '!' )
		{
			return FALSE;
		}
	}

	return TRUE;
}

//���ο� �ٸ� �����̰� �ִ��� Ȯ�� �ϴ� �Լ�..
BOOL CGun::CheckGunHeight( int aAcrossIndex, int aHeightIndex )
{
	//���� �����̸� ���� ��ǥ�� �߽����� ���� �� ��ġ�� �˾Ƴ���..
	int leftblockindex = 0;
	for( int index = 0; index < aHeightIndex; ++index )
	{
		if( theMap[index][aAcrossIndex] == '@' )
			leftblockindex = index;
	}

	//���� �����̸� ���� ��ǥ�� �߽����� ������ �� ��ġ�� �˾Ƴ���..
	int rightblockindex = 0;
	for( index = aHeightIndex; index < TOTAL_LENGTH; ++index )
	{
		if( theMap[index][aAcrossIndex] == '@' )
		{
			rightblockindex = index;
			break;
		}
	}

	//���� ���� �������� �����̰� �����ϴ��� Ȯ��..
	for( index = 0; index < TOTAL_LENGTH; ++index )
	{
		//���� ���ο� �ٸ� �����̰� �߰ߵǸ�..
		if( theMap[index][aAcrossIndex] == '!' )
		{
			return FALSE;
		}
	}
	
	return TRUE;
}

void CGun::SetTotalGunNum( int& aNewGunTotalNum, vector<int>& aNewGunCoordinate )
{
	//�������� ������ ���ϰ� �� ���� ��ǥ ����..
	for( int findex = 0; findex < TOTAL_LENGTH; ++findex )
	{
		for( int sindex = 0; sindex < TOTAL_LENGTH; ++sindex )
		{
			if( theMap[findex][sindex] == '!' )
			{
				//���������� ������ ���Ѵ�..
				aNewGunTotalNum++;

				//�����̰� ��ġ���ִ� ��ǥ�� ������ �д�..
				int Coordinate = findex * TOTAL_LENGTH + sindex;
				aNewGunCoordinate.push_back(Coordinate);
			}
		}
	}
}

void CGun::CheckSameMaxGunNum(  int aNewGunTotalNum, vector<int> aNewGunCoordinate )
{
	//������ ����� �� ������ ������ ��ġ�� �Ȱ����� ��� ���Ѵ�..
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
				//���� ��ġ�� ������ �״��� ����Ŵ..
				if( (*GunSaveiter) == (*NewGuniter) )
				{
					sameposition = TRUE;
					break;
				}

				//���� ��ġ�� �ϳ��� ���ٸ� false�� �ǰ���..
				sameposition = FALSE;
				++GunSaveiter;
			}
			
			++NewGuniter;
		}
	
		//���� ��ġ�� �����ϸ� ����..
		if( sameposition == TRUE )
		{
			return;
		}

		//���� ��ġ�� ������ ��������Ʈ���� �˾ƺ���..
		++iter;
	}

	//���� ��ġ�� ������ϸ� ����..
	if( sameposition == FALSE )
	{
		theGunCoordinate_list.push_back(aNewGunCoordinate);
	}
}

void CGun::View()
{
	cout << theMaxGunNum << endl;

	//�� ���� ��ġ�� Ȯ������..
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