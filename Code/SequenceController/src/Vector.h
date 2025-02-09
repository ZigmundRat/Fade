#include <math.h>

#define DATAITEMUNUSED ((VectorDataItem *)0xFFFFFFFFFFFFFFFF)

class VectorDataItem
{
public:
    VectorDataItem()
    {
        _data = 0;
        _pNext = DATAITEMUNUSED;
    }

    float _data;
    VectorDataItem *_pNext;
};

class VectorDataItemProviderChunk
{
    int _size;
    int _probe; // first spot to look for free items. Is set to 1 after the location where
                // the previous free item was found.

    VectorDataItem *_pDataItems;

public:
    VectorDataItemProviderChunk(int size)
    {
        _size = size;
        _pDataItems = new VectorDataItem[size];
        _probe = 0;
    }

    int GetSize()
    {
        return _size;
    }

    VectorDataItem *GetDataItems()
    {
        return _pDataItems;
    }
};

class VectorDataItemProvider
{
    const int MaxChunks = 16;

    VectorDataItemProviderChunk *_pDataItemChunk[16];
    int _chunkCount;
    int _inUseCount;

    VectorDataItem *_pFreeList;

public:
    VectorDataItemProvider()
    {
        _chunkCount = 0;
        _pFreeList = 0;
        _inUseCount = 0;
        AddNewChunk();
    }

    ~VectorDataItemProvider()
    {
        for (int chunk = 0; chunk < _chunkCount; chunk++)
        {
            delete _pDataItemChunk[chunk];
        }
        _chunkCount = 0;
    }

    void CopyItemsToFreeList(VectorDataItem *pDataItem, int size)
    {
        for (int i = 0; i < size; i++)
        {
            VectorDataItem *pFreeTop = _pFreeList;

            pDataItem->_pNext = pFreeTop;
            _pFreeList = pDataItem;

            pDataItem++;
        }
    }

    void AddNewChunk()
    {
      bool logging = false;

        if (logging)
        {
            Serial.print("In Use Count: ");
            Serial.flush();
            Serial.print(GetInUseCount());
            Serial.print(" ");
            //Serial.println(EspFunctions::GetHeapCapsFreeSize());
            //Serial.println(EspFunctions::GetHeapCapsLargestFreeBlock());
            Serial.flush();
        }


        VectorDataItemProviderChunk *pNewChunk = new VectorDataItemProviderChunk(Environment.VectorItemChunkSize);

        if (logging)
        {
            Serial.println("Chunk allocated");
            Serial.flush();
        }

        _pDataItemChunk[_chunkCount] = pNewChunk;

        if (logging)
        {
            Serial.print("ChunkCount: ");
            Serial.println(_chunkCount);
            Serial.flush();
        }

        CopyItemsToFreeList(_pDataItemChunk[_chunkCount]->GetDataItems(), _pDataItemChunk[_chunkCount]->GetSize());

        if (logging)
        {
            Serial.println("Copied to free list");
            Serial.flush();
        }

        _chunkCount++;
    }

    VectorDataItem *GetDataItem()
    {
        if (_pFreeList != 0)
        {
            VectorDataItem *pDataItem = _pFreeList;
            _pFreeList = pDataItem->_pNext;
            pDataItem->_pNext = 0;
            _inUseCount++;

            //Serial.print("!!\t");
            //Serial.print((int)pDataItem);
            //Serial.println("\t>");

            return pDataItem;
        }

        // Allocate new chunk...

        if (_chunkCount == MaxChunks)
        {
            Serial.println("System Error: VectorDataItemProvider Maximum Chunks");
            return 0;
        }

        AddNewChunk();

        return GetDataItem();
    }

    int GetInUseCount()
    {
        return _inUseCount;
    }

    void ReleaseDataItem(VectorDataItem *pItem)
    {
        pItem->_pNext = _pFreeList;
        _pFreeList = pItem;
        _inUseCount--;

        //Serial.print("!!\t");
        //Serial.print((int)pItem);
        //Serial.println("\t<");

    }
};

class Vector
{
public:
    static VectorDataItemProvider *_pVectorDataItemProvider;
    static int _pVectorDataItemProviderSize;

    VectorDataItem *_pFirstItem;
    int _itemCount;

    VectorDataItem *_pLastMatch;
    int _lastMatchIndex;

    static int GetDataItemProviderInUseCount()
    {
        return _pVectorDataItemProvider->GetInUseCount();
    }

    static void RestartVectorDataProvider()
    {
        if (_pVectorDataItemProvider != 0)
        {
            delete _pVectorDataItemProvider;
            _pVectorDataItemProvider = 0;
        }
    }

    Vector()
    {
        if (_pVectorDataItemProvider == 0)
        {
            _pVectorDataItemProvider = new VectorDataItemProvider();
        }

        _itemCount = 0;
        _pFirstItem = 0;
    }

    ~Vector()
    {
        Cleanup();
    }

    void Cleanup()
    {
        if (_pFirstItem == 0)
        {
            return;
        }

        VectorDataItem *pItem = _pFirstItem;

        while (pItem->_pNext != 0)
        {
            VectorDataItem *pNext = pItem->_pNext;

            _pVectorDataItemProvider->ReleaseDataItem(pItem);

            pItem = pNext;
        }
        _pVectorDataItemProvider->ReleaseDataItem(pItem);

        _pFirstItem = 0;
        _itemCount = 0;
        _pLastMatch = 0;
        _lastMatchIndex = 0;
    }

    VectorDataItem *GetItemByIndex(int index)
    {
        VectorDataItem *pItem;
        int i;

        if (_pFirstItem == 0)
        {
            _pFirstItem = _pVectorDataItemProvider->GetDataItem();
            _pFirstItem->_pNext = 0;
            _pLastMatch = _pFirstItem;
            _lastMatchIndex = 0;
            _itemCount = 1;
        }

        // Forward traversal optimization; we keep track of the index and the pointer of the last get
        if (index >= _lastMatchIndex)
        {
            i = _lastMatchIndex;
            pItem = _pLastMatch;
        }
        else
        {
            i = 0;
            pItem = _pFirstItem;
        }

        for (; i < index; i++)
        {
            if (pItem->_pNext == 0)
            {
                pItem->_pNext = _pVectorDataItemProvider->GetDataItem();
                if (pItem->_pNext == 0)
                {
                    return 0;
                }
                pItem->_pNext->_data = NAN;

                _itemCount++;
                pItem->_pNext->_pNext = 0;
            }

            pItem = pItem->_pNext;
        }

        _pLastMatch = pItem;
        _lastMatchIndex = index;

        return pItem;
    }

    bool SetItem(int index, float value)
    {
        VectorDataItem *pItem = GetItemByIndex(index);

        if (pItem == 0)
        {
            Serial.print("VectorItemAddFail: Environment.VectorItemChunkSize: ");
            Serial.println(Environment.VectorItemChunkSize);
            return false;
        }

        pItem->_data = value;
        return true;
    }

    float GetItem(int index) const
    {
        VectorDataItem *pItem = ((Vector *)this)->GetItemByIndex(index); // stupid const correctness.

        return pItem->_data;
    }

    int GetItemCount()
    {
        return _itemCount;
    }
};

VectorDataItemProvider *Vector::_pVectorDataItemProvider = 0;
int Vector::_pVectorDataItemProviderSize = -1;