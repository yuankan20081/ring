#include "ring.hpp"
#include <iostream>
#include <cstring>

using namespace std;

int main()
{
  Ring buf(1024);
  const char *word = "Hello World";
  cout << "write: " << buf.Write(word, strlen(word)) << endl;
  char b[100] = {0};
  cout << "read: " << buf.Read(b, sizeof(b)) << endl;
  cout << b << endl;
  word = "this is another test?!";
  cout << "write: " << buf.Write(word, strlen(word)) << endl;
  memset(b, 0, sizeof(b));
  cout << "read: " << buf.Read(b, sizeof(b)) << endl;
  cout << b << endl;
  return 0;
}
