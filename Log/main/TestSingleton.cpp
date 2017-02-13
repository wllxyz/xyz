#include "WllSingleton.h"
#include <iostream>
using namespace std;

class A
{
public:
	A(){ value=0; }
	int Get() { return value; }
	void Set(int value) { this->value = value; }
private:
	int value;
};

int main()
{
	A* instance = Singleton<A>::GetInstance();
	instance->Set(5);
	
	cout<<instance->Get()<<endl;
	cout<<instance<<endl;
	cout<<Singleton<A>::GetInstance()<<endl;
	cout<<Singleton<A>::GetInstance()->Get()<<endl;
	cout<<GetInstance<A>()<<endl;
	cout<<GetInstance<A>()->Get()<<endl;
	
	return 0;
}

