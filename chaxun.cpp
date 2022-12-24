#include "chaxun.h"
#include <iostream>
#include <QFile>
#include <urlmon.h>
#include <QImage>
#include <QPainter>
#include <QThreadPool>
#include "../rizhi/sslhttp.h"

void Chaxun::setBop(Baseop *vbop)
{
    bop=vbop;
}

void Chaxun::reset(){
    xuyaoShuminghao=false;
    duoyuShuminghao=false;
    xuyaoJuhao=false;
    xinxilanXuyaoZuozhe=false;
    xinxilanXuyaoPingtai=false;
    xinxilanXuyaoLeixing=false;
    xinxilanXuyaoZhuangtai=false;
    xinxilanXuyaoZhongwenming=false;
    pingtaiShiBaidu=false;
    youGaishutu=false;
}

void Chaxun::baseAjJiacu(bool isYijimulu){
    UINT sjc[][2][3]={
        //半屏
        {},
        //满屏
        {
            //一级目录x 二级目录x   y
            {678,          739,  132},//高屏幕
            {889,          492,  747},
        }
    };
    POINT cp;
    GetCursorPos(&cp);
    dian(isYijimulu?0:1,2);
    Sleep(50);
    bop->ShiftJia(VK_END);
    bop->CtrlJia('B');
    bop->MoveTo(cp.x,cp.y);
}

void Chaxun::fenxiJiguan(const QString& webData){
    UINT sj[][2]={
        //结束
        {678, 360},//高屏幕
        {678, 360},
    };
    //snd<<"111";
    //snd<<webData;
    //图片网址
    //https://dss2.baidu.com/6ONYsjip0QIZ8tyhnq/it/u=786219334,2744497180&fm=83&app=66&f=JPEG?w=267&h=356&s=E57411C757AAB4FB4A44D99D0300C001
    QRegularExpression reg("J-lemma-title[\\s\\S]*?<h1 ?>(.*?)</h1>");
    auto ix=reg.match(webData);
    if(!ix.hasMatch()){
        snd<<QString("词条名称没有找到");
        return;
    }
    QString renming=ix.captured(1);
    QString jiguan;
    reg.setPattern("lemma-desc\">(.*?)籍.*?烈士</div>");
    ix=reg.match(webData);
    if(ix.hasMatch()){
        jiguan=ix.captured(1);
    }
    if(jiguan==""){
        reg.setPattern("牺牲(?:情况|时间|战役地点)");
        ix=reg.match(webData);
        if(ix.hasMatch()){//是烈士
            reg.setPattern(">籍贯：(.*?)<");
            ix=reg.match(webData);
            if(ix.hasMatch()){
                jiguan=ix.captured(1);
            }
        }
    }

    if(jiguan!=""){//找到籍贯了
        bop->LeftClick(sj[bop->pingmu][0], sj[bop->pingmu][1]);
        bop->SetCLB(QString("%1，%2籍烈士。").arg(renming).arg(jiguan));
        bop->CtrlJia('V');
    }
}

/// 关闭讨论 反馈属实，修改完成 等级是简单
void Chaxun::guanbitaolun(TaolunDengji td){
    UINT sjc[][2][11]={
        //半屏
        {},
        //满屏
        {
            //结束         确认23   已修改45   结束语67   简单89   普通y10
            {1043, 210,   932,766, 796,414,  861,469, 871,690, 645  },//高屏幕
            {889, 492,   747,147, 807,214, 938,314},
        }
    };
    COLORREF ys[][2]={//确认颜色
                      {0xFAC8B6},//高屏幕
                      {0x1B1B1B}
                     };

    //点击结束
    dian(0,1);
    //等待确认出来
    while(!yanse(2,3, ys[bop->pingmu][0])){
        Sleep(300);
    }
    //点击已修改
    dian(4,5);
    //点击结束语
    dian(6,7);
    //填写
    bop->SetCLB("反馈属实，词条已修改完成");
    bop->CtrlJia('V');
    Sleep(100);
    if(td==tdJiandan){
        dian(8,9);//点击简单
    }else{
        dian(8,10);//点击普通
    }
    //点击确认
    dian(2,3);
}

bool Chaxun::gaishuXinxilan(){
    UINT sjc[][20]={
        //搜索      替换2,3        查找输入框45     替换输入框x6  关搜索x7
        {679,132,  611,177,       225,179,       471,        678,
         //  概述空白 89   作者10 11     平台12 13
         293,439,     322,655,     636,587},
        {638,340,  920,86,        243,178,   323,451,       619,356,       70,147,
         41,327,        938,85,                35,231}
    };

    int pingmu=bop->pingmu;

    if(xuyaoShuminghao){
        bop->LeftClick(sjc[pingmu][0],sjc[pingmu][1]);//搜索
        //等待“替换”出来
        while(!bop->Color(sjc[pingmu][2],sjc[pingmu][3],0xF5F5F5)){
            Sleep(100);
        }
        Sleep(100);
        //点击输入查找的内容：书名
        bop->LeftClick(sjc[pingmu][4],sjc[pingmu][5]);
        Sleep(100);
        bop->SetCLB(sm);
        bop->CtrlJia('V');
        //点击输入替换的内容：《书名》
        bop->LeftClick(sjc[pingmu][6],sjc[pingmu][5]);
        Sleep(100);
        bop->SetCLB(QString("《%1》").arg(sm));
        bop->CtrlJia('V');
        //点击替换
        bop->LeftClick(sjc[pingmu][2],sjc[pingmu][3]);
        Sleep(100);
        //点击关闭
        bop->LeftClick(sjc[pingmu][7],sjc[pingmu][3]);
        Sleep(50);
    }
    if(duoyuShuminghao){
        bop->LeftClick(sjc[pingmu][0],sjc[pingmu][1]);//搜索
        //等待“替换”出来
        while(!bop->Color(sjc[pingmu][2],sjc[pingmu][3],0xF5F5F5)){
            Sleep(100);
        }
        Sleep(100);
        //点击输入查找的内容：《》
        bop->LeftClick(sjc[pingmu][4],sjc[pingmu][5]);
        Sleep(100);
        bop->SetCLB("《》");
        bop->CtrlJia('V');
        //点击替换--删除
        bop->LeftClick(sjc[pingmu][2],sjc[pingmu][3]);
        Sleep(100);
        //点击关闭
        bop->LeftClick(sjc[pingmu][7],sjc[pingmu][3]);
        Sleep(50);
    }
    if(xuyaoJuhao){
        //点击概述部分底部空白
        bop->LeftClick(sjc[pingmu][8],sjc[pingmu][9]);
        Sleep(50);
        bop->KeyPress(VK_END);
        bop->SetCLB("。");
        bop->CtrlJia('V');
    }
    if(xinxilanXuyaoZuozhe){
        //点击信息栏 作者
        bop->LeftClick(sjc[pingmu][10],sjc[pingmu][11]);
        Sleep(50);
        bop->SetCLB(zz);
        bop->CtrlJia('V');
    }
    if(xinxilanXuyaoPingtai){
        if(!pingtaiShiBaidu){
            snd<<QString("连载平台不是百度小说");
            return false;
        }else{
            bop->LeftClick(sjc[pingmu][12],sjc[pingmu][13]);
            bop->CtrlJia('A');
            Sleep(50);
            bop->SetCLB("百度小说");
            bop->CtrlJia('V');
        }
    }
    if(xinxilanXuyaoLeixing){
        snd<<QString("补充类型");
        return false;
    }
    if(xinxilanXuyaoZhongwenming){
        snd<<QString("补充中文名");
        return false;
    }
    return true;
}

