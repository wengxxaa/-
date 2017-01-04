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

	//΢�ŵ�¼
	void wx_Login();

	//�Ƿ���Ե�¼
	bool if_Login();

	//΢����Ϣˢ��
	void wx_updateUserInfo();

	//��ȡ�û���Ϣ
	WeChatUserInfo* getUserInfo() { return _userInfo; }

	//�ύ΢����Ϣ
	void submitWeChatData();

private:
	Login() {
		_userInfo = nullptr;
	};

	//��ǰ�û�
	WeChatUserInfo* _userInfo;

	//���а����û�
	std::vector<WeChatUserInfo*> _allInfo;
};

#endif // !LOGIN_H_

