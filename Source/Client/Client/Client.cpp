// Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Client.h"
#include "afxsock.h"
#include <conio.h>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;
bool hengio() {
	int timmer = 0;
	int s = 0;
	int m = 0;
	cout << "           ";
	while (true) {
		cout << m / 60 << ":" << s << " ";
		s++;
		Sleep(1000);
		if (s == 59) {
			break;
			return false;
		}
		//system("cls");
	}
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{

		// TODO: code your application's behavior here.
		CSocket client;
		char sAdd[1000];
		unsigned int port = 1234; //Cung port voi server
		AfxSocketInit(NULL);

		cout << "           Nhap dia chi IP cua server: ";
		fflush(stdin);
		gets_s(sAdd);
		cout << endl;
		//1. Tao socket
		client.Create();
		if (client.Connect(CA2W(sAdd), port))
		{
			printf("           Ban da ket noi toi server\n\n");

			system("Color 6");

			cout << "           ======================================================" << endl;
			cout << "           ||                                                  ||" << endl;
			cout << "           ||              C  O  N  F  E  T  T  I              ||" << endl;
			cout << "           ||                                                  ||" << endl;
			cout << "           ||                W  E  L  C  O  M  E               ||" << endl;
			cout << "           ||                                                  ||" << endl;
			cout << "           ======================================================" << endl << endl << endl;

			//Nhan tu server, cho biet day la client thu may
			int id;
			client.Receive((char*)&id, sizeof(id), 0);
			printf("\n           Ban la nguoi choi thu %d\n", id + 1);

			char ClientMsg[500];
			int MsgSize;
			char* temp;
			cout << "\n           --------------Nhap nickname cua ban(vd: @Nickname):------------ \n";
			cout << "           Nhap o day: ";
			fflush(stdin);
			gets_s(ClientMsg);
			while (ClientMsg[0] != '@') {
				cout << "           Nickname khong hop le, vui long nhap lai: ";
				fflush(stdin);
				gets_s(ClientMsg);
			}
			MsgSize = strlen(ClientMsg);
			client.Send(&MsgSize, sizeof(MsgSize), 0);
			// Gui di nickname voi do dai la MsgSize
			client.Send(ClientMsg, MsgSize, 0);

			//Nhan thong bao nickname co hop le khong
			int x;
			client.Receive((char*)&x, sizeof(x), 0);
			while (x > 1) {
				cout << "           Nhap nickname bi trung voi nguoi choi truoc, vui long nhap lai: ";
				fflush(stdin);
				gets_s(ClientMsg);
				while (ClientMsg[0] != '@') {
					cout << "           Nickname khong hop le, vui long nhap lai: ";
					fflush(stdin);
					gets_s(ClientMsg);
				}
				MsgSize = strlen(ClientMsg);
				client.Send(&MsgSize, sizeof(MsgSize), 0);
				// Gui di nickname voi do dai la MsgSize
				client.Send(ClientMsg, MsgSize, 0);

				//Nhan thuong bao nicknam co hop le khong
				client.Receive((char*)&x, sizeof(x), 0);
			}
			cout << "           Nickname hop le" << endl << endl;

			cout << "\n           -------------------------- \n";
			int socauhoi;
			client.Receive((char*)&socauhoi, sizeof(socauhoi), 0);
			cout << "           So cau hoi can tra loi: " << socauhoi << endl;

			int socaudung;
			int j = 0;
			string w = "0";
			while (j < socauhoi) {
				client.Receive((char*)&MsgSize, sizeof(int), 0); // Neu nhan loi thi tra ve la SOCKET_ERROR.			
				temp = new char[MsgSize + 1];
				temp[MsgSize] = '\0';
				client.Receive((char*)temp, MsgSize, 0);

				cout << "\n           ------------- Cau hoi " << j + 1 << "------------- \n";
				cout << "           " << temp << endl;
				cout << "           Ban co 60s de tra loi" << endl;
				strcpy(ClientMsg, "NULL");

				//hengio();
				int s = 0;
				int o = 0;
				strcpy(ClientMsg, w.c_str());
				cout << "           Nhap dap an: ";
				while (s < 60) {
					Sleep(1000);
					s++;
					if (_kbhit()) {
						fflush(stdin);
						gets_s(ClientMsg);
						if (ClientMsg[0] != NULL) {
							MsgSize = strlen(ClientMsg);
							client.Send(&MsgSize, sizeof(MsgSize), 0);
							client.Send(ClientMsg, MsgSize, 0);
						}

						else {
							char dapansai[2] = "0";
							strcpy(ClientMsg, dapansai);
							MsgSize = strlen(ClientMsg);
							client.Send(&MsgSize, sizeof(MsgSize), 0);
							client.Send(ClientMsg, MsgSize, 0);
						}
						o = 1;
						break;
					}
				}
				if (o == 0) {
					char dapansai[2] = "0";
					strcpy(ClientMsg, dapansai);
					MsgSize = strlen(ClientMsg);
					client.Send(&MsgSize, sizeof(MsgSize), 0);
					client.Send(ClientMsg, MsgSize, 0);
				}

				client.Receive((char*)&x, sizeof(x), 0);
				int k = x;
				client.Receive((char*)&x, sizeof(x), 0);
				int p = x;
				cout << "\n           So cau da tra loi dung: " << k << "/" << p + 1 << " cau" << endl << endl;
				j++;
				socaudung = k;
			}

			//Nhan danh sach nhung nguoi thang cuoc:
			client.Receive((char*)&MsgSize, sizeof(int), 0); // Neu nhan loi thi tra ve la SOCKET_ERROR.			
			temp = new char[MsgSize + 1];
			temp[MsgSize] = '\0';
			client.Receive((char*)temp, MsgSize, 0);
			cout << "\n           --------------Nhung nguoi chien thang------------ \n";
			cout << "           " << temp <<endl;

			cout << "\n           -------------------------- \n";
			int win;
			client.Receive((char*)&win, sizeof(win), 0);
			if (socaudung == win) {
				cout << "           $ $ $ $ BAN LA NGUOI THANG CUOC $ $ $ $" << endl;
			}
			else cout << "           CHUC BAN MAY MAN LAN SAU" << endl;
			cout << "\n           -------------------------- \n";
			cout << "           XIN CHAO VA HEN GAP LAI" << endl;
			cout << endl;
			delete temp;
			system("pause");
		}
		else
			printf("           Khong connect duoc toi server....");
		client.Close();
	}
	getchar();
	return nRetCode;
}