/*
* userNetwork.h
*
*  Created on: 23 janv. 2014
*      Author: Sarkal
*/
#ifdef step4
#ifndef __USERNETWORK__H__
#define __USERNETWORK__H__

#include "synch.h"

class Socket {

	public :
		int getId();
		void setId(int);
		char* getName();
		void setName(char*);
		int getTo();
		void setTo(int);
		int getFrom();
		void setFrom(int);
		char* getBuffer();
		void setBuffer(char*);

		int do_Send(int idSocket, char* message);
		char* do_Receive(int idSocket);
		int do_SockInit(int newTo, int newFrom, char* newBuffer);
		void do_SockClose(int idSocket);

	private:
		int id;
		int nextId;
		char *name;
		int to;
		int from;
		char *buffer;

};
#endif // __USERNETWORK__H__
#endif //step6
