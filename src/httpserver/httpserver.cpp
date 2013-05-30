#include "StdAfx.h"
#include "server.h"

HttpServer a(80);
static DWORD WINAPI mainT(LPVOID lpParam)
 {
	a.start();
	return 1;
 }

int main(){
	HANDLE       mainThread;
	LPDWORD		ThreadId=0;
	mainThread=CreateThread(NULL,NULL,mainT,NULL, NULL, ThreadId); 
	int flag=1;
	while (true)
	{
		string order;
		cin>>order;
		if (order=="stop" && flag==1)
		{
				a.stop();
				flag=0;
		}else if (order=="stop")
		{fflush(stdin);
			cout<<"The http server is alread stoped "<<endl;
		}

		if (order=="start" && flag==0)
		{
			a.start();
			flag=1;
		}else if (order=="start")
		{
			cout<<"The http server is running"<<endl;
		}
		if (order=="changePort" && flag==0)
		{
			int p;
			cin>>p;
			a.PORT=p;
		} else if (order=="changePort")
		{
			fflush(stdin);
			cout<<"Please stop the service before you change the PORT"<<endl;
		}

		if (order=="changePort" && flag==0)
		{
			int p;
			cin>>p;
			a.PORT=p;
		} else if (order=="changeDic")
		{
			fflush(stdin);
		}
	}
	return 0;
}
