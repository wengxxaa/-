#ifndef CONFIGSTATE_H_
#define CONFIGSTATE_H_

#include<iostream>
#include "cocos2d.h"

USING_NS_CC;

using namespace std;

//IOS模式
#define IOSMODE 0

#define TTF_PATH  "fonts/arial.ttf"
#define FNT_PATH "animation/fonts/shuzi_11.fnt"
#define HANZI_XML "config/hanzi_config.xml"
#define SAVE_FILE "savadata.dat"

//#define  AUTO_MODEL

static const float DROP_UPDATE_TIME=0.1f;	//方块每行下落需要的时间
static const float DROP_STOP_TIME=1.0f;	//方块停顿的时间
static const float DROP_MOVE_TIME=0.1f;	//棋子滑动需要的时间
static const float CHESS_REMOVE_TIME=0.1f;	//棋子消除的时间
static const float CHESS_CHECK_TIME=0.2f;	//棋盘cheak的时间

static const int  AtlasYOff=-8;
static const int LEVELREWARD=5;	//奖励最高次数
static const float OPA_BEIJING=0.85f;	//背景的透明度
static const float MENU_SCALE=1.15f;	//按钮放大

//pay
#if(CC_TARGET_PLATFORM==CC_PLATFORM_IOS|| IOSMODE)
static const int PAY_2_DIAMOND = 10;	//
static const int PAY_DIAMOND[9]={20,400,55,120,220,400,400,10,400};
#else
static const int PAY_2_DIAMOND = 5;	//
static const int PAY_DIAMOND[10]={20,60,120,220,400,400,400,10,400,400};
#endif
//

//the time of hongbao
static const int TIME_HONGBAO[7]={180,300,480,900,1200,1800,1800};	
//static const int TIME_HONGBAO[7]={10,10,10,10,10,10,10};	
	
//the Daily Reward
static const int DAILY_REWARD[7]={10,53,52,51,20,54,30};	

//box model
static const int CHESS_SIZE_W=11;	//方块棋盘的宽
static const int CHESS_SIZE_H=14;	//方块棋盘的高
static const float CHESSMAN_SCALE=1.0f;	//方块的缩放
static const int CHESS_SIZE_SIZE=42;	//棋子的高
static const int CHESSMAN_LEVEL=2;	//棋子的层
static const int EASE_STRENGTH=5;	//下落强度

//1010 model
static const int CHESS_SIZE_W_1010=9;	
static const int CHESS_SIZE_H_1010=10;	
static const int CHESSMAN_SIZE_1010=52;	
static const float CHESSMAN_SCALE_1010=1.25f;	
static const int CHESS_SCORE=10;	

//start model
static const int CHESS_SIZE_W_START=10;	
static const int CHESS_SIZE_H_START=10;	
static const int CHESSMAN_SIZE_START=48;
static const float CHESSMAN_SCALE_START=1.25f;
static const int START_INDEX_NUMBER=5;	
static const Vec2 START_SCORE_PT=Vec2(240,711);	

//liujiao model
static const int CHESS_SIZE_W_ANGLE=9;	
static const int CHESS_SIZE_H_ANGLE=9;	
static const int CHESSMAN_SIZE_W_ANGLE=53;	
static const int CHESSMAN_SIZE_H_ANGLE=47;	
static const float CHESSMAN_SCALE_ANGLE=1.0f;	
static const Vec2 ANGLE_SCORE_PT=Vec2(240,711);	

//line model
static const int CHESS_SIZE_W_LINE=9;
static const int CHESS_SIZE_H_LINE=10;
static const int CHESSMAN_SIZE_LINE=52;	
static const float CHESSMAN_SCALE_LINE=1.25f;	

//fillup model
static const int CHESS_SIZE_W_FILLUP = 7;
static const int CHESS_SIZE_H_FILLUP = 7;
static const int CHESSMAN_SIZE_FILLUP = 98;
static const float CHESSMAN_SCALE_FILLUP = 1.5f;
static const int FILLUP_INDEX_NUMBER = 5;
static const Vec2 FILLUP_SCORE_PT = Vec2(575, 1170);

//the shape of box model
typedef enum _SHAPE_INDEX
{
	L_SHAPE,
	L_RESHAPE,
	SQUARE_SHAPE,
	ANGLE_SHAPE,
	T_SHAPE,
	LINE_SHAPE,
	SWAGER_SHAPE,
	SWAGER_RESHAPE,
	POINT_SHAPE,
	LINESMALL_SHAPE,
	U_SHAPE,
	SQUAREBIG_SHAPE,
	LBIG_SHAPE,
}ShapeIndex;

//the angle of box
typedef enum _ANGLE_INDEX
{
	ANGLE_0,
	ANGLE_90,
	ANGLE_180,
	ANGLE_270,
}AngleIndex;

//
typedef enum _DIRECTION_INDEX
{
	DIRECTION_D,
	DIRECTION_R,
	DIRECTION_L,
}DirectionIndex;

//the state of game
typedef enum _GAME_STATE
{
	STATE_RUN,
	STATE_STOP,
	STATE_PAUSE,
	STATE_FAILD,
}GameState;

//the state of touch
typedef enum _TOUCH_STATE
{
	TOUCH_TRUE,	
	TOUCH_FALSE,	
}TouchState;

//
typedef struct _POINT_TIP
{
	ShapeIndex shape;
	AngleIndex angle;
	pair<int,int>  position;

	_POINT_TIP(){};

	_POINT_TIP(ShapeIndex s,AngleIndex a,pair<int,int> p):shape(s),angle(a),position(p){};

	bool operator==(const _POINT_TIP &t)const
	{
		return (shape==t.shape&&angle==t.angle&&position==t.position);
	}

	_POINT_TIP& operator=(const _POINT_TIP& _Right)
	{	
		shape = _Right.shape;
		angle = _Right.angle;
		position=_Right.position;
		return (*this);
	}

}PointTip;

//the two model of box model
typedef enum _GAME_MODEL
{
	MODEL_SLIP=1,	
	MODEL_POINT,	
}GameModel;

//the color of box
typedef enum _BOX_COLOR_
{
	BOX_NORMAL=1,	
	BOX_GRAY,
}BoxColor;

//the model of game
typedef enum _GAME_PROJECT
{
	GAME_TETRIS,
	GAME_1010,
	GAME_START,
	GAME_ANGLE,
	GAME_FILLUP,
}GameProject;

#endif
