
#ifndef USER_WINDOWS_MESSAGE_H
#define USER_WINDOWS_MESSAGE_H

#define WM_USER0				(WM_USER  + 001)	// 作为一个对话框内部消息用	// (WM_USER + 001) ~ (WM_USER + 200)
#define WM_USER1				(WM_USER0 + 201)	// 作为公共消息用				// (WM_USER + 201) ~ (WM_USER + 400)
#define WM_USER2				(WM_USER1 + 201)	// 作为指令应答消息用			// (WM_USER + 401) ~ (WM_USER + 600)

// 自定义键盘空格键消息
#define WM_KEY_SPACE			(WM_USER1 + 0)	// SendMessage()方式
// 自定义消息：文本、指令、应答信息显示消息
#define WM_USER_CLEAN_TXT		(WM_USER1 + 1)	// 清除编辑框文本

// 
#define WM_USER_PER_SWITCH		(WM_USER1 + 2)	// 
#define WM_USER_DELETE_SUBDLG	(WM_USER1 + 3)	// 
#define WM_USER_DELETE_ARAPI	(WM_USER1 + 4)	// 

#endif  // USER_WINDOWS_MESSAGE_H
