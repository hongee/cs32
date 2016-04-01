#ifndef DISK_LIST_H
#define DISK_LIST_H

#include "BinaryFile.h"
#include <cstring>

const BinaryFile::Offset NULLADDR = 0;

class DiskList
{
  public:
    DiskList(const std::string& filename);
    bool push_front(const char* data);
    bool remove(const char* data);
    void printAll();
  private:
    struct MetaData {
      BinaryFile::Offset head;
      BinaryFile::Offset noRemoved;
    }

    struct DiskListNode {
      char data[256]; //255 + 0 byte
      bool isRemoved;
      BinaryFile::Offset next;
      BinaryFile::Offset next_removed;
    };

    BinaryFile m_bf;
    MetaData m_meta;

    void updateMeta();
};


#endif //DISK_LIST_H
