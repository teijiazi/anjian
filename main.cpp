#include "../baikePub/shouci.h"
#include "chaxun.h"
#include <winver.h>
#include <windows.h>
#include "../baikePub/BaikeTypeData.h"
using namespace std;

#include <QApplication>

///
/// 写点说明，时间长了就忘了。
/// 全自动，必须F2启动：在任务列表页面，鼠标放在词条上，按F2启动
/// 已经打开的，F1启动，填完以后，还得返回到F2启动，才能全自动。
/// 原理：
/// F2启动，记录鼠标的位置，以便自动移动到下一个词条上。
///

//void shifang()
//{
//    for(mvk& mv:modvk)
//    {
//        bool jg=UnregisterHotKey(NULL, mv.at);
//        if(!jg){
//            snd<<"unreg err="<<GetLastError()<<",id="<<mv.at<<",key="<<mv.idstr;
//        }
//    }
//}
BOOL WINAPI ConsoleCtrlhandler(DWORD dwCtrlType)
{
    //控制台消息循环抓不住wm_close wm_quit wm_destroy
    //需要使用SetConsoleCtrlHandler(ConsoleCtrlhandler, TRUE);在退出前释放注册的hotkey
    //经使用发现hotkey在程序结束时自动失效，不必释放。
    //如果提供更改hotkey的功能则可以使用释放
    if(dwCtrlType == CTRL_CLOSE_EVENT)
    {
        snd<<"closeEvent";
        HWND hwndGuanbi = FindWindowW(NULL, L"关闭");
        if(hwndGuanbi!=NULL){
            DWORD tid=GetWindowThreadProcessId(hwndGuanbi,NULL);
            PostThreadMessageW(tid,WM_USER+msgCloseGbid,0,0);
        }
        //shifang();
    }
    return TRUE;
}

int main(int argc, char *argv[])
{
    UINT sjc[][2][1]={
        //半屏幕
        {
            //放弃y
            {7},
            {7}
        },
        //满屏幕
        {
            //---------------高100%
            //放弃y
            {753},
            //---------------低90%
            {682}

        }
    };
    QApplication a(argc, argv);

    Q_UNUSED(argc);
    QThreadPool::globalInstance()->setMaxThreadCount(2);

    Baseop bop;

    int tijiao2=bop.vkAdd("tijiao2",VK_F9);
    int quxiao1=bop.vkAdd("quxiao1",VK_F7);
    int quxiao2=bop.vkAdd("quxiao2",VK_F8);

    bool isRebootself=bop.init(L"辅助按键--F6强制关闭",pzManping,false,
             argv[0],
            QString("\r\n1 Edge浏览器，页面100%显示"
                                     "\r\n\r\n2 临时按键"),
            QString("\r\n1 Edge浏览器，页面90%显示"
                                     "\r\n\r\n2 临时按键"));
    bop.renwuZuobiao(sjc[bop.banping][bop.pingmu][0]);
    Shouci sc(&bop);
    Chaxun dydata;
    dydata.setBop(&bop);
    //
    int xuanze=0;
    int lianxu=1;
    int gtl=0;
    TaolunDengji taolunDengji=tdJiandan;
    sow<<QString("按键F1、F9表示提交，用于：");
    sow<<QString("0 更正错误");
    sow<<QString("1 修改图片");
    sow<<QString("2 修改概述");
    sow<<QString("3 修改目录");
    sow<<QString("请输入：");
    cin>>xuanze;
    sow<<QString("提交以后是否连续处理");
    sow<<QString("1 连续处理");
    sow<<QString("2 不连续处理");
    sow<<QString("请输入：");
    cin>>lianxu;
    sow<<QString("按键F2的作用是：");
    sow<<QString("0 关闭讨论（修改完成，等级简单，F3切换）");
    sow<<QString("1 启动连续的词条编辑");
    sow<<QString("请输入：");
    cin>>gtl;
    sow<<QString("你输入的是：%1,%2,%3").arg(xuanze).arg(lianxu).arg(gtl);
    SetConsoleCtrlHandler(ConsoleCtrlhandler, TRUE);
    MSG msg;
    PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
    while (GetMessage(&msg, NULL, 0, 0))
    {
        switch(msg.message)
        {
        //case WM_DESTROYCLIPBOARD://控制台窗口收不到这个消息
        //    sow<<"delclip";
        //    break;
        case WM_USER+msgHotkey:
            bop.zhucerejian();
            if(isRebootself){
                isRebootself=false;
                sow<<"cq rcv";
                bool jg=bop.xiayige();
                if(jg){
                    sc.runscript();
                }else{
                    sow<<"chognqi error";
                }
            }
            break;
        case WM_HOTKEY:
        {
            UINT ctrlalt=LOWORD(msg.lParam);
            UINT anjian=HIWORD(msg.lParam);
            if(ajjc(vkiBtnleft)){
                bop.LeftClick();
            }else if(ajjc(vkiTijiao)){
                bop.tijiao(tijiaoFenlei(xuanze),lianxu==1);//修改
            }else if(ajjc(vkiLianxu)){
                if(gtl==0){
                    dydata.guanbitaolun(taolunDengji);
                }else if(gtl==1){
                    sc.runscript();//记录鼠标位置并打开词条
                    //dydata.xiugaiMuluXinxilan();//开始填表
                }
            }else if(ajjc(vkiFangqi)){
                bop.fangqi();//自动找到下一个词条，打开。
            }else if(ajjc(vkiTianbiao)){
                if(gtl==0){
                    if(taolunDengji==tdJiandan){
                        taolunDengji=tdPutong;
                    }else{
                        taolunDengji=tdJiandan;
                    }
                    sow<<QString("关闭等级修改为：%1").arg(taolunDengji==0?"简单":"普通");
                }
                //dydata.xiugaiMuluXinxilan();
                //-----------------------------------------------------------------
            }else if(ajjc(tijiao2)){
                bop.tijiao(tijiaoFenlei(xuanze),lianxu==1);
            }else if(ajjc(quxiao1)){
                dydata.baseAjJiacu(true);
            }else if(ajjc(quxiao2)){
                dydata.baseAjJiacu(false);
            }
        }
            break;
        default:
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return a.exec();
}
