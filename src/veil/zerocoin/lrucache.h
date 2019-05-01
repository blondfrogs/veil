// Copyright (c) 2019 The Veil Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#ifndef VEIL_LRUCACHE_H
#define VEIL_LRUCACHE_H

#include <veil/zerocoin/witness.h>
#include <unordered_map>
#include <list>

class PrecomputeLRUCache
{
private:
    std::list<std::pair<uint256, CoinWitnessCacheData> > cache_list;
    std::map<uint256, std::list<std::pair<uint256, CoinWitnessCacheData> >::iterator> mapCacheLocation;
    std::map<uint256, CoinWitnessCacheData> mapDirtyWitnessData;

public:
    void AddNew(const uint256& hash, CoinWitnessCacheData& data);
    void AddToCache(const uint256& hash, CoinWitnessCacheData& serialData);
    bool Contains(const uint256& hash) const;
    void Clear();
    void FlushToDisk(CPrecomputeDB* pprecomputeDB);
    CoinWitnessData GetWitnessData(const uint256& hash);
    PrecomputeLRUCache();
    void MoveDirtyToLRU(const uint256& hash);
    void MoveLastToDirtyIfFull();
    void Remove(const uint256& hash);
    int Size() const;
    int DirtyCacheSize() const;
};

template<typename cache_key_t, typename cache_value_t>
class CLRUCache
{
public:
    typedef typename std::pair<cache_key_t, cache_value_t> key_value_pair_t;
    typedef typename std::list<key_value_pair_t>::iterator list_iterator_t;

    CLRUCache(size_t max_size) : maxSize(max_size)
    {
    }
    CLRUCache()
    {
        SetNull();
    }

    void Put(const cache_key_t& key, const cache_value_t& value)
    {
        auto it = cacheItemsMap.find(key);
        cacheItemsList.push_front(key_value_pair_t(key, value));
        if (it != cacheItemsMap.end())
        {
            cacheItemsList.erase(it->second);
            cacheItemsMap.erase(it);
        }
        cacheItemsMap[key] = cacheItemsList.begin();

        if (cacheItemsMap.size() > maxSize)
        {
            auto last = cacheItemsList.end();
            last--;
            cacheItemsMap.erase(last->first);
            cacheItemsList.pop_back();
        }
    }

    void Erase(const cache_key_t& key)
    {
        auto it = cacheItemsMap.find(key);
        if (it != cacheItemsMap.end())
        {
            cacheItemsList.erase(it->second);
            cacheItemsMap.erase(it);
        }
    }

    const cache_value_t& Get(const cache_key_t& key)
    {
        auto it = cacheItemsMap.find(key);
        if (it == cacheItemsMap.end())
        {
            throw std::range_error("There is no such key in cache");
        }
        else
        {
            cacheItemsList.splice(cacheItemsList.begin(), cacheItemsList, it->second);
            return it->second->second;
        }
    }

    bool Exists(const cache_key_t& key) const
    {
        return cacheItemsMap.find(key) != cacheItemsMap.end();
    }

    size_t Size() const
    {
        return cacheItemsMap.size();
    }


    void Clear()
    {
        cacheItemsMap.clear();
        cacheItemsList.clear();
    }

    void SetNull()
    {
        maxSize = 0;
        Clear();
    }

    size_t MaxSize() const
    {
        return maxSize;
    }


    void SetSize(const size_t size)
    {
        maxSize = size;
    }

    const std::unordered_map<cache_key_t, list_iterator_t>& GetItemsMap()
    {
        return cacheItemsMap;
    };

    const std::list<key_value_pair_t>& GetItemsList()
    {
        return cacheItemsList;
    };


    CLRUCache(const CLRUCache& cache)
    {
        this->cacheItemsList = cache.cacheItemsList;
        this->cacheItemsMap = cache.cacheItemsMap;
        this->maxSize = cache.maxSize;
    }

private:
    std::list<key_value_pair_t> cacheItemsList;
    std::unordered_map<cache_key_t, list_iterator_t> cacheItemsMap;
    size_t maxSize;
};

#endif //VEIL_LRUCACHE_H