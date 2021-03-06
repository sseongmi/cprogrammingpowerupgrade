#include "blockInfo.h"
#include<time.h>
#include "point.h"
#include "keyCurControl.h"
#include "common.h"
//#include "boardInfo.h"
extern int boardInfo[11][12];
extern void MarkToMap();
extern void BoardUpdate();
static int currentBlockModel;
static int rotateSte;
static int curPosX, curPosY;

enum{LEFT,RIGHT,UP,DOWN};

void InitNewBlockPos(int x, int y)
{
	if (x < 0 || y < 0)
		return 0;

	curPosX = x;
	curPosY = y;

	SetCurrentCursorPos(x, y);
}

void ChooseBlock(void)
{
	srand((unsigned int)time(NULL));
	currentBlockModel = (rand() % NUM_OF_BLOCK_MODEL) * 4;
}

int GetCurrentBlockIdx(void)
{
	return currentBlockModel+rotateSte;
}

void ShowBlock(char blockInfo[][4])
{
	int y, x;
	point curPos = GetCurrentCursorPos();

	for (y = 0; y < 4; y++)
	{
		for (x = 0; x < 4; x++)
		{
			SetCurrentCursorPos(curPos.x + (x*2), curPos.y+y);

			if (blockInfo[y][x] == 1)
				printf("■");
		}
	}
	SetCurrentCursorPos(curPos.x, curPos.y);
}

void DeleteBlock(char blockInfo[][4])
{
	int y, x;
	point curPos = GetCurrentCursorPos();

	for (y = 0; y < 4; y++)
	{
		for (x = 0; x < 4; x++)
		{
			SetCurrentCursorPos(curPos.x + x * 2, curPos.y + y);
			if (blockInfo[y][x] == 1)
				printf("  "); //공백두칸
		}
	}
	SetCurrentCursorPos(curPos.x, curPos.y);
}
int BlockDown(void)
{
	curPosY += 1;
	if(CheckCollision())
	{
		curPosY -= 1;
		MarkToMap();
		BoardUpdate();
		return 1;
	}
	DeleteBlock(blockModel[GetCurrentBlockIdx()]);
	SetCurrentCursorPos(curPosX, curPosY);
	ShowBlock(blockModel[GetCurrentBlockIdx()]);
	return 0;
}

void ShiftLeft(void)
{
	curPosX -= 2;
	if (CheckCollision())
	{
			curPosX += 2;
			return;
	}
	DeleteBlock(blockModel[GetCurrentBlockIdx()]);
	SetCurrentCursorPos(curPosX, curPosY);
	ShowBlock(blockModel[GetCurrentBlockIdx()]);
}
void ShiftRight(void)
{
	curPosX += 2;
	if(CheckCollision())
	{
			curPosX -= 2;
			return;
	}
	DeleteBlock(blockModel[GetCurrentBlockIdx()]);
	SetCurrentCursorPos(curPosX, curPosY);
	ShowBlock(blockModel[GetCurrentBlockIdx()]);
}
void RotateBlock(void)
{
	int afterRotSte,deficiency = 0;
	int beforeRotSte = rotateSte;
	point beforePos = { curPosX,curPosY };

	DeleteBlock(blockModel[GetCurrentBlockIdx()]);

	afterRotSte = rotateSte + 1;
	afterRotSte %= 4;
	rotateSte = afterRotSte;
	deficiency = CheckCollision();
	curPosX -= 2 * deficiency;

	if (CheckCollision())
	{	
		rotateSte = beforeRotSte;
		curPosX = beforePos.x;
		ShowBlock(blockModel[GetCurrentBlockIdx()]);
		return;
	}
	SetCurrentCursorPos(curPosX, curPosY);
	ShowBlock(blockModel[GetCurrentBlockIdx()]);
}
void MarkToMap(void)
{
	int cur_model = GetCurrentBlockIdx();
	point curPos = GetCurrentCursorPos();
	int i, j;
	for (i = curPos.y; i < curPos.y + blockDetails[cur_model].row; i++)
	{
		for (j = (curPos.x - 10) / 2; j < (curPos.x - 10) / 2 + blockDetails[cur_model].col; j++)
		{
			if (blockModel[cur_model][i - curPos.y][j - (curPos.x - 10) / 2] == 1 && boardInfo[i][j] == 0)
			{
				boardInfo[i][j] = 1;
			}
		}
	}
}
int CheckCollision(void)
{
	int num=GetCurrentBlockIdx();
	point boardPos = { (curPosX - 10) / 2, curPosY };

	for (int i = 0; i < blockDetails[num].row; i++)
	{
		for (int j = 0; j < blockDetails[num].col; j++)
		{
			if (blockModel[num][i][j ] == 1 && boardInfo[boardPos.y+i][boardPos.x+j] == 1)
				return blockDetails[num].col-j;
		}
	}
	return 0;
}
void BoardUpdate(void)
{
	int temp[30][12] = { 0 };
	int blockNum = GetCurrentBlockIdx();
	int deliteLineSum=blockDetails[blockNum].row;
	int delete_pos=-1, delete_num=0;
	for (int y = 0; y < 30; y++)
	{
		for (int x = 1; x < 11; x++)
		{
			if (boardInfo[y][x] == 1)
			{
				if (x == 10)
				{
					delete_num++;
					if (delete_pos == -1)
					{
						delete_pos = y;
					}
				}
				continue;
			}else{
				break;
			}
		}
	}
	//복사
	for (int y = 0; y < delete_pos; y++)
	{
		for (int x = 1; x < 11; x++)
		{
			temp[y][x] = boardInfo[y][x];
		}
	}
	//붙여넣기
	for (int y = 0; y < delete_num; y++)
	{
		for (int x = 1; x < 11; x++)
		{
			boardInfo[y][x]=0;
		}
	}
	for (int y = delete_num; y < delete_num+delete_pos; y++)
	{
		for (int x = 1; x < 11; x++)
		{
			boardInfo[y][x] = temp[y-delete_num][x];
		}
	}
	for (int y = 0 ; y < 30;  y++)
	{
		for (int x = 1; x < 11; x++)
		{
			SetCurrentCursorPos(10+ x * 2,  y);
			if (boardInfo[y][x] == 1)
				printf("■"); 
			else
				printf("  ");
		}
	}
	SetCurrentCursorPos(curPosX, curPosY);
}
