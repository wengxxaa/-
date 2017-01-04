#ifndef LOGIN_H_
#define LOGIN_H_

#include"vigame_social.h"
using namespace vigame::social;

class Login
{
public:
	static Login* getInstance()
	{
		static Login login;
		return &login;
	}

	//微信登录
	void wx_Login();

	//是否可以登录
	bool if_Login();

	//微信信息刷新
	void wx_updateUserInfo();

	//获取用户信息
	WeChatUserInfo* getUserInfo() { return _userInfo; }

	//提交微信信息
	void submitWeChatData();

private:
	Login() {
		_userInfo = nullptr;
	};

	//当前用户
	WeChatUserInfo* _userInfo;

	//排行榜总用户
	std::vector<WeChatUserInfo*> _allInfo;
};

#endif // !LOGIN_H_

