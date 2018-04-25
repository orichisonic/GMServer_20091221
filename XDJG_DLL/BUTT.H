//参数表示	ip 发送地址， lpBoardMessaeg 发送内容， port 发送端口
//返回值表示 0 发送失败  1 发送成功
extern "C" __declspec(dllexport) int XDJG_SendBulletin(char *ip, char *lpBoardMessaeg, int port);