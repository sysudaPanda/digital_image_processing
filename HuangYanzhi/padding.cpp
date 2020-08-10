#include "padding.h"


int borderInterpolate( int p, int len, int borderType )
{
    if( (unsigned)p < (unsigned)len ) // 转换为无符号类型，左边界和上边界：p一般是负数，右边界和下边界，p一般是大于len的
        ;
    else if( borderType == BORDER_REPLICATE ) // 重复类型，每次对应原图的位置是0或len-1
        p = p < 0 ? 0 : len - 1;
    else if( borderType == BORDER_REFLECT || borderType == BORDER_REFLECT_101 ) // 反射/映射
    {
        int delta = borderType == BORDER_REFLECT_101;
        if( len == 1 )
            return 0;
        do
        {
            if( p < 0 ) // 反射：左边界或101：右边界
                p = -p - 1 + delta;
            else
                p = len - 1 - (p - len) - delta;
        }
        while( (unsigned)p >= (unsigned)len );
    }
    else if( borderType == BORDER_WRAP ) // 包装
    {
        if( p < 0 )//左边界
            p -= ((p-len+1)/len)*len;
        if( p >= len )//右边界
            p %= len;
    }
    else if( borderType == BORDER_CONSTANT ) // 常量，另外处理
        p = -1;

    return p;
}

void copyMakeBorder( const uchar* src, int w, int h,
                        uchar* dst, int nw, int nh,
                        int top, int left, int cn, int borderType )
{
    int i, j, k, elemSize = 1;

    int* tab = new int [(nw - w)*cn];
    int right = nw - w - left;
    int bottom = nh - h - top;

    for( i = 0; i < left; i++ )//左边界
    {
        j = borderInterpolate(i - left, w, borderType)*cn; // 计算出原图中对应的位置
        for( k = 0; k < cn; k++ ) // 每个通道的处理
            tab[i*cn + k] = j + k;
    }

    for( i = 0; i < right; i++ )// 右边界
    {
        j = borderInterpolate(w + i, w, borderType)*cn;
        for( k = 0; k < cn; k++ )
            tab[(i+left)*cn + k] = j + k;
    }

    w *= cn;
    nw *= cn;
    left *= cn;
    right *= cn;

    uchar* dstInner = dst + nw*top + left*elemSize;

    for( i = 0; i < h; i++, dstInner += nw, src += w )// 从原图中复制数据到扩展的边界中
    {
        if( dstInner != src )
            memcpy(dstInner, src, w*elemSize);
        for( j = 0; j < left; j++ )
            dstInner[j - left] = src[tab[j]];
        for( j = 0; j < right; j++ )
            dstInner[j + w] = src[tab[j + left]];
    }

    nw *= elemSize;
    dst += nw*top;

    for( i = 0; i < top; i++ )//上边界
    {
        j = borderInterpolate(i - top, h, borderType);
        memcpy(dst + (i - top)*nw, dst + j*nw, nw); // 进行整行的复制
    }

    for( i = 0; i < bottom; i++ )//先边界
    {
        j = borderInterpolate(i + h, h, borderType);
        memcpy(dst + (i + h)*nw, dst + j*nw, nw); // 进行整行的复制
    }
    delete [] tab;
}


void copyMakeConstBorder( const uchar* src, int w, int h,
                             uchar* dst, int nw, int nh,
                             int top, int left, int cn, const uchar *value )
{
    int i, j;
    uchar* constBuf = new uchar[nw*cn]; //calloc, malloc -> free
    int right = nw - w - left;
    int bottom = nh - h - top;

    memset(dst, 0, nw*nh*cn);

    for( i = 0; i < nw; i++ )// 初始化 常量buf的值
    {
        for( j = 0; j < cn; j++ )
            constBuf[i*cn + j] = value[j];
    }

    w *= cn;
    nw *= cn;
    left *= cn;
    right *= cn;

    uchar* dstInner = dst + nw*top + left;
    for( i = 0; i < h; i++, dstInner += nw, src += w )// 复制原图数据和扩展左右边界
    {
        memcpy( dstInner, src, w );
        memcpy( dstInner - left, constBuf, left );
        memcpy( dstInner + w, constBuf, right );
    }

    //p += nw*top;
    dst += nw*top;
    for( i = 0; i < top; i++ )
        memcpy(dst + (i - top)*nw, constBuf, nw);//扩展上边界

    for( i = 0; i < bottom; i++ )
        memcpy(dst + (i + h)*nw, constBuf, nw);//扩展下边界

    delete [] constBuf;
}

void copyMakeBorder(uchar *src, int w, int h, int cn, int top, int bottom,
                    int left, int right, BorderType borderType, const uchar *value, uchar *dst)
{
    int nw = w+left+right;
    int nh = h+top+bottom;

    if(top == 0 && left == 0 && bottom == 0 && right == 0)
    {
        memcpy(dst, src, nw*nh*cn);
        return;
    }

    if( borderType != BORDER_CONSTANT )
        copyMakeBorder( src, w, h, dst, nw, nh,
                       top, left, cn, borderType );
    else
        copyMakeConstBorder( src, w, h, dst, nw, nh,
                            top, left, cn, value );
}

void copyRemoveBorder(const uchar *src, int w, int h, int cn, int top, int bottom,
                    int left, int right,  uchar *dst)
{
    int nw = w-left-right;
    int nh = h-top-bottom;

    w *= cn;
    nw *= cn;
    left *= cn;

    for (int i=0; i<nh; i++)
    {
        memcpy(dst+i*nw, src+(i+top)*w+left, nw);
    }
}
