#ifndef MIGUYINYUE_H
#define MIGUYINYUE_H
#include "../rizhi/httpreq.h"
#include "../baikePub/baseop.h"
#include <QMutexLocker>

enum LianzaiZhuangtai{
    lzLianzaizhong=0,//!<连载中
    lzZanting=1,     //!<暂停更新
    lzYiwanjie=2,    //!<完结
    lzMeiyou       //!<没有找到
};

enum TaolunDengji{
    tdJiandan,
    tdPutong
};

/// 书籍信息栏填写。
/// xuyao*** 此项没填，需要数据。默认都需要，如果从词条现有页面数据中找到的，改为不需要。
/// *** 不空则可以填写。
class ShujiXinxilan{
public:
    explicit ShujiXinxilan():xuyaoShuming(true),
        xuyaoBieming(true),
        xuyaoZuozhe(true),
        xuyaoLeibie(true),
        xuyaoYuanzuopin(true),
        xuyaoYizhe(true),
        xuyaoChubanshe(true),
        xuyaoChubanShijian(true),
        xuyaoYeshu(true),
        xuyaoDingjia(true),
        xuyaoKaiben(true),
        xuyaoZhuangzheng(true),
        xuyaoISBN(true)
    {}
    bool xuyaoShuming;//书名
    bool xuyaoBieming;//别名
    bool xuyaoZuozhe;//作者
    bool xuyaoLeibie;//类别
    bool xuyaoYuanzuopin;//原作品
    bool xuyaoYizhe;//译者
    bool xuyaoChubanshe;//出版社
    bool xuyaoChubanShijian;//出版时间
    bool xuyaoYeshu;//页数
    bool xuyaoDingjia;//定价
    bool xuyaoKaiben;//开本
    bool xuyaoZhuangzheng;//装帧
    bool xuyaoISBN;//ISBN
    QString shuming;//书名
    QString bieming;//别名
    QString zuozhe;//作者
    QString liebie;//类别
    QString yuanzuopin;//原作品
    QString yizhe;//译者
    QString chubanshe;//出版社
    QString chubanShijian;//出版时间
    QString yeshu;//页数
    QString dingjia;//定价数字
    QString dingjiaBeizhu;//定价备注
    QString dingjiaDanwei;//美元，日元，元
    QString kaiben;//开本
    QString zhuangzheng;//装帧
    QString iSBN;//ISBN
};

class Chaxun: public QObject
{
    Q_OBJECT
public:
    explicit Chaxun(QObject *parent = 0):QObject(parent),
        bop(nullptr),xuyaoShuminghao(false),
        duoyuShuminghao(false),
        xuyaoJuhao(false),
        xinxilanXuyaoZuozhe(false),
        xinxilanXuyaoPingtai(false),
        xinxilanXuyaoLeixing(false),
        xinxilanXuyaoZhuangtai(false),
        xinxilanXuyaoZhongwenming(false),
        pingtaiShiBaidu(false),
        youGaishutu(false)
    {
    }
    ~Chaxun(){}
    void setBop(Baseop *vbop);
    Baseop *bop;
    //
    QString zz;
    QString pingtai;
    QString sm;
    bool gaishuXinxilan();
    bool xuyaoShuminghao;
    bool duoyuShuminghao;
    bool xuyaoJuhao;
    bool xinxilanXuyaoZuozhe;
    bool xinxilanXuyaoPingtai;
    bool xinxilanXuyaoLeixing;
    bool xinxilanXuyaoZhuangtai;
    bool xinxilanXuyaoZhongwenming;
    bool pingtaiShiBaidu;
    bool youGaishutu;
    void reset();
    void zidongHuatu(const QString &sm, QString &zz);
    //
    QNetworkAccessManager nam;
    void guanbitaolun(TaolunDengji td);
    void fenxiJiguan(const QString &webData);
    /// 取消一级目录或二级目录，改成加粗。
    void baseAjJiacu(bool isYijimulu);
};

#endif // MIGUYINYUE_H
