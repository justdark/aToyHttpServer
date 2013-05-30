#ifndef _SERVER_
#define _SERVER_

#include <string>
#include <winsock2.h>
#include <stdio.h>
#include <iostream>
#pragma comment(lib,"ws2_32.lib")
#include <vector>
#include <fcntl.h>
#include <map>
#include <fstream>
#include <string>
#include "response.h"
using namespace std;
#define SIZE_BUFFER 2048

  class HttpServer
  {
  public:
	  int PORT;
	  string HTTP_FILE;
	  string stopsymbol;
	  WSAData wsadata;
	  HttpServer(int a):PORT(a)
	  {}
	  void start();
	  void stop();

	 

		static void make_response(SOCKET s,char *buffer,int len)
		{
		  response res(s,string(buffer));
		  map <string, string>::iterator m1_Iter;
		  for ( m1_Iter=res.value.begin();m1_Iter!=res.value.end();m1_Iter++)
			 cout<<m1_Iter->first<<"  : "<<m1_Iter->second<<endl;;
		}
		static  bool GetAddressBySocket(SOCKET m_socket,SOCKADDR_IN &m_address)//¶ÁÈ¡IPºÍPort
		{
			memset(&m_address, 0, sizeof(m_address));
			int nAddrLen = sizeof(m_address);
			if(::getpeername(m_socket, (SOCKADDR*)&m_address, &nAddrLen) != 0)
			{
			 printf("Get IP address by socket failed!n");
			 return false;
			}
			//cout<<"IP: "<<::inet_ntoa(m_address.sin_addr)<<"  PORT: "<<ntohs(m_address.sin_port)<<endl;
			return true;
		}

	 static DWORD WINAPI AnswerThread(LPVOID lparam)
		{
		int len = 0;  
		char buffer[SIZE_BUFFER]; 
		SOCKET  ClientSocket=(SOCKET)(LPVOID)lparam;

		memset(buffer,0,SIZE_BUFFER);

		 if((len = recv(ClientSocket,buffer,SIZE_BUFFER,0))>0){
			 SOCKADDR_IN ad;
			 GetAddressBySocket(ClientSocket,ad);
			 cout<<"from  "<<::inet_ntoa(ad.sin_addr)<<"  request for: ";
			 make_response(ClientSocket,buffer,len);
			//printf("%s",buffer); 
			//send(ClientSocket, protocolHead,strlen(protocolHead),0);
			}
		
		return 1; 
		}
  };


#endif