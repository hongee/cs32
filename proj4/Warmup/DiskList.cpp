#include <iostream>
#include <string>
#include "DiskList.h"
#include <cassert>

DiskList::DiskList(const std::string& filename)
: m_head(0)
{
  if(!m_bf.openExisting(filename)) {
    cerr << filename << " not found. Creating the file." << endl;
    m_bf.createNew(filename);
    //the address (offset) of the first node is stored in an int at offset 0
    //the first node is null
    BinaryFile::Offset i = NULLADDR;
    if(!m_bf.write(i, 0)) {
      cerr << "Writing initial head failed!" << endl;
    } else {
      m_head = i;
      cerr << "Successfully wrote initial head, next item to be written at address: " << m_nextWrite << endl;
    }
  } else {
    //the file exists. retrieve the head pointer.
    if(!m_bf.read(m_head, 0)) {
      cerr << "Error reading head address from existing file." << endl;
    } else {
      cerr << "Head address found, is: " << m_head << endl;
    }
  }
}

bool DiskList::push_front(const char* data) {
  if(strlen(data) >= 256) {
    cerr << "Length of string exceeds or equals 256. Aborting." << endl;
    return false;
  }

  BinaryFile::Offset writeToLoc;
  bool isOverwriting = false;

  //check if there are places to overwrite
  if(m_head != NULLADDR) {
    DiskListNode n;
    for(BinaryFile::Offset i = sizeof(BinaryFile::Offset); i < m_bf.fileLength(); i += sizeof(DiskListNode)) {
      if(!m_bf.read(n, i)) {
        cerr << "Read failed! Aborting." << endl;
        break;
      } else if(n.isRemoved) {
        cerr << "Found removed!" << endl;
        isOverwriting = true;
        writeToLoc = i;
        break;
      }
    }
  }

  DiskListNode n;
  strcpy(n.data, data);
  n.next = m_head;
  n.isRemoved = false;

  if(isOverwriting) {
    if(m_bf.write(n, writeToLoc)) {
      cerr << "Successfully wrote to location: " << writeToLoc << endl;
      m_head = writeToLoc;
      updateHead();
      return true;
    }
  }

  if(!m_bf.write(n, m_nextWrite)) {
    cerr << "Attempt to write new node failed!" << endl;
  } else {
    cerr << "Successfully wrote to location: " << m_nextWrite << endl;
    m_head = m_nextWrite;
    m_nextWrite += sizeof(DiskListNode);
    updateHead();
    return true;
  }
}

void DiskList::updateHead() {
  if(!m_bf.write(m_head, 0)) {
    cerr << "Failed to update file head!" << endl;
  }
}

bool DiskList::remove(const char* data) {
  if(m_head == NULLADDR) return false;

  bool hasRemoved = false;

  DiskListNode i, prev;
  i.next = m_head;
  prev.next = m_head;
  BinaryFile::Offset prevOffset = 0;
  do {
    if(!m_bf.read(i, i.next)) {
      cerr << "Read from file at add: " << i.next << " failed! Aborting." << endl;
      return false;
    } else {
      if(!i.isRemoved && strcmp(i.data,data) == 0) {
        hasRemoved = true;
        i.isRemoved = true;
        if(prevOffset == 0) {
          //is at the head
          m_head = i.next;
          m_bf.write(i, prev.next);
          updateHead();
        } else {
          m_bf.write(i, prev.next);
          prev.next = i.next;
          m_bf.write(prev, prevOffset);
        }
      } else {
        prevOffset = prev.next;
        prev = i;
      }
    }
  } while(i.next != NULLADDR);

  return hasRemoved;
}

void DiskList::printAll() {
  //the list is empty
  if(m_head == NULLADDR) return;
  DiskListNode i;
  i.next = m_head;
  do {
    if(!m_bf.read(i, i.next)) {
      cerr << "Read from file at add: " << i.next << " failed! Aborting." << endl;
      return;
    } else {
      cout << i.data << endl;
    }
  } while(i.next != NULLADDR);
}

int main() {
  DiskList a("pikachu.dat");
  a.push_front("charmander");
  a.push_front("pidgey");
  a.push_front("pikachu");
  a.printAll();
  cerr << "----------------" << endl;
  a.remove("pidgey");
  a.remove("charmander");
  a.printAll();
}
