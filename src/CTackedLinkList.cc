/*
 * CTackedLinkList.cpp
 *
 *  Created on: 2013-9-29
 *      Author: Fanjing-LAB
 */
#include "common.h"
#include "helper.h"
#include "bplus.h"
#include "ctlink.h"
#include "CILink.h"
#include "CArrayList.h"
#include "Generator.h"
#include "PreciseTimer.h"
#include "ASMTimer.h"

//#define CT_TEST_1
//#define CT_TEST_1_Z
//#define CT_TEST_2
//#define CT_TEST_3
//#define CT_TEST_3_B
//#define CT_TEST_3_C
//#define CT_TEST_4
//#define CT_TEST_4_F
//#define CT_TEST_5
//#define CT_TEST_6
#define CT_TEST_0

template<class T> string m_toStr(T tmp) {
    stringstream ss;
    ss << tmp;
    return ss.str();
}

struct ControlStack {
    // synchronizer between main thread and record thread.
    unsigned int multiple;
    unsigned int n; // the num of requests processed
    unsigned int t; // the seconds passed
    string logName; // the name of the log file
    bool stopFlag;
};

DWORD WINAPI RecordFor5(LPVOID tmp) {
    ControlStack* p = (ControlStack*) tmp;
    unsigned int pre_num = 0;
    unsigned int interval = 1000 / p->multiple;
    while(1){
        if(p->stopFlag){
            break;
        }
        ofstream file(p->logName.c_str(), ios::app);
        p->t++;
        file << p->t << "\t" << p->n << "\t" << p->n - pre_num << endl;
        pre_num = p->n;
        file.close();
        Sleep(interval); //ms
    }
    return 0;
}

int main() {
    Helper H;
    PreciseTimer pt;

    // for test
#ifdef CT_TEST_0
    {
        ASMTimer at;
        cout << at.getCounts() << ":" << at.getCountsNoEC() << endl;
        int a = 100;
        at.start();
        Sleep(a);

        at.end();
        cout << at.getMilliseconds() << "ms" << endl;
    }
#endif
    // experiment 1z
#ifdef CT_TEST_1_Z
    {
        unsigned int REQUEST_NUM=100000;
        Request* rq = new Request[REQUEST_NUM];
        ofstream file2("result1z.log");
        file2 << "T/t\t64t\t126t\t256t\t512t" << endl;
        file2.close();
        unsigned int max_reserve_time = 131072;
        srand(0);
        for (unsigned int j = 0; j < REQUEST_NUM; j++){
            rq[j].td = 1;
            rq[j].bw = 1;
            rq[j].ts = H.U_Randint(1, max_reserve_time - rq[j].td);
        }
        // i stands for T in cost formula
        for (unsigned int i = 4; i <= 1024; i += 4){
            ofstream file("result1z.log", ios::app);
            file << i;
            cout << i << endl;
            // j stands for t in cost formula
            for (unsigned int j = 64; j <= 512; j *= 2){
                {
                    //CTLink
                    CTLink* ct = new CTLink(max_reserve_time / i,
                            max_reserve_time);
                    unsigned int t = 0;
                    // 1st. first round to fill the CTLink
                    for (unsigned int k = 0; k < REQUEST_NUM; k++){
                        ct->Insert(rq[k]);
                        t += j;
                        ct->SetTime(t);
                        // the CTLink is full.
                        if (t > max_reserve_time){
                            break;
                        }
                    }
                    // 2nd. second round to test the performance
                    pt.start();
                    for (unsigned int k = 0; k < REQUEST_NUM; k++){
                        ct->Insert(rq[k]);
                        t += j;
                        ct->SetTime(t);
                    }
                    pt.end();
                    file << "\t" << pt.getMicroseconds();
                    delete ct;
                }
            }
            file << endl;
            file.close();
        }
    }
#endif
    // experiment 1
#ifdef CT_TEST_1
    {
        unsigned int REQUEST_NUM=100000;
        Request* rq = new Request[REQUEST_NUM];
        ofstream file2("result1.log");
        file2 << "T/t\t4t\tc\t4i\tc\t8t\tc\t8i\tc\t16t\tc\t16i\tc\t32t\tc\t32i\tc" << endl;
        file2.close();
        unsigned int max_reserve_time = 131072;
        srand(0);
        for (unsigned int j = 0; j < REQUEST_NUM; j++){
            rq[j].td = 1;
            rq[j].bw = 1;
            rq[j].ts = H.U_Randint(1, max_reserve_time - rq[j].td);
        }
        // i stands for T in cost formula
        for (unsigned int i = 4; i <= 1024; i += 4){
            ofstream file("result1.log", ios::app);
            file << i;
            cout << i << endl;
            // j stands for t in cost formula
            for (unsigned int j = 4; j <= 32; j *= 2){
                {
                    //CTLink
                    CTLink* ct = new CTLink(max_reserve_time / i,
                            max_reserve_time);
                    unsigned int t = 0;
                    // 1st. first round to fill the CTLink
                    for (unsigned int k = 0; k < REQUEST_NUM; k++){
                        ct->Insert(rq[k]);
                        t += j;
                        ct->SetTime(t);
                        // the CTLink is full.
                        if (t > max_reserve_time){
                            break;
                        }
                    }
                    // 2nd. second round to test the performance
                    pt.start();
                    for (unsigned int k = 0; k < REQUEST_NUM; k++){
                        ct->Insert(rq[k]);
                        t += j;
                        ct->SetTime(t);
                    }
                    pt.end();
                    file << "\t" << pt.getMicroseconds() << "\t"
                    << pt.getCounts();
                    delete ct;
                }
                {
                    //CILink
                    CILink* ci = new CILink(max_reserve_time / i,
                            max_reserve_time);
                    unsigned int t = 0;
                    // 1st. first round to fill the CTLink
                    for (unsigned int k = 0; k < REQUEST_NUM; k++){
                        ci->Insert(rq[k]);
                        t += j;
                        ci->SetTime(t);
                        // the CTLink is full.
                        if (t > max_reserve_time){
                            break;
                        }
                    }
                    // 2nd. second round to test the performance
                    pt.start();
                    for (unsigned int k = 0; k < REQUEST_NUM; k++){
                        ci->Insert(rq[k]);
                        t += j;
                        ci->SetTime(t);
                    }
                    pt.end();
                    file << "\t" << pt.getMicroseconds() << "\t"
                    << pt.getCounts();
                    delete ci;
                }
            }
            file << endl;
            file.close();
        }
    }
#endif

    // experiment 2
#ifdef CT_TEST_2

    {
        unsigned int REQUEST_NUM=100000;
        Request* rq = new Request[REQUEST_NUM];
        ofstream file1("result2.log");
        file1 << "TD/T\t128\tc\t256\tc\t512\tc\t1024\tc" << endl;
        file1.close();
        unsigned int max_reserve_time = 131072;
        for (unsigned int i = 256; i <= 2048; i += 8){
            ofstream file("result2.log", ios::app);
            file << i;
            cout << i << endl;
            // generate the request set.
            srand(0);
            for (unsigned int k = 0; k < REQUEST_NUM; k++){
                rq[k].ts = H.U_Randint(1, max_reserve_time - i);
                rq[k].td = i;
                rq[k].bw = 1;
            }
            for (unsigned int j = 128; j <= 1024; j *=2){
                CTLink* ct = new CTLink(max_reserve_time/j, max_reserve_time);
                unsigned int t = 0;
                // 1st. first round to fill the CTLink
                for (unsigned int k = 0; k < REQUEST_NUM; k++){
                    ct->Insert(rq[k]);
                    t += 4;
                    ct->SetTime(t);
                    // the CTLink is full.
                    if(t>max_reserve_time){
                        break;
                    }
                }
                // 2nd. second round to test the performance
                pt.start();
                for (unsigned int k = 0; k < REQUEST_NUM; k++){
                    ct->Insert(rq[k]);
                    t += 4;
                    ct->SetTime(t);
                }
                pt.end();
                file << "\t" << pt.getMicroseconds() << "\t" << pt.getCounts();
                delete ct;
            }
            file << endl;
            file.close();
        }
    }

#endif

    // experiment 3
#ifdef CT_TEST_3
    {
        unsigned int REQUEST_NUM = 100000;
        Request* rq = new Request[REQUEST_NUM];
        // 1.initialize the log file
        ofstream file4("result3.log");
        file4 << "";
        file4.close();
        unsigned int max_reserve_time = 1048576;
        srand(0);
        for (unsigned int i = 0; i < REQUEST_NUM; i++){
            rq[i].td = 4;
            rq[i].bw = 1;
            rq[i].ts = H.U_Randint(1, max_reserve_time - rq[i].td);
        }
        ofstream file("result3.log", ios::app);
        for (int i = 16; i <= 8192; i += 16){
            cout<< i <<endl;
            {

                //CTLink
                CTLink* ct[1000];
                for (int var = 0; var < 1000; ++var){
                    ct[var] = new CTLink(i, max_reserve_time);
                }
                unsigned int t = 0;
                unsigned int j = 0;
                unsigned int k = 0;
                pt.start();
                for (int var = 0; var < 1000; ++var){
                    t = 0;
                    for (j = 0; j < 20; j++, k++){
                        ct[var]->Insert(rq[k]);
                        t += 4;
                        ct[var]->SetTime(t);
                    }
                }
                pt.end();
                file << i << "\t" << pt.getMicroseconds() << "\t"
                << pt.getCounts();
                for (int var = 0; var < 1000; ++var){
                    delete ct[var];
                }
            }
            {
                //CILink
                CILink* ci[1000];
                for (int var = 0; var < 1000; ++var){
                    ci[var] = new CILink(i, max_reserve_time);
                }
                unsigned int t = 0;
                unsigned int j = 0;
                unsigned int k = 0;
                pt.start();
                for (int var = 0; var < 1000; ++var){
                    t = 0;
                    for (j = 0; j < 20; j++, k++){
                        ci[var]->Insert(rq[k]);
                        t += 4;
                        ci[var]->SetTime(t);
                    }
                }
                pt.end();
                file << "\t" << pt.getMicroseconds() << "\t" << pt.getCounts()
                << endl;
                for (int var = 0; var < 1000; ++var){
                    delete ci[var];
                }
            }
        }
        file.close();
    }
#endif
    // experiment 3c
#ifdef CT_TEST_3_C
    {
        unsigned int REQUEST_NUM=100000;
        Request* rq = new Request[REQUEST_NUM];
        // 1.initialize the log file
        ofstream file4("result3.log");
        file4 << "";
        file4.close();
        unsigned int max_reserve_time = 1048576;
        srand(0);
        for (unsigned int i = 0; i < REQUEST_NUM; i++){
            rq[i].td = 128;
            rq[i].bw = 1;
            rq[i].ts = H.U_Randint(1, max_reserve_time - rq[i].td);
        }
        ofstream file("result3.log", ios::app);
        for (int i = 4; i <= 4096; i+=4){
            {
                //CTLink
                CTLink* ct = new CTLink(max_reserve_time/i,max_reserve_time);
                unsigned int t = 0;
                // 1st. first round to fill the CTLink
//				for (unsigned int k = 0; k < REQUEST_NUM; k++) {
//					ct->Insert(rq[k]);
//					t += 128;
//					ct->SetTime(t);
//					// the CTLink is full.
//					if(t>max_reserve_time){
//						break;
//					}
//				}
                pt.start();
                for (unsigned int k = 0; k < REQUEST_NUM; k++){
                    ct->Insert(rq[k]);
                    t += 128;
                    ct->SetTime(t);
                }
                pt.end();
                file << i << "\tCT" <<"\t" << pt.getMilliseconds() << "\t" << pt.getCounts() << endl;
                delete ct;
            }
        }

        for (int i = 4; i <= 4096; i+=4){
            {
                //CILink
                CILink* ci = new CILink(max_reserve_time/i,max_reserve_time);
                unsigned int t = 0;
                // 1st. first round to fill the CTLink
//				for (unsigned int k = 0; k < REQUEST_NUM; k++) {
//					ci->Insert(rq[k]);
//					t += 128;
//					ci->SetTime(t);
//					// the CTLink is full.
//					if(t>max_reserve_time){
//						break;
//					}
//				}
                pt.start();
                for (unsigned int k = 0; k < REQUEST_NUM; k++){
                    ci->Insert(rq[k]);
                    t += 128;
                    ci->SetTime(t);
                }
                pt.end();
                file << i << "\tCI" << "\t" << pt.getMilliseconds() << "\t" << pt.getCounts() << endl;
                delete ci;
            }
        }
        file.close();
    }
#endif
    // experiment 3 b
#ifdef CT_TEST_3_B
    {
        unsigned int REQUEST_NUM=5000;
        Request* rq = new Request[REQUEST_NUM];
        // 1.initialize the log file
        ofstream file3("result3.log");
        file3 << "ID\tus\tc"<< endl;
        file3.close();
        unsigned int max_reserve_time = 65536;
        srand(0);
        for (unsigned int i = 0; i < REQUEST_NUM; i++){
            rq[i].td = 2;
            rq[i].bw = 1;
            rq[i].ts = H.U_Randint(1, max_reserve_time - rq[i].td);
        }
        ofstream file("result3.log", ios::app);
        for (int i = 512; i <= 8192; i*=2){
            cout<< i<<endl;
            {
                //CILink
                CTLink* ct[1000];
                for (int var = 0; var < 1000; ++var){
                    ct[var] = new CTLink(i, max_reserve_time);
                }
                unsigned int t = 0;
                unsigned int k = 0;
                unsigned int x = 20;
                pt.start();
                for (; k < REQUEST_NUM; k++){
                    x--;
                    for (int var = 0; var < 1000; ++var){
                        ct[var]->Insert(rq[k]);
                        t += 4;
                        ct[var]->SetTime(t);
                    }
                    if(x==0){
                        pt.end();
                        x=20;
                        file << i << "\tCT" << "\t" << pt.getMicroseconds()
                        << "\t" << pt.getCounts() << endl;
                        pt.start();
                    }
                }
                pt.end();
                for (int var = 0; var < 1000; ++var){
                    delete ct[var];
                }
            }

            {
                //CILink
                CILink* ci[1000];
                for (int var = 0; var < 1000; ++var){
                    ci[var] = new CILink(i, max_reserve_time);
                }
                unsigned int t = 0;
                unsigned int k = 0;
                unsigned int x = 20;
                pt.start();
                for (; k < REQUEST_NUM; k++){
                    x--;
                    for (int var = 0; var < 1000; ++var){
                        ci[var]->Insert(rq[k]);
                        t += 4;
                        ci[var]->SetTime(t);
                    }
                    if (x == 0){
                        pt.end();
                        x = 20;
                        file << i << "\tCI" << "\t" << pt.getMicroseconds()
                        << "\t" << pt.getCounts() << endl;
                        pt.start();
                    }
                }
                pt.end();
                for (int var = 0; var < 1000; ++var){
                    delete ci[var];
                }
            }
        }
        file.close();
    }
#endif

    // experiment 4
#ifdef CT_TEST_4
    {
        unsigned int REQUEST_NUM=100000;
        Request* rq = new Request[REQUEST_NUM];
        // 1.initialize the log file
        ofstream file4("result4.log");
        file4 << "";
        file4.close();
        unsigned int max_reserve_time = 32768;
        srand(0);
        for (unsigned int i = 0; i < REQUEST_NUM; i++){
            rq[i].ts = H.U_Randint(1, max_reserve_time - 4);
            rq[i].td = 4;
            rq[i].bw = 1;
        }
        ofstream file("result4.log", ios::app);
        for (int i = 256; i <= 4096; i*=2){
            {
                //CTLink
                CTLink* ct = new CTLink(i,max_reserve_time);
                unsigned int t = 0;
                // 1st. first round to fill the CTLink
                for (unsigned int k = 0; k < REQUEST_NUM; k++){
                    ct->Insert(rq[k]);
                    t += 4;
                    ct->SetTime(t);
                    // the CTLink is full.
                    if(t>max_reserve_time){
                        break;
                    }
                }
                pt.start();
                for (unsigned int k = 0; k < REQUEST_NUM; k++){
                    ct->Insert(rq[k]);
                    t += 4;
                    ct->SetTime(t);
                }
                pt.end();
                file << i << "\tCT" <<"\t" << pt.getMilliseconds() << "\t" << pt.getCounts() << endl;
                delete ct;
            }
            {
                //CILink
                CILink* ci = new CILink(i,max_reserve_time);
                unsigned int t = 0;
                // 1st. first round to fill the CTLink
                for (unsigned int k = 0; k < REQUEST_NUM; k++){
                    ci->Insert(rq[k]);
                    t += 4;
                    ci->SetTime(t);
                    // the CTLink is full.
                    if(t>max_reserve_time){
                        break;
                    }
                }
                pt.start();
                for (unsigned int k = 0; k < REQUEST_NUM; k++){
                    ci->Insert(rq[k]);
                    t += 4;
                    ci->SetTime(t);
                }
                pt.end();
                file << i << "\tCI" << "\t" << pt.getMilliseconds() << "\t" << pt.getCounts() << endl;
                delete ci;
            }
        }
        file.close();
    }
#endif
#ifdef CT_TEST_4_F
    {
        unsigned int REQUEST_NUM = 100000;
        Request* rq = new Request[REQUEST_NUM];
        // 1.initialize the log file
        ofstream file4("result4f.log");
        file4 << "";
        file4.close();
        unsigned int max_reserve_time = 1048576;
        srand(0);
        for (unsigned int i = 0; i < REQUEST_NUM; i++){
            rq[i].td = 4;
            rq[i].bw = 1;
            rq[i].ts = H.U_Randint(1, max_reserve_time - rq[i].td);
        }
        ofstream file("result4f.log", ios::app);
        for (int i = 16; i <= 8192; i += 16){
            {
                //CTLink
                CTLink* ct = new CTLink(i,max_reserve_time);
                unsigned int t = 0;
                // 1st. first round to fill the CTLink
                for (unsigned int k = 0; k < REQUEST_NUM; k++){
                    ct->Insert(rq[k]);
                    t += 4;
                    ct->SetTime(t);
                    // the CTLink is full.
                    if(t>max_reserve_time){
                        break;
                    }
                }
                pt.start();
                for (unsigned int k = 0; k < REQUEST_NUM; k++){
                    ct->Insert(rq[k]);
                    t += 4;
                    ct->SetTime(t);
                }
                pt.end();
                file << i << "\t" << pt.getMilliseconds();
                delete ct;
            }
            {
                //CILink
                CILink* ci = new CILink(i,max_reserve_time);
                unsigned int t = 0;
                // 1st. first round to fill the CTLink
                for (unsigned int k = 0; k < REQUEST_NUM; k++){
                    ci->Insert(rq[k]);
                    t += 4;
                    ci->SetTime(t);
                    // the CTLink is full.
                    if(t>max_reserve_time){
                        break;
                    }
                }
                pt.start();
                for (unsigned int k = 0; k < REQUEST_NUM; k++){
                    ci->Insert(rq[k]);
                    t += 4;
                    ci->SetTime(t);
                }
                pt.end();
                file <<"\t" << pt.getMilliseconds();
                delete ci;
            }
            file<< endl;
        }
        file.close();
    }
#endif
    // experiment 5
#ifdef CT_TEST_5
    {
        unsigned int REQUEST_NUM=1000000;
        Request* rq = new Request[REQUEST_NUM];
        unsigned int max_reserve_time = 432000;
        unsigned int index_num = 108000;
        unsigned int td_down = 30;
        unsigned int td_up = 1800;
        unsigned int multiple = 5;
        unsigned int record_num = 2000;

        // initialize the log file.
        for (unsigned int i = 0; i < REQUEST_NUM; i++){
            rq[i].td = H.U_Randint(td_down, td_up);
            rq[i].bw = 1;
            rq[i].ts = H.U_Randint(1, max_reserve_time - rq[i].td);
        }
        {
            ofstream file5("result5-CT.log");
            file5 << "";
            file5.close();
            //CTLink
            ofstream file("result5-CT.log", ios::app);
            file << "CTLink" << endl;
            file << "TIME\t" << "NUM\t" << "DIFF" << endl;
            file.close();

            ControlStack n;
            n.logName = "result5-CT.log";
            n.multiple = multiple;
            n.t = 0;
            n.n = 0;
            n.stopFlag = false;
            CTLink* ct = new CTLink(index_num,max_reserve_time);
            ct->iMaxResource = UINT_MAX;
            HANDLE hThread2 = CreateThread(NULL, 0, RecordFor5, &n, 0,
                    NULL);
            for (unsigned int k = 0; k < REQUEST_NUM; k++){
                ct->Insert(rq[k]);
                ct->SetTime(n.t/multiple);
                n.n++;
                if (n.t < record_num){
                    if (k == REQUEST_NUM - 1){
                        k = 0;
                    }
                } else{
                    n.stopFlag = true;
                    break;
                }
            }
            CloseHandle(hThread2);
            delete ct;
        }
        // sleep 200ms to wait the RecordThread to stop
        Sleep(2000/multiple);
        {
            ofstream file5("result5-CI.log");
            file5 << "";
            file5.close();
            //CTLink
            ofstream file("result5-CI.log", ios::app);
            file << "CILink" << endl;
            file << "TIME\t" << "NUM\t" << "DIFF" << endl;
            file.close();

            ControlStack n;
            n.logName = "result5-CI.log";
            n.multiple = multiple;
            n.t = 0;
            n.n = 0;
            n.stopFlag = false;
            CILink* ci = new CILink(index_num,max_reserve_time);
            ci->iMaxResource = UINT_MAX;
            HANDLE hThread2 = CreateThread(NULL, 0, RecordFor5, &n, 0,
                    NULL);
            for (unsigned int k = 0; k < REQUEST_NUM; k++){
                ci->Insert(rq[k]);
                ci->SetTime(n.t/multiple);
                n.n++;
                if (n.t < record_num){
                    if (k == REQUEST_NUM - 1){
                        k = 0;
                    }
                } else{
                    n.stopFlag = true;
                    break;
                }
            }
            CloseHandle(hThread2);
            delete ci;
        }
        // sleep 200ms to wait the RecordThread to stop
        Sleep(2000/multiple);
        {
            ofstream file5("result5-CA.log");
            file5 << "";
            file5.close();
            //CArrayList
            ofstream file("result5-CA.log", ios::app);
            file << "CArrayList" << endl;
            file << "TIME\t" << "NUM\t" << "DIFF" << endl;
            file.close();

            ControlStack n;
            n.logName = "result5-CA.log";
            n.multiple = multiple;
            n.t = 0;
            n.n = 0;
            n.stopFlag = false;
            CArrayList* ca = new CArrayList(max_reserve_time);
            ca->max_resource = UINT_MAX;
            HANDLE hThread2 = CreateThread(NULL, 0, RecordFor5, &n, 0,
                    NULL);
            for (unsigned int k = 0; k < REQUEST_NUM; k++){
                ca->Insert(rq[k]);
                ca->setTime(n.t/multiple);
                n.n++;
                if (n.t < record_num){
                    if (k == REQUEST_NUM - 1){
                        k = 0;
                    }
                } else{
                    n.stopFlag = true;
                    break;
                }
            }
            CloseHandle(hThread2);
            delete ca;
        }
        // sleep 200ms to wait the RecordThread to stop
        Sleep(2000/multiple);
        {
            ofstream file5("result5-BP.log");
            file5 << "";
            file5.close();
            //Bplus
            ofstream file("result5-BP.log", ios::app);
            file << "Bplus" << endl;
            file << "TIME\t" << "NUM\t" << "DIFF" << endl;
            file.close();

            ControlStack n;
            n.logName = "result5-BP.log";
            n.multiple = multiple;
            n.t = 0;
            n.n = 0;
            n.stopFlag = false;
            Bplus* bp = new Bplus();
            Request tmpR;

            bp->RMAX = UINT_MAX;
            HANDLE hThread2 = CreateThread(NULL, 0, RecordFor5, &n, 0,
                    NULL);
            for (unsigned int k = 0; k < REQUEST_NUM; k++){
                tmpR.bw = rq[k].bw;
                tmpR.td = rq[k].td;
                tmpR.ts = rq[k].ts+n.t/multiple;
                bp->Insert(tmpR);
                bp->Delete(n.t/multiple);
                n.n++;
                if (n.t < record_num){
                    if (k == REQUEST_NUM - 1){
                        k = 0;
                    }
                } else{
                    n.stopFlag = true;
                    break;
                }
            }
            CloseHandle(hThread2);
            delete bp;
        }
        // sleep 200ms to wait the RecordThread to stop
        Sleep(2000/multiple);
    }
#endif

    // experiment 6
#ifdef CT_TEST_6

    {
        unsigned int REQUEST_NUM=100000;
        Request* rq = new Request[REQUEST_NUM];
        unsigned int max_reserve_time = 86400;
        srand(0);
        unsigned int *interval = new unsigned int[REQUEST_NUM];
        for (unsigned int i = 0; i < REQUEST_NUM; i++){
            rq[i].td = H.U_Randint(60, 3600);
            rq[i].ts = H.U_Randint(1, max_reserve_time - rq[i].td);
            rq[i].bw = H.U_Randint(6, 16);
            interval[i] = H.U_Randint(5, 15);
        }
        ofstream file6("result6.log");
        file6 << "tnum/cost\tCTLink\tCIlink" << endl;
        file6.close();
        {
            for (unsigned int i = 256; i <= 8192; i *= 2){
                ofstream file("result6.log", ios::app);
                file << i;
                cout << i << endl;
                {
                    //CTLink
                    CTLink* ct = new CTLink(i, max_reserve_time);
                    unsigned int t = 0;
                    pt.start();
                    for (unsigned int k = 0; k < REQUEST_NUM; k++){
                        ct->Insert(rq[k]);
                        t += interval[k];
                        ct->SetTime(t);
                    }
                    pt.end();
                    file << "\t" << pt.getMilliseconds() <<"\t"<<pt.getCounts()<<"\t"<<pt.getCountsEC();
                    delete ct;
                }
                {
                    // CILink
                    CILink* ci = new CILink(i, max_reserve_time);
                    unsigned int t = 0;
                    pt.start();
                    for (unsigned int k = 0; k < REQUEST_NUM; k++){
                        ci->Insert(rq[k]);
                        t += interval[k];
                        ci->SetTime(t);
                    }
                    pt.end();
                    file << "\t" << pt.getMilliseconds() <<"\t"<<pt.getCounts()<<"\t"<<pt.getCountsEC();
                    delete ci;
                }
                file << endl;
                file.close();
            }
        }
        //CArrayList
        {
            ofstream file("result6.log", ios::app);
            cout << "CArrayList:" << endl;
            CArrayList* ca = new CArrayList(max_reserve_time);
            unsigned int t = 0;
            pt.start();
            for (unsigned int k = 0; k < REQUEST_NUM; k++){
                ca->Insert(rq[k]);
                t += interval[k];
                ca->setTime(t);
            }
            pt.end();
            file << "\t" << pt.getMilliseconds() <<"\t"<<pt.getCounts()<<"\t"<<pt.getCountsEC();
            delete ca;
            file << endl;
            file.close();
        }
        //Bplus
        {
            ofstream file("result6.log", ios::app);
            cout << "Bplus:" << endl;
            Bplus* bp = new Bplus();
            unsigned int t = 0;
            pt.start();
            for (unsigned int k = 0; k < REQUEST_NUM; k++){
                rq[k].ts += t;
                bp->Insert(rq[k]);
                t += interval[k];
                if ((k + 1) % 100 == 0){
                    bp->Delete(t);
                }
            }
            pt.end();
            file << "\t" << pt.getMilliseconds() <<"\t"<<pt.getCounts()<<"\t"<<pt.getCountsEC();
            delete bp;
            file << endl;
            file.close();
        }
    }
#endif
}
