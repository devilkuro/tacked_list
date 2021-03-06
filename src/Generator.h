/*
 * Generator.h
 *
 *  Created on: 2013-9-20
 *      Author: Fanjing
 */

#ifndef _GENERATOR_H_
#define _GENERATOR_H_

#include "helper.h"
#include "common.h"

/*
 *
 */
class Generator {
public:
    Generator();
    virtual ~Generator();
private:
    Helper hp;
public:
    unsigned int bw_down;
    unsigned int bw_up;
    unsigned int ts_down;
    unsigned int ts_up;
    unsigned int td_down;
    unsigned int td_up;
    unsigned int interval;
public:
    void setGenerator(unsigned int bw_down, unsigned int bw_up,
            unsigned int ts_down, unsigned int ts_up, unsigned int td_ave,
            unsigned int td_limit, unsigned int interval, unsigned int seed = 0);
    bool output(const char* fileName, unsigned int t); // create a file to record the request list. Return true if succeed.
    unsigned int getNext(Request* rq); //generate a request and return the interval before this request.
};

#endif /* GENERATOR_H_ */
