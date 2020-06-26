// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Server.h"
#include "afxsock.h"
#include "math.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <time.h>
#include <ctime>
#include <cstdlib> 
#include <vector>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

//Kiem tra co phai la so nguyen to hay khong
bool isPrime(int n) {
	if (n < 2)
		return false;
	for (int i = 2; i <= (int)sqrt((double)n); i++) {
		if (n%i == 0) {
			return false;
		}
	}
	return true;
}

int kiemtraten(vector<string> nickname, string ten) {
	int dem = 0;
	for (int i = 0; i < nickname.size(); i++)
		if (nickname[i] == ten)
			dem++;
	return dem;
}

void laycauhoi(string cauhoi[]) {
	fstream file("Cauhoi.txt");
	string noidung;
	if (file.is_open())
	{
		int i = 0;
		//getline - doc toi khi xuong dong
		while (getline(file, noidung))
		{
			cauhoi[i] = noidung;
			i++;
		}
		file.close();
	}
}

struct kieucauhoi {
	int chiso;
	string cauhoi;
};

kieucauhoi Layngaunhien1cau(string x[]) {
	kieucauhoi c;
	int id = rand() % 20 + 0;
	c.chiso = id;
	c.cauhoi = x[id];
	//cout << id <<endl;
	return c;
}

bool kiemtratrungnhau(vector<int> k, int id) {
	for (int i = 0; i < k.size(); i++) {
		if (id == k[i]) {
			return false;
		}
	}
	return true;
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
		CSocket server;
		unsigned int port = 1234;
		int i;
		AfxSocketInit(NULL);

		server.Create(port);
		server.Listen(5);

		//Nhap so luong client
		printf("Nhap so luong nguoi choi (khong duoc bo trong): ");
		int num_client;
		scanf("%d", &num_client);
		printf("\nDang lang nghe ket noi tu nguoi choi...\n");
		//Tao mang chua cac socket client
		CSocket * sockClients = new CSocket[num_client];

		for (i = 0; i < num_client; i++) {
			server.Accept(sockClients[i]);
			printf("Da tiep nhan nguoi choi %d/%d\n", i + 1, num_client);

			//Bao cho client biet minh la client thu may
			sockClients[i].Send((char*)&i, sizeof(int), 0);
		}

		vector<string> nickname;

		char ServerMsg[100];
		int MsgSize;
		char *temp;

		//Nhan nickname
		for (int i = 0; i < num_client; i++) {
			sockClients[i].Receive((char*)&MsgSize, sizeof(int), 0);
			temp = new char[MsgSize + 1];
			temp[MsgSize] = '\0';
			sockClients[i].Receive((char*)temp, MsgSize, 0);
			nickname.push_back(temp);
		}

		for (int i = 0; i < num_client; i++) {
			if (nickname[i] == "") {
				cout << "Nguoi choi 1 da thoat" << endl;
			}
		}

		cout << endl;

		for (int i = num_client - 1; i >= 0; i--) {
			//kiem tra ten:
			int dem = kiemtraten(nickname, nickname[i]);
			// Gui di thong bao nickname co hop le khong
			sockClients[i].Send((char*)&dem, sizeof(int), 0);
			while (dem > 1) {
				/*nickname.pop_back();
				nickname.erase(nickname.begin() + 1);*/

				sockClients[i].Receive((char*)&MsgSize, sizeof(int), 0); // Neu nhan loi thi tra ve la SOCKET_ERROR.			
				temp = new char[MsgSize + 1];
				temp[MsgSize] = '\0';
				sockClients[i].Receive((char*)temp, MsgSize, 0);

				//nickname.push_back(temp);
				nickname[i] = temp;
				for (int z = 0; z < num_client; z++) {
					cout << nickname[z] << " ";
				}

				dem = kiemtraten(nickname, nickname[i]);
				sockClients[i].Send((char*)&dem, sizeof(int), 0);
			}
		}
		
		for (int i = 0; i < num_client; i++) {
			cout << "Nguoi choi " << i + 1 << ": " << nickname[i] << endl;
		}
		cout << endl;

		//Gui di so cau hoi:
		int socauhoi;
		cout << "Nhap so cau hoi: "; cin >> socauhoi;
		for (int i = 0; i < num_client; i++) {
			sockClients[i].Send((char*)&socauhoi, sizeof(int), 0);
		}

		//mang random cau hoi:
		string cauhoi[20];
		laycauhoi(cauhoi);
		string* c = new string[socauhoi];
		vector<int> id;
		srand(time(NULL));
		for (int i = 0; i < socauhoi; i++) {
			kieucauhoi kc = Layngaunhien1cau(cauhoi);
			while (kiemtratrungnhau(id, kc.chiso) == false)
			{
				kc = Layngaunhien1cau(cauhoi);
			}
			c[i] = kc.cauhoi;
			id.push_back(kc.chiso);
		}

		string* dapan = new string[socauhoi];
		cout << "Cac cau hoi xe duoc gui: " << endl;
		for (int i = 0; i < socauhoi; i++) {
			dapan[i] = c[i].substr(0, 1);
			c[i] = c[i].substr(1, c[i].length());
			cout << c[i] << endl;
		}

		cout << endl;

		//Mang so cau tra loi dung:
		int* socaudung = new int[num_client];
		for (int i = 0; i < num_client; i++) {
			socaudung[i] = 0;
		}

		//Gui tuan tu cac cau hoi da duoc random va nhan cac cau tra loi:
		int j = 0;
		while (j < socauhoi) {
			for (int i = 0; i < num_client; i++) {
				strcpy(ServerMsg, c[j].c_str());
				MsgSize = strlen(ServerMsg);

				sockClients[i].Send(&MsgSize, sizeof(MsgSize), 0);
				sockClients[i].Send(ServerMsg, MsgSize, 0);
			}
			for (int i = 0; i < num_client; i++) {
				sockClients[i].Receive((char*)&MsgSize, sizeof(int), 0);			
				temp = new char[MsgSize + 1];
				temp[MsgSize] = '\0';
				sockClients[i].Receive((char*)temp, MsgSize, 0);
				cout << "Cau " << j + 1 << ", nguoi choi " << i + 1 << " tra loi: " << temp << endl;

				if (temp == dapan[j]) socaudung[i] ++;

				//Gui di so cau dung cho nguoi choi:
				sockClients[i].Send((char*)&socaudung[i], sizeof(int), 0);
				//Gui di so cau da tra loi choi nguoi choi:
				sockClients[i].Send((char*)&j, sizeof(int), 0);
			}
			j++;
		}
		cout << endl;
		for (int i = 0; i < num_client; i++) {
			cout << "Nguoi choi " << i + 1 << " dung: " << socaudung[i] << " cau" << endl;
		}

		int win = socaudung[0];
		for (int i = 0; i < num_client; i++) {
			for (int j = 0; j < num_client; j++) {
				if (socaudung[j] > socaudung[i]) {
					win = socaudung[j];
				}
			}
		}

		string nhungnguoichienthang = "";
		cout << "Nhung nguoi choi danh chien thang: " <<endl;
		for (int i = 0; i < num_client; i++) {
			if (socaudung[i] == win) {
				nhungnguoichienthang += nickname[i] + "\n           ";
				cout << nickname[i] << "\n";
			}
		}
		cout << endl;
		//Gui danh sach nhung nguoi chien thang:
		for (int i = 0; i < num_client; i++) {
			strcpy(ServerMsg, nhungnguoichienthang.c_str());
			MsgSize = strlen(ServerMsg);

			sockClients[i].Send(&MsgSize, sizeof(MsgSize), 0);
			sockClients[i].Send(ServerMsg, MsgSize, 0);
		}

		for (int i = 0; i < num_client; i++) {
			//Gui di so cau hoi cua nguoi chien thang:
			sockClients[i].Send((char*)&win, sizeof(int), 0);
			sockClients[i].Close();
		}

		delete temp;
		getchar();
		server.Close();
	}
	getchar();
	return nRetCode;
}

