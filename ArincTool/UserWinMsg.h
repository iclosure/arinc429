
#ifndef USER_WINDOWS_MESSAGE_H
#define USER_WINDOWS_MESSAGE_H

#define WM_USER0				(WM_USER  + 001)	// ��Ϊһ���Ի����ڲ���Ϣ��	// (WM_USER + 001) ~ (WM_USER + 200)
#define WM_USER1				(WM_USER0 + 201)	// ��Ϊ������Ϣ��				// (WM_USER + 201) ~ (WM_USER + 400)
#define WM_USER2				(WM_USER1 + 201)	// ��Ϊָ��Ӧ����Ϣ��			// (WM_USER + 401) ~ (WM_USER + 600)

// �Զ�����̿ո����Ϣ
#define WM_KEY_SPACE			(WM_USER1 + 0)	// SendMessage()��ʽ
// �Զ�����Ϣ���ı���ָ�Ӧ����Ϣ��ʾ��Ϣ
#define WM_USER_CLEAN_TXT		(WM_USER1 + 1)	// ����༭���ı�

// 
#define WM_USER_PER_SWITCH		(WM_USER1 + 2)	// 
#define WM_USER_DELETE_SUBDLG	(WM_USER1 + 3)	// 
#define WM_USER_DELETE_ARAPI	(WM_USER1 + 4)	// 

#endif  // USER_WINDOWS_MESSAGE_H
