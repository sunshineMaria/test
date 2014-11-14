#include <iostream>
#include "./testclass.cpp"

using namespace std;
int main(){
  TestClass tc;
  tc.Init();
  cout<<"tc.a="<<tc.a<<endl;
  cout<<"hello world."<<endl;
}
