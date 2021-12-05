#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../header.h"

float fadd(float xf, float yf){
    union {
        int i;
        float f;
    } x;
    union {
        int i;
        float f;
    } y;
    union {
        int i;
        float f;
    } z;

    x.f = xf;
    y.f = yf;

    int s1 = (x.i >> 31) & 0b1;
    int s2 = (y.i >> 31) & 0b1;
    int e1 = (x.i >> 23) & 0xff;
    int e2 = (y.i >> 23) & 0xff;
    int m1 = x.i & 0x7fffff;
    int m2 = y.i & 0x7fffff;
    int f1,f2;
    if (e1 == 0){
        f1 = 1;
    }else{
        f1 = 0;
    }
    if (e2 == 0){
        f2 = 1;
    }else{
        f2 = 0;
    }
    unsigned long long m1a,m2a;
    m1a = (1 << 23) + m1;
    m2a = (1 << 23) + m2;
    int f;
    if ((x.i & 0x7fffffff) > (y.i & 0x7fffffff)){
        f = 1;
    }else{
        f = 0;
    }
    unsigned int de;
    if (e1 > e2){
        de = (e1 - e2) & 0xff;
    }else{
        de = (e2 - e1) & 0xff;
    }
    unsigned long long m1b,m1c,m2b,m2c;
    int t1,t2;
    if (f1){
        m1b = 0;
    }else{
        m1b = (m1a << 31) & 0xffffffffffffff;
    }
    if (de < 64){
        m1c = (m1b >> de) & 0xffffffffffffff;
    }else{
        m1c = 0;
    }
    if ((m1c & 0x1fffffff) == 0){
        t1 = 0;
    }else{
        t1 = 1;
    }
    if (f2){
        m2b = 0;
    }else{
        m2b = (m2a << 31) & 0xffffffffffffff;
    }
    if (de < 64){                         
        m2c = (m2b >> de) & 0xffffffffffffff;
    }else{
        m2c = 0; 
    }
    if ((m2c & 0x1fffffff) == 0){
        t2 = 0;
    }else{
        t2 = 1;
    }
    unsigned long long mia;
    unsigned int ms,tstck,es,ss,fs;
    if (f){
        ms = m1a;
        mia = (m2c >> 29) & 0x7ffffff;
        tstck = t2;
        es = e1;
        ss = s1;
    }else{
        ms = m2a;
        mia = (m1c >> 29) & 0x7ffffff;
        tstck = t1;
        es = e2;
        ss = s2;
    }
    if (s1 == s2){
        fs = 1;
    }else{
        fs = 0;
    }

    unsigned int mye,esi,fc,se,myf,gb,eyd;
    if (fs){
        mye = ((ms << 3) + (mia << 1) + tstck) & 0xfffffff; 
    }else{
        mye = ((ms << 3) - ((mia << 1) + tstck)) & 0xfffffff;;
    }
    
    esi = (es + 1) & 0xff;

    unsigned int myd = (mye >> 2) & 0x3ffffff;
    if ((myd >> 25) & 0b1){
        se = 0;
    }else if ((myd >> 24) & 0b1){
        se = 1;
    }else if ((myd >> 23) & 0b1){
        se = 2;
    }else if ((myd >> 22) & 0b1){
        se = 3;
    }else if ((myd >> 21) & 0b1){
        se = 4;
    }else if ((myd >> 20) & 0b1){
        se = 5;
    }else if ((myd >> 19) & 0b1){
        se = 6;
    }else if ((myd >> 18) & 0b1){
        se = 7;
    }else if ((myd >> 17) & 0b1){
        se = 8;
    }else if ((myd >> 16) & 0b1){
        se = 9;
    }else if ((myd >> 15) & 0b1){
        se = 10;
    }else if ((myd >> 14) & 0b1){
        se = 11;
    }else if ((myd >> 13) & 0b1){
        se = 12;
    }else if ((myd >> 12) & 0b1){
        se = 13;
    }else if ((myd >> 11) & 0b1){
        se = 14;
    }else if ((myd >> 10) & 0b1){
        se = 15;
    }else if ((myd >> 9) & 0b1){
        se = 16;
    }else if ((myd >> 8) & 0b1){
        se = 17;
    }else if ((myd >> 7) & 0b1){
        se = 18;
    }else if ((myd >> 6) & 0b1){
        se = 19;
    }else if ((myd >> 5) & 0b1){
        se = 20;
    }else if ((myd >> 4) & 0b1){
        se = 21;
    }else if ((myd >> 3) & 0b1){
        se = 22;
    }else if ((myd >> 2) & 0b1){
        se = 23;
    }else if ((myd >> 1) & 0b1){
        se = 24;
    }else if ((myd >> 0) & 0b1){
        se = 25;
    }else{
        se = 26;
    }
    if ((((mye >> 2) & 0xffffff) == 0xffffff) && (((mye >> 27) & 0b1) == 0) && ((mye >> 26) & 0b1 || (mye >> 1) & 0b1)){
        fc = 1;
    }else{
        fc = 0;
    }
    myf = mye << (se & 0b11);
    unsigned int se_in = se & 0b11;
    unsigned int mye_in = mye & 0b11111;
    if (se_in == 0b00){
        if (((mye_in >> 3) & 0b1) && (((mye_in >> 4) & 0b1) || ((mye_in & 0b111) != 0))){
            gb = 1;
        }else{
            gb = 0;
        }
    }else if (se_in == 0b01){
        if (((mye_in >> 2) & 0b1) && (((mye_in >> 3) & 0b1) || ((mye_in & 0b11) != 0))){
            gb = 1;
        }else{
            gb = 0;
        }
    }else if (se_in == 0b10){
        if (((mye_in >> 1) & 0b1) && (((mye_in >> 2) & 0b1) || ((mye_in & 0b1) != 0))){
            gb = 1;
        }else{
            gb = 0;
        }
    }else if (se_in == 0b11){
        if ((mye_in &0b1) && ((mye_in >> 1) & 0b1)){
            gb = 1;
        }else{
            gb = 0;
        }
    }else{
        gb = 0;
    }
    if (fc){
        eyd = esi;
    }else{
        eyd = es;
    }

    unsigned int my,eye,eyf,eyfi,eyr;
    if (((se >> 2) & 0b111) == 0){
        my = (myf + (gb << 4)) & 0xfffffff;
        eye = eyd;
    }else{
        my = (myf << (((se >> 2) & 0b111) << 2)) & 0xfffffff;
        eye = es;
    }
    eyf = (eye - se) & 0b111111111;
    eyfi = (eyf + 1) & 0b111111111;
    if (((eyfi >> 8) & 0b1) || se == 0b11010){
        eyr = 0;
    }else{
        eyr = eyfi & 0b11111111;
    }
    z.i = (ss << 31) + (eyr << 23) + ((my >> 4) & 0x7fffff);

    return z.f;
}

float fsub(float xf, float yf){
    union {
        int i;
        float f;
    } x;
    union {
        int i;
        float f;
    } y;
    union {
        int i;
        float f;
    } z;

    x.f = xf;
    y.f = yf;

    int sy1 = (y.i >> 31) & 0b1;
    int sy2;
    if (sy1){
        sy2 = 0;
    }else{
        sy2 = 1;
    }

    y.i = (sy2 << 31) + (y.i & 0x7fffffff);

    z.f = fadd(x.f,y.f);
    
    return z.f;
}

float fmul(float xf, float yf){
    union {
        int i;
        float f;
    } x;
    union {
        int i;
        float f;
    } y;
    union {
        int i;
        float f;
    } z;

    x.f = xf;
    y.f = yf;

    int hx1 = ((x.i >> 11) & 0xfff) + (1 << 12);
    int hx2 = ((y.i >> 11) & 0xfff) + (1 << 12);
    int lx1 = x.i & 0x7ff;
    int lx2 = y.i & 0x7ff;
    int hh = (hx1 * hx2) & 0x3ffffff;
    int hl = (hx1 * lx2) & 0x3ffffff;
    int lh = (lx1 * hx2) & 0x3ffffff;
    int e1 = (((x.i >> 23) & 0xff) + ((y.i >> 23) & 0xff) + 129) & 0x1ff;
    int sy = ((x.i >> 31) & 0b1)^((y.i >> 31) & 0b1);
    int fex1,fex2;
    if (((x.i >> 23) & 0xff) == 0){
        fex1 = 1;
    }else{
        fex1 = 0;
    }
    if (((y.i >> 23) & 0xff) == 0){
        fex2 = 1;
    }else{
        fex2 = 0;
    }
    int a = (hh + ((hl >> 11) & 0x7fff) + ((lh >> 11) & 0x7fff) + 2) & 0x3ffffff;
    int e2 = (e1 + 1) & 0x1ff;
    int ze, zm;
    if ((((e1 >> 8) & 0b1) == 0) || fex1 || fex2){
        ze = 0;
    }else if ((a >> 25) & 0b1){
        ze = e2 & 0xff;
    }else{
        ze = e1 & 0xff;
    }
    if ((a >> 25) & 0b1){
        zm = (a >> 2) & 0x7fffff;
    }else{
        zm = (a >> 1) & 0x7fffff;
    }
    z.i = (sy << 31) + (ze << 23) + zm;

    return z.f;
}

float finv(float xf){
    union{
        int i;
        float f;
    } x;
    union{
        int i;
        float f;
    } x1;
    union{
        int i;
        float f;
    } x2;
    union{
        int i;
        float f;
    } y;

    x.f = xf;
    int s = (x.i >> 31) & 0b1;
    int e = (x.i >> 23) & 0xff;
    int mh = (x.i >> 13) & 0x3ff; 
    int ml = x.i & 0x1fff;
    unsigned int a0 = 1024 + mh;
    double x01 = (1/(double) a0)*2*2*2*2*2*2*2*2*2*2;
    double x02 = (1/(double) (a0+1))*2*2*2*2*2*2*2*2*2*2;
    x1.f = (float) x01;
    x2.f = (float) x02;
    int fcons = x1.i - 3;
    int fgrad = x1.i - x2.i;
    unsigned int c_tb = fcons & 0x7fffff;
    unsigned int g_tb = (fgrad >> 1) & 0x1fff;
    int a = g_tb * ml;
    int m = (c_tb - ((a >> 12) & 0x3fff)) & 0x7fffff;
    int e1 = (253 - e) & 0x1ff;
    int ey;
    if (e == 0){
        ey = 0;
    }else if((e1 >> 8) & 0b1){
        ey = 0;
    }else{
        ey = e1 & 0xff;
    }
    y.i = (s << 31) + (ey << 23) + m;

    return y.f;
}

float fsqrt(float xf){
    union{
        int i;
        float f;
    } x;
    union{
        int i;
        float f;
    } x1;
    union{
        int i;
        float f;
    } x2;
    union{
        int i;
        float f;
    } y;

    x.f = xf;
    int s = (x.i >> 31) & 0b1;
    int e = (x.i >> 23) & 0xff;
    int i = (x.i >> 14) & 0x1ff;
    int ml = x.i & 0x3fff;
    unsigned int c_tb,g_tb;
    if (e % 2 == 0){
        unsigned int a0 = 512 + i;
        double x01 = sqrt((double)a0/2/2/2/2/2/2/2/2/2/2);
        double x02 = sqrt(((double)a0+1)/2/2/2/2/2/2/2/2/2/2);
        x1.f = (float) x01;
        x2.f = (float) x02;
        int fcons = x1.i;
        int fgrad = x2.i - x1.i;
        c_tb = fcons & 0x7fffff;
        g_tb = (fgrad >> 1) & 0x1fff;
    }else{
        unsigned int a0 = 512 + i;
        double x01 = sqrt((double)a0/2/2/2/2/2/2/2/2/2);
        double x02 = sqrt(((double)a0+1)/2/2/2/2/2/2/2/2/2);
        x1.f = (float) x01;
        x2.f = (float) x02;
        int fcons = x1.i;
        int fgrad = x2.i - x1.i;
        c_tb = fcons & 0x7fffff;
        g_tb = (fgrad >> 1) & 0x1fff;
    }
    int a = g_tb * ml;
    int m = (c_tb + ((a >> 13) & 0x3fff)) & 0x7fffff;
    int e1 = (((e >> 1) & 0x7f) - 0b00111111) & 0xff;
    int ey;
    if (e == 0){
        ey = 0;
    }else{
        ey = (0b01111110 + (e & 0b1) + e1) & 0xff;
    }
    y.i = (s << 31) + (ey << 23) + m;
    
    return y.f;
}

float itof(int xi){
    union{
        int i;
        float f;
    } y;
    int xabs;
    int exp;
    int flag;
    int tmp;
    int s = (xi >> 31) & 0b1;

    if (s){
        xabs = ~xi + 1;
    }else{
        xabs = xi;
    }
    if ((xabs >> 31) & 0b1){
        exp = 158;
        tmp = (xabs >> 8) & 0x7fffff;
        flag = (xabs >> 7) & 0b1;
    }else if ((xabs >> 30) & 0b1){
        exp = 157;
        tmp = (xabs >> 7) & 0x7fffff;
        flag = (xabs >> 6) & 0b1;
    }else if ((xabs >> 29) & 0b1){
        exp = 156;
        tmp = (xabs >> 6) & 0x7fffff;
        flag = (xabs >> 5) & 0b1;
    }else if ((xabs >> 28) & 0b1){
        exp = 155;
        tmp = (xabs >> 5) & 0x7fffff;
        flag = (xabs >> 4) & 0b1;
    }else if ((xabs >> 27) & 0b1){
        exp = 154;
        tmp = (xabs >> 4) & 0x7fffff;
        flag = (xabs >> 3) & 0b1;
    }else if ((xabs >> 26) & 0b1){
        exp = 153;
        tmp = (xabs >> 3) & 0x7fffff;
        flag = (xabs >> 2) & 0b1;
    }else if ((xabs >> 25) & 0b1){
        exp = 152;
        tmp = (xabs >> 2) & 0x7fffff;
        flag = (xabs >> 1) & 0b1;
    }else if ((xabs >> 24) & 0b1){
        exp = 151;
        tmp = (xabs >> 1) & 0x7fffff;
        flag = xabs & 0b1;
    }else if ((xabs >> 23) & 0b1){
        exp = 150;
        tmp = xabs & 0x7fffff;
        flag = 0;
    }else if ((xabs >> 22) & 0b1){
        exp = 149;
        tmp = (xabs & 0x3fffff) << 1;
        flag = 0;
    }else if ((xabs >> 21) & 0b1){
        exp = 148;
        tmp = (xabs & 0x1fffff) << 2;
        flag = 0;
    }else if ((xabs >> 20) & 0b1){
        exp = 147;
        tmp = (xabs & 0xfffff) << 3;
        flag = 0;
    }else if ((xabs >> 19) & 0b1){
        exp = 146;
        tmp = (xabs & 0x7ffff) << 4;
        flag = 0;
    }else if ((xabs >> 18) & 0b1){
        exp = 145;
        tmp = (xabs & 0x3ffff) << 5;
        flag = 0;
    }else if ((xabs >>17) & 0b1){
        exp = 144;
        tmp = (xabs & 0x1ffff) << 6;
        flag = 0;
    }else if ((xabs >> 16) & 0b1){
        exp = 143;
        tmp = (xabs & 0xffff) << 7;
        flag = 0;
    }else if ((xabs >> 15) & 0b1){
        exp = 142;
        tmp = (xabs & 0x7fff) << 8;
        flag = 0;
    }else if ((xabs >> 14) & 0b1){
        exp = 141;
        tmp = (xabs & 0x3fff) << 9;
        flag = 0;
    }else if ((xabs >> 13) & 0b1){
        exp = 140;
        tmp = (xabs & 0x1fff) << 10;
        flag = 0;
    }else if ((xabs >> 12) & 0b1){
        exp = 139;
        tmp = (xabs & 0xfff) << 11;
        flag = 0;
    }else if ((xabs >> 11) & 0b1){
        exp = 138;
        tmp = (xabs & 0x7ff) << 12;
        flag = 0;
    }else if ((xabs >> 10) & 0b1){
        exp = 137;
        tmp = (xabs & 0x3ff) << 13;
        flag = 0;
    }else if ((xabs >> 9) & 0b1){
        exp = 136;
        tmp = (xabs & 0x1ff) << 14;
        flag = 0;
    }else if ((xabs >> 8) & 0b1){
        exp = 135;
        tmp = (xabs & 0xff) << 15;
        flag = 0;
    }else if ((xabs >> 7) & 0b1){
        exp = 134;
        tmp = (xabs & 0x7f) << 16;
        flag = 0;
    }else if ((xabs >> 6) & 0b1){
        exp = 133;
        tmp = (xabs & 0x3f) << 17;
        flag = 0;
    }else if ((xabs >> 5) & 0b1){
        exp = 132;
        tmp = (xabs & 0x1f) << 18;
        flag = 0;
    }else if ((xabs >> 4) & 0b1){
        exp = 131;
        tmp = (xabs & 0xf) << 19;
        flag = 0;
    }else if ((xabs >> 3) & 0b1){
        exp = 130;
        tmp = (xabs & 0x7) << 20;
        flag = 0;
    }else if ((xabs >> 2) & 0b1){
        exp = 129;
        tmp = (xabs & 0x3) << 21;
        flag = 0;
    }else if ((xabs >> 1) & 0b1){
        exp = 128;
        tmp = (xabs & 0x1) << 22;
        flag = 0;
    }else if (xabs & 0b1){
        exp = 127;
        tmp = 0;
        flag = 0;
    }else{
        exp = 0;
        tmp = 0;
        flag = 0;
    }
    int m2 = (tmp + flag) & 0x7fffff;
    int e2 = (exp + 1) & 0xff;
    int c;
    if ((tmp == 0x7fffff) && (flag == 1)){
        c = 1;
    }else{
        c = 0;
    }
    if(c){
        y.i = (s << 31) + (e2 << 23);
    }else{
        y.i = (s << 31) + (exp << 23) + m2;
    }
    
    return y.f;
}

int ftoi(float xf){
    union{
        int i;
        float f;
    } x;

    int y;

    x.f = xf;
    int s = (x.i >> 31) & 0b1;
    int e = (x.i >> 23) & 0xff;
    int m = x.i & 0x7fffff;

    int e2 = (149 - e) & 0xff;
    int m2 = (1 << 23) + m;
    int m3 = (m2 >> e2) & 0xffffff;
    int f = m3 & 0b1;
    int m4 = m3 >> 1;
    int m5 = (m4 + f) & 0xffffff;
    int e3 = (e - 150) & 0xff;
    int m6 = (m2 << e3) & 0xffffffff;
    int tmp;
    if (e < 126){
        tmp = 0;
    }else if (e == 126){
        tmp = 1;    
    }else if (e < 150){
        tmp = m5;
    }else{
        tmp = m6;
    }
    if (s){
        y = ~tmp + 1;
    }else{
        y = tmp;
    }

    return y;
}

float ffloor(float xf){
    union{
        int i;
        float f;
    } x;
    union{
        int i;
        float f;
    } y;

    x.f = xf;
    int s = (x.i >> 31) & 0b1;
    int e = (x.i >> 23) & 0xff;
    int m = x.i & 0x7fffff;

    if (e < 127) {
        if (s && e == 0 && m == 0) {
            y.i = 0x80000000;
        }else if (s){
            y.i = 0xbf800000;
        }else{
            y.i = 0x00000000;
        }
    }else if (e < 150){
        int e1 = (150 - e) & 0xff;
        int e2 = (e + 1) & 0xff;
        int e4 = (23 - e1) & 0xff;
        int m1 = (m >> e1) << e1;
        int m2 = (m1 + (1 << e1)) & 0xffffff;
        int e3,m3;
        if (s && ((m2 >> 23) & 0b1)){
            if (((m << e4) & 0x7fffff) == 0) {
                e3 = e;
            }else{
                e3 = e2;
            }
        }else{
            e3 = e;
        }
        if (s && (((m << e4) & 0x7fffff) == 0)){
            m3 = m1;
        }else if(s){
            m3 = m2 & 0x7fffff;
        }else{
            m3 = m1;
        }
        y.i = (s << 31) + (e3 << 23) + m3; 
    }else{
        y.i = x.i;
    }

    return y.f;
}

float ffabs(float xf){
    union{
        int i;
        float f;
    }x;
    union{
        int i;
        float f;
    }y;

    x.f = xf;
    y.i = x.i & 0x7fffffff;

    return y.f;
}

int fslt(float xf, float yf){
    union{
        int i;
        float f;
    }x;
    union{
        int i;
        float f;
    }y;
    x.f = xf;
    y.f = yf;
    int sx = (x.i >> 31) & 0b1;
    int sy = (y.i >> 31) & 0b1;

    int res;
    if(x.i < y.i){
        if (~(sx | sy)){
            res = 1;
        }else{
            res = 0;
        }
    }else{
        if (sx | sy){
            res = 1;
        }else{
            res = 0;
        }
    }

    return res;
}

int fsle(float xf, float yf){
    union{
        int i;
        float f;
    }x;
    union{
        int i;
        float f;
    }y;
    x.f = xf;
    y.f = yf;
    int sx = (x.i >> 31) & 0b1;
    int sy = (y.i >> 31) & 0b1;

    int res;
    if (x.i == y.i){
        res = 1;
    }else if (x.i < y.i){
        if (~(sx | sy)){
            res = 1;
        }else{
            res = 0;
        }
    }else{
        if (sx | sy){
            res = 1;
        }else{
            res = 0;
        }
    }

    return res;
}

float fdiv(float xf, float yf){
    union{
        int i;
        float f;
    }x;
    union{
        int i;
        float f;
    }y;
    union{
        int i;
        float f;
    }x2;
    union{
        int i;
        float f;
    }y2;
    x.f = xf;
    y.f = yf;

    int ex = (x.i >> 23) & 0xff;
    int ey = (y.i >> 23) & 0xff;
    int ex2,ey2;
    if (ey == 253){
        if (ex == 0){
            ex2 = 0;
        }else{
            ex2 = ex - 1;
        }
        ey2 = ey - 1;
    }else{
        ex2 = ex;
        ey2 = ey;
    }
    x2.i = (((x.i >> 31) & 0b1) << 31) + (ex2 << 23) + (x.i & 0x7fffff);
    y2.i = (((y.i >> 31) & 0b1) << 31) + (ey2 << 23) + (y.i & 0x7fffff);

    float inv = finv(y2.f);
    float z = fmul(x2.f,inv);

    return z;
}

float fexp2(float xf, int yi){
    union{
        int i;
        float f;
    }x;
    union{
        int i;
        float f;
    }z;
    x.f = xf;
    int e = (x.i >> 23) & 0xff;
    int e1 = (e + (yi & 0x1ff)) & 0x1ff;
    int e2;
    if (e == 0){
        e2 = 0;
    }else if((e1 >> 8) && ((yi & 0x1ff) >> 8)){
        e2 = 0;
    }else{
        e2 = e1;
    }
    z.i = (((x.i >> 31) & 0b1) << 31) + (e2 << 23) + (x.i & 0x7fffff);

    return z.f;
}
