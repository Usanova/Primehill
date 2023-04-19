//---------------------------------------------------------------------------
// SL
#include <vcl.h>
#include <windows.h>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#include <wininet.h>
#pragma comment(lib,"wininet")

#include <stdlib.h>
#include <string.h>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "rapidxml.hpp"

#include "httpClient.h"
#include "checkXml.h"

#pragma pack(1)
#pragma hdrstop

using namespace std;
//---------------------------------------------------------------------------
//   Important note about DLL memory management when your DLL uses the
//   static version of the RunTime Library:
//
//   If your DLL exports any functions that pass String objects (or structs/
//   classes containing nested Strings) as parameter or function results,
//   you will need to add the library MEMMGR.LIB to both the DLL project and
//   any other projects that use the DLL.  You will also need to use MEMMGR.LIB
//   if any other projects which use the DLL will be performing new or delete
//   operations on any non-TObject-derived classes which are exported from the
//   DLL. Adding MEMMGR.LIB to your project will change the DLL and its calling
//   EXE's to use the BORLNDMM.DLL as their memory manager.  In these cases,
//   the file BORLNDMM.DLL should be deployed along with your DLL.
//
//   To avoid using BORLNDMM.DLL, pass string information using "char *" or
//   ShortString parameters.
//
//   If your DLL uses the dynamic version of the RTL, you do not need to
//   explicitly add MEMMGR.LIB as this will be done implicitly for you
//---------------------------------------------------------------------------

#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
		ofstream fnews("init.txt",ios::out|ios::binary);
		return 1;
}
//---------------------------------------------------------------------------
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
	INT64 access_amount;
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

INT64* getAccessAmountByNumber(Info *info, int number)
{
	 switch(number) {
		case 1 : return &info->access_amount;
		case 2 : return &info->acces_amount2;
		case 3 : return &info->access_amount3;
		case 4 : return &info->access_amount4;
		case 5 : return &info->access_amount5;
		case 6 : return &info->access_amount6;
		case 7 : return &info->access_amount7;
		case 8 : return &info->access_amount8;
	}
}

void setAccessAmountByNumber(Info *info, int number, INT64 value)
{
	 switch(number) {
		case 1 : info->access_amount = value; break;
		case 2 : info->acces_amount2 = value; break;
		case 3 : info->access_amount3 = value; break;
		case 4 : info->access_amount4 = value; break;
		case 5 : info->access_amount5 = value; break;
		case 6 : info->access_amount6 = value; break;
		case 7 : info->access_amount7 = value; break;
		case 8 : info->access_amount8 = value; break;
	}
}

 struct Transactionlist {
	INT64  Card;
	INT64  Kind;
	INT64  Summa;
};

vector<char> ToANSI(const char *utf8str)
{

	// UTF-8 to wstring
	wstring_convert<codecvt_utf8<wchar_t>> wconv;
	wstring wstr = wconv.from_bytes(utf8str);
	// wstring to vector

	vector<char> buf(wstr.size()+1);
	use_facet<ctype<wchar_t>>(locale(".1251")).narrow(wstr.data(), wstr.data() + wstr.size(), '?', buf.data());
	buf[wstr.size()] = '\0';

	return buf;
}

vector<char> fileToString(string filename)
{
	ifstream file(filename);
	//	rapidxml::file<> xmlFile("conf.xml"); // Default template is char
//	rapidxml::xml_document<> doc;
//	doc.parse<0>(xmlFile.data());
	stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	std::string content(buffer.str());
	vector<char> result(content.size() + 1);
	CopyMemory(result.data(), content.c_str(), content.size());
	result.data()[content.size()] = '\0';


	return result;
}

void getJsonResponseForGuestInfo(long long card, vector<char> &res)
{
		rapidjson::Value json_val;
		rapidjson::Document json_doc;
		auto& allocator = json_doc.GetAllocator();

		json_doc.SetObject();

		json_val.SetUint64(card);
		if(to_string(card).size() == 11)
			json_doc.AddMember("Phone", json_val, allocator);
		else
			json_doc.AddMember("CardToken", json_val, allocator);

		rapidjson::StringBuffer json_buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(json_buffer);
		json_doc.Accept(writer);
		const char* json_str = json_buffer.GetString();

		res.resize(strlen(json_str) + 1);
		CopyMemory(res.data(), json_str, strlen(json_str) + 1);
}

int SendGetDiscount(httpClient& hc, checkXml& checkXml, string& token, string clientId_str,
		ofstream& getCardInfoResultLog,
		Info*& info,
		const char *acceptTypes[],
		vector<char>& szData);

extern "C" int __declspec(dllexport) __stdcall GetCardInfoEx(__int64 Card,
 DWORD Restaurant,
	DWORD UnitNo,
	Info *info, char *InpBuf, int InpLen, WORD InpKind,
	const char *OutBuf, DWORD OutLen, WORD OutKind) {
		ofstream getCardInfoInput("getCardInfoInput.txt",ios::out|ios::binary);
		getCardInfoInput << InpBuf;
		getCardInfoInput.close();

		ofstream getCardInfoResultLog("getCardInfoResultLog.txt",ios::out|ios::binary);

		vector<char> szData(1024);

		info->deleted = 0;
		info->need_withdraw = 0 ;
		info->expired = 0;
		info->not_valid = 0;
		info->manager_confirm = 0;

		rapidxml::xml_document<> doc;
		vector<char> file_ar = fileToString("conf.xml");
		doc.parse<0>(file_ar.data());
		rapidxml::xml_node<> *configuration = doc.first_node("Configuration");

		checkXml checkXml;
		try
		{
			checkXml.init(InpBuf);
		}
		catch(const rapidxml::parse_error)
		{ getCardInfoResultLog << "Input xml parse error"; return 1; }
		catch(XmlElementNotFound& e)
		{ getCardInfoResultLog << e.message; return 1; }

		rapidjson::Document jsonGuestInfo;
		rapidjson::Document jsonDiscont;
		rapidjson::ParseResult ok;

		httpClient hc;

		string apiAddress = string(configuration->first_node("ApiAddress")->value());
		bool ssl = string(configuration->first_node("SSL")->value()) == "true" ? true : false;
//		bool goToApi = string(configuration->first_node("goToApi")->value()) == "true" ? true : false;
//		if(!goToApi)
//			return 0;
		char* token_c_str;
		try
		{
			token_c_str = checkXml.getToken(configuration);
		}
		catch(XmlElementNotFound& e)
		{ getCardInfoResultLog << e.message; return 1; }

		if(token_c_str == NULL)
		{ getCardInfoResultLog << "Error finding token"; return 1; }

		string token = string(token_c_str);

		int error_code = hc.Init(apiAddress.c_str(), ssl);
		if(error_code != 0)
		{ getCardInfoResultLog << "Error server connection init"; return 1; }

		string guestInfoUrl = "v1/GuestInfo/" + token;
		static const char *acceptTypes[] = {"*/*", NULL};

		vector<char> guestInfoJsonRequet;
		getJsonResponseForGuestInfo(Card, guestInfoJsonRequet);

		try
		{
			szData = hc.SendRequest("POST", "Content-Type:application/json;",
				guestInfoUrl.c_str(), guestInfoJsonRequet.data(), acceptTypes);
		}
		catch(TimeoutExceeded& e)
		{
				CopyMemory(info->screen_comment, e.message.c_str(), strlen(e.message.c_str())+1);
				getCardInfoResultLog  << e.message; return 0;
		 }

		if(szData.size() == 0)
		{ getCardInfoResultLog << "Error sendin GuestInfo request, Error code: " << hcLastError; return 1;}


		ok = jsonGuestInfo.Parse(szData.data());
		if(!ok)
			{ getCardInfoResultLog << "Error: " << szData.data(); return 1;}
		int status =  jsonGuestInfo["Status"].GetInt();
		if(status == 2)
		{
		   const char* errorMessage =   ToANSI(jsonGuestInfo["ErrorMessage"].GetString()).data();
		   if(strcmp(errorMessage, "Карта заблокирована. Продажа по карте невозможна.") == 0)
		   {
				info->blocked = 1;
				CopyMemory(info->block_reason, errorMessage, strlen(errorMessage)+1);
				{ getCardInfoResultLog << errorMessage; return 0;}
		   }
		   else if(strcmp(errorMessage,"Карта не найдена, обновите карту! License expired, please contact Prime Hill.") == 0)
		   {
				CopyMemory(info->screen_comment, errorMessage, strlen(errorMessage)+1);
				{ getCardInfoResultLog  << errorMessage; return 0;}
		   }
		   return 1;
		}
		if(status != 1)
			{ getCardInfoResultLog << "Error: " << szData.data(); return 1;}

		string clientId_str = jsonGuestInfo["ClientId"].GetString();

		int dataSize = szData.size();
		int getDiscountRes = SendGetDiscount(hc, checkXml, token, clientId_str, getCardInfoResultLog,
			info, acceptTypes,szData);
		if(getDiscountRes != -1)
			return   getDiscountRes;

		hc.Close();

		ofstream getDiscountResponse("getDiscountResponse.txt",ios::out|ios::binary);
		dataSize = szData.size();
		getDiscountResponse << szData.data();

		ok = jsonDiscont.Parse(szData.data());

		if(!ok || jsonDiscont["Status"].GetInt() != 1)
				{ getCardInfoResultLog << "Error: " << szData.data(); return 1;}

		info->blocked = 0;

		vector<char> fio =   ToANSI(jsonDiscont["FIO"].GetString());
		CopyMemory(info->card_owner, fio.data(), fio.size());

		long long clientId = atol(jsonGuestInfo["ClientId"].GetString());
		info->owner_id = clientId;
		info->account_number = clientId;

		info->not_pay_type = jsonDiscont["NotPayType"].GetInt64();

		info->bonus_num = jsonDiscont["BonusId"].GetInt64();

		info->discount_num = jsonDiscont["DiscountId"].GetInt64();

		info->max_discount = jsonDiscont["SumDiscount"].GetDouble()*100;

		for (rapidxml::xml_node<>* bonus = configuration->first_node("BonusAccount");
				bonus; bonus = bonus->next_sibling("BonusAccount"))
		{
			int bonusAccount = atoi(bonus->first_node("FarcardsSubAccount")->value());
			//*getAccessAmountByNumber(info, bonusAccount) = jsonDiscont["MaxBonus"].GetDouble()*100;
			setAccessAmountByNumber(info, bonusAccount, jsonDiscont["MaxBonus"].GetDouble()*100);
		}

		int depositAccount = atoi(configuration->first_node("DepositAccount")
			->first_node("FarcardsSubAccount")->value());
		//*getAccessAmountByNumber(info, depositAccount) =  jsonDiscont["MaxDeposit"].GetDouble()*100;
		 setAccessAmountByNumber(info, depositAccount, jsonDiscont["MaxDeposit"].GetDouble()*100);
		if(jsonDiscont.HasMember("Message"))
		{
			vector<char> message =   ToANSI(jsonDiscont["Message"].GetString());
			CopyMemory(info->comment, message.data(), message.size());
		}

		vector<char> loyaltyLevel =   ToANSI(jsonDiscont["LoyaltyLevel"].GetString());
		CopyMemory(info->screen_comment, loyaltyLevel.data(), loyaltyLevel.size());

		vector<char> printMessage =   ToANSI(jsonDiscont["PrintMessage"].GetString());
		CopyMemory(info->printer_comment, printMessage.data(), printMessage.size());

		getCardInfoResultLog << "Success";
		getCardInfoResultLog.close();
		return 0;
}


int SendGetDiscount(httpClient& hc, checkXml& checkXml, string& token, string clientId_str ,
	ofstream& getCardInfoResultLog,
		Info*& info,
		const char *acceptTypes[],
		vector<char>& szData)
{
		rapidjson::ParseResult ok;


		string getDiscountUrl = "v1/GetDiscount/" + token;

		vector<char> getDiscountJsonRequet;
		try
		{
			checkXml.getJsonResponseForGetDiscount(clientId_str, getDiscountJsonRequet);
		}
		catch(XmlElementNotFound& e)
		{ getCardInfoResultLog << e.message; return 1; }

		ofstream jsonRequest("getDiscountJsonRequet.txt",ios::out|ios::binary);
		jsonRequest << getDiscountJsonRequet.data();

		try
		{
			szData = hc.SendRequest("POST", "Content-Type:application/json;",
				getDiscountUrl.c_str(), getDiscountJsonRequet.data(), acceptTypes);
		}
		catch(TimeoutExceeded& e)
		{
				CopyMemory(info->screen_comment, e.message.c_str(), strlen(e.message.c_str())+1);
				getCardInfoResultLog  << e.message; return 0;
		 }
		catch(...)
		{
			 getCardInfoResultLog << "Error: GetDiscount, Error code: " << hcLastError; return 1;
		}

		if(szData.size() == 0)
		{ getCardInfoResultLog << "Error: GetDiscount, Error code: " << hcLastError; return 1;}


		return -1;
}


extern "C" int __declspec(dllexport) __stdcall TransactionsEx( DWORD Count,
					Transactionlist *transactionlist,
					CHAR* InpBuf,
					DWORD InpLen,						// длина информации от кассы
					WORD InpKind,						// тип информации от кассы (1-XML)
					CHAR* OutBuf,						// дополнительная ответная информация
					DWORD OutLen,						// длина ответной информации
					WORD OutKind)
{
		ofstream transactionInput("transactionInput.txt",ios::out|ios::binary);
		transactionInput << InpBuf;
		transactionInput.close();

		ofstream transactionsResultLog("transactionsResultLog.txt",ios::out|ios::binary);

		vector<char> szData(1024);

		rapidxml::xml_document<> doc;
		vector<char> file_ar = fileToString("conf.xml");
		doc.parse<0>(file_ar.data());
		rapidxml::xml_node<> *configuration = doc.first_node("Configuration");
//
		checkXml checkXml;
        try
		{
			checkXml.init(InpBuf);
		}
		catch(const rapidxml::parse_error)
		{ transactionsResultLog << "Input xml parse error"; return 1; }
		catch(XmlElementNotFound& e)
		{ transactionsResultLog << e.message; return 1; }


		httpClient hc;

		rapidjson::Document jsonResultRequest;

		string apiAddress = string(configuration->first_node("ApiAddress")->value());
		bool ssl = string(configuration->first_node("SSL")->value()) == "true" ? true : false;
		char* token_c_str;
		try
		{
			token_c_str = checkXml.getToken(configuration);
		}
		catch(XmlElementNotFound& e)
		{ transactionsResultLog << e.message; return 1; }

		if(token_c_str == NULL)
			{ transactionsResultLog << "Error finding token"; return 1; }
		string token = string(token_c_str);
		static const char *acceptTypes[] = {"*/*", NULL};

		int error_code = hc.Init(apiAddress.c_str(), ssl);
		if(error_code != 0)
			{ transactionsResultLog << "Error server connection init"; return 1; }

		int checkChmod;
		try
		{
			checkChmod = checkXml.getChmode();
		}
		catch(XmlElementNotFound& e)
		{transactionsResultLog << e.message; return 1;}

		if(checkChmod == 1 || checkChmod == 4)
		{
			string closeOrderUrl = "v1/CloseOrder/" + token;

			vector<int> bonusCurrencyCodes;
			for (rapidxml::xml_node<>* bonus = configuration->first_node("BonusAccount");
				bonus; bonus = bonus->next_sibling("BonusAccount"))
			{
				int bonusCurrencyCode = atoi(bonus->first_node("CurrencyCode")->value());
				bonusCurrencyCodes.push_back(bonusCurrencyCode) ;
			}

			int depositCurrencyCode = atoi(configuration->first_node("DepositAccount")
				->first_node("CurrencyCode")->value());

			vector<char> closeOrderRequet;
			char* client_id;
			bool getDiscountSended;
			try
			{
				checkXml.getJsonResponseForCloseOrder(
					bonusCurrencyCodes, depositCurrencyCode, closeOrderRequet,
					client_id, getDiscountSended);
			}
			catch(ClientIdNotFound& e)
			{ transactionsResultLog << "ClientId not found"; return 1;}
			catch(XmlElementNotFound& e)
			{ transactionsResultLog << e.message; return 1; }

			if(!getDiscountSended)
			{
				Info info_;
				Info* info = &info_;
				int getDiscountRes = SendGetDiscount(hc, checkXml, token, string(client_id),
					transactionsResultLog,
					info,
					acceptTypes,
					szData);
				if(getDiscountRes != -1)
					return   getDiscountRes;
			}

			ofstream jsonRequest("closeOrderRequest.txt",ios::out|ios::binary);
			jsonRequest << closeOrderRequet.data();

			try
			{
				szData = hc.SendRequest("POST", "Content-Type:application/json;",
					closeOrderUrl.c_str(), closeOrderRequet.data(), acceptTypes);
			}
			catch(TimeoutExceeded& e)
			{
					transactionsResultLog  << e.message; return 0;
			 }
		}
		else
		{
			string returnOrderUrl = "v1/ReturnOrder/" + token;

			vector<char> returnOrderRequet;
			try
			{
				checkXml.getJsonResponseForReturnOrder(returnOrderRequet);
			}
            catch(XmlElementNotFound& e)
			{ transactionsResultLog << e.message; return 1; }

			ofstream jsonRequest("returnOrderRequest.txt",ios::out|ios::binary);
			jsonRequest << returnOrderRequet.data();

            try
			{
				szData = hc.SendRequest("POST", "Content-Type:application/json;",
					returnOrderUrl.c_str(), returnOrderRequet.data(), acceptTypes);
			}
			catch(TimeoutExceeded& e)
			{
					transactionsResultLog  << e.message; return 0;
			 }
		}

		if(szData.size() == 0)
			{ transactionsResultLog << "Error sending Transaction request, Error code: " << hcLastError; return 1;}

		rapidjson::ParseResult ok = jsonResultRequest.Parse(szData.data());
		if(!ok || jsonResultRequest["Status"].GetInt() != 1)
			{ transactionsResultLog << "Error: " << szData.data(); return 1;}

		hc.Close();

		transactionsResultLog << "Success";
		return 0;
}


