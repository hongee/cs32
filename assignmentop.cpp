/***
  * Assignment Operator
  */

#include <iostream>

using namespace std;

struct Book
{
   string title;
   string author;
};

class CSNerd
{
public:
   CSNerd(string name) {
     m_myBook = nullptr;
     m_myName = name;
   }
   void giveBook(string t, string a) {
     m_myBook = new Book;
     m_myBook->title = t;
     m_myBook->author = a;
   }
   ~CSNerd() {
     delete m_myBook;
   }
   CSNerd& operator=(const CSNerd& src) {
     if(&src == this) return *this;
     delete m_myBook;
     m_myBook = new Book(*src.m_myBook); //??
     //*m_myBook = src.*m_myBook //- this doesnt work either
     m_myName = src.m_myName;
     return *this;
   }

   Book *m_myBook;
   string m_myName;
};

int main() {
 CSNerd a("Zick");
 CSNerd b("Nick");
 a.giveBook("Bla", "CLRS");

 b=a;

 cout << b.m_myBook->title;

}


/*
  REMINDER for operators
  - return *this for constructor to enable chaining
  - check that we are not alias of each other (this == whatevers passed in)
  - memory management
*/
