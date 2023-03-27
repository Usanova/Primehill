#pragma hdrstop
#pragma argsused
#include <windows.h>
#include <stdio.h>
#include <fstream>
#include <sstream>

using namespace std;

struct Info {
	WORD  size;
    BYTE  deleted;
	BYTE need_withdraw;
    BYTE expired;
	BYTE not_valid;
	BYTE manager_confirm;
	BYTE blocked;
	char block_reason[256];
	char card_owner[40];
	INT64 owner_id;
	DWORD account_number;
	DWORD not_pay_type;
	WORD bonus_num;
    WORD discount_num;
	INT64 max_discount;
	INT64  access_amount;
	INT64 acces_amount2;
	INT64 access_amount3;
	INT64 access_amount4;
	INT64 access_amount5;
	INT64 access_amount6;
	INT64 access_amount7;
	INT64 access_amount8;
	char comment[256];
	char screen_comment[256];
	char printer_comment[256];
};

 struct Transactionlist {
	INT64  Card;
	INT64  Kind;
	INT64  Summa;
};

extern "C" int __declspec(dllexport) __stdcall GetCardInfoEx(__int64 Card, DWORD Restaurant, DWORD UnitNo,
	Info *info, char *InpBuf, int InpLen, WORD InpKind,
	const char *OutBuf, DWORD OutLen, WORD OutKind);

extern "C" int __declspec(dllexport) __stdcall TransactionsEx( DWORD Count,
					Transactionlist *transactionlist,
					CHAR* InpBuf,
					DWORD InpLen,						// ����� ���������� �� �����
					WORD InpKind,						// ��� ���������� �� ����� (1-XML)
					CHAR* OutBuf,						// �������������� �������� ����������
					DWORD OutLen,						// ����� �������� ����������
					WORD OutKind);

#ifdef _WIN32
#include <tchar.h>
#else
  typedef char _TCHAR;
  #define _tmain main
#endif


int _tmain(int argc, _TCHAR* argv[])
{

	setlocale(LC_ALL, "Russian");
    std::locale::global(std::locale("Russian"));


	Info i;

	ifstream file("input1.xml");
	stringstream b;
	b << file.rdbuf();
	file.close();
	std::string content(b.str());
	char* input = &content[0];
	auto len = strlen(input);

	ifstream t_file("transaction1.xml");
	stringstream b1;
	b1 << t_file.rdbuf();
	t_file.close();
	std::string content1(b1.str());
char* input1 = &content1[0];
//	auto len1 = strlen(input1);

	const char* r = new char[12];
	GetCardInfoEx(1170188, 12, 12, &i, input, strlen(input), 5, r, 12, 12);
	//100655
	//101015
	//TransactionsEx(777888,0, input1, strlen(input1), 0, 0, 0, 0);
	return 0;
}
