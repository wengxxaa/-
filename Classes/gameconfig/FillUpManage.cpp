#include"FillUpManage.h"
#include"BaseConfig.h"
#include"cocostudio/CocoStudio.h"
#include "cocos-ext.h"
#include <spine/spine-cocos2dx.h>
#include "spine/spine.h"
#include "BaseConfig.h"
#include "../gamescene/GameScene.h"
using namespace spine;
USING_NS_CC_EXT;
using namespace cocostudio;

static const int ENUM = 2;
static const int CHANGEENUM = 3;
static const int HEIGHTOFFEST = 132;
static const int TIME = 60;
static const Vec2 PROPBARPOS = Vec2(220,704);
static const Vec2 TIMEBARPOS = Vec2(263, 101);
static const float REDUCESUDE = 0.2f;
static const float PROPDROPTIME = 0.5f;
static const float BINGDONG = 8.0f;
static const float FENGKUANG = 8.0f;

static const Vec2 PROPPOS = Vec2(394,704);

static const char* BOXCOLOR[5] = {
	"blue","green","purple","pink","yellow"
};
static const char* XIAOCHUCOLOR[5] = {
	"blue","green","purple","red","yellow"
};

static const char* FENGKUANGCOLOR[5] = {
	"fang","hua","san","xing","yueya"
};

static const Vec2 PARSTART = Vec2(-167,0);
static const int PARLENGTH = 330;

static const string SPINEPATH = "FillUpMotion/spine/";

static const string TTFPATH = "animation/fonts/hanzi2.TTF";

bool FillUpManage::init()
{
	if (!Layer::init())
		return false;

	_jiashi = false;

	initData();
	initChess();

	beganGame();

	//test();

	cj::LogShow("--游戏开始--");

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(FillUpManage::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(FillUpManage::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(FillUpManage::onTouchEnded, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	listener->setSwallowTouches(true);

	return true;
}

//初始化棋盘
void FillUpManage::initChess()
{
	Vec2 vec[4];
	vec[0] = WINORIGIN + Vec2(WINSIZE.width / 2.0f - _ChessSize*_Width / 2, 0 + HEIGHTOFFEST);
	vec[1] = WINORIGIN + Vec2(WINSIZE.width / 2.0f + _ChessSize*_Width / 2, 0 + HEIGHTOFFEST);
	vec[2] = WINORIGIN + Vec2(WINSIZE.width / 2.0f + _ChessSize*_Width / 2, _Height*_ChessSize + HEIGHTOFFEST);
	vec[3] = WINORIGIN + Vec2(WINSIZE.width / 2.0f - _ChessSize*_Width / 2, _Height*_ChessSize + HEIGHTOFFEST);

	auto node = DrawNode::create();
	this->addChild(node, 10);
	node->drawSolidPoly(vec, 4, Color4F(1.0f, 1.0f, 1.0f, 0.0f));

	_clip = ClippingNode::create(node);
	this->addChild(_clip, 10);
	_clip->setInverted(false);

	_waveNode = NodeGrid::create();
	_clip->addChild(_waveNode,10);
}

//初始化数据
void FillUpManage::initData()
{
	//video
	cjMusic::playBackgroundMusic("video/FillUp/bgm_fillup.mp3",true);

	_Width =atoi(GameDataInstance()->d_fillup2.at(make_pair(3,'A')).c_str());
	_Height=atoi(GameDataInstance()->d_fillup2.at(make_pair(3, 'B')).c_str());
	_Time= atoi(GameDataInstance()->d_fillup2.at(make_pair(3, 'C')).c_str());
	//_DropTime= atof(GameDataInstance()->d_fillup2.at(make_pair(3, 'D')).c_str());
	_DropTime = 0.15f;
	_BaseScore= atoi(GameDataInstance()->d_fillup2.at(make_pair(3, 'E')).c_str());
	_BoxIndex= atoi(GameDataInstance()->d_fillup2.at(make_pair(3, 'F')).c_str());
	_BoxChangeNum = atoi(GameDataInstance()->d_fillup2.at(make_pair(3, 'G')).c_str());
	_ChessSize = 67;
	_ChessScale = 1.0f;
	_lianji = 0;
	_daduanshow = true;
	_gameFaild = false;
	_timeDecrease = false;
	_propbool = false;
	_changeColor = false;
	_stoptime = 0;
	_guide = false;
	_guidenumber = -1;
	_lizivec.clear();

	if (!GameData::getSaveData()->_fillup._guider)
	{
		GameDataInstance()->_guide = true;
		_guide = true;
	}

	for (int i = 0; i < 3; i++)
	{
		_propdata[i].score = atoi(GameDataInstance()->d_fillup1.at(make_pair(3 + i, 'B')).c_str());
		_propdata[i].multiple= atoi(GameDataInstance()->d_fillup1.at(make_pair(3 + i, 'C')).c_str());
		_propdata[i].eliminateTime= atof(GameDataInstance()->d_fillup1.at(make_pair(3 + i, 'D')).c_str());
		_propdata[i].multipletime = atoi(GameDataInstance()->d_fillup1.at(make_pair(3 + i, 'E')).c_str());
		_propdata[i].prop = cj::getIntByString(GameDataInstance()->d_fillup1.at(make_pair(3 + i, 'F')));
		_propdata[i].eliminateScore=atof(GameDataInstance()->d_fillup1.at(make_pair(3 + i, 'H')).c_str());
		_propdata[i].interrupt = atoi(GameDataInstance()->d_fillup1.at(make_pair(3 + i, 'I')).c_str());
		_propdata[i].scoreReduce = atof(GameDataInstance()->d_fillup1.at(make_pair(3 + i, 'J')).c_str());
	}

	for (int i = 3; i < 100; i++)
	{
		_ScoreConfig[atoi(GameDataInstance()->d_fillup3.at(make_pair(i, 'A')).c_str())] = atoi(GameDataInstance()->d_fillup3.at(make_pair(i, 'B')).c_str());
	}

	_chesscolor.clear();
	for(int i=0;i<_Width;i++)
		for (int j = 0; j < _Height; j++)
		{
			_chesscolor.push_back(0);
		}

	if (GameData::getSaveData()->_fillup._istip)
	{
		_timenumber = GameData::getSaveData()->_fillup._time;
		_level = GameData::getSaveData()->_fillup._level;
		_score = GameData::getSaveData()->_fillup._score;
		_propid = GameData::getSaveData()->_fillup._propid;
		_propscore = GameData::getSaveData()->_fillup._propscore;
		_multiple = GameData::getSaveData()->_fillup._multiple;
		_multipletime = GameData::getSaveData()->_fillup._multipletime;
		_eliminatetime = GameData::getSaveData()->_fillup._eliminatetime;
		_hightestLevel= GameData::getSaveData()->_fillup._hightestlevel;
		for (int i = 0; i < 3; i++)
		{
			_specialProp[i] = GameData::getSaveData()->_fillup._specialProp[i];
		}
	}
	else
	{
		_timenumber = _Time;
		_level = 0;
		_score = 0;
		_propscore = 0;
		if (!_guide&&GameData::getSaveData()->_fillup._firstEnter)
		{
			_propscore = _propdata[_level].score-1;
			GameData::getSaveData()->_fillup._firstEnter = false;
			GameData::getInstance()->dataSave();
		}
		_propid= _propdata[0].prop[rand() % (_propdata[0].prop.size())];
		_multiple = 1;
		_multipletime = 0;
		_eliminatetime= _propdata[0].eliminateTime;
		_hightestLevel = false;

		for (int i = 0; i < 3; i++)
		{
			_specialProp[i] = GameData::getSaveData()->_fillupprop[i];
			if (GameData::getSaveData()->_fillupprop[i])
			{
				cj::clickUMCount(25 + i);
				GameData::getSaveData()->_fillupprop[i] = false;
			}
		}
		GameData::getInstance()->dataSave();

		specialPropPage();

	}

	_csbNode = CSLoader::createNode("animation/background_fillup_1.csb");
	this->addChild(_csbNode,2);
	_csbNode->setPosition(WINORIGIN);

	auto tool =_csbNode->getChildByName("tool");

	char* barcolor[3] = { "blue","purple","purple" };
	//道具进度条
	for (int i = 0; i < 3; i++)
	{
		_propbar[i] = ProgressTimer::create(Sprite::create(cjTTFLabel::getNameByInt("FillUp/bar/bar_%d.png",i+1)));
		tool->addChild(_propbar[i]);
		_propbar[i]->setType(ProgressTimer::Type::BAR);
		_propbar[i]->setBarChangeRate(Vec2(1,0));
		_propbar[i]->setMidpoint(Vec2(0, 1));
		_propbar[i]->setPercentage(0);
		
		char barname[100];
		sprintf(barname, "FillUpMotion/spine/gundongtiao/%s/guangyun_%s.plist", barcolor[i], barcolor[i]);
		_proppar[i] = ParticleSystemQuad::create(barname);
		tool->addChild(_proppar[i]);
		_proppar[i]->setAutoRemoveOnFinish(true);
		_proppar[i]->setPosition(PARSTART);
		_proppar[i]->setPositionType(ParticleSystem::PositionType::GROUPED);
		_proppar[i]->setTotalParticles(15);
		
		sprintf(barname, "FillUpMotion/spine/gundongtiao/%s/guangdian_%s.plist", barcolor[i], barcolor[i]);
		auto dianpar = ParticleSystemQuad::create(barname);
		_proppar[i]->addChild(dianpar);
		dianpar->setAutoRemoveOnFinish(true);
		//->setPositionType(ParticleSystem::PositionType::RELATIVE);

		_proppar[i]->setVisible(false);
		if (i < _level)
		{
			_propbar[i]->setPercentage(100);
			_proppar[i]->setPosition(PARSTART + Vec2(PARLENGTH, 0));
		}
		else if (i > _level)
		{
			_propbar[i]->setPercentage(0);
		}
		else
		{
			schedule([this](float) {
				barHandle();
			}, 0.0f, 0, 0.1f, "barHandle");
		}
	}

	//奖励道具
	if (!_hightestLevel)
	{
		auto prop = Sprite::create(cjTTFLabel::getNameByInt("FillUp/prop/prop%02d.png", _propid));
		this->addChild(prop, 8, "prop");
		prop->setPosition(PROPPOS);

		toolSliderMotion(false);
	}
	else
	{
		auto prop = Sprite::create(cjTTFLabel::getNameByInt("FillUp/prop/prop%02d.png", 6));
		this->addChild(prop, 8, "prop");
		prop->setPosition(PROPPOS);
		toolSliderMotion(true);
	}

	//时间条
	auto time=_csbNode->getChildByName("time");
	_timebar = ProgressTimer::create(Sprite::create("FillUp/bar/bar_time.png"));
	time->addChild(_timebar);
	_timebar->setType(ProgressTimer::Type::BAR);
	_timebar->setBarChangeRate(Vec2(1,0));
	_timebar->setMidpoint(Vec2(0, 1));
	_timebar->setPercentage(_timenumber*1.0f * 100 / _Time);

	_timelabel = cjAtlasLabel::createWithInt(_timenumber, ATLAS_PATH11);
	this->addChild(_timelabel,2);
	_timelabel->setPosition(_csbNode->getChildByName("time_frame")->getPosition());
	_timelabel2 = cjAtlasLabel::createWithInt(_timenumber, ATLAS_PATH12);
	this->addChild(_timelabel2, 2);
	_timelabel2->setPosition(_timelabel->getPosition());

	if (_timenumber <= 5)
		_timelabel->setVisible(false);
	else
		_timelabel2->setVisible(false);

	timeUpdate(0.0f);
	schedule(schedule_selector(FillUpManage::timeUpdate), 1.0f);

	//	
	auto tishiyu = SkeletonAnimation::createWithFile(SPINEPATH+"tishiyu_export/tishiyu.json", SPINEPATH + "tishiyu_export/tishiyu.atlas");
	this->addChild(tishiyu, 12,"tishiyu");
	tishiyu->setPosition(WINSIZE / 2.0f);
}

//开始游戏
void FillUpManage::beganGame()
{

	startDrop();

	if (!_guide)
	{
		showText(TEXT::yubei);
		setGameState(STATE_STOP);
		GameDataInstance()->_guide = true;
		runAction(Sequence::create(DelayTime::create(1.1f), CallFunc::create([this]() {
			showText(TEXT::kaishi);
			_timeDecrease = true;
			GameDataInstance()->_guide = false;
			if (getGameState() == STATE_STOP)
				setGameState(STATE_RUN);
		}), nullptr));
	}
	else
	{
		setGameState(STATE_STOP);
		guideTitle();
		runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create([this]() {
			startGuide();
		}), DelayTime::create(0.5f), CallFunc::create([=]() {
			if (getGameState() == STATE_STOP)
				setGameState(STATE_RUN);
		}), nullptr));
	}

	//this->schedule(schedule_selector(FillUpManage::eliminateLizi));
}

//开始游戏前的道具
void FillUpManage::specialPropPage()
{
	//特殊道具2
	if (_specialProp[1])
	{
		_specialProp[1] = false;
		_score = 50000;

	}

}

//test
void FillUpManage::test()
{


}

void FillUpManage::testupdate(float ft)
{
	auto test1 = (Label*)getChildByName("test1");
	auto test2 = (Label*)getChildByName("test2");
	auto test3 = (Label*)getChildByName("test3");
	auto test4 = (Label*)getChildByName("test4");
	test1->setString(cjTTFLabel::getNameByInt("multiple: %d", _multiple));
	test2->setString(cjTTFLabel::getNameByInt("multiple time: %d", _multipletime));
	test3->setString(cjTTFLabel::getNameByInt("eliminate time: %d", _eliminatetime));
	test4->setString(cjTTFLabel::getNameByInt("%d", _propbar[_level]->getPercentage()));
}

//复活
void FillUpManage::Resurrection()
{
	//video
	cjMusic::playBackgroundMusic("video/FillUp/bgm_fillup.mp3", true);

	_jiashi = true;

	setGameState(STATE_RUN);
	_timenumber = 10;
	_gameFaild = false;
	_timelabel->setVisible(true);
	_timelabel->setString(cjTTFLabel::getNameByInt("%d", _timenumber));
	_timelabel2->setVisible(false);
	_timebar->runAction(ProgressTo::create(0.1f, 100 * _timenumber*1.0f / _Time));
	this->schedule(schedule_selector(FillUpManage::timeUpdate),1.0f);
}

//开始下落
void FillUpManage::startDrop(bool key)
{
	for(int i=0;i<_Width;i++)
		for (int j = 0; j < _Height; j++)
		{
			int tempcolor;
			int level = 1;
			if (_guide)
			{
				if (i == 1 && (j == 5 || j == 6))
				{
					tempcolor = 1;
					level = 10;
				}
				else
					tempcolor = rand() % (_BoxIndex - 1) + 2;
			}
			else if (key)
			{
				tempcolor = _chesscolor[i*_Height+j];
			}
			else
			{
				if (GameData::getSaveData()->_fillup._istip)
					tempcolor = GameData::getSaveData()->_fillup._chessInfo[i*_Height + j];
				else
					tempcolor = rand() % _BoxIndex + 1;
			}
			auto box = Sprite::create(cjTTFLabel::getNameByInt("FillUp/box/box%d-01.png",tempcolor));
			box->setScale(_ChessScale);
			box->setAnchorPoint(Vec2(0.5f, 0.0f));
			_waveNode->addChild(box, level, i*_Height + j);
			box->setPosition(posToposition2(make_pair(i,j)));
			_chesscolor[i*_Height+j] = tempcolor;

			
		}
}

//位置转换
Vec2 FillUpManage::posToposition(pair<int, int> pos)
{
	return Vec2(WINSIZE.width/2.0f+(pos.first-(_Width-1)/2.0f)*_ChessSize, (HEIGHTOFFEST) +(pos.second+0.5f)*_ChessSize);
}
Vec2 FillUpManage::posToposition2(pair<int, int> pos)
{
	return Vec2(WINSIZE.width / 2.0f + (pos.first - (_Width - 1) / 2.0f)*_ChessSize, (HEIGHTOFFEST) + (pos.second)*_ChessSize);
}

pair<int, int> FillUpManage::positionTopos(Vec2 vec)
{
	int x = (vec.x - WINSIZE.width / 2.0f) / _ChessSize + _Width / 2.0f;
	int y = (vec.y - (HEIGHTOFFEST)) / _ChessSize;
	return make_pair(x, y);
}

//得到颜色相同的方块
vector<pair<int, int>> FillUpManage::boxGet(pair<int, int> pos)
{
	vector<pair<int, int>> vec;
	boxAdd(pos, vec);

	return vec;
}

//疯狂模式
void FillUpManage::crazyAdd(vector<pair<int, int>> &vec)
{
	//疯狂模式

	auto vectt = vec;
	for (auto &pos : vectt)
	{
		//x-1
		if ((pos.first - 1) >= 0 && (std::find(vec.begin(), vec.end(), make_pair(pos.first - 1, pos.second)) == vec.end()))
		{
			vec.push_back(make_pair(pos.first - 1, pos.second));
		}

		//x+1
		if ((pos.first + 1)<_Width && (std::find(vec.begin(), vec.end(), make_pair(pos.first + 1, pos.second)) == vec.end()))
		{
			vec.push_back(make_pair(pos.first + 1, pos.second));
		}

		//y-1
		if ((pos.second - 1) >= 0 && (std::find(vec.begin(), vec.end(), make_pair(pos.first, pos.second - 1)) == vec.end()))
		{
			vec.push_back(make_pair(pos.first, pos.second - 1));
		}

		//y+1
		if ((pos.second + 1)<_Height && (std::find(vec.begin(), vec.end(), make_pair(pos.first, pos.second + 1)) == vec.end()))
		{
			vec.push_back(make_pair(pos.first, pos.second + 1));
		}
	}

}

void FillUpManage::boxAdd(pair<int, int> pos, vector<pair<int, int>> &vec)
{
	vec.push_back(pos);
	//x-1
	if ((pos.first - 1) >= 0 && (std::find(vec.begin(), vec.end(), make_pair(pos.first - 1, pos.second)) == vec.end()) && getChessInfo(pos) == getChessInfo(make_pair(pos.first - 1, pos.second)))
	{
		boxAdd(make_pair(pos.first - 1, pos.second), vec);
	}

	//x+1
	if ((pos.first + 1)<_Width && (std::find(vec.begin(), vec.end(), make_pair(pos.first + 1, pos.second)) == vec.end()) && getChessInfo(pos) == getChessInfo(make_pair(pos.first + 1, pos.second)))
	{
		boxAdd(make_pair(pos.first + 1, pos.second), vec);
	}

	//y-1
	if ((pos.second - 1) >= 0 && (std::find(vec.begin(), vec.end(), make_pair(pos.first, pos.second - 1)) == vec.end()) && getChessInfo(pos) == getChessInfo(make_pair(pos.first, pos.second - 1)))
	{
		boxAdd(make_pair(pos.first, pos.second - 1), vec);
	}

	//y+1
	if ((pos.second + 1)<_Height && (std::find(vec.begin(), vec.end(), make_pair(pos.first, pos.second + 1)) == vec.end()) && getChessInfo(pos) == getChessInfo(make_pair(pos.first, pos.second + 1)))
	{
		boxAdd(make_pair(pos.first, pos.second + 1), vec);
	}
}

bool FillUpManage::onTouchBegan(Touch *touch, Event *unused_event)
{
	if (getGameState() != STATE_RUN||_gameFaild)
		return false;

	auto pos = positionTopos(touch->getLocation());

	if (_guide)
	{
		bool tempkey = true;
		if (_guidenumber == 2)
		{
			if ((pos.first == 1) && (pos.second == 5 || pos.second == 6))
			{
				auto colorvec = boxGet(pos);
				boxEliminate(colorvec);
			}
			else
				tempkey = false;
		}
		if (tempkey)
		{
			this->removeChildByName("guidebacklayer");
			this->removeChildByName("guidelayer");
			this->removeChildByName("hand");
			_clip->removeChildByName("guidebacklayer");
			setGameState(STATE_STOP);
			float tt = 0.0f;
			if (_guidenumber == 2)
			{
				tt=1.0f;
			}
			runAction(Sequence::create(DelayTime::create(tt), CallFunc::create([=]() {
				setGuide1();
			}), nullptr));
		}
	}

	else if (pos.first >= 0 && pos.first < _Width&&pos.second >= 0 && pos.second < _Height&&touch->getLocation().y>= HEIGHTOFFEST)
	{
		//判断当前点击的方块已经下落到当前位置
		bool tempjudge = true;

		auto colorvec = boxGet(pos);
		for (auto &i : colorvec)
		{
			auto tempbox = _waveNode->getChildByTag(i.first*_Height + i.second);
			if (tempbox)
			{
				auto tempPos = positionTopos(tempbox->getPosition());
				if (tempPos != i)
				{
					tempjudge = false;
					break;
				}
			}
		}
		//

		if (tempjudge)
		{
			if (colorvec.size() >= ENUM)
			{
				_lianji++;
				showLiJi(posToposition(pos));

				if (_hightestLevel)
					crazyAdd(colorvec);
				if (colorvec.size() >= _BoxChangeNum)
					_changeColor = true;
				boxEliminate(colorvec);
			}
			else
			{
				touchFaild();

				//打断
				showText(TEXT::daduan, posToposition(pos));
				//
			}
		}
	}

	return false;
}

//显示连击数
void FillUpManage::showLiJi(Vec2 pos)
{
	return;
	Node *node = Node::create();
	this->addChild(node, 11);
	node->setPosition(pos+Vec2(50,20));
	node->setScale(0.8f);

	node->runAction(Sequence::create(MoveBy::create(0.5f, Vec2(0, 20)), CallFunc::create([=]() {
		node->removeFromParent();
	}), nullptr));
	node->runAction(FadeOut::create(1.0f));

	auto sp1 = Sprite::create("FillUp/prop/lianji.png");
	node->addChild(sp1);
	sp1->setAnchorPoint(Vec2(1.0f, 0.5f));

	auto fnt = cjAtlasLabel::createWithInt(_lianji, ATLAS_PATH13);
	node->addChild(fnt);
	fnt->setAnchorPoint(Vec2(0.0f, 0.5f));
	fnt->setPosition(sp1->getPosition() + Vec2(5, 0));
}

//方块填充
void FillUpManage::boxFillUp()
{
	float time = 0.1f;
	float speed = 20.0f;
	for (int i = 0; i < _Width; i++)
	{
		int yoffest = 0;
		for (int j = 0; j < _Height; j++)
		{
			if (getChessInfo(make_pair(i, j)) == 0)
			{
				yoffest++;
			}
			else if(yoffest>0)
			{
				auto box = _waveNode->getChildByTag(i*_Height + j);
				auto pos = posToposition2(make_pair(i, j - yoffest));
				box->removeChildByName("gray");
				box->stopAllActions();
				box->setColor(Color3B(255, 255, 255));
				box->setScale(1.0f);
				box->runAction(Sequence::create(MoveTo::create(yoffest/speed, pos), CallFunc::create([=]() {
					auto sc1 = ScaleTo::create(time, 1.05f, 0.85f);
					auto sc2 = ScaleTo::create(time, 0.95f, 1.05f);
					auto sc3 = ScaleTo::create(time, 1.02f, 0.9f);
					auto sc5 = ScaleTo::create(time, 1.0f);
					box->runAction(Sequence::create(sc1, sc2,sc3,sc5,nullptr));
				}), nullptr));
				box->setTag(i*_Height + j - yoffest);
				_chesscolor[i*_Height +j - yoffest] = _chesscolor[i*_Height +j];
				_chesscolor[i*_Height +j] = 0;
			}
		}
		for (int j = 0; j < yoffest; j++)
		{
			int tempcolor = rand() % _BoxIndex + 1;
			auto box = Sprite::create(cjTTFLabel::getNameByInt("FillUp/box/box%d-01.png", tempcolor));
			box->setScale(_ChessScale);
			box->setAnchorPoint(Vec2(0.5f, 0.0f));
			_waveNode->addChild(box, 1, i*_Height + _Height - yoffest + j);
			box->setPosition(posToposition2(make_pair(i, _Height + j)));
			auto pos = posToposition2(make_pair(i, _Height - yoffest + j));
			box->runAction(Sequence::create(MoveTo::create(yoffest/speed, pos), CallFunc::create([=]() {
				auto sc1 = ScaleTo::create(time, 1.05f, 0.85f);
				auto sc2 = ScaleTo::create(time, 0.95f, 1.05f);
				auto sc3 = ScaleTo::create(time, 1.02f, 0.9f);
				auto sc5 = ScaleTo::create(time, 1.0f);
				box->runAction(Sequence::create(sc1, sc2, sc3, sc5, nullptr));
			}), nullptr));
			_chesscolor[i*_Height + _Height - yoffest + j] = tempcolor;
		}
	}

	//runAction(Sequence::create(DelayTime::create(1.1f), CallFunc::create([=]() {
		if (canTouchJudge())
		{
			if (_propbool)
			{
				if(_level==1)
					setGameState(STATE_STOP);
				propHandle();
			}
			else
			{
				//变色
				if(!_gameFaild)
					changeBoxColor();

				if (!_guide)
				{
					if (getGameState() == STATE_STOP)
						setGameState(STATE_RUN);
				}
			}

		}
		else
		{
			//重新分布
			reDrop();
		}
	//}), nullptr));
}

//变色
void FillUpManage::changeBoxColor()
{
	if (_changeColor)
	{
		_changeColor = false;

		vector<pair<int, int>> getbox;
		int tempkey = rand() % 4;
		if (tempkey == 0)
		{
			for (int i = 0; i<_Width; i++)
				for (int j = 0; j < _Height; j++)
				{
					getbox = boxGet(make_pair(i, j));
					if (getbox.size() >= CHANGEENUM)
					{
						changeBoxColor1(getbox);
						return;
					}
					else
						getbox.clear();
				}
		}
		else if (tempkey == 1)
		{
			for (int i = 0; i<_Width; i++)
				for (int j = (_Height-1); j >=0; j--)
				{
					getbox = boxGet(make_pair(i, j));
					if (getbox.size() >= CHANGEENUM)
					{
						changeBoxColor1(getbox);
						return;
					}
					else
						getbox.clear();
				}
		}
		else if (tempkey == 2)
		{
			for (int i = (_Width-1); i>=0; i--)
				for (int j = (_Height-1); j >=0; j--)
				{
					getbox = boxGet(make_pair(i, j));
					if (getbox.size() >= CHANGEENUM)
					{
						changeBoxColor1(getbox);
						return;
					}
					else
						getbox.clear();
				}
		}
		else if (tempkey == 3)
		{
			for (int i = (_Width-1); i>=0; i--)
				for (int j = 0; j < _Height; j++)
				{
					getbox = boxGet(make_pair(i, j));
					if (getbox.size() >= CHANGEENUM)
					{
						changeBoxColor1(getbox);
						return;
					}
					else
						getbox.clear();
				}
		}
	}
}

void FillUpManage::changeBoxColor1(vector<pair<int, int>> &vec)
{
	for (auto &pos : vec)
	{
		//x-1
		if ((pos.first - 1) >= 0 && (std::find(vec.begin(), vec.end(), make_pair(pos.first - 1, pos.second)) == vec.end()) && getChessInfo(pos) != getChessInfo(make_pair(pos.first - 1, pos.second)))
		{
			chaneBoxId(make_pair(pos.first - 1, pos.second), getChessInfo(pos));
			return;
		}

		//x+1
		if ((pos.first + 1)<_Width && (std::find(vec.begin(), vec.end(), make_pair(pos.first + 1, pos.second)) == vec.end()) && getChessInfo(pos) != getChessInfo(make_pair(pos.first + 1, pos.second)))
		{
			chaneBoxId(make_pair(pos.first + 1, pos.second), getChessInfo(pos));
			return;
		}

		//y-1
		if ((pos.second - 1) >= 0 && (std::find(vec.begin(), vec.end(), make_pair(pos.first, pos.second - 1)) == vec.end()) && getChessInfo(pos) != getChessInfo(make_pair(pos.first, pos.second - 1)))
		{
			chaneBoxId(make_pair(pos.first, pos.second - 1), getChessInfo(pos));
			return;
		}

		//y+1
		if ((pos.second + 1)<_Height && (std::find(vec.begin(), vec.end(), make_pair(pos.first, pos.second + 1)) == vec.end()) && getChessInfo(pos) != getChessInfo(make_pair(pos.first, pos.second + 1)))
		{
			chaneBoxId(make_pair(pos.first, pos.second + 1), getChessInfo(pos));
			return;
		}
	}
}

//改变颜色
void FillUpManage::chaneBoxId(pair<int, int> pos, int id)
{
	removeBox(pos);
	auto box = Sprite::create(cjTTFLabel::getNameByInt("FillUp/box/box%d-01.png", id));
	box->setScale(_ChessScale);
	box->setAnchorPoint(Vec2(0.5f, 0.0f));
	_waveNode->addChild(box, 1, pos.first*_Height + pos.second);
	box->setPosition(posToposition2(pos));
	_chesscolor[pos.first*_Height + pos.second] = id;

	//addParticle(pos.first, pos.second, id,2);
}

//重新分配
void FillUpManage::reDrop()
{
	for(int i=0;i<_Width;i++)
		for (int j = 0; j < _Height; j++)
		{
			removeBox(make_pair(i, j));
		}

	GameData::getSaveData()->_fillup._istip = false;
	startDrop();
}

//方块消除
void FillUpManage::boxEliminate(vector<pair<int, int>> &vec,float discount)
{
	//
	_eliminatetime = _propdata[_level].eliminateTime;

	addScore(vec, discount);

	Vector<FiniteTimeAction*> action;
	int k = 0;
	for (auto &i : vec)
	{
		k++;
		//lizivec lizi(i.first, i.second, getChessInfo(i));
		//_lizivec.push_back(lizi);

		if ((vec.size() <= 4)||(vec.size()>4&&(k % 2 != 0)))
		{
			auto pos = i;
			action.pushBack(DelayTime::create(1.0f / 60.0f));
			action.pushBack(CallFunc::create([=]() {
				addParticle(pos.first, pos.second,getChessInfo(make_pair(pos.first,pos.second)));
			}));
		}

		removeBox(i);
	}
	runAction(Sequence::create(action));

	if(!_gameFaild)
		boxFillUp();
}

//加分
void FillUpManage::addScore(vector<pair<int, int>> &vec,float discount)
{
	//疯狂模式打折
	float fkdis = 1.0f;
	if (_hightestLevel)
		fkdis = 0.8f;

	int num = vec.size();
	if (num > 1 && num < 100)
	{
		int addnum = _ScoreConfig.at(num)*_multiple*discount*fkdis;

		if (_gameFaild||_jiashi)
			addnum = _ScoreConfig.at(num);

		//特殊道具3
		float beishu = 1.0f;
		
		if (_specialProp[2]&&!_jiashi)
		{

			beishu *= 1.35f;
		}
		
		//

		addnum *= beishu;

		_score += addnum;
		scoreMotion(addnum, vec[0], beishu);

		if (_multiple >= 2 && _multiple <= 4)
			cjMusic::playEffect(cjTTFLabel::getNameByInt("video/FillUp/jiabei_%d.mp3", _multiple).c_str());

		int soundnum = 1;
		if (num > 6)
			soundnum = 3;
		else if (num > 3)
			soundnum = 2;
		cjMusic::playEffect(cjTTFLabel::getNameByInt("video/FillUp/xiaochu_%d.mp3", soundnum).c_str());

		if (!_hightestLevel)
		{
			_propscore += 1;
			barHandle();
		}

		char name[100];
		sprintf(name, "消除 %d 个棋子，得分： %d, 分数打折：%02.1f", num, addnum,discount);
		cj::LogShow(_timenumber,name);


	}
}

//棋盘可点击判断
bool FillUpManage::canTouchJudge()
{
	for (int i = 0; i<_Width; i++)
		for (int j = 0; j < _Height; j++)
		{
			if (boxGet(make_pair(i, j)).size() >= ENUM)
				return true;
		}
	return false;
}

//倒计时
void FillUpManage::timeUpdate(float ft)
{
	if (!_timeDecrease)
		return;

	//加倍
	if (_multipletime > 0)
	{
		_multipletime--;
		if(_multipletime==0)
			cj::LogShow(_timenumber, "分数加倍结束");
	}
	else
	{
		if (_level == 2&&_hightestLevel)
		{
			barRestart();
		}
		_multiple = 1;
	}

	//点击时间间隔
	if (_eliminatetime > 0)
	{
		_eliminatetime--;
	}
	else
	{
		touchFaild();
	}

	
	if (_stoptime > 0)
	{
		_stoptime--;
		if (_stoptime == 0)
		{
			auto spine_bing = dynamic_cast<SkeletonAnimation*>(this->getChildByName("spine_bing"));
			if (spine_bing)
			{
				spine_bing->clearTrack(0);
				spine_bing->setToSetupPose();
				spine_bing->addAnimation(1, "bing_end", false);

				cjMusic::playEffect("video/FillUp/bingdong2.mp3");

				runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([this]() {
					this->removeChildByName("spine_bingdong");
					this->removeChildByName("spine_bing");
					this->removeChildByName("par_bing");
				}), nullptr));
			}
		}
	}
	else
	{
		if (_timenumber > 0)
		{
			_timenumber--;

			if (_timenumber <= 10 && _timenumber >= 5)
				cjMusic::playEffect("video/FillUp/daoshu1.mp3");
			else if (_timenumber <5)
				cjMusic::playEffect("video/FillUp/daoshu2.mp3");

			_timebar->runAction(ProgressTo::create(1.0f, 100 * _timenumber * 1.0f / _Time));
			//GameData::getSaveData()->_fillup._istip = true;
			dataSave();

			if (_timenumber <= 10)
			{
				timeSliderMotion();
				_timelabel->setVisible(false);
				_timelabel2->setVisible(true);
				_timelabel2->setString(cjTTFLabel::getNameByInt("%d", _timenumber));
			}
			else
			{
				_timelabel->setVisible(true);
				_timelabel2->setVisible(false);
				_timelabel->setString(cjTTFLabel::getNameByInt("%d", _timenumber));
			}
			
			if (_timenumber == 0)
			{
				//cjMusic::playEffect("video/FillUp/shijiandao.mp3");

				GameData::getSaveData()->_fillup._istip = false;
				dataSave();
				this->unschedule(schedule_selector(FillUpManage::timeUpdate));
				setGameState(STATE_STOP);
				_gameFaild = true;
				showText(TEXT::shijiandao);

				runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=]() {
					endGame();
				}), nullptr));
			}
		}
	}
}

void FillUpManage::barRestart()
{
	_level = 0;
	_propscore = 0;
	_hightestLevel = false;
	toolSliderMotion(false);
	for (int i = 0; i < 3; i++)
	{
		_propbar[i]->setPercentage(0);
		_proppar[i]->setVisible(false);
		_proppar[i]->setPosition(PARSTART);
	}

	this->removeChildByName("prop");
	_propid = _propdata[_level].prop[rand() % (_propdata[_level].prop.size())];
	auto sprop = Sprite::create(cjTTFLabel::getNameByInt("FillUp/prop/prop%02d.png", _propid));
	this->addChild(sprop, 14, "prop");
	sprop->setPosition(PROPPOS);
}

//点击失败和时间过长
void FillUpManage::touchFaild()
{
	if (_propdata[_level].interrupt)
	{
		_lianji = 0;
		_multiple = 1;
		_multipletime = 0;
		_eliminatetime = _propdata[_level].eliminateTime;

		_propscore *= _propdata[_level].scoreReduce;

		_level = 0;
		for (int i = 1; i >= 0; i--)
		{
			if (_propscore > _propdata[i].score)
				_level = i + 1;
		}

		Vector<FiniteTimeAction*> action;
		for (int i = 2; i >= 0; i--)
		{
			int temp = _propbar[i]->getPercentage();
			if (_level < i&&temp > 0)
			{
				action.pushBack(CallFunc::create([=]() {
					_propbar[i]->runAction(ProgressTo::create(temp/100.0f*REDUCESUDE,0.0f));
					_proppar[i]->stopAllActions();
					_proppar[i]->runAction(Sequence::create(MoveTo::create(temp / 100.0f*REDUCESUDE, PARSTART), CallFunc::create([=]() {
						_proppar[i]->setVisible(false);
					}), nullptr));
				}));
				action.pushBack(DelayTime::create(temp / 100.0f*REDUCESUDE));
			}
			if (_level == i)
			{
				float percentage;
				if (_level == 0)
					percentage = _propscore*100.0f / _propdata[0].score;
				else
					percentage=(_propscore-_propdata[_level-1].score)*100.0f / (_propdata[_level].score- _propdata[_level-1].score);
				action.pushBack(CallFunc::create([=]() {
					_propbar[_level]->runAction(ProgressTo::create((100.0f - percentage) *0.1f/ 100.0f, percentage));
					if(_propbar[_level]->getPercentage()==100)
						_proppar[_level]->setVisible(true);
					_proppar[_level]->stopAllActions();
					_proppar[_level]->runAction(Sequence::create(MoveTo::create((100.0f - percentage) *0.1f / 100.0f, PARSTART + Vec2(0, percentage*PARLENGTH / 100.0f)), CallFunc::create([=]() {
						if (percentage <= 2)
							_proppar[_level]->setVisible(false);
					}), nullptr));
				}));
			}
		}
		action.pushBack(CallFunc::create([=]() {
			_hightestLevel = false;
			toolSliderMotion(false);

			this->removeChildByName("prop");
			_propid = _propdata[_level].prop[rand() % (_propdata[_level].prop.size())];
			auto sprop = Sprite::create(cjTTFLabel::getNameByInt("FillUp/prop/prop%02d.png", _propid));
			this->addChild(sprop, 14, "prop");
			sprop->setPosition(PROPPOS);
		}));
		runAction(Sequence::create(action));
	}
}

//分数动画
void FillUpManage::scoreMotion(int score, pair<int, int> pt,int multiple)
{
	if (score<=0)
		return;

	_daduanshow = true;

	int colorid = _chesscolor[pt.first*_Height + pt.second];
	assert(colorid > 0);

	char scorefont[30];
	char chaname[30];
	sprintf(scorefont, "ziti/fillup/score_%s.png", BOXCOLOR[colorid - 1]);
	sprintf(chaname, "FillUp/ziti/x_%s.png", BOXCOLOR[colorid - 1]);

	int offest = 0;
	if (pt.first == 0)
		offest = _ChessSize*0.5f;
	else if (pt.first == (_Width - 1))
		offest = -_ChessSize*0.5f;

	auto scorefnt = cjAtlasLabel::createWithInt((score/multiple), scorefont);
	scorefnt->setCascadeOpacityEnabled(true);
	this->addChild(scorefnt, 15);
	scorefnt->setPosition(posToposition(pt)+Vec2(offest,-0.0f));
	Vector<FiniteTimeAction*> action;
	action.pushBack(ScaleTo::create(0.3f, 1.5f));
	action.pushBack(ScaleTo::create(0.2f, 0.9f));
	action.pushBack(ScaleTo::create(0.1f, 1.0f));
	action.pushBack(DelayTime::create(0.3f));
	action.pushBack(MoveBy::create(0.5f, Vec2(0, _ChessSize*0.6f)));
	action.pushBack(CallFunc::create([scorefnt]() {
		scorefnt->removeFromParent();
	}));
	scorefnt->runAction(Sequence::create(action));
	scorefnt->runAction(Sequence::create(DelayTime::create(0.9f), FadeOut::create(0.5f),nullptr));

	if (multiple > 1)
	{
		float sc = 0.6f;

		auto cha = Sprite::create(chaname);
		scorefnt->addChild(cha);
		cha->setScale(sc);
		cha->setAnchorPoint(Vec2(0.5f, 0.0f));
		cha->setPosition(scorefnt->getContentSize().width + cha->getContentSize().width*sc-3, 0);

		auto multiplefont = cjAtlasLabel::createWithInt(multiple, scorefont);
		scorefnt->addChild(multiplefont);
		multiplefont->setScale(sc);
		multiplefont->setAnchorPoint(Vec2(0.5f, 0.0f));
		multiplefont->setPosition(cha->getPosition() + Vec2((cha->getContentSize().width + multiplefont->getContentSize().width) / 2.0f*sc,2));
	}
}

//道具进度条处理
void FillUpManage::barHandle()
{
	if (_level == 0)
	{
		float key = MIN(100, _propscore*100.0f / _propdata[_level].score);
		_propbar[_level]->runAction(ProgressTo::create(REDUCESUDE, key));
		if (key >= 2)
		{
			_proppar[_level]->setVisible(true);
			_proppar[_level]->stopAllActions();
			_proppar[_level]->runAction(MoveTo::create(REDUCESUDE, PARSTART + Vec2(key*PARLENGTH / 100.0f, 0)));
		}
	}
	else
	{
		float key = MIN(100,(_propscore - _propdata[_level - 1].score)*100.0f / (_propdata[_level].score - _propdata[_level - 1].score));
		_propbar[_level]->runAction(ProgressTo::create(REDUCESUDE, key));
		if (key >= 2)
		{
			_proppar[_level]->setVisible(true);
			_proppar[_level]->stopAllActions();
			_proppar[_level]->runAction(MoveTo::create(REDUCESUDE, PARSTART + Vec2(key * PARLENGTH / 100.0f, 0)));
		}
	}		
	if (_propscore >= _propdata[_level].score&&!_hightestLevel)
	{
		//

		int lev = _level;
		runAction(Sequence::create(DelayTime::create(REDUCESUDE), CallFunc::create([lev,this]() {
			if (lev == (_level - 1))
			{
				if(_proppar[lev])
					_proppar[lev]->setVisible(false);
			}
		}), nullptr));

		if (_level < 2)
		{
			_propbool = true;
			_multiple = _propdata[_level].multiple;
			_multipletime = _propdata[_level].multipletime;
			_level++;
			int key = (_propscore - _propdata[_level - 1].score)*100.0f / (_propdata[_level].score - _propdata[_level - 1].score);
			if(_propbar[_level])
				_propbar[_level]->runAction(Sequence::create(DelayTime::create(REDUCESUDE), ProgressTo::create(REDUCESUDE, key), nullptr));
			if(key>=2&&_proppar[_level])
				_proppar[_level]->setVisible(true);
			if (_proppar[_level])
			{
				_proppar[_level]->stopAllActions();
				_proppar[_level]->runAction(MoveTo::create(REDUCESUDE, PARSTART + Vec2(key * PARLENGTH / 100.0f, 0)));
			}
		}
		else if (_level == 2)
		{
			_propscore = _propdata[_level].score;
			_propbool = true;
			_multiple = _propdata[_level].multiple;
			_multipletime = _propdata[_level].multipletime;
		}
	}
}

void FillUpManage::propHandle()
{
	_propbool=false;

	auto prop = this->getChildByName("prop");

	int x;
	int y;
	if (_propid == 4)
	{
		auto pos = getMostestBox();
		x = pos.first;
		y = pos.second;
	}
	else
	{
		x = rand() % (_Width - 2) + 1;
		y = rand() % (_Height - 2) + 1;
	}

	ccBezierConfig bezier;
	bezier.controlPoint_1 = prop->getPosition();
	bezier.controlPoint_2 = prop->getPosition()+Vec2(-200,100);
	bezier.endPosition = posToposition(make_pair(x,y));

	prop->runAction(ScaleTo::create(PROPDROPTIME, 1.2f));

	Vector<FiniteTimeAction*> action;
	if (_propid != 3)
	{
		if(_propid==4)
			action.pushBack(BezierTo::create(PROPDROPTIME, bezier));
		else if(_propid<4)
			action.pushBack(MoveTo::create(PROPDROPTIME, bezier.endPosition)); 
		//else if (_propid == 5)
		//{
		//	auto pos = this->getChildByName("timesp")->getPosition();
		//	action.pushBack(MoveTo::create(PROPDROPTIME, pos));
		//}
	}
	action.pushBack(CallFunc::create([=]() {
		int key = 0;
		if (_level <= 2&&!_hightestLevel)
		{
			key = _propdata[_level].prop[rand() % (_propdata[_level].prop.size())];
			auto sprop = Sprite::create(cjTTFLabel::getNameByInt("FillUp/prop/prop%02d.png", key));
			this->addChild(sprop, 14, "prop");
			sprop->setTag(_level);
			sprop->setPosition(PROPPOS);
		}

		propEliminate(x, y);
		_propid = key;

		prop->removeFromParent();
	}));

	prop->runAction(Sequence::create(action));

}

void FillUpManage::propEliminate(int x,int y)
{
	cj::LogShow(_timenumber, cjTTFLabel::getNameByInt("得到道具:%d", _propid));
	vector<pair<int, int>> vec;
	//炸弹
	if (_propid == 1)
	{
		for (int i = x - 1; i <= x + 1; i++)
			for (int j = y - 1; j <= y + 1; j++)
			{
				if (i >= 0 && i < _Width&&j >= 0 && j < _Height)
					vec.push_back(make_pair(i, j));
			}
		boxEliminate(vec,_propdata[_level].eliminateScore);

		auto bombcsb = CSLoader::createNode("animation/prop_skill_bomb.csb");
		this->addChild(bombcsb, 10);
		bombcsb->setPosition(posToposition(make_pair(x, y)));
		auto action = CSLoader::createTimeline("animation/prop_skill_bomb.csb");
		action->gotoFrameAndPlay(0, false);
		bombcsb->runAction(action);
		action->setLastFrameCallFunc([=]() {
			bombcsb->removeFromParent();
		});
	}
	//刷子
	else if (_propid == 2)
	{
		useProp2();
	}
	//重排
	else if (_propid == 3)
	{
		auto spinenode = SkeletonAnimation::createWithFile(SPINEPATH + "chongpai_Export/export/chongpai.json", SPINEPATH + "chongpai_Export/export/chongpai.atlas");
		this->addChild(spinenode,11,"spine_chongpai");
		spinenode->addAnimation(0, "daoju", false);
		spinenode->setPosition(WINSIZE/2.0f);

		this->runAction(Sequence::create(DelayTime::create(0.4f), CallFunc::create([this]() {
			auto xwpar = ParticleSystemQuad::create(SPINEPATH + "chongpai_Export/lizi/xw.plist");
			this->addChild(xwpar, 11,"xwpar");
			xwpar->setAutoRemoveOnFinish(true);
			xwpar->setPosition(posToposition(make_pair(3, 3)) + Vec2(0, _Height / 2.0f));
		}), DelayTime::create(1.0f), CallFunc::create([this]() {
			this->removeChildByName("xwpar");
			this->removeChildByName("spine_chongpai");
		}), nullptr));

		cjMusic::playEffect("video/FillUp/chongpai.mp3");

		useProp3(make_pair(x, y));
	}
	//闪电
	else if (_propid == 4)
	{
		useProp4(make_pair(x, y));
	}
	//冰冻时间
	else if (_propid == 5)
	{
		auto spinenode = SkeletonAnimation::createWithFile(SPINEPATH + "bdms_export/bddaoju_export/export/bddaoju.json", SPINEPATH + "bdms_export/bddaoju_export/export/bddaoju.atlas");
		this->addChild(spinenode, 12,"spine_bingdong");
		spinenode->addAnimation(0, "daoju", false);
		spinenode->setPosition(WINSIZE / 2.0f);

		runAction(Sequence::create(DelayTime::create(0.3f), CallFunc::create([=]() {
			auto spinenode2 = SkeletonAnimation::createWithFile(SPINEPATH + "bdms_export/bing_EXPort/bing.json", SPINEPATH + "bdms_export/bing_EXPort/bing.atlas");
			this->addChild(spinenode2, 11,"spine_bing");
			spinenode2->addAnimation(0, "bing_start", false);
			spinenode2->addAnimation(0, "bing_loop", true);
			spinenode2->setPosition(WINSIZE / 2.0f);

			auto parlayer = Layer::create();
			this->addChild(parlayer, 11, "par_bing");
			for (int i = 0; i < 4; i++)
			{
				auto par = SkeletonAnimation::createWithFile(SPINEPATH + "bq/skeleton.json", SPINEPATH + "bq/skeleton.atlas");
				parlayer->addChild(par);
				par->addAnimation(0, "animation_start", false);
				par->addAnimation(1, "animation", true);
				par->setPosition(WINSIZE / 2.0f);
				if (i == 1)
				{
					par->setRotation(90);
					par->setPosition(par->getPosition() + Vec2(0, 160));
				}
				else if (i == 2)
				{
					par->setRotation(180);
				}
				else if (i == 3)
				{
					par->setRotation(270);
					par->setPosition(par->getPosition() - Vec2(0, 160));
				}
			}

			cjMusic::playEffect("video/FillUp/bingdong1.mp3");



		}), nullptr));

		useProp5();
	}
	//疯狂
	else if (_propid == 6)
	{
		useProp6();
	}
}

void FillUpManage::dataSave()
{
	if (_guide)
		return;
	GameData::getSaveData()->_fillup._level = _level;
	GameData::getSaveData()->_fillup._score = _score;
	GameData::getSaveData()->_fillup._propid = _propid;
	GameData::getSaveData()->_fillup._time = _timenumber;
	GameData::getSaveData()->_fillup._propscore = _propscore;
	GameData::getSaveData()->_fillup._multiple=_multiple;
	GameData::getSaveData()->_fillup._multipletime=_multipletime;
	GameData::getSaveData()->_fillup._eliminatetime = _eliminatetime;
	GameData::getSaveData()->_fillup._hightestlevel = _hightestLevel;
	for(int i=0;i<_Width;i++)
		for (int j = 0; j < _Height; j++)
		{
			GameData::getSaveData()->_fillup._chessInfo[i*_Height + j] = getChessInfo(make_pair(i, j));
		}
	GameDataInstance()->dataSave();
}

//消除粒子
void FillUpManage::eliminateLizi(float ft)
{
	if (_lizivec.size() > 0)
	{
		auto lizi = _lizivec.begin();
		addParticle(lizi->x, lizi->y, lizi->color);
		_lizivec.erase(lizi);
	}
}

void FillUpManage::addParticle(int x, int y, int color,int model)
{
	//particle
	char name1[30];
	sprintf(name1, "xc/%s.json", XIAOCHUCOLOR[color - 1]);
	char name2[30];
	sprintf(name2,"xc/%s.atlas", XIAOCHUCOLOR[color - 1]);

	auto xiaochu = SkeletonAnimation::createWithFile(SPINEPATH + name1, SPINEPATH + name2);
	_clip->addChild(xiaochu, 15);
	xiaochu->setPosition(posToposition(make_pair(x, y)));
	xiaochu->addAnimation(1, XIAOCHUCOLOR[color - 1], false);
	xiaochu->runAction(Sequence::create(DelayTime::create(0.4f), CallFunc::create([xiaochu]() {
		xiaochu->removeFromParent();
	}), nullptr));
}

void FillUpManage::removeBox(pair<int, int> pos)
{
	_waveNode->removeChildByTag(pos.first*_Height + pos.second);
	_chesscolor[pos.first*_Height + pos.second] = 0;
}

//游戏结束
void FillUpManage::endGame()
{
	//特殊道具1
	if (_specialProp[0])
	{
		_specialProp[0] = false;
		GameData::getSaveData()->_fillupprop[2] = false;
		GameData::getInstance()->dataSave();

		showText(TEXT::jxxc);
		runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create([=]() {
			useProp2();
		}), nullptr));
	}
	else
	{
		setGameState(STATE_FAILD);
	}
}

//棋盘统一消除
void FillUpManage::useProp2()
{
	vector<pair<int, int>> vec;
	bool caneli = false;
	for(int i=0;i<_Width;i++)
		for (int j = 0; j< _Height; j++)
		{
			if (getChessInfo(make_pair(i,j))>0)
			{
				vec = boxGet(make_pair(i, j));
				if (vec.size() >= ENUM)
				{
					caneli = true;
					break;
				}
			}
			if (caneli)
				break;
		}

	if (caneli)
	{
		boxEliminate(vec);
		runAction(Sequence::create(DelayTime::create(0.1f), CallFunc::create([this]() {
			useProp2();
		}), nullptr));
	}
	else
	{
		boxFillUp();
		runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([this]() {
			setGameState(STATE_FAILD);
		}), nullptr));
	}
}

//重排
void FillUpManage::useProp3(pair<int, int> pos)
{
	showText(TEXT::fengkuang);
	auto centerpos = posToposition(make_pair(3, 3)) + Vec2(0, _Height / 2.0f);

	float wavett = 0.2f;

	this->runAction(Sequence::create(DelayTime::create(0.6f), CallFunc::create([=]() {

		_waveNode->runAction(Sequence::create(Ripple3D::create(1.0f, CCSizeMake(35, 25), centerpos, 550, 3, 40),
			CallFunc::create([this]() {
			_waveNode->runAction(Ripple3D::create(0.1f, CCSizeMake(32, 24), Vec2(0,0), 10, 1, 0));
		}), nullptr));
		//return false;

		//排版类型
		assert(GameDataInstance()->d_fillupreset.size() > 0);
		int type = rand() % (GameDataInstance()->d_fillupreset.size()) + 1;

		for (int i = 0; i<_Width; i++)
			for (int j = 0; j < _Height; j++)
			{
				int num = MAX(abs(int((_Width-1)/2-i)),abs(int((_Height-1)/2.0f-j)));
				auto delaytime = DelayTime::create(num*wavett);
				auto call = CallFunc::create([=]() {

					auto resetcsb = CSLoader::createNode("animation/prop_skill_reset.csb");
					_waveNode->addChild(resetcsb, 8);
					resetcsb->setScale(1.2f);
					resetcsb->setPosition(posToposition(make_pair(i, j)));

					auto resetcsbaction = CSLoader::createTimeline("animation/prop_skill_reset.csb");
					resetcsb->runAction(resetcsbaction);
					resetcsbaction->play("red", false);
					resetcsbaction->setLastFrameCallFunc([=]() {
						resetcsb->removeFromParent();
					});

					this->runAction(Sequence::create(DelayTime::create(0.1f), CallFunc::create([=]() {
						removeBox(make_pair(i, j));

						int key = getResetId(i, j, type);

						auto box = Sprite::create(cjTTFLabel::getNameByInt("FillUp/box/box%d-01.png", key));
						box->setAnchorPoint(Vec2(0.5f, 0.0f));
						box->setScale(_ChessScale);
						_waveNode->addChild(box, 1, i*_Height + j);
						box->setPosition(posToposition2(make_pair(i, j)));
						_chesscolor[i*_Height + j] = key;
					}),nullptr));
				});
				_waveNode->runAction(Sequence::create(delaytime, call, nullptr));
			}

	}), nullptr));
	this->runAction(Sequence::create(DelayTime::create(1.5f), CallFunc::create([=]() {
		_waveNode->removeFromParent();
		_waveNode = nullptr;
		_waveNode = NodeGrid::create();
		_clip->addChild(_waveNode, 10);
		startDrop(true);
		
		if (getGameState() == STATE_STOP)
			setGameState(STATE_RUN);
	}),nullptr));
}

//闪电
void FillUpManage::useProp4(pair<int, int> pos)
{
	_timeDecrease = false;

	LayerColor* pPropChildLayer = LayerColor::create(Color4B(0, 0, 0, OPA_BEIJING * 255));

	LayerColor* pPropChildLayer2 = LayerColor::create(Color4B(0, 0, 0, OPA_BEIJING * 255));
	this->addChild(pPropChildLayer2, 9, 9);

	int color = getChessInfo(pos);
	std::string colorName;
	std::string actionName_Line;

	switch (color)
	{
	case 4:
		colorName = "red";
		break;
	case 5:
		colorName = "yellow";
		break;
	case 2:
		colorName = "blue";
		break;
	case 1:
		colorName = "green";
		break;
	case 3:
		colorName = "purple";
		break;
	}

	int cleanNo = 0;
	pair<int, int> pStar1;
	pair<int, int> pStar2;
	Vector<FiniteTimeAction*> arrayOfActions;

	vector<pair<int, int>> selectbox;
	for (int i = 0; i<_Width; i++)
		for (int j = 0; j < _Height; j++)
		{
			if (getChessInfo(make_pair(i, j)) == getChessInfo(pos))
				selectbox.push_back(make_pair(i, j));
		}

	addScore(selectbox, _propdata[_level].eliminateScore);

	for (auto &i : selectbox)
	{
		auto node = _waveNode->getChildByTag(i.first*_Height + i.second);
		node->setLocalZOrder(101);
	}

	//int _shandianid = cjMusic::playEffect("video/shandian.mp3", true);

	do
	{
		if (selectbox.empty())
		{
			arrayOfActions.pushBack(DelayTime::create(0.1f));
			arrayOfActions.pushBack(CallFunc::create([=]() {
				pPropChildLayer->removeFromParent();
				pPropChildLayer2->removeFromParent();
			}));
			break;
		}

		cleanNo++;
		if (cleanNo == 1)
		{
			pStar1 = pos;
		}
		else
		{
			pStar1 = pStar2;
		}

		selectbox.erase(std::find(selectbox.begin(), selectbox.end(), pStar1));
		if (selectbox.size() > 0)
		{
			pStar2 = selectbox[rand() % selectbox.size()];
		}
		else
		{
			pStar2 = make_pair(-1, -1);
		}

		Vec2 position1 = posToposition(pStar1);

		arrayOfActions.pushBack(DelayTime::create(0.1f));

		arrayOfActions.pushBack(CallFunc::create([=]() {
			auto particleNodes = Node::create();
			_waveNode->addChild(particleNodes, 102);

			auto skeletonNode = CSLoader::createNode("animation/xiaochu.csb");
			skeletonNode->setPosition(position1);
			particleNodes->addChild(skeletonNode, 3);

			auto action = CSLoader::createTimeline("animation/xiaochu.csb");
			skeletonNode->runAction(action);

			action->play("red_line", false);
			action->setLastFrameCallFunc([skeletonNode]() {
				skeletonNode->removeFromParent();
			});

			auto playParticle1 = CallFunc::create([=]() {
				addParticle(pStar1.first, pStar1.second, getChessInfo(pStar1));
				removeBox(pStar1);
			});

			particleNodes->runAction(Sequence::create(
				DelayTime::create(1 / 30.f),
				playParticle1,
				nullptr));

		}));

		if (pStar2.first >= 0)
		{
			Vec2 position2 = posToposition(pStar2);
			arrayOfActions.pushBack(CallFunc::create([=]() {
				auto propSkillNode = CSLoader::createNode("animation/prop_skill_laser.csb");

				std::string lineNameArray[] = { "red", "yellow", "blue", "green", "purple" };
				int randLine = rand() % 5;
				auto colorSprite = static_cast<Sprite*>(propSkillNode->getChildByName("skill4_red"));
				colorSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(
					"animation/daoju_images/skill4_" + lineNameArray[randLine] + ".png"));

				propSkillNode->setPosition(position1);
				float angle = -1 * CC_RADIANS_TO_DEGREES((position2 - position1).getAngle());
				propSkillNode->setRotation(angle);
				propSkillNode->setScaleX(position1.getDistance(position2) / colorSprite->getContentSize().width);
				_waveNode->addChild(propSkillNode, 102);

				auto propSkillAction = CSLoader::createTimeline("animation/prop_skill_laser.csb");
				propSkillNode->runAction(propSkillAction);

				propSkillAction->play("red", false);

				propSkillAction->setLastFrameCallFunc([propSkillNode]() {
					propSkillNode->removeFromParent();
				});
			}));
		}
	} while (true);
	arrayOfActions.pushBack(CallFunc::create([=]() {
		boxFillUp();
		_timeDecrease = true;
	}));

	pPropChildLayer->runAction(Sequence::create(arrayOfActions));

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event) {
		return true;
	};
	pPropChildLayer2->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, pPropChildLayer2);

	//Director::getInstance()->getRunningScene()->addChild(pPropChildLayer, 10);
	_waveNode->addChild(pPropChildLayer, 100);
}

//时间冻结
void FillUpManage::useProp5()
{
	showText(TEXT::fengkuang);
	_stoptime = BINGDONG;
	if(getGameState()==STATE_STOP)
		setGameState(STATE_RUN);
}

//疯狂
void FillUpManage::useProp6()
{
	_hightestLevel = true;
	toolSliderMotion(true);
	if(getGameState()==STATE_STOP)
		setGameState(STATE_RUN);
}

//得到最多颜色的位置
pair<int, int> FillUpManage::getMostestBox()
{
	int a[5] = { 0 };
	for(int i=0;i<_Width;i++)
		for (int j = 0; j < _Height; j++)
		{
			a[_chesscolor[i*_Height + j] - 1]++;
		}
	int key = 0;
	for (int i = 1; i < 5; i++)
	{
		if (a[i] > a[key])
			key = i;
	}
	int x;
	int y;
	do {
		x = rand() % (_Width - 2) + 1;
		y = rand() % (_Height - 2) + 1;
	} while ((key+1) != _chesscolor[x*_Height + y]);
	return make_pair(x,y);
}

//时间进度条最后10秒动画
void FillUpManage::timeSliderMotion()
{
	auto bar_hong = _csbNode->getChildByName("time")->getChildByName("time_loading02");
	auto bar_hong_frame1= _csbNode->getChildByName("time_frame")->getChildByName("time_frame03");
	auto bar_hong_frame2 = _csbNode->getChildByName("time_frame")->getChildByName("time_frame04");
	auto bar_hong_tool = _csbNode->getChildByName("tool")->getChildByName("tool_loading_broken");
	auto bar_hong_red = _csbNode->getChildByName("broken")->getChildByName("broken_red");
	bar_hong_red->runAction(FadeIn::create(0.4f));
	bar_hong_tool->runAction(FadeIn::create(0.4f));
	bar_hong_frame1->runAction(FadeIn::create(0.4f));
	bar_hong->runAction(FadeIn::create(0.4f));
	bar_hong_frame2->runAction(Sequence::create(DelayTime::create(0.2f),FadeOut::create(0.4f), CallFunc::create([=]() {
		bar_hong_frame1->setOpacity(0);
		bar_hong_frame2->setOpacity(0);
		bar_hong_tool->setOpacity(0);
		bar_hong_red->setOpacity(0);
		bar_hong->setOpacity(0);
	}), nullptr));
}

//疯狂模式开启与关闭
void FillUpManage::toolSliderMotion(bool open)
{
	if (open)
	{
		showText(TEXT::fengkuang);

		_csbNode->getChildByName("tool")->getChildByName("tool_loading_full")->setVisible(true);
		_csbNode->getChildByName("tool_frame")->getChildByName("tool_frame_full")->setVisible(true);

		//auto mancao = SkeletonAnimation::createWithFile(SPINEPATH + "mancao_export/mancao.json", SPINEPATH + "mancao_export/mancao.atlas");
		//this->addChild(mancao, 1, "mancao");
		//mancao->setPosition(Vec2(220, 704));
		//mancao->addAnimation(0, "loop", true);

		cjMusic::playEffect("video/FillUp/fengkuang.mp3");

		auto spinenode = SkeletonAnimation::createWithFile(SPINEPATH + "fkms_export/EXPort/fkms.json", SPINEPATH + "fkms_export/EXPort/fkms.atlas");
		this->addChild(spinenode, 12, "spine_fkms");
		spinenode->addAnimation(0, "fkms_start", false);
		spinenode->addAnimation(0, "fkms_loop", true);
		spinenode->setPosition(WINSIZE / 2.0f);

		this->runAction(Sequence::create(DelayTime::create(0.4f), CallFunc::create([=]() {
			auto parlayer = Layer::create();
			_clip->addChild(parlayer, 12, "layer_fkms");
			auto spinelizi = SkeletonAnimation::createWithFile(SPINEPATH + "fkms_export/fkms_lizi/fkms_lizi.json", SPINEPATH + "fkms_export/fkms_lizi/fkms_lizi.atlas");
			spinelizi->addAnimation(0, "lizi", true);
			spinelizi->setPosition(posToposition(make_pair(3,3))+Vec2(0, _ChessSize/2.0f));
			parlayer->addChild(spinelizi);
		}), nullptr));
	}
	else
	{
		_csbNode->getChildByName("tool")->getChildByName("tool_loading_full")->setVisible(false);
		_csbNode->getChildByName("tool_frame")->getChildByName("tool_frame_full")->setVisible(false);
		//this->removeChildByName("mancao");

		auto spinenode =dynamic_cast<SkeletonAnimation*>(this->getChildByName("spine_fkms"));
		if (spinenode)
		{
			spinenode->setToSetupPose();
			spinenode->removeFromParent();

			_clip->removeChildByName("layer_fkms");
		}
	}
}

//重排模板
int FillUpManage::getResetId(int x, int y, int key)
{
	int res=1;
	auto vec = GameDataInstance()->d_fillupreset.at(key);
	if (vec.size()>0)
	{
		res = vec[y*_Width + x];
	}
	return res;
}

//弹字幕
void FillUpManage::showText(TEXT text,Vec2 Pos)
{
	//if (text == TEXT::daduan && !_daduanshow)
	//	return;
	auto tishiyu = dynamic_cast<SkeletonAnimation*>(this->getChildByName("tishiyu"));
	tishiyu->setToSetupPose();
	tishiyu->setPosition(Pos);
	string name;
	switch (text)
	{
	case daduan:
		cjMusic::playEffect("video/FillUp/daduan.mp3");
		name = "daduan";
		_daduanshow = false;
		_lianji = 0;
		break;
	case kaishi:
		//cjMusic::playEffect("video/FillUp/kaishi.mp3");
		name = "kaishi";
		break;
	case shijiandao:
		name = "shijiandao";
		break;
	case yubei:
		cjMusic::playEffect("video/FillUp/yubei.mp3");
		name = "yubei";
		break;
	case TEXT::fengkuang:
		name = "fengkuang";
		break;
	case TEXT::jxxc:
		name = "jxxc";
		break;
	default:
		break;
	}

	if (text == TEXT::daduan)
	{
		auto pos = positionTopos(Pos);
		auto box = _waveNode->getChildByTag(pos.first*_Height+pos.second);
		if (box)
		{
			char name[30];
			sprintf(name, "FillUp/box/el_%s1.png", BOXCOLOR[_chesscolor[pos.first*_Height + pos.second] - 1]);
			auto gray = Sprite::create(name);
			box->addChild(gray, 1, "gray");
			gray->setPosition(box->getContentSize() / 2.0f);
		}
		box->runAction(Sequence::create(DelayTime::create(0.2f), CallFunc::create([box]() {
			if (box)
				box->removeChildByName("gray");
		}), nullptr));
	}
	else
	{
		if (name.size()>0)
			tishiyu->addAnimation(0, name, false);
	}
}

//新手指引
void FillUpManage::setGuideText(Node *node)
{
	_timeDecrease = false;
	for (int i = 1; i < 7; i++)
	{
		auto text = node->getChildByName("zi")->getChildByName(cjTTFLabel::getNameByInt("Text_%d",i).c_str());
		if (i == _guidenumber)
			text->setVisible(true);
		else
			text->setVisible(false);
	}
	if (_guidenumber == 6)
	{
		auto back = dynamic_cast<ui::ImageView*>(node->getChildByName("Image_1"));
		back->setContentSize(Size(380, 380));
	}
	else
	{
		if(_guidenumber!=2)
		{
			node->setPosition(node->getPosition() + Vec2(0, 100));
		}
	}
}

void FillUpManage::guideTitle()
{
	string sst = "xinshouzhiyin";
	if (_guidenumber >6)
		sst = "zhiyinjieshu";
	auto fnt2 = LabelTTF::create(cjTTFLabel::getStringByKey(sst.c_str()), TTFPATH, 70);
	this->addChild(fnt2, 20);
	fnt2->setPosition(Vec2(WINSIZE / 2.0f) - Vec2(0, 150));
	fnt2->runAction(FadeIn::create(0.3f));
	fnt2->runAction(Sequence::create(MoveBy::create(0.3f, Vec2(0, 200)), DelayTime::create(0.2f), ScaleTo::create(0.2f, 1.2f), ScaleTo::create(0.2f, 1.0f), CCDelayTime::create(0.5f), CCFadeOut::create(0.3f),
		CallFunc::create([fnt2, this]() {
		fnt2->removeFromParent();
	}), nullptr));

	return;
}

void FillUpManage::startGuide()
{
	auto colorlay = LayerColor::create(Color4B(0, 0, 0, 255 * OPA_BEIJING), WINSIZE.width, WINSIZE.height);
	this->addChild(colorlay, 11, "guidebacklayer");
	colorlay->setPosition(WINORIGIN);

	auto layer = Layer::create();
	this->addChild(layer, 20, "guidelayer");

	auto backcsb = CSLoader::createNode("animation/guider_fillup.csb");
	layer->addChild(backcsb);
	backcsb->setPosition(WINORIGIN);

	_guidenumber = 1;
	setGuideText(backcsb);

	//跳过指引
	auto image =dynamic_cast<ui::Button*>(backcsb->getChildByName("tiaoguo"));
	image->addTouchEventListener([=](Ref *rf,ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::BEGAN)
		{
			colorlay->removeFromParent();
			ignoreGuide();
		}
	});
	//auto ignore = cjMenu::createWithImage2(ignorestr, [=](Ref *mu) {
	//	colorlay->removeFromParent();
	//	ignoreGuide();
	//});
	//layer->addChild(ignore);
	//ignore->setPosition(ignoreps);
}

void FillUpManage::endGuide()
{
	guideTitle();

	runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create([=]() {
		GameDataInstance()->_guide = false;
		GameData::getSaveData()->_fillup._guider = true;
		GameData::getInstance()->dataSave();
		auto manage = static_cast<GameScene*>(Director::getInstance()->getRunningScene()->getChildByTag(99));
		manage->restart();
	}), nullptr));
}

void FillUpManage::setGuide1()
{
	_guidenumber++;
	if (_guidenumber > 6)
	{
		if(_guidenumber==7)
			endGuide();
	}
	else
	{
		runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([this]() {
			setGameState(STATE_RUN);
		}), nullptr));

		auto colorlay = LayerColor::create(Color4B(0, 0, 0, 255 * OPA_BEIJING), WINSIZE.width, WINSIZE.height);
		this->addChild(colorlay, 9, "guidebacklayer");
		colorlay->setPosition(WINORIGIN);

		auto colorlay2=LayerColor::create(Color4B(0, 0, 0, 255 * OPA_BEIJING), WINSIZE.width, WINSIZE.height);
		_clip->addChild(colorlay2, 99, "guidebacklayer");

		if (_guidenumber == 2)
		{
			for (int i = 0; i < 2; i++)
			{
				auto box = Sprite::create(cjTTFLabel::getNameByInt("FillUp/box/box%d-01.png", 1));
				colorlay2->addChild(box);
				box->setPosition(posToposition(make_pair(1, 5 + i)));
			}

			auto hand = handNode::createHand(0.4f);
			colorlay2->addChild(hand);
			hand->setPosition(posToposition(make_pair(1, 6)));
			//hand->runAction(Sequence::create(DelayTime::create(3.0f), CallFunc::create([hand]() {
			//	hand->removeFromParent();
			//}), nullptr));
		}
		else if (_guidenumber == 3)
		{
			auto tempcsb= CSLoader::createNode("animation/background_fillup_1.csb");
			colorlay->addChild(tempcsb, 1);
			tempcsb->setPosition(WINORIGIN);
			auto tool = tempcsb->getChildByName("tool");
			tool->setVisible(false);
			auto tool_frame = tempcsb->getChildByName("tool_frame");
			tool_frame->setVisible(false);

			auto timesp = Sprite::create("FillUp/bar/bar_time.png");
			colorlay ->addChild(timesp);
			timesp->setPosition(Vec2(263, 101));
			auto timefnt = cjAtlasLabel::createWithInt(_timenumber, ATLAS_PATH11);
			colorlay->addChild(timefnt,2);
			timefnt->setPosition(Vec2(87, 100));

			auto hand = handNode::createHand(0.4f);
			colorlay->addChild(hand);
			hand->setGlobalZOrder(11);
			hand->setPosition(Vec2(263,100));
			hand->runAction(Sequence::create(DelayTime::create(3.0f), CallFunc::create([hand]() {
				hand->removeFromParent();
			}), nullptr));
		}
		else if (_guidenumber == 4 || _guidenumber == 5)
		{
			auto tempcsb = CSLoader::createNode("animation/background_fillup_1.csb");
			colorlay->addChild(tempcsb, 1);
			tempcsb->setPosition(WINORIGIN);
			auto time = tempcsb->getChildByName("time");
			time->setVisible(false);
			auto time_frame = tempcsb->getChildByName("time_frame");
			time_frame->setVisible(false);

			auto sp = this->getChildByName("prop");
			sp->setZOrder(11);

			auto hand = handNode::createHand(0.4f);
			this->addChild(hand,100,"hand");
			hand->setPosition(Vec2(240, 704));
			hand->setGlobalZOrder(11);
			hand->runAction(Sequence::create(DelayTime::create(3.0f), CallFunc::create([hand]() {
				hand->removeFromParent();
			}), nullptr));
		}
		else if (_guidenumber == 6)
		{
			auto sp = this->getChildByName("prop");
			sp->setZOrder(8);
		}

		auto layer = Layer::create();
		this->addChild(layer, 20, "guidelayer");

		auto backcsb = CSLoader::createNode("animation/guider_fillup.csb");
		layer->addChild(backcsb);
		backcsb->setPosition(WINORIGIN);

		if (_guidenumber == 2)
		{
			auto text = backcsb->getChildByName("text_jixu");
			text->setVisible(false);
		}

		setGuideText(backcsb);

		//跳过指引
		auto image = dynamic_cast<ui::Button*>(backcsb->getChildByName("tiaoguo"));
		image->addTouchEventListener([=](Ref *rf, ui::Widget::TouchEventType type) {
			if (type == ui::Widget::TouchEventType::BEGAN)
			{
				colorlay->removeFromParent();
				ignoreGuide();
			}
		});
	}
}

void FillUpManage::setGuide2()
{

}

void FillUpManage::ignoreGuide()
{
	cj::clickUMCount(24);
	_guide = false;
	_guidenumber = 0;
	GameDataInstance()->_guide = false;
	GameData::getSaveData()->_fillup._guider = true;
	GameData::getInstance()->dataSave();
	auto manage = static_cast<GameScene*>(Director::getInstance()->getRunningScene()->getChildByTag(99));
	manage->restart();
}