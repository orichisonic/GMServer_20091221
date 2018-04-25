#ifndef PALLOGINFO_H_
#define PALLOGINFO_H_

#include "OperatorCG2.h"

class CCG2Info
{
private:
	SOCKET m_hServerSocket;

	//用于保存完整数据库执行操作的sql语句
	char operszSql[2048];

	//用于保存在远程数据库执行操作的sql语句
	char operRemoteSql[2048];

	//用于保存执行操作的用户名
	char m_UserName[128];

	//记录日志信息
	char strLog[2048];

	//根据服务器名称获取需要的IP
	char logdbip[20];

	//服务器名称
	char cg2_serveripname[40];

	//服务器IP
	char cg2_serverip[40];
	
	//昵称
	char cg2_nickname[40];
	
	//帐号
	char cg2_username[40];

	//玩家ID
	char cg2_usersn[40];

	//公会
	char cg2_guildname[40];

	//公会Id
	int cg2_guildid;

	//用户角色号
	int cg2_charno;

	//保存页索引
	int index;

	//保存页面大小
	int iPageSize;

	//操作者
	int userByID;

	//文件位置
	char filepath[256];

	//当前时间
	char curTime[128];
	
	//返回消息
	char RetMessage[256];

	//读取配置文件所返回的内容
	char retMessage[10][256];

private:

	//用于保存查询数据库返回的结果
	COperVector DBVect;

	//用于保存查询数据库返回的结果2
	COperVector DBVect1;

	//用于保存解析客户端数据得到的结果
	COperVector recvVect;

	//用于写日志信息
	CLogFile logFile;

	//用于中间的辅助操作
	COperatorCG2 operAssist;

public:

	//构造函数
	CCG2Info();

	//析构函数
	~CCG2Info();

	//初始化类中的变量
	bool initialize(CSession * pSess,SOCKET m_Socket, unsigned char * pbuf, int length,CEnumCore::Message_Tag_ID m_message_tag_id);

	//删除类中的变量
	bool Destroy();

	//角色基本资料
	bool CG2_Account_Query();


	//角色详细资料
	bool CG2_Account_Detail_Query();

	//角色属性资料
	bool CG2_Account_Attribute_Query();


	//角色公会资料
	bool CG2_Account_Guild_Query();

	//角色称号资料
	bool CG2_Account_Title_Query();

	//角色社會關係
	bool CG2_Account_Address_Query();

	//角色技能資訊
	bool CG2_Account_Skill_Query();

	//师生关系查询
	bool CG2_Account_Teach_Query();

	//日志查询
	bool CG2_Log_Query();

	//查询职业种类
	bool CG2_Job_Query();

	//查询技能类别
	bool CG2_GetSkill_Query();

	//查询技能名称
	bool CG2_SkillList_Query();

	//模糊查询道具
	bool CG2_SearchItem_Query();

	//查询道具类别
	bool CG2_Get_Item_Query();

	//道具名称查询
	bool CG2_Get_Item_Detail_Query();

	//添加银行道具
	bool CG2_InsertBankItem_Query();

	//批量银行道具添加（导入）
	bool CG2_InsertBankItemALL_Query();
		
	//添加道具
	bool CG2_Insert_Item_Query();

	//修改玩家角色职业
	bool CG2_UpdateJob_Query();

	//修改技能等级
	bool CG2_Update_Account_Skill_Query();

	//添加角色技能
	bool CG2_Insert_Account_Skill_Query();
	
	//帐号登陆登出记录
	bool CG2_Use_Query();

	//强制玩家下线
	bool CG2_KickAccount_Query();

	//强制玩家下线(没有回答)
	bool CG2_KickAccount_Query_NoAnswer();

	//查询所有封停帐号信息
	bool CG2_BanAccount_Query();
	
	//查询特定帐号有没有被封停
	bool CG2_Account_BANISHMENT_QUERY();

	//封停玩家帐号
	bool CG2_Account_Close();

	//解封玩家帐号
	bool CG2_Account_Open();


	//查看公告
	bool CG2_SearchNote_Query();


	//发送公告
	bool CG2_SendNote_Query();


	//停止发送公告
	bool CG2_DeleteNote_Query();

	//查询备份玩家角色信息
	bool CG2_BakAccount_Query();


	//恢复角色操作
	bool CG2_Recovery_Account_Qurey();

	//玩家爱情树
	bool CG2_LoveTree_Query();

	//玩家婚姻情况
	bool CG2_Marriage_Info_Query();

	//公会信息
	bool CG2_SearchGuild_Query();

	//公会成员信息(并库)
	bool CG2_SearchGuild_Detail_Query();

	//解散公会
	bool CG2_CloseGuild_Query();
		
	//修改公会名字
	bool CG2_RenameGuild_Query();

	//公会等级修改
	bool CG2_UpdateGuildLv_Query();

	//公会荣誉修改
	bool CG2_UpdateGuildHonor_Query();

	//查询背包道具信息
	bool CG2_PackItem_Query();

	//删除角色道具栏道具
	bool CG2_BagItem_Del_Query();	

	//角色临时背包信息
	bool CG2_TempItem_Query();

	//删除临时道具栏道具
	bool CG2_TmpItem_Del_Query();

	//身上装备信息
	bool CG2_CharItem_Query();

	//删除随身装备道具
	bool CG2_EquipItem_Del_Query();
	
	//收费道具查询
	bool CG2_ShopItem_Query();

	//宠物基本资料
	bool CG2_PetInfo_Query();

	//宠物背包信息
	bool CG2_PetItem_Query();

	//宠物技能信息
	bool CG2_PetSkill_Query();

	//银行存款信息
	bool CG2_BankInfo_Query();

	//设置银行金钱
	bool CG2_SetBankMoney_Query();
	
	//银行物品信息
	bool CG2_BankItem_Query();

	//删除银行道具
	bool CG2_BankItem_Del_Query();

	//银行宠物信息
	bool CG2_BankPet_Query();

	//修改宠物属性
	bool CG2_Update_Pet_Query();

	//添加宠物
	bool CG2_AddPet_Query();

	//修改宠物等级
	bool CG2_UpdatePetLevel_Query();

	//宠物图鉴信息
	bool CG2_PetPic_Query();

	//宠物列表
	bool CG2_PetList_Query();

	//添加宠物技能
	bool CG2_Insert_Pet_Skill_Query();

	//添加宠物技能
	bool CG2_Update_Pet_Skill_Query();

	//设置/删除宠物图鉴
	bool CG2_PetPic_Del_Query();

	//添加临时密码
	bool CG2_Insert_TmpPwd_Query();

	//查看临时密码
	bool CG2_SearchPassWord_Query();

	//恢复临时密码
	bool CG2_Recover_TmpPwd_Query();

	//角色换位
	bool CG2_ChangeCharNo_Query();

	//移动角色坐标
	bool CG2_ChangeMap_Query();

	//查询地图列表
	bool CG2_MapList_Query();

	//模糊查询地图
	bool CG2_SearchMap_Query();

	//发送问答信息
	bool CG2_SendAsk_Query();

	//商城信息查询
	bool CG2_UserShoppingInfo_Query();

	//发送道具密码锁
	bool CG2_Send_ItemPassword();

	//等到Email地址
	bool CG2_Get_EmailAddress();

	////账号激活查询
	bool CG2_Account_Active_Query();

	//通过激活码查询
	bool CG2_Sn_Active_Query();


	int CG2_GMInsert_Query(char *ServerIP, char *UserName,char * UserNick,char * strEndTime,char * Reason,char * memo,int iFunc);


};
#endif