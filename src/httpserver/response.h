#ifndef __RESPONSE_H_
#define __RESPONSE_H_
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
using namespace std;

const string src("a");
class response
{
public:
	string response_code;
	SOCKET s;
	string request;
	int Content_Length;
	vector<string> line_request;
	map<string,string> value;
	string Content_Type;
	string file;
	string extension;
	string Connection;
	string Server;
	void work()
	{
		fstream f;
		string sss=src+file;
		cout<<file<<endl;
		f.open(sss,ios::in);
		if(!f)
		{
			response_code="HTTP/1.1 404 Not Found";
			output_header("a/404.html");
			output_file("a/404.html");
			f.close();
			return;
		}
		response_code="HTTP/1.1 200 OK";
		if (extension=="php")
		{
			//sss=phps::make_php(sss);
		}
		output_header(sss.c_str());
		output_file(sss.c_str());
		f.close();
	}
	void output_header(const char *f)
	{
		int length;
		ifstream is;
		is.open (f, ios::binary);
		is.seekg (0, ios::end);
		length = is.tellg();
		length+=2;
		char pp[25];
		itoa(length,pp,10);
		string contentlength="Content-Length: "+string(pp)+"\r\n";

		string temp=response_code+"\r\n"+Server+"\r\n"+contentlength+"Content-Type: "+Content_Type+"\r\n"
			       +Connection+"\r\n";

		char *bufs=new char[ temp.length() + 1 ];
		strcpy( bufs,temp.c_str());
		int len=temp.length();
		sendall(s,bufs,&len);
	}
	void output_file(const char* f)
	{
		 FILE* read_from; 
		 int readed = -1;   
		 read_from = fopen(f, "rb"); 
		 if(read_from != NULL) 
		 {        
			char read_buf[128];        //读文件时的字节缓存数组

			send(s, "\r\n", 2, 0);      //再加一个"\r\n" 不能缺少 格式要求
			while(!feof(read_from)) 
			{      //判断文件是否已经结束
				fread(read_buf, 16,8, read_from);   //读取
				int len = sizeof(read_buf);
				if (sendall(s, read_buf, &len) == -1) 
				{ //发送数据
					printf("Sending error!");    //出现发送错误显示到控制台继续发送
					continue;
				}
			}
		}
		 closesocket(s);
		//send(s, "\r\n", 2, 0); 
	}
	static int sendall(int s, char *buf, int *len) 
	  {
		 //cout<<buf;
		int total = 0;           // 已经发送字节数
		int bytesleft = *len;                                   //还剩余多少字节
		int n=0;
		while(total < *len) 
		{
			n = send(s, buf+total, bytesleft, 0);
			if (n == -1) { break; }
			total += n;
			bytesleft -= n;
		}
		*len = total;           // 返回实际发送出去的字节数
		return n==-1?-1:0;          // 成功发送返回0 失败-1
	 }

	static vector<string> split(string& str,const char* c)
	{
		char *cstr, *p;
		vector<string> res;
		cstr = new char[str.size()+1];
		strcpy(cstr,str.c_str());
		p = strtok(cstr,c);
		while(p!=NULL)
		{
			res.push_back(p);
			p = strtok(NULL,c);
		}
		return res;
	}
	response(SOCKET ss,string a)
	{
		s=ss;
		request=a;
		line_request=split(request,"\n");
		//for (int i=0;i<line_request.size();i++)
		//	cout<<line_request[i]<<endl;
		Server="Server: DHS/1.0";
		Connection="Connection: close";
		if (line_request[0][0]=='G' || line_request[0][0]=='g')
		{
			make_get_request();
		} else
		if (line_request[0][0]=='P' || line_request[0][0]=='p')
		{
			make_post_request();
		} else
		make_bad_request();
		work();
	}
	void make_bad_request(){};
	void make_post_request()
	{
		vector<string> content=split(line_request[0]," ");
		string vv;
		int ct;
		for (int i=0;i<line_request.size();i++)
			if(line_request[i].substr(0,14)=="Content-Length")
			{
				ct=atoi(line_request[i].substr(15,string::npos).c_str());
				break;
			}
		//cout<<ct<<endl;
		file=content[1];
		if (file.length()==1 && file[0]=='/')
			file="/index.html";
		int position=request.find_first_of("\n\n");
		if (position==string::npos)
			position=request.find_first_of("\r\n\r\n");
		vv=request.substr(request.length()-ct,ct);
		vector<string> v=split(vv,"&");
		//cout<<vv<<endl;
		for (int i=0;i<v.size();i++)
		{
			int p=v[i].find('=');
			value[v[i].substr(0,p)]=v[i].substr(p+1,v[i].length()-p-1);
		}

		extension="";
		for (int i=file.length()-1;i>=0;i--)
		{
			if (file[i]=='.')	
			{
				extension=file.substr(i+1,file.length()-i-1);
				break;
			}
		}
		//map <string, string>::iterator m1_Iter;
		//for ( m1_Iter=value.begin();m1_Iter!=value.end();m1_Iter++)
		//	cout<<m1_Iter->first<<"  "<<m1_Iter->second<<endl;
		if (extension=="jpg" || extension=="jpeg" || extension=="jpe")
			Content_Type = "image/jpeg";
			else if (extension=="gif")
			Content_Type = "image/gif";
			else if (extension=="png")
			Content_Type="image/png";
			else if (extension=="pdf")
			Content_Type="application/pdf";
			else Content_Type="text/html";
	};
	void make_get_request()
	{
		vector<string> content=split(line_request[0]," ");
		vector<string> con;
		if (content[1].find('?')!=string::npos)
		{
			con=split(content[1],"?");
			if (con[1].length()>0)
			{
				vector<string> v=split(con[1],"&");
				for (int i=0;i<v.size();i++)
				{
					int p=v[i].find('=');
					value[v[i].substr(0,p)]=v[i].substr(p+1,v[i].length()-p-1);
				}
			}
			file=con[0];
		} else
			file=content[1];

		if (file.length()==1 && file[0]=='/')
			file="/index.html";

		extension="";
		for (int i=file.length()-1;i>=0;i--)
		{
			if (file[i]=='.')	
			{
				extension=file.substr(i+1,file.length()-i-1);
				break;
			}
		}
		//map <string, string>::iterator m1_Iter;
		//for ( m1_Iter=value.begin();m1_Iter!=value.end();m1_Iter++)
		//	cout<<m1_Iter->first<<"  "<<m1_Iter->second<<endl;
		if (extension=="jpg" || extension=="jpeg" || extension=="jpe")
			Content_Type = "image/jpeg";
			else if (extension=="gif")
			Content_Type = "image/gif";
			else if (extension=="png")
			Content_Type="image/png";
			else if (extension=="pdf")
			Content_Type="application/pdf";
			else Content_Type="text/html";
		
	}
};

#endif