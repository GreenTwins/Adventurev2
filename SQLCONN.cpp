#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include "SQLCONN.h"
#include "Player.h"
#include <iostream>
#include "Game.h"
#include <codecvt>
#include <locale>
#include <string>




#define SQL_RESULT_LEN 240
#define SQL_RETURN_CODE_LEN 1240
SQLHENV sqlEnv;
SQLHDBC sqlConnection;
SQLHSTMT sqlStatement;

SQLCONN::SQLCONN() {
	sqlEnv = SQL_NULL_HENV;
	sqlConnection = SQL_NULL_HDBC;
	sqlStatement = SQL_NULL_HSTMT;

	SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnv);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		std::cerr << "Connection unsuccessful" << std::endl;
		return;
	}
	ret = SQLSetEnvAttr(sqlEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		std::cerr << "Attr failed" << std::endl;
		return;
	}
	ret = SQLAllocHandle(SQL_HANDLE_DBC, sqlEnv, (SQLHANDLE*)&sqlConnection);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		std::cerr << "allocation failed" << std::endl;
		return;
	}
	//std::cout << "connection established" << std::endl;
}

SQLCONN::~SQLCONN() {
	// Free statement handle if it's allocated
	if (sqlStatement != SQL_NULL_HSTMT) {
		SQLFreeHandle(SQL_HANDLE_STMT, sqlStatement);
	}

	// Disconnect and free the connection handle
	if (sqlConnection != SQL_NULL_HDBC) {
		SQLDisconnect(sqlConnection);
		SQLFreeHandle(SQL_HANDLE_DBC, sqlConnection);
	}

	// Free the environment handle
	if (sqlEnv != SQL_NULL_HENV) {
		SQLFreeHandle(SQL_HANDLE_ENV, sqlEnv);
	}
	/*SQLFreeHandle(SQL_HANDLE_DBC, sqlconnectionhandle);
	SQLFreeHandle(SQL_HANDLE_ENV, sqlenvhandle);*/
}
SQLCONN& SQLCONN::createInstance() {
	static SQLCONN instance;
	return instance;
}
bool SQLCONN::connect() {
	SQLWCHAR connStr[] = L"DRIVER={SQL Server};SERVER=localhost\\sqlexpress;DATABASE=master;Integrated Security=True;";
	SQLWCHAR outConnStr[SQL_MAX_MESSAGE_LENGTH];
	SQLSMALLINT outConnStrLen;
	SQLRETURN ret = SQLDriverConnect(sqlConnection, NULL, connStr, SQL_NTS, outConnStr, sizeof(outConnStr), &outConnStrLen, SQL_DRIVER_NOPROMPT);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		SQLWCHAR sqlState[6], message[SQL_RETURN_CODE_LEN];
		SQLINTEGER nativeError;
		SQLSMALLINT length;
		SQLGetDiagRecW(SQL_HANDLE_DBC, sqlConnection, 1, sqlState, &nativeError, message, SQL_RETURN_CODE_LEN, &length);
		std::wcerr << "Connection failed with error: " << message << std::endl;
		return false;
	}
	std::wcout << L"Connected to the database successfully!" << std::endl;
	return true;
}
bool SQLCONN::isConnectionActive() {
	SQLINTEGER infoValue;
	SQLRETURN ret = SQLGetConnectAttr(sqlConnection, SQL_ATTR_CONNECTION_DEAD, &infoValue, 0, NULL);

	if (ret == SQL_ERROR) {
		return false;
	}
	return true;
}
void SQLCONN::disconnect() {
	//if (isConnectionActive()) {
	//	SQLDisconnect(sqlConnection);
	//}
	if (sqlConnection != SQL_NULL_HDBC) {
		SQLDisconnect(sqlConnection);
	}
}
void NullTerminateString(SQLCHAR* str, SQLLEN length) {
	if (length > 0 && str[length - 1] != '\0') {
		str[length] = '\0';
	}
}
bool SQLCONN::displayNames() {
	/*if (!connect()) {
		return false;
	}*/
	SQLHSTMT sqlStatement;
	SQLAllocHandle(SQL_HANDLE_STMT, sqlConnection, &sqlStatement);

	SQLWCHAR* sqlQuery = (SQLWCHAR*)L"SELECT playerName FROM Player_Table";
	//gotta do this annoying imbue since SQLWCHAR is a wchar_t
	//std::wcout.imbue(std::locale());
	SQLRETURN ret = SQLExecDirect(sqlStatement, sqlQuery, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		SQLWCHAR errorMsg[SQL_MAX_MESSAGE_LENGTH];
		SQLSMALLINT msgLen;
		SQLGetDiagRec(SQL_HANDLE_STMT, sqlStatement, 1, NULL, NULL, errorMsg, SQL_MAX_MESSAGE_LENGTH, &msgLen);
		std::wcout << "Error exe query: " << errorMsg << std::endl;
		SQLFreeHandle(SQL_HANDLE_STMT, sqlStatement);
		return false;
	}
	SQLCHAR PlayerName[255];

	SQLLEN playerNameLength;

	int i = 1;
	while (SQLFetch(sqlStatement) == SQL_SUCCESS) {
		//SQLGetData(hStmt, 4, SQL_C_CHAR, enemyName, sizeof(enemyName), NULL);
		SQLGetData(sqlStatement, 1, SQL_C_CHAR, PlayerName, sizeof(PlayerName), &playerNameLength);
		PlayerName[playerNameLength] = '\0';
		std::string playersName(reinterpret_cast<char*>(PlayerName), playerNameLength);
		std::cout << i << ".) " << playersName << std::endl;
		++i;

		playerList.push_back(playersName);
	}
	ret = SQLFreeStmt(sqlStatement, SQL_CLOSE);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		SQLWCHAR errorMsg[SQL_MAX_MESSAGE_LENGTH];
		SQLSMALLINT msgLen;
		SQLGetDiagRec(SQL_HANDLE_STMT, sqlStatement, 1, NULL, NULL, errorMsg, SQL_MAX_MESSAGE_LENGTH, &msgLen);
		std::wcout << "Error freeing data: " << errorMsg << std::endl;
		SQLFreeHandle(SQL_HANDLE_STMT, sqlStatement);
		return false;
	}

	SQLFreeHandle(SQL_HANDLE_STMT, sqlStatement);
	return true;
}

bool SQLCONN::sqlSave() {
	if (isConnectionActive()) {
		SQLDisconnect(sqlConnection);
	}
	if (!connect()) {
		return false;
	}
	//load data into vars
	std::string playerName, bodyType, playerJob, className, subClassName;
	int HP, maxHP, MP, maxMP, Str, Def, Spd, dodge, level, Intel, dext, endur, fatigue, maxFatigue, gold, xp;
	int currentLocation;
	float stamina, maxStamina, perc, maxPerc;

	playerName = Game::getinstance().playerN.getName();
	bodyType = Game::getinstance().playerN.getBodyType();
	playerJob = Game::getinstance().playerN.getActiveJob();
	className = Game::getinstance().playerN.getClass();
	subClassName = Game::getinstance().playerN.getSubClass();
	level = Game::getinstance().playerN.getLvl();
	HP = Game::getinstance().playerN.getHP();
	maxHP = Game::getinstance().playerN.getMaxHP();
	MP = Game::getinstance().playerN.getMP();
	maxMP = Game::getinstance().playerN.getMaxMP();
	Str = Game::getinstance().playerN.getStr();
	Def = Game::getinstance().playerN.getDef();
	Spd = Game::getinstance().playerN.getSpd();
	Intel = Game::getinstance().playerN.getInt();
	dext = Game::getinstance().playerN.getDex();
	endur = Game::getinstance().playerN.getEnd();
	fatigue = Game::getinstance().playerN.getFatigue();
	maxFatigue = Game::getinstance().playerN.getMaxFatigue();
	//dodge = Game::getinstance().playerN.getDodge();
	gold = Game::getinstance().playerN.getGold();
	xp = Game::getinstance().playerN.getXP();
	stamina = Game::getinstance().playerN.getStamina();
	maxStamina = Game::getinstance().playerN.getMaxStamina();
	perc = Game::getinstance().playerN.getPrec();
	maxPerc = Game::getinstance().playerN.getMaxPrec();


	SQLHSTMT hStmt;
	SQLAllocHandle(SQL_HANDLE_STMT, sqlConnection, &hStmt);
	SQLWCHAR* sqlQuery = nullptr;
	SQLRETURN ret;
	//check and see if theres already data available for the given name and....something else
	if (Game::getinstance().newChar) {
		//save to sql
		std::cout << "Saving new data..." << std::endl;

		sqlQuery = (SQLWCHAR*)L"INSERT INTO Player (playerName, bodyType, playerJob, className, subClassName, HP, maxHP, MP, maxMP, stamina,maxStamina,fatigue,maxFatigue,perc, maxPerc, Str, Def, Spd, Intel, dext, endur,level, gold,xp)"
			L"VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";

		ret = SQLPrepare(hStmt, sqlQuery, SQL_NTS);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			SQLCHAR sqlState[6], message[SQL_RETURN_CODE_LEN];
			SQLINTEGER nativeError;
			SQLSMALLINT length;
			SQLGetDiagRecW(SQL_HANDLE_STMT, hStmt, 1, (SQLWCHAR*)sqlState, &nativeError, (SQLWCHAR*)message, SQL_RETURN_CODE_LEN, &length);
			std::wcerr << "SQLPrepare failed with error: " << message << std::endl;
			SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
			return false;
		}

		SQLLEN stringLength = SQL_NTS;
		ret = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)playerName.c_str(), 0, &stringLength);
		ret = SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)bodyType.c_str(), 0, &stringLength);
		ret = SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)playerJob.c_str(), 0, &stringLength);
		ret = SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)className.c_str(), 0, &stringLength);
		ret = SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)subClassName.c_str(), 0, &stringLength);
		ret = SQLBindParameter(hStmt, 6, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &HP, 0, NULL);
		ret = SQLBindParameter(hStmt, 7, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &maxHP, 0, NULL);
		ret = SQLBindParameter(hStmt, 8, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &MP, 0, NULL);
		ret = SQLBindParameter(hStmt, 9, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &maxMP, 0, NULL);
		ret = SQLBindParameter(hStmt, 10, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &stamina, 0, NULL);
		ret = SQLBindParameter(hStmt, 11, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &maxStamina, 0, NULL);
		ret = SQLBindParameter(hStmt, 12, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &fatigue, 0, NULL);
		ret = SQLBindParameter(hStmt, 13, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &maxFatigue, 0, NULL);
		ret = SQLBindParameter(hStmt, 14, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &perc, 0, NULL);
		ret = SQLBindParameter(hStmt, 15, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &maxPerc, 0, NULL);
		ret = SQLBindParameter(hStmt, 16, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &Str, 0, NULL);
		ret = SQLBindParameter(hStmt, 17, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &Def, 0, NULL);
		ret = SQLBindParameter(hStmt, 18, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &Spd, 0, NULL);
		ret = SQLBindParameter(hStmt, 19, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &Intel, 0, NULL);
		ret = SQLBindParameter(hStmt, 20, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &dext, 0, NULL);
		ret = SQLBindParameter(hStmt, 21, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &endur, 0, NULL);
		ret = SQLBindParameter(hStmt, 22, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &level, 0, NULL);
		ret = SQLBindParameter(hStmt, 23, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &gold, 0, NULL);
		ret = SQLBindParameter(hStmt, 24, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &xp, 0, NULL);

		

	}
	else {
		
		sqlQuery = (SQLWCHAR*)L"UPDATE Player SET playerName=?, bodyType=?, playerJob=?, className=?, subClassName=?, HP=?, maxHP=?, MP=?, maxMP=?, stamina=?, maxStamina=?, fatigue=? ,maxFatigue=? , percision=?, maxPercision=?, Strength=?, Def=?, Spd=?, Intel=?, dext=?, endur=?,level=?, Gold=?,XP=?WHERE playerName=?";

		ret = SQLPrepare(hStmt, sqlQuery, SQL_NTS);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			SQLCHAR sqlState[6], message[SQL_RETURN_CODE_LEN];
			SQLINTEGER nativeError;
			SQLSMALLINT length;
			SQLGetDiagRecW(SQL_HANDLE_STMT, hStmt, 1, (SQLWCHAR*)sqlState, &nativeError, (SQLWCHAR*)message, SQL_RETURN_CODE_LEN, &length);
			std::wcerr << "SQLPrepare failed with error: " << message << std::endl;
			SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
			return false;
		}
		SQLLEN stringLength = SQL_NTS;

		ret = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)playerName.c_str(), 0, &stringLength);
		ret = SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)bodyType.c_str(), 0, &stringLength);
		ret = SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)playerJob.c_str(), 0, &stringLength);
		ret = SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)className.c_str(), 0, &stringLength);
		ret = SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)subClassName.c_str(), 0, &stringLength);
		ret = SQLBindParameter(hStmt, 6, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &HP, 0, NULL);
		ret = SQLBindParameter(hStmt, 7, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &maxHP, 0, NULL);
		ret = SQLBindParameter(hStmt, 8, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &MP, 0, NULL);
		ret = SQLBindParameter(hStmt, 9, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &maxMP, 0, NULL);
		ret = SQLBindParameter(hStmt, 10, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &stamina, 0, NULL);
		ret = SQLBindParameter(hStmt, 11, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &maxStamina, 0, NULL);
		ret = SQLBindParameter(hStmt, 12, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &fatigue, 0, NULL);
		ret = SQLBindParameter(hStmt, 13, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &maxFatigue, 0, NULL);
		ret = SQLBindParameter(hStmt, 14, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &perc, 0, NULL);
		ret = SQLBindParameter(hStmt, 15, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &maxPerc, 0, NULL);
		ret = SQLBindParameter(hStmt, 16, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &Str, 0, NULL);
		ret = SQLBindParameter(hStmt, 17, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &Def, 0, NULL);
		ret = SQLBindParameter(hStmt, 18, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &Spd, 0, NULL);
		ret = SQLBindParameter(hStmt, 19, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &Intel, 0, NULL);
		ret = SQLBindParameter(hStmt, 20, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &dext, 0, NULL);
		ret = SQLBindParameter(hStmt, 21, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &endur, 0, NULL);
		ret = SQLBindParameter(hStmt, 24, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &level, 0, NULL);
		ret = SQLBindParameter(hStmt, 25, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &gold, 0, NULL);
		ret = SQLBindParameter(hStmt, 26, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &xp, 0, NULL);
	}


	ret = SQLExecute(hStmt);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		SQLCHAR sqlState[SQL_SQLSTATE_SIZE + 1], message[SQL_MAX_MESSAGE_LENGTH];
		SQLINTEGER nativeError;
		SQLSMALLINT length;
		SQLGetDiagRecW(SQL_HANDLE_STMT, hStmt, 1, (SQLWCHAR*)sqlState, &nativeError, (SQLWCHAR*)message, SQL_RETURN_CODE_LEN, &length);
		std::wcerr << "SQLExecute failed with error: " << message << std::endl;
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		disconnect();
		return false;
	}

	std::cout << "Player data save!" << std::endl;
	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	disconnect();

	if (Game::getinstance().newChar) {
		if (!Game::getinstance().playerN.getID()) {
			return false;
		}
		Game::getinstance().newChar = false;
	}

	return true;
}

bool SQLCONN::loadPlayerData(const std::string& a) {
	if (!connect()) {
		return false;
	}
	SQLHSTMT hStmt;

	SQLAllocHandle(SQL_HANDLE_STMT, sqlConnection, &hStmt);

	SQLWCHAR* sqlQuery = (SQLWCHAR*)L"SELECT * FROM Player_Table WHERE playerName = ?";
	SQLRETURN ret = SQLPrepare(hStmt, sqlQuery, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		SQLCHAR sqlState[6], message[SQL_MAX_MESSAGE_LENGTH];
		SQLINTEGER nativeError;
		SQLSMALLINT length;
		SQLGetDiagRecW(SQL_HANDLE_STMT, hStmt, 1, (SQLWCHAR*)sqlState, &nativeError, (SQLWCHAR*)message, SQL_RETURN_CODE_LEN, &length);
		std::wcerr << "SQLPrepare failed with error: " << message << std::endl;
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		return false;
	}

	ret = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 255, 0, (SQLPOINTER*)a.c_str(), a.size(), NULL);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		SQLCHAR sqlState[6], message[SQL_RETURN_CODE_LEN];
		SQLINTEGER nativeError;
		SQLSMALLINT length;
		SQLGetDiagRecW(SQL_HANDLE_STMT, hStmt, 1, (SQLWCHAR*)sqlState, &nativeError, (SQLWCHAR*)message, SQL_RETURN_CODE_LEN, &length);
		std::wcerr << "SQLBindParameter failed with error: " << message << std::endl;
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		return false;
	}
	ret = SQLExecute(hStmt);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		SQLCHAR sqlState[SQL_SQLSTATE_SIZE + 1], message[SQL_MAX_MESSAGE_LENGTH];
		SQLINTEGER nativeError;
		SQLSMALLINT length;
		SQLGetDiagRecW(SQL_HANDLE_STMT, hStmt, 1, (SQLWCHAR*)sqlState, &nativeError, (SQLWCHAR*)message, SQL_MAX_MESSAGE_LENGTH, &length);
		std::wcerr << "SQLExecute failed with error: " << message << std::endl;
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		return false;
	}
	std::cout << "fetching...." << std::endl;

	while (SQLFetch(hStmt) == SQL_SUCCESS) {
		// Assuming columns in order of EnemyID, SpawnLocation, EnemyName, HP, MP, STR, DEF, SPD, DODGE, SKILL1, SKILL2, SKILL3
		SQLINTEGER playerID, HP, maxHP, MP, maxMP, stamina, maxStamina, fatigue, maxFatigue, precision, maxPrecision, Strength, Def, Spd, Intel, dext, endur, level, Gold, XP, currentLocation;
		bool hasWings, hasLegs;
		SQLCHAR playerName[255], bodyType[255], playerJob[255], className[255], subClassName[255]; // Assuming max length of 50 for name

		SQLGetData(hStmt, 1, SQL_C_LONG, &playerID, 0, NULL);
		SQLGetData(hStmt, 2, SQL_C_CHAR, &playerName, sizeof(playerName), NULL);
		SQLGetData(hStmt, 3, SQL_C_CHAR, &bodyType, sizeof(bodyType), NULL);
		SQLGetData(hStmt, 4, SQL_C_CHAR, &playerJob, sizeof(playerJob), NULL);
		SQLGetData(hStmt, 5, SQL_C_CHAR, &className, sizeof(className), NULL);
		SQLGetData(hStmt, 6, SQL_C_CHAR, &subClassName, sizeof(subClassName), NULL);
		SQLGetData(hStmt, 7, SQL_C_LONG, &HP, 0, NULL);
		SQLGetData(hStmt, 8, SQL_C_LONG, &maxHP, 0, NULL);
		SQLGetData(hStmt, 9, SQL_C_LONG, &MP, 0, NULL);
		SQLGetData(hStmt, 10, SQL_C_LONG, &maxMP, 0, NULL);
		SQLGetData(hStmt, 11, SQL_C_LONG, &stamina, 0, NULL);
		SQLGetData(hStmt, 12, SQL_C_LONG, &maxStamina, 0, NULL);
		SQLGetData(hStmt, 13, SQL_C_LONG, &fatigue, 0, NULL);
		SQLGetData(hStmt, 14, SQL_C_LONG, &maxFatigue, 0, NULL);
		SQLGetData(hStmt, 15, SQL_C_LONG, &precision, 0, NULL);
		SQLGetData(hStmt, 16, SQL_C_LONG, &maxPrecision, 0, NULL);
		SQLGetData(hStmt, 17, SQL_C_LONG, &Strength, 0, NULL);
		SQLGetData(hStmt, 18, SQL_C_LONG, &Def, 0, NULL);
		SQLGetData(hStmt, 19, SQL_C_LONG, &Spd, 0, NULL);
		SQLGetData(hStmt, 20, SQL_C_LONG, &Intel, 0, NULL);
		SQLGetData(hStmt, 21, SQL_C_LONG, &dext, 0, NULL);
		SQLGetData(hStmt, 22, SQL_C_LONG, &endur, 0, NULL);
		SQLGetData(hStmt, 23, SQL_C_BIT, &hasWings, 0, NULL);
		SQLGetData(hStmt, 24, SQL_C_BIT, &hasLegs, 0, NULL);
		SQLGetData(hStmt, 25, SQL_C_LONG, &level, 0, NULL);
		SQLGetData(hStmt, 26, SQL_C_LONG, &Gold, 0, NULL);
		SQLGetData(hStmt, 27, SQL_C_LONG, &XP, 0, NULL);
		SQLGetData(hStmt, 28, SQL_C_LONG, &currentLocation, 0, NULL);


		// Populate 
		std::string name = a;
		
		Player p1(name);
		p1.setID(playerID);
		p1.loaded = true;
		p1.setLocation(currentLocation);
		//p1.init();
		p1.setHP(HP);
		p1.setMaxHP(maxHP);
		p1.setMP(MP);
		p1.setMaxMp(maxMP);
		p1.setStr(Strength);
		p1.setDef(Def);
		p1.setSpd(Spd);
		



		std::string convertedSkill1 = reinterpret_cast<char*>(bodyType);
		std::string convertedSkill2 = reinterpret_cast<char*>(playerJob);
		std::string convertedSkill3 = reinterpret_cast<char*>(className);
		std::string convertedSkill4 = reinterpret_cast<char*>(subClassName);


		
		p1.setLvl(level);
		p1.setGold(Gold);
		p1.setXP(XP);

	}
	//load player inventory
	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	disconnect();
	return true;

}