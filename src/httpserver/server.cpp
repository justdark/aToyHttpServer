#include "StdAfx.h"
#include "server.h"
void HttpServer::start()
{
	stopsymbol="";
	cout<<"start service successful"<<endl;
	WSAStartup(MAKEWORD(2,0),&wsadata);  
	SOCKET ssocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);//创建一个TCP的Socket，但是没有绑定  
	struct sockaddr_in server;  
	server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");  //inet_addr("192.168.1.107");  
	server.sin_family = AF_INET;  
	server.sin_port = htons(PORT);//创建一个端口地址，指定地址和端口号  
	   
	if(bind(ssocket,(sockaddr*)&server,sizeof(sockaddr_in)) == SOCKET_ERROR)
	{  
		std::cout<<"error bind"<<std::endl;  
	}//绑定TCP Socket到指定的端口地址  
	if(listen(ssocket, 4) == SOCKET_ERROR)
	{  
		std::cout<<"listen error"<<std::endl;  
	}  
	fd_set read_fdset;//读文件描述符  
	std::vector<int> sockets;//读端口的集合，每监听到一个请求，就会新建一个端口，并且存放到该集合中  
	HANDLE       hThread;
	DWORD        dwThreadId; 
	SOCKET       sClient; 
	while(true){  
		if (stopsymbol=="stop")
		{
			closesocket(ssocket);
			break;
		}
		FD_ZERO(&read_fdset);  
		FD_SET(ssocket,&read_fdset); 
		int n = select(0,&read_fdset,NULL,NULL,0);  
		if(FD_ISSET(ssocket,&read_fdset))
		{//如果监听端口接收到了读的请求，也就是客户端发起了请求连接  
			SOCKET sClient = accept(ssocket,NULL,0);//创建一个新的端口 

			if(n!=SOCKET_ERROR)
			{  
				hThread=CreateThread(NULL,NULL,HttpServer::AnswerThread,(LPVOID)sClient, 0, &dwThreadId); 
			//if(hThread==NULL)    
			//	printf("CreatThread AnswerThread() failed./n"); 
			//else 
			//	printf("CreateThread OK./n");    
			}  
		}  
	}  

}

void HttpServer::stop()
{
	stopsymbol="stop";
	cout<<"stop service successful"<<endl;
}
