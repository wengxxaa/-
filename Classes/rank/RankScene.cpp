#include "RankScene.h"
#include "cocostudio/CocoStudio.h"
#include "../cj/cjconfig.h"
#include "../cj/GameData.h"
#include "../gamescene/GameScene.h"
#include "../gameconfig/PayScene.h"
#include "ui/UIButton.h"
#include "NetWork.h"
#include "CallHelper.h"

const std::string _icon_url = "http://pic.to8to.com/attch/day_160218/20160218_d968438a2434b62ba59dH7q5KEzTS6OH.png";

const string modelname[3] = { "jingdian","liujiao","guodong" };

//排行榜主界面
void RankScene::rankMainPage(int type)
{
	//重新从服务器读取一次数据
	if (!RankScene::getInstance()->getUserData(101) && !RankScene::getInstance()->getUserData(102) && !RankScene::getInstance()->getUserData(103))
		NetWork::getInstance()->getRankData(2);

	cj::clickUMCount(3);

	_type = type;

	auto colorlay = LayerColor::create(Color4B(0, 0, 0, 255 * 0.95f), WINSIZE.width, WINSIZE.height);
	Director::getInstance()->getRunningScene()->addChild(colorlay, 100, "ranklay");
	colorlay->setTag(100);
	colorlay->setPosition(WINORIGIN);

	auto backcsb = CSLoader::createNode("animation/RankPage.csb");
	colorlay->addChild(backcsb,1,"backcsb");
	backcsb->setPosition(WINORIGIN);

	_time = ((24*3600)-((time(0)+8*3600) % (24 * 3600)));
	_second = 99;
	char name[100];
	sprintf(name, "%02d:%02d:%02d:%02d", _time / 3600, (_time % 3600) / 60, _time % 60, _second);
	_timeLabel = cjTTFLabel::createWithString(name, 36);
	colorlay->addChild(_timeLabel);
	_timeLabel->setColor(Color3B::WHITE);
	_timeLabel->setAnchorPoint(Vec2(0.0f, 0.5f));
	_timeLabel->setPosition(Vec2(232, 754));
	colorlay->runAction(RepeatForever::create(Sequence::createWithTwoActions(
		DelayTime::create(1 / 33.0f), CallFunc::create([=]() {
		_second -= 3;
		if (_second == 0)
		{
			_time--;
			if (_time == 0)
			{
				_time = 24 * 3600-1;
			}
		}
		else if(_second<0)
			_second = 99;
		char name[100];
		sprintf(name, "%02d:%02d:%02d:%02d", _time / 3600, (_time % 3600) / 60, _time % 60, _second);
		_timeLabel->setString(name);
	}))));

	Layer *modellayer[3];
	for (int i = 0; i < 3; i++)
	{
		modellayer[i] = Layer::create();

		colorlay->addChild(modellayer[i], 2, cjTTFLabel::getNameByInt("layer%d", i + 1));

		if (_InfoData[i].size() > 0)
		{
			rankSort(101 + i);

			ScrollView* sview = rankList(i);
			modellayer[i]->addChild(sview,1,"sview");
			sview->setPosition(Vec2(240, 275));

			auto user = getUserData(101 + i);
			if (user)
			{
				auto me = rankBanner(*user, 2,101+i);
				modellayer[i]->addChild(me,1,"ziji");
				me->setPosition(Vec2(240, 190));
			}
		}
		else
		{

			auto tip = LabelTTF::create(cjTTFLabel::getStringByKey("rankdataload").c_str(), TTF_PATH, 25);
			modellayer[i]->addChild(tip, 1, "tip");
			tip->setPosition(Vec2(240, 415));
			modellayer[i]->runAction(RepeatForever::create((ActionInterval*)Sequence::createWithTwoActions(DelayTime::create(0.2f),
				CallFunc::create([=]() {
				if (_InfoData[i].size() > 0)
				{
					rankSort(101 + i);

					auto modellay = colorlay->getChildByName(cjTTFLabel::getNameByInt("layer%d", i + 1));
					ScrollView* sview = rankList(i);
					modellay->addChild(sview, 1, "sview");
					sview->setPosition(Vec2(240, 275));

					auto user = getUserData(101 + i);
					if (user)
					{
						auto me = rankBanner(*user, 2,101+i);
						modellay->addChild(me,1,"ziji");
						me->setPosition(Vec2(240, 190));
					}

					modellay->removeChildByName("tip");

					modellay->stopAllActions();
				}
			}))));
		}
	}

	for (int i = 0; i < 3; i++)
	{
		auto button = dynamic_cast<ui::Button*>(backcsb->getChildByName(modelname[i])->getChildByName("lv"));
		button->addTouchEventListener([i, this](Ref*, ui::Widget::TouchEventType type) {
			if (type == ui::Widget::TouchEventType::BEGAN)
			{
				setRank(101 + i);
			}
		});
		auto tt = backcsb->getChildByName(modelname[i]);
		if (i == 2)
		{
			tt->setVisible(false);
		}

		//if (i == 2)
		//	tt->setVisible(false);
		//else
		//	tt->setPosition(tt->getPosition() + Vec2(40 + 50 * i,0));
	}

	setRank(type);

	auto begin = dynamic_cast<ui::Button*>(backcsb->getChildByName("Button_Start"));
	begin->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
		{
			int umid = 9;
			switch (_type)
			{
			case 101:
				umid = 5;
				GameData::getInstance()->d_Project = GAME_START;
				break;
			case 102:
				umid = 6;
				GameData::getInstance()->d_Project = GAME_ANGLE;
				break;
			case 103:
				umid = 9;
				GameData::getInstance()->d_Project = GAME_FILLUP;
				break;
			default:
				break;
			}

			cj::clickUMCount(umid);

			//进入游戏
			GameData::getInstance()->d_time = time(NULL);
			if (GameData::getInstance()->d_Project == GAME_FILLUP&&GameData::getSaveData()->_fillup._guider)
			{
				auto filluplay = Director::getInstance()->getRunningScene()->getChildByName("FILLUPPROP");
				if (filluplay)
					colorlay->removeFromParent();
				else
				{
					GameData::getInstance()->d_restart = false;
					PayScene::getInstance()->fillupProp();
				}
			}
			else
				Director::getInstance()->replaceScene(GameScene::createScene());
		}
	});

	if (GameData::getInstance()->d_restart)
		begin->setPosition(begin->getPosition() + Vec2(0, 15));

	auto cha = dynamic_cast<ui::Button*>(backcsb->getChildByName("Button_Cha"));
	cha->addTouchEventListener([=](Ref*, ui::Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
			colorlay->removeFromParent();
	});

#if(CC_TARGET_PLATFORM!=CC_PLATFORM_WIN32)
	colorlay->runAction(RepeatForever::create((ActionInterval*)Sequence::createWithTwoActions(DelayTime::create(1.0f), CallFunc::create([this]() {
		imgUpdate();
	}))));
#endif


	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event) {
		return true;
	};
	colorlay->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, colorlay);

}

//滑动框
ScrollView* RankScene::rankList(int key)
{
	auto scrollView = ScrollView::create();

	auto containerLayer = LayerColor::create(Color4B(0, 0, 0, 0));
	containerLayer->setIgnoreAnchorPointForPosition(false);
	containerLayer->setAnchorPoint(Vec2(0.5f, 0.0f));
	containerLayer->setContentSize(Size(406, 1613));

	int num = MIN(50,_InfoData[key].size());
	for (int i = 0; i < num; i++)
	{
		scrollView->runAction(Sequence::create(DelayTime::create(i / 5 * 1.0f/60), CallFunc::create([=]() {
			_userImg[key].push_back(false);
			auto node = rankBanner(_InfoData[key][i],1,101+key);
			int order = _InfoData[key][i].order;
			containerLayer->addChild(node, 1, order);
			node->setPosition(203, 80 * (num - order + 0.5f) + 3);
		}), nullptr));
	}

	_userselfImg[key] = false;


	scrollView->setContainer(containerLayer);
	scrollView->ignoreAnchorPointForPosition(false);
	scrollView->setAnchorPoint(Vec2(0.5f, 0.0f));
	scrollView->setViewSize(Size(406,330));
	scrollView->setContentSize(Size(406, 80*num+13));
	scrollView->setDirection(ScrollView::Direction::VERTICAL);
	scrollView->setTouchEnabled(true);
	scrollView->setBounceable(false);
	scrollView->setContentOffsetInDuration(Vec2(0, -(80*num+13-330)), 0.0f);
	
	return scrollView;
}

//排行banner
Node* RankScene::rankBanner(InfoData data,int type,int rank)
{
	Node *node = Node::create();
	node->setContentSize(Size(403, 81));
	node->setAnchorPoint(Vec2(0.5f, 0.5f));
	string backname;

	int key = data.order;

	if (type == 2)
	{
		backname = "animation/Rank/paihangbangzi.png";

		//if (NetWork::getInstance()->getBonusInfo(type-1)&&!NetWork::getInstance()->getBonusInfo(type-1)->liqu)
		//{
		//	auto liqu = cjMenu::createWithImage2("animation/Rank/liqu.png", [=](Ref *rf) {
		//		node->removeChildByName("liqu");
		//	});
		//	node->addChild(liqu,1,"liqu");
		//	liqu->setPosition(Vec2(335, 40));
		//}
	}
	else
	{
		if (key<4)
			backname = "animation/Rank/paihangbanghuang.png";
		else
			backname = "animation/Rank/paihangbanglan.png";
	}
	auto backsp = Sprite::create(backname);
	node->addChild(backsp);
	backsp->setPosition(node->getContentSize() / 2.0f);

	int num[3] = { 100,50,10 };
	if (key >= 1 && key <= 3)
	{
		if (data.score == 0)
		{
			auto levelsp = Sprite::create("animation/Rank/weidenglu.png");
			node->addChild(levelsp);
			levelsp->setPosition(Vec2(42, 40));
		}
		else
		{
			auto level = Sprite::create(cjTTFLabel::getNameByInt("animation/Rank/%d.png", key));
			node->addChild(level);
			level->setPosition(Vec2(42, 40));
		}

		//if (type == 2)
		//{
		//	
		//}
		//else
		//{
		//	auto reward = Sprite::create(cjTTFLabel::getNameByInt("animation/Rank/huafei%d.png", num[key - 1]));
		//	node->addChild(reward);
		//	reward->setPosition(Vec2(335, 38));
		//}
	}
	else
	{
		if (data.score == 0)
		{
			auto levelsp = Sprite::create("animation/Rank/weidenglu.png");
			node->addChild(levelsp);
			levelsp->setPosition(Vec2(42, 40));
		}
		else
		{
			string str = ATLAS_PATH7;
			if (type == 2)
				str = ATLAS_PATH8;
			auto level = cjAtlasLabel::createWithInt(key, str);
			node->addChild(level);
			level->setPosition(Vec2(42, 40));
			level->setScale(1.2f);
		}
	}

	int picorder = type;
	if (type == 1)
	{
		auto user = getUserData(rank);
		if(user&&user->order==key)
			picorder = 2;
	}
	auto tex = getIcon(picorder);
	if (data.image)
	{
		tex = data.image;
		if (key <= 50 && type == 1)
			_userImg[rank - 101][key - 1] = true;
		else if (type == 2)
			_userselfImg[rank - 101] = true;
	}

	auto touxiang = getTouXiang(tex);
	touxiang->setPosition(Vec2(105, 40));
	node->addChild(touxiang, 1, "touxiang");

	auto username = LabelTTF::create(data.name, TTF_PATH, 25);
	node->addChild(username);
	username->setAnchorPoint(Vec2(0.0f, 0.5f));
	username->setPosition(Vec2(150, 58));
	Color3B col;
	if (type == 2)
		col = Color3B(34, 129, 4);
	else if (type == 1)
	{
		if (key <= 3)
			col = Color3B(216, 132, 1);
		else
			col = Color3B(22, 117, 166);
	}
	username->setColor(col);

	auto score = cjAtlasLabel::createWithInt(data.score, ATLAS_PATH10);
	node->addChild(score);
	score->setAnchorPoint(Vec2(0.0f, 0.5f));
	score->setPosition(Vec2(150, 23));

	if (type == 1)
	{
		if (key<=3&&data.score!=0)
		{
			auto bonus = Sprite::create(cjTTFLabel::getNameByInt("animation/Rank/huafei%d.png", key));
			node->addChild(bonus);
			bonus->setPosition(Vec2(330, 40));
		}
		else
		{
			auto bonus = LabelTTF::create(data.bonus, TTF_PATH, 25);
			node->addChild(bonus);
			bonus->setPosition(Vec2(330, 40));
		}
	}
	else if (type == 2)
	{
		if (NetWork::getInstance()->getBonusInfo(rank -101)&&NetWork::getInstance()->getBonusInfo(rank -101)->liqu)
		{
			auto liqusp = cjMenu::createWithImage2("animation/Rank/liqu.png", [=](Ref *rf) {
				auto reward = NetWork::getInstance()->getBonusInfo(rank -101)->reward;
				
				if (reward.size() > 0)
				{
					GameData::getSaveData()->_diamondNumber += reward[0].second;
					PayScene::getInstance()->dailyClickTip(reward[0].second, 2);
				}

				node->removeChildByName("liqu");
				NetWork::getInstance()->setBonusInfo(rank -101, false);
				GameData::getSaveData()->_bonusTime[rank - 101] = GameDataInstance()->d_bonusTime[rank - 101];
				GameDataInstance()->dataSave();

			});
			node->addChild(liqusp,1,"liqu");
			liqusp->setPosition(Vec2(330, 40));
		}
		else
		{
			auto bonus = LabelTTF::create(data.bonus, TTF_PATH, 25);
			node->addChild(bonus);
			bonus->setPosition(Vec2(330, 40));
		}
	}

	return node;
}

Node* RankScene::rankBanner2(InfoData data, int type)
{
	Node *node = Node::create();
	node->setContentSize(Size(365, 100));
	node->setAnchorPoint(Vec2(0.5f, 0.5f));
	string backname;

	int key = data.order;

	if (type == 1)
	{
		backname = "animation/Rank/paiminglan.png";
	}
	else
	{
		backname = "animation/Rank/paiminglv.png";
	}
	auto backsp = Sprite::create(backname);
	node->addChild(backsp);
	backsp->setPosition(node->getContentSize() / 2.0f);

	if (key >= 1 && key <= 3)
	{
		auto level = Sprite::create(cjTTFLabel::getNameByInt("animation/Rank/%d.png", key));
		node->addChild(level);
		level->setPosition(Vec2(42, 50));
	}
	else
	{
		string str = ATLAS_PATH7;
		if (type == 2)
			str = ATLAS_PATH8;
		auto level = cjAtlasLabel::createWithInt(key, str);
		node->addChild(level);
        level->setScale(1.3f);
		level->setPosition(Vec2(42, 50));
	}

	auto tex = getIcon(type);
	if (data.image)
	{
		tex = data.image;
	}
	auto touxiang = getTouXiang(tex);
	node->addChild(touxiang,1,"touxiang");
	touxiang->setPosition(Vec2(115, 50));


	auto username = LabelTTF::create(data.name, TTF_PATH, 25);
	node->addChild(username);
	username->setAnchorPoint(Vec2(0.0f, 0.5f));
	username->setPosition(Vec2(180, 70));
	Color3B col;
	if (type == 2)
		col = Color3B(34, 129, 4);
	else if (type == 1)
	{
		if (key <= 3)
			col = Color3B(216, 132, 1);
		else
			col = Color3B(22, 117, 166);
	}
	username->setColor(col);

	auto score = cjAtlasLabel::createWithInt(data.score, ATLAS_PATH10);
	node->addChild(score);
	score->setAnchorPoint(Vec2(0.0f, 0.5f));
	score->setPosition(Vec2(180, 30));
	score->setScale(1.2f);

	return node;
}

void RankScene::timeUpdate(float ft)
{
	
}

//选择排行榜
void RankScene::setRank(int type)
{
	_type = type;
	auto ranklay = Director::getInstance()->getRunningScene()->getChildByName("ranklay");
	auto backcsb = ranklay->getChildByName("backcsb");
	for (int i = 0; i < RANKNUM; i++)
	{
		auto button = backcsb->getChildByName(modelname[i]);
		auto button1 = dynamic_cast<ui::Button*>(button->getChildByName("lv"));
		auto button2= dynamic_cast<ui::Button*>(button->getChildByName("huang"));
		auto color = ranklay->getChildByName(cjTTFLabel::getNameByInt("layer%d", i+1));
		if (i == (type-101))
		{
			button->setZOrder(-1);
			button1->setOpacity(0);
			button2->setOpacity(255);
			color->setVisible(true);
		}
		else
		{
			button->setZOrder(-2);
			button1->setOpacity(255);
			button2->setOpacity(0);
			color->setVisible(false);			
		}
	}
}

//增加玩家
void RankScene::addUser(InfoData &user,int type)
{
	if(type>100)
		_InfoData[type-101].push_back(user);
}

//添加假数据
void RankScene::addVirtualUser()
{
	for (int i = 0; i < 200; i++)
	{
		InfoData user;
		user.isuser = false;
		user.order = i+1;
		user.image = getIcon(1);
		char f1 = 'a' + (rand() & 20);
		char name[30];
		sprintf(name, "%c_%d%d%d%d%d", f1, rand() % 10, rand() % 10, rand() % 10, rand() % 10, rand() % 10);
		user.name = name;
		user.score = rand() % (10000);
		RankScene::getInstance()->addUser(user,101);
		user.score = rand() % (5000);
		RankScene::getInstance()->addUser(user, 102);
	}
}

//玩家分数排序
void RankScene::rankSort(int type)
{
	sortAlgorithm(_InfoData[type - 101], 0, _InfoData[type - 101].size() - 1);
	int order = 1;
	for (auto &k : _InfoData[type - 101])
	{
		k.order = order;
		++order;
	}
}

void RankScene::sortAlgorithm(std::vector<InfoData> &data,int left,int right)
{
	if (left >= right)
		return;
	int low = left;
	int high = right;
	InfoData key = data[low];
	
	while (low < high)
	{
		while (low < high&&data[high].score <= key.score)
			--high;
		data[low] = data[high];

		while (low < high&&data[low].score >= key.score)
			++low;
		data[high] = data[low];
	}
	data[low] = key;
	sortAlgorithm(data, left, low - 1);
	sortAlgorithm(data, low + 1, right);
}

//提交玩家数据
void RankScene::sumbitUserScore(int score, int topType)
{
if(!RANK)
	return;

	auto user = getUserData(topType);
	if (user)
	{
		user->score = score;

		std::thread([=]() {
			if (CallHelper::submitGameBonus(0, score, 0, 0, topType).size() > 0)
				NetWork::getInstance()->getRankData();
		}).detach();
	}

	cj::LogText("sumbitScore");
}

//设置玩家头像和名字
void RankScene::setUserInfor(std::string name, std::string url)
{
	for (int i = 0; i < RANKNUM; i++)
	{
		auto user = getUserData(101+i);
		if (user)
		{
			user->name = name;

			NetWork::getInstance()->getUserIcon(url, user->imsi,101+i);
		}
	}
}

//设置服务器用户的头像
void RankScene::setServerIcon(int type)
{
	for (auto &i:_InfoData[type-101])
	{
		if (i.urlname.length() > 0)
		{
			NetWork::getInstance()->getUserIcon(i.urlname, i.imsi,type);
		}
	}
}

//获取当前玩家
InfoData* RankScene::getUserData(int type)
{
	for (auto &i : _InfoData[type-101])
	{
		if (i.isuser)
		{
			return &i;
		}
	}

	return nullptr;
}

InfoData* RankScene::getUserData(int order, int type)
{
	if (order <= _InfoData[type - 101].size())
		return &_InfoData[type - 101][order-1];

	return nullptr;
}

InfoData* RankScene::getUserDataByImei(std::string imsi,int type)
{
	for (auto &i : _InfoData[type - 101])
	{
		if (i.imsi == imsi)
		{
			return &i;
		}
	}

	return nullptr;
}

//添加玩家数据
void RankScene::addGameUserData()
{
	//自己
	for (int i = 0; i < 2; i++)
	{
		InfoData self;
		self.image = getIcon(1);
		self.isuser = true;
		self.name = "ME";
		self.order = self.order = 1;
		if(i==0)
			self.score = GameData::getSaveData()->_start._highestscore;
		else if(i==1)
			self.score = GameData::getSaveData()->_angle._highestscore;
		addUser(self,101+i);
	}
}

//获取默认头像
Texture2D* RankScene::getIcon(int key)
{
	auto img = new Image;
	img->initWithImageFile(cjTTFLabel::getNameByInt("rank/pic%d.png", key));
	auto texture = new Texture2D();
	texture->initWithImage(img);
	img->release();
	return texture;
}

//清除排行榜数据
void RankScene::cleanInfoData(int type)
{
	_InfoData[type - 101].clear();
}

//用户头像显示更新
void RankScene::imgUpdate()
{
	for (int i = 0; i < 3; i++)
	{
		if (_InfoData[i].size() > 0)
		{
			auto layer = Director::getInstance()->getRunningScene()->getChildByName("ranklay")->getChildByName(cjTTFLabel::getNameByInt("layer%d", i + 1));
			auto sview = dynamic_cast<ScrollView*>(layer->getChildByName("sview"));
			auto contianer = sview->getContainer();
			int num = MIN(50, _InfoData[i].size());
			for (int j = 0; j < num; j++)
			{
				auto data = _InfoData[i][j];
				int order = data.order;
				if (data.image&&!_userImg[i][j])
				{
					auto node = contianer->getChildByTag(order);
					if (node)
					{
						_userImg[i][j] = true;
						auto touxiang = node->getChildByName("touxiang")->getChildByName("touxiang");
						touxiang->removeChildByName("pic");

						auto tex = data.image;						
						auto pic = Sprite::createWithTexture(tex);
						touxiang->addChild(pic, 1, "pic");
						pic->setPosition(Vec2(105, 40));
						pic->setScaleX(60.0f / pic->getContentSize().width);
						pic->setScaleY(60.0f / pic->getContentSize().height);
					}
				}
			}

			auto user = getUserData(101+i);
			if (user&&user->order > 50&&user->image&&!_userselfImg[i])
			{
				auto node = layer->getChildByName("ziji");
				if (node)
				{
					_userselfImg[i] = true;
					auto touxiang = node->getChildByName("touxiang")->getChildByName("touxiang");
					touxiang->removeChildByName("pic");

					auto tex = user->image;
					auto pic = Sprite::createWithTexture(tex);
					touxiang->addChild(pic, 1, "pic");
					pic->setPosition(Vec2(105, 40));
					pic->setScaleX(60.0f / pic->getContentSize().width);
					pic->setScaleY(60.0f / pic->getContentSize().height);
				}
			}

		}
	}
}

//得到头像
Node* RankScene::getTouXiang(Texture2D* tex)
{
	Node *node = Node::create();

	auto kuang2 = Sprite::create("rank/pic3.png");
	node->addChild(kuang2);

	auto kuang= Sprite::create("rank/pic.png");
	node->addChild(kuang, 20);

	int width = 29;
	int k = 1;

	Vec2 vec[4];
	vec[0] = Vec2(-width, -width+k);
	vec[1] = Vec2(-width, width+k);
	vec[2] = Vec2(width, width+k);
	vec[3] = Vec2(width, -width+k);

	auto tt = DrawNode::create();
	node->addChild(tt, 10);
	tt->drawSolidPoly(vec, 4, Color4F(1.0f, 1.0f, 1.0f, 0.0f));

	auto touxiang = ClippingNode::create(tt);
	node->addChild(touxiang, 10, "touxiang");
	touxiang->setInverted(false);

	auto pic = Sprite::createWithTexture(tex);
	touxiang->addChild(pic);
	pic->setScaleX(65.0f / pic->getContentSize().width);
	pic->setScaleY(65.0f / pic->getContentSize().height);
//	pic->setPosition(Vec2(0, 1));

	node->setScale(0.9f);

	return node;
}