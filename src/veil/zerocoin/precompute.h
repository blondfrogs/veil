/**
* @file       precompute.h
*
* @brief      Precompute zercoins spend proofs.
*
* @author     blondfrogs https://github.com/blondfrogs presstab https://github.com/presstab fuzzbawls https://github.com/fuzzbawls,
* @date       May 2019
*
* @copyright  Copyright 2019 The Veil Developers, Copyright 2019 The PIVX Developers
* @license    This project is released under the MIT license.
**/

#ifndef VEIL_PRECOMPUTE_H
#define VEIL_PRECOMPUTE_H

#include "lrucache.h"
#include "boost/thread.hpp"

static const int DEFAULT_PRECOMPUTE_BPC = 100; // BPC = Blocks Per Cycle
static const int MIN_PRECOMPUTE_BPC = 100;
static const int MAX_PRECOMPUTE_BPC = 2000;

class Precompute
{
private:
    int nBlocksPerCycle;
    boost::thread_group* pthreadGroupPrecompute;

public:

    PrecomputeLRUCache lru;

    Precompute();
    void SetNull();
    boost::thread_group* GetThreadGroupPointer();
    void SetThreadGroupPointer(void* threadGroup);
    void SetThreadPointer();
    bool StartPrecomputing(std::string& strStatus);

    void StopPrecomputing();
    void SetBlocksPerCycle(const int& nNewBlockPerCycle);
    int GetBlocksPerCycle();
};

void ThreadPrecomputeSpends();
void LinkPrecomputeThreadGroup(void* pthreadgroup);
void DumpPrecomputes();


#endif //VEIL_PRECOMPUTE_H
