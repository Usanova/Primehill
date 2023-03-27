#pragma hdrstop
#pragma argsused
#include <windows.h>

#include <stdio.h>

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

extern "C" int __declspec(dllexport) __stdcall GetCardInfoEx(__int64 Card, DWORD Restaurant, DWORD UnitNo,
	Info *info, char *InpBuf, int InpLen, WORD InpKind,
	const char *OutBuf, DWORD OutLen, WORD OutKind);

int _tmain(int argc, _TCHAR* argv[]) 
{
	Info i;
    const char* r = new char[12];
	GetCardInfoEx(100655, 12, 12, &i, new char[1], 1, 5, r, 12, 12);
	return 0;
}
