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
#include <cmath>
#include <thread>



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

}

SQLCONN& SQLCONN::createInstance() {
	static SQLCONN instance;
	return instance;
}
bool SQLCONN::connect() {
	//SQLWCHAR connStr[] = L"DRIVER={SQL Server};SERVER=localhost\\sqlexpress;DATABASE=master;Integrated Security=True;";

	SQLWCHAR connStr[] = L"Driver={SQL Server};Server=DESKTOP-D8PTEQJ\\SQLEXPRESS;Database=Adventure;Integrated Security=True;TrustServerCertificate=True;";
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

bool SQLCONN::getplayerID() {
	//disconnect();

	//if (!connect()) {
	//	return false;
	//}
	SQLINTEGER  playerID;
	SQLHSTMT hStmt;
	SQLAllocHandle(SQL_HANDLE_STMT, sqlConnection, &hStmt);
	SQLWCHAR* sqlQuery = nullptr;
	SQLRETURN ret;
	sqlQuery = (SQLWCHAR*)L"SELECT playerID FROM Player_Table WHERE playerName=?";
	ret = SQLPrepare(hStmt, sqlQuery, SQL_NTS);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		SQLCHAR sqlState[6], message[SQL_MAX_MESSAGE_LENGTH];
		SQLINTEGER nativeError;
		SQLSMALLINT length;
		SQLGetDiagRecW(SQL_HANDLE_STMT, hStmt, 1, (SQLWCHAR*)sqlState, &nativeError, (SQLWCHAR*)message, SQL_RETURN_CODE_LEN, &length);
		std::wcerr << "SQLPrepare failed with error: " << message << std::endl;
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		return false;
	}

	std::string name = Game::getinstance().playerN.getName();
	ret = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, 255, 0, (SQLPOINTER*)name.c_str(), name.size(), NULL);
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
	if (SQLFetch(hStmt) == SQL_SUCCESS) {
		SQLGetData(hStmt, 1, SQL_C_LONG, &playerID, 0, NULL);
		Game::getinstance().playerN.setID(playerID);
	}
	//create error handle here
	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	//disconnect();
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


bool SQLCONN::saveplayerSkills() {
	/*if (isConnectionActive()) {
		SQLDisconnect(sqlConnection);
	}
	if (!connect()) {
		return false;
	}*/
	//load data into vars
	int skillID, atkAmt, player_ID, skillLevel;
	std::string skillName, ReqTypes, skillTypes, skillEffect, appType, bodyReq;
	float ReqPayment, EffectAmt;

	if (Game::getinstance().playerN.getID() == 0) {
		if (!getplayerID()) {
			std::cout << "\n Failed to get player ID \n";
			return false;
		}
	}
	player_ID = Game::getinstance().playerN.getID();

	//if not new char then delete entry and make new one

	std::vector<Skills>allSkills;
	for (auto& Skill : Game::getinstance().playerN.listofSkills) {
		allSkills.push_back(Skill);
	}

	for (auto& Skill : Game::getinstance().playerN.ClassSkills) {
		allSkills.push_back(Skill);
	}

	for (auto& skill : allSkills) {
		skillName = skill.getSkillName();
		ReqTypes = skill.getRequirementType();
		skillTypes = skill.getSkillType();
		skillEffect = skill.getSkillEffect();
		appType = skill.getAppType();
		bodyReq = skill.getBodyReq();

		atkAmt = skill.getatkAmt();
		ReqPayment = skill.getRequirementPayment();
		EffectAmt = skill.getSkillEffectAmt();
		skillLevel = skill.getSkillLvl();


		SQLHSTMT hStmt;
		SQLAllocHandle(SQL_HANDLE_STMT, sqlConnection, &hStmt);
		SQLWCHAR* sqlQuery = nullptr;
		SQLRETURN ret;
		//check and see if theres already data available for the given name and....something else

			//save to sql
		std::cout << "Saving new data..." << std::endl;

		sqlQuery = (SQLWCHAR*)L"INSERT INTO Skills_Table (skillName, ReqTypes, ReqPayment, skillTypes, skillEffect, EffectAmt, atkAmt, appType, player_ID, skillLevel, bodyReq)"
			L"VALUES (?,?,?,?,?,?,?,?,?,?,?)";

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
		ret = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)skillName.c_str(), 0, &stringLength);
		ret = SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)ReqTypes.c_str(), 0, &stringLength);
		ret = SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &ReqPayment, 0, NULL);
		ret = SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)skillTypes.c_str(), 0, &stringLength);
		ret = SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)skillEffect.c_str(), 0, &stringLength);
		ret = SQLBindParameter(hStmt, 6, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &EffectAmt, 0, NULL);
		ret = SQLBindParameter(hStmt, 7, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &atkAmt, 0, NULL);
		ret = SQLBindParameter(hStmt, 8, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)appType.c_str(), 0, &stringLength);
		ret = SQLBindParameter(hStmt, 9, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &player_ID, 0, NULL);
		ret = SQLBindParameter(hStmt, 10, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &skillLevel, 0, NULL);
		ret = SQLBindParameter(hStmt, 11, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)bodyReq.c_str(), 0, &stringLength);





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
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		std::cout << "Skill: " << skillName << " saved!" << std::endl;
	}



	//disconnect();

	if (Game::getinstance().newChar) {
		if (!Game::getinstance().playerN.getID()) {
			return false;
		}
		Game::getinstance().newChar = false;
	}

	return true;
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
	//if (isConnectionActive()) {
	//	SQLDisconnect(sqlConnection);
	//}
	//if (!connect()) {
	//	return false;
	//}
	//load data into vars
	std::string playerName, bodyType, playerJob, className, subClassName;
	int HP, maxHP, MP, maxMP, Str, Def, Spd, dodge, level, Intel, dext, endur, fatigue, maxFatigue, gold, XP, currentLocation;
	float stamina, maxStamina, precision, maxPrecision;

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
	XP = Game::getinstance().playerN.getXP();
	stamina = Game::getinstance().playerN.getStamina();
	maxStamina = Game::getinstance().playerN.getMaxStamina();
	precision = Game::getinstance().playerN.getPrec();
	maxPrecision = Game::getinstance().playerN.getMaxPrec();
	currentLocation = Game::getinstance().playerN.getLocation();

	SQLHSTMT hStmt;
	SQLAllocHandle(SQL_HANDLE_STMT, sqlConnection, &hStmt);
	SQLWCHAR* sqlQuery = nullptr;
	SQLRETURN ret;
	//check and see if theres already data available for the given name and....something else
	if (Game::getinstance().newChar) {
		//save to sql
		std::cout << "Saving new data..." << std::endl;

		sqlQuery = (SQLWCHAR*)L"INSERT INTO Player_Table (playerName, bodyType, playerJob, className, subClassName, HP, maxHP, MP, maxMP,stamina, maxStamina,fatigue,maxFatigue, precision, maxPrecision,Strength, Def, Spd, Intel, dext, endur,level, Gold,XP,currentLocation)"
			L"VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";

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
		ret = SQLBindParameter(hStmt, 10, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &stamina, 0, NULL);
		ret = SQLBindParameter(hStmt, 11, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &maxStamina, 0, NULL);
		ret = SQLBindParameter(hStmt, 12, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &fatigue, 0, NULL);
		ret = SQLBindParameter(hStmt, 13, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &maxFatigue, 0, NULL);
		ret = SQLBindParameter(hStmt, 14, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &precision, 0, NULL);
		ret = SQLBindParameter(hStmt, 15, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &maxPrecision, 0, NULL);
		ret = SQLBindParameter(hStmt, 16, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &Str, 0, NULL);
		ret = SQLBindParameter(hStmt, 17, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &Def, 0, NULL);
		ret = SQLBindParameter(hStmt, 18, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &Spd, 0, NULL);
		ret = SQLBindParameter(hStmt, 19, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &Intel, 0, NULL);
		ret = SQLBindParameter(hStmt, 20, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &dext, 0, NULL);
		ret = SQLBindParameter(hStmt, 21, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &endur, 0, NULL);
		ret = SQLBindParameter(hStmt, 22, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &level, 0, NULL);
		ret = SQLBindParameter(hStmt, 23, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &gold, 0, NULL);
		ret = SQLBindParameter(hStmt, 24, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &XP, 0, NULL);
		ret = SQLBindParameter(hStmt, 25, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &currentLocation, 0, NULL);


	}
	else {

		sqlQuery = (SQLWCHAR*)L"UPDATE Player_Table SET playerName=?, bodyType=?, playerJob=?, className=?, subClassName=?, HP=?, maxHP=?, MP=?, maxMP=?, stamina=?, maxStamina=?, fatigue=? ,maxFatigue=? , percision=?, maxPercision=?, Strength=?, Def=?, Spd=?, Intel=?, dext=?, endur=?,level=?, Gold=?,XP=?WHERE playerName=?";

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
		ret = SQLBindParameter(hStmt, 14, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &precision, 0, NULL);
		ret = SQLBindParameter(hStmt, 15, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &maxPrecision, 0, NULL);
		ret = SQLBindParameter(hStmt, 16, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &Str, 0, NULL);
		ret = SQLBindParameter(hStmt, 17, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &Def, 0, NULL);
		ret = SQLBindParameter(hStmt, 18, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &Spd, 0, NULL);
		ret = SQLBindParameter(hStmt, 19, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &Intel, 0, NULL);
		ret = SQLBindParameter(hStmt, 20, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &dext, 0, NULL);
		ret = SQLBindParameter(hStmt, 21, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &endur, 0, NULL);
		ret = SQLBindParameter(hStmt, 24, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &level, 0, NULL);
		ret = SQLBindParameter(hStmt, 25, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &gold, 0, NULL);
		ret = SQLBindParameter(hStmt, 26, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &XP, 0, NULL);
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
	//disconnect();

	if (Game::getinstance().newChar) {
		if (Game::getinstance().playerN.getID() == 0) {
			//get player ID. Player is auto saved once created and passes the first dungeon
			if (!getplayerID()) {
				std::cout << "\n Failed to get playerID\n";
				return false;
			}

		}
		Game::getinstance().newChar = false;
	}

	return true;
}

bool SQLCONN::loadPlayerData(const std::string& a) {
	/*if (isConnectionActive()) {
		SQLDisconnect(sqlConnection);
	}
	if (!connect()) {
		return false;
	}*/

	//using SQL transactions with my set up doesnt require individual connections and disconnections
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
		/*SQLGetData(hStmt, 23, SQL_C_BIT, &hasWings, 0, NULL);
		SQLGetData(hStmt, 24, SQL_C_BIT, &hasLegs, 0, NULL);*/
		SQLGetData(hStmt, 23, SQL_C_LONG, &level, 0, NULL);
		SQLGetData(hStmt, 24, SQL_C_LONG, &Gold, 0, NULL);
		SQLGetData(hStmt, 25, SQL_C_LONG, &XP, 0, NULL);
		SQLGetData(hStmt, 26, SQL_C_LONG, &currentLocation, 0, NULL);


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
		p1.setStamina(stamina);
		p1.setMaxStamina(maxStamina);
		p1.setDef(Def);
		p1.setSpd(Spd);
		p1.setInt(Intel);
		p1.setDex(dext);
		p1.setEnD(endur);
		p1.setPrec(precision);
		p1.setMaxPrec(maxPrecision);
		//p1.setHasLegs(hasLegs);
		//p1.setHasWings(hasWings);

		std::string convertedbodyType = reinterpret_cast<char*>(bodyType);
		std::string convertedplayerJob = reinterpret_cast<char*>(playerJob);
		std::string convertedclassName = reinterpret_cast<char*>(className);
		std::string convertedsubClassName = reinterpret_cast<char*>(subClassName);

		p1.setBodyType(convertedbodyType); //this is redundant if body type is humanoid as its the default set it new Player call
		p1.setActiveJob(convertedplayerJob);
		p1.setClass(convertedclassName);
		p1.setSubClass(convertedsubClassName);

		p1.setLvl(level);
		p1.setGold(Gold);
		p1.setXP(XP);

		Game::getinstance().playerN = std::move(p1);
	}
	//load player inventory
	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	//disconnect();
	return true;

}


bool SQLCONN::loadPlayerHitbox() {
	/*if (isConnectionActive()) {
		SQLDisconnect(sqlConnection);
	}*/
	if (Game::getinstance().playerN.getID() == 0) {
		//get player ID. Player is auto saved once created and passes the first dungeon
		if (!getplayerID()) {
			std::cout << "\n Failed to get playerID\n";
			return false;
		}

	}

	/*if (isConnectionActive()) {
		SQLDisconnect(sqlConnection);
	}
	if (!connect()) {
		return false;
	}*/
	SQLHSTMT hStmt;

	SQLAllocHandle(SQL_HANDLE_STMT, sqlConnection, &hStmt);

	//SQLWCHAR* sqlQuery = (SQLWCHAR*)L"SELECT * FROM body_Part WHERE player_ID = ?";
	SQLWCHAR* sqlQuery = (SQLWCHAR*)L"SELECT * FROM body_Part WHERE playerID = ?";
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
	SQLINTEGER sqlID = Game::getinstance().playerN.getID();
	ret = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_C_LONG, 0, 0, &sqlID, 0, NULL);
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
		SQLINTEGER defense, armorDef, bodyPartID, bodyPartHP;
		SQLCHAR bodyPartName[255];
		bool hasArmor, hasWeapon;

		SQLGetData(hStmt, 2, SQL_C_CHAR, &bodyPartName, sizeof(bodyPartName), NULL);
		SQLGetData(hStmt, 3, SQL_C_LONG, &bodyPartHP, 0, NULL);
		SQLGetData(hStmt, 4, SQL_C_LONG, &defense, 0, NULL);
		SQLGetData(hStmt, 5, SQL_C_BIT, &hasArmor, 0, NULL);
		SQLGetData(hStmt, 6, SQL_C_LONG, &armorDef, 0, NULL);
		SQLGetData(hStmt, 7, SQL_C_BIT, &hasWeapon, 0, NULL);
		SQLGetData(hStmt, 8, SQL_C_LONG, &bodyPartID, 0, NULL);


		//NullTerminateString(bodyPartName, sizeof(bodyPartName));
		std::string convertedbodyPartName = reinterpret_cast<char*>(bodyPartName);

		if (convertedbodyPartName == "Head") {
			Game::getinstance().playerN.Head.HP = bodyPartHP;
			Game::getinstance().playerN.Head.armorDef = armorDef;
			Game::getinstance().playerN.Head.hasArmor = hasArmor;
			Game::getinstance().playerN.Head.def = defense;
			Game::getinstance().playerN.Head.hasWeapon = hasWeapon;
		}
		else if (convertedbodyPartName == "RightArm") {
			Game::getinstance().playerN.RightArm.HP = bodyPartHP;
			Game::getinstance().playerN.RightArm.armorDef = armorDef;
			Game::getinstance().playerN.RightArm.hasArmor = hasArmor;
			Game::getinstance().playerN.RightArm.def = defense;
			Game::getinstance().playerN.RightArm.hasWeapon = hasWeapon;
		}
		else if (convertedbodyPartName == "RightLeg") {
			Game::getinstance().playerN.RightLeg.HP = bodyPartHP;
			Game::getinstance().playerN.RightLeg.armorDef = armorDef;
			Game::getinstance().playerN.RightLeg.hasArmor = hasArmor;
			Game::getinstance().playerN.RightLeg.def = defense;
			Game::getinstance().playerN.RightLeg.hasWeapon = hasWeapon;
		}
		else if (convertedbodyPartName == "LeftArm") {
			Game::getinstance().playerN.LeftArm.HP = bodyPartHP;
			Game::getinstance().playerN.LeftArm.armorDef = armorDef;
			Game::getinstance().playerN.LeftArm.hasArmor = hasArmor;
			Game::getinstance().playerN.LeftArm.def = defense;
			Game::getinstance().playerN.LeftArm.hasWeapon = hasWeapon;
		}
		else if (convertedbodyPartName == "LeftLeg") {
			Game::getinstance().playerN.LeftLeg.HP = bodyPartHP;
			Game::getinstance().playerN.LeftLeg.armorDef = armorDef;
			Game::getinstance().playerN.LeftLeg.hasArmor = hasArmor;
			Game::getinstance().playerN.LeftLeg.def = defense;
			Game::getinstance().playerN.LeftLeg.hasWeapon = hasWeapon;
		}
		else if (convertedbodyPartName == "Torso") {
			Game::getinstance().playerN.Torso.HP = bodyPartHP;
			Game::getinstance().playerN.Torso.armorDef = armorDef;
			Game::getinstance().playerN.Torso.hasArmor = hasArmor;
			Game::getinstance().playerN.Torso.def = defense;
			Game::getinstance().playerN.Torso.hasWeapon = hasWeapon;
		}
		else if (convertedbodyPartName == "Abdomen") {
			Game::getinstance().playerN.Abdomen.HP = bodyPartHP;
			Game::getinstance().playerN.Abdomen.armorDef = armorDef;
			Game::getinstance().playerN.Abdomen.hasArmor = hasArmor;
			Game::getinstance().playerN.Abdomen.def = defense;
			Game::getinstance().playerN.Abdomen.hasWeapon = hasWeapon;
		}
		else if (convertedbodyPartName == "Tail") {
			Game::getinstance().playerN.Tail.HP = bodyPartHP;
			Game::getinstance().playerN.Tail.armorDef = armorDef;
			Game::getinstance().playerN.Tail.hasArmor = hasArmor;
			Game::getinstance().playerN.Tail.def = defense;
			Game::getinstance().playerN.Tail.hasWeapon = hasWeapon;
		}
		else if (convertedbodyPartName == "Thorax") {
			Game::getinstance().playerN.Thorax.HP = bodyPartHP;
			Game::getinstance().playerN.Thorax.armorDef = armorDef;
			Game::getinstance().playerN.Thorax.hasArmor = hasArmor;
			Game::getinstance().playerN.Thorax.def = defense;
			Game::getinstance().playerN.Thorax.hasWeapon = hasWeapon;
		}
		else {
			Game::getinstance().playerN.Trunk.HP = bodyPartHP;
			Game::getinstance().playerN.Trunk.armorDef = armorDef;
			Game::getinstance().playerN.Trunk.hasArmor = hasArmor;
			Game::getinstance().playerN.Trunk.def = defense;
			Game::getinstance().playerN.Trunk.hasWeapon = hasWeapon;
		}

	}

	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	//disconnect();
	return true;
}

bool SQLCONN::playerSkillsLoading() {
	if (Game::getinstance().playerN.getID() == 0) {
		//get player ID. Player is auto saved once created and passes the first dungeon
		if (!getplayerID()) {
			std::cout << "\n Failed to get playerID\n";
			return false;
		}
	}
	/*if (isConnectionActive()) {
		SQLDisconnect(sqlConnection);
	}
	if (!connect()) {
		return false;
	}*/

	SQLHSTMT hStmt;

	SQLAllocHandle(SQL_HANDLE_STMT, sqlConnection, &hStmt);

	SQLWCHAR* sqlQuery = (SQLWCHAR*)L"SELECT * FROM Skills_Table WHERE player_ID = ?";
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
	SQLINTEGER sqlID = Game::getinstance().playerN.getID();
	ret = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_C_LONG, 0, 0, &sqlID, 0, NULL);
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
		SQLINTEGER ReqPayment, EffectAmt, atkAmt, skillID, playerID;
		SQLFLOAT bodyPartHP;
		SQLCHAR skillName[255], ReqTypes[255], skillTypes[255], skillEffect[255], appType[255];
		bool hasArmor, hasWeapon;

		SQLGetData(hStmt, 1, SQL_C_LONG, &skillID, 0, NULL);
		SQLGetData(hStmt, 2, SQL_C_CHAR, &skillName, sizeof(skillName), NULL);
		SQLGetData(hStmt, 3, SQL_C_CHAR, &ReqTypes, sizeof(ReqTypes), NULL);
		SQLGetData(hStmt, 4, SQL_C_LONG, &ReqPayment, 0, NULL);
		SQLGetData(hStmt, 5, SQL_C_CHAR, &skillTypes, sizeof(skillTypes), NULL);
		SQLGetData(hStmt, 6, SQL_C_CHAR, &skillEffect, sizeof(skillEffect), NULL);
		SQLGetData(hStmt, 7, SQL_C_LONG, &EffectAmt, 0, NULL);
		SQLGetData(hStmt, 8, SQL_C_LONG, &atkAmt, 0, NULL);
		SQLGetData(hStmt, 9, SQL_C_CHAR, &appType, sizeof(appType), NULL);


		const std::string& convertedskillName = reinterpret_cast<char*>(skillName);
		const std::string& convertedReqTypes = reinterpret_cast<char*>(ReqTypes);
		const std::string& convertedskillTypes = reinterpret_cast<char*>(skillTypes);
		const std::string& convertedskillEffect = reinterpret_cast<char*>(skillEffect);
		const std::string& convertedappType = reinterpret_cast<char*>(appType);



		Skills newSkill;
		newSkill.updateSkillName(convertedskillName);
		newSkill.updateSkillType(convertedskillTypes);
		newSkill.updateReqType(convertedReqTypes);
		newSkill.updateSkillEffect(convertedskillEffect);
		newSkill.setapplicationType(convertedappType);

		newSkill.updateSkillEffectAmt(EffectAmt);
		newSkill.updaterequirementPayment(ReqPayment);
		newSkill.updateatkAmt(atkAmt);
		newSkill.setSkillID(skillID);

		if ((newSkill.getAppType() == "Effect") || (newSkill.getSkillType() == "Passive") || (newSkill.getAppType() == "enhance")) {
			Game::getinstance().playerN.addClassSkill(std::move(newSkill));
		}
		else {
			Game::getinstance().playerN.addSkill(std::move(newSkill));
		}


	}
	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	Game::getinstance().playerN.setNumAtks();

	//disconnect();
	return true;
}

bool SQLCONN::saveplayerHitBox() {
	/* if (isConnectionActive()) {
		 SQLDisconnect(sqlConnection);
	 }
	if (!connect()) {
		return false;
	}*/

	std::string bodyPartName;
	int defense, armorDef, bodyPartID, bodyPartHP;
	bool hasWeapon, hasArmor;


	int player_ID = Game::getinstance().playerN.getID();


	for (auto& body : Game::getinstance().playerN.upperHitBox) {
		bodyPartName = body.bodyName;
		defense = body.def;
		armorDef = body.armorDef;
		hasWeapon = body.hasWeapon;
		hasArmor = body.hasArmor;
		bodyPartHP = std::round(body.HP);

		int hasArmorValue = hasArmor ? 1 : 0; // Convert boolean to integer
		int hasWeaponValue = hasWeapon ? 1 : 0; // Convert boolean to integer

		//if new char then new entry
		SQLHSTMT hStmt;
		SQLAllocHandle(SQL_HANDLE_STMT, sqlConnection, &hStmt);

		SQLWCHAR* sqlQuery = nullptr;
		SQLRETURN ret;
		//sqlQuery = (SQLWCHAR*)L"INSERT INTO body_Part (bodyPartName, bodyPartHP, defense, hasArmor, armorDef, hasWeapon, player_ID )"
		sqlQuery = (SQLWCHAR*)L"INSERT INTO body_Part (bodyPartName, bodyPartHP, defense, hasArmor, armorDef, hasWeapon, playerID )"
			L"VALUES (?,?,?,?,?,?,?)";

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

		SQLLEN itemNameLength = static_cast<SQLLEN>(bodyPartName.length());


		ret = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 255, 0, (SQLPOINTER)bodyPartName.c_str(), itemNameLength, &itemNameLength);
		ret = SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &bodyPartHP, 0, NULL);
		ret = SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &defense, 0, NULL);
		ret = SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_BIT, SQL_BIT, 0, 0, &hasArmorValue, 0, NULL);
		ret = SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &armorDef, 0, NULL);
		ret = SQLBindParameter(hStmt, 6, SQL_PARAM_INPUT, SQL_C_BIT, SQL_BIT, 0, 0, &hasWeaponValue, 0, NULL);
		ret = SQLBindParameter(hStmt, 7, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &player_ID, 0, NULL);





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
		std::cout << "Hitbox " << bodyPartName << " saved!\n";
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}



	//disconnect();

	return true;
}


bool SQLCONN::saveAllData() {
	if (isConnectionActive()) {
		SQLDisconnect(sqlConnection);
	}
	if (!connect()) {
		return false;
	}

	SQLRETURN ret;
	SQLHSTMT hStmt;
	SQLAllocHandle(SQL_HANDLE_STMT, sqlConnection, &hStmt);

	// Begin transaction
	ret = SQLSetConnectAttr(sqlConnection, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_OFF, SQL_NTS);
	if (!SQL_SUCCEEDED(ret)) {
		std::cerr << "Failed to begin transaction" << std::endl;
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		disconnect();
		return false;
	}

	// Call your SQL functions within the transaction
	bool success = false;
	if (sqlSave() && saveplayerHitBox() && saveplayerSkills()) {
		success = true;
	}

	// Commit or rollback transaction based on success
	if (success) {
		ret = SQLEndTran(SQL_HANDLE_DBC, sqlConnection, SQL_COMMIT);
		if (!SQL_SUCCEEDED(ret)) {
			std::cerr << "Failed to commit transaction" << std::endl;
			success = false;
		}
	}
	else {
		ret = SQLEndTran(SQL_HANDLE_DBC, sqlConnection, SQL_ROLLBACK);
		if (!SQL_SUCCEEDED(ret)) {
			std::cerr << "Failed to rollback transaction" << std::endl;
		}
	}

	// End transaction
	ret = SQLSetConnectAttr(sqlConnection, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, SQL_NTS);

	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	disconnect();

	return success;


}
bool SQLCONN::loadAllData() {
	if (isConnectionActive()) {
		SQLDisconnect(sqlConnection);
	}
	if (!connect()) {
		return false;
	}

	SQLRETURN ret;
	SQLHSTMT hStmt;
	SQLAllocHandle(SQL_HANDLE_STMT, sqlConnection, &hStmt);

	// Begin transaction
	ret = SQLSetConnectAttr(sqlConnection, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_OFF, SQL_NTS);
	if (!SQL_SUCCEEDED(ret)) {
		std::cerr << "Failed to begin transaction" << std::endl;
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		disconnect();
		return false;
	}

	// Call your SQL functions within the transaction
	bool success = false;
	if (loadPlayerData(Game::getinstance().playerN.getName()) && loadPlayerHitbox() && playerSkillsLoading()) {
		success = true;
	}

	// Commit or rollback transaction based on success
	if (success) {
		ret = SQLEndTran(SQL_HANDLE_DBC, sqlConnection, SQL_COMMIT);
		if (!SQL_SUCCEEDED(ret)) {
			std::cerr << "Failed to commit transaction" << std::endl;
			success = false;
		}
	}
	else {
		ret = SQLEndTran(SQL_HANDLE_DBC, sqlConnection, SQL_ROLLBACK);
		if (!SQL_SUCCEEDED(ret)) {
			std::cerr << "Failed to rollback transaction" << std::endl;
		}
	}

	// End transaction
	ret = SQLSetConnectAttr(sqlConnection, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, SQL_NTS);

	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	disconnect();

	return success;
}
void SQLCONN::LoadEnemies(std::string locType, int location, std::vector<Enemy>&el) {
	//}loadEnemies will be called if its a cache miss
	std::vector<Enemy>* _listptr = nullptr;
	if (locType == "Cave") {
		_listptr = &CaveEnemies;
	}
	else if (locType == "Forest") {
		_listptr = &ForestEnemies;
	}
	if (_listptr->empty()) {
		std::thread EnemyStats(&SQLCONN::getEnemies, this, location, locType, std::ref(el));
		std::thread EnemySkills(&SQLCONN::getEnemySkills, this);

		EnemyStats.join();
		EnemySkills.join();
	}
	else {
		for (auto enemy : *_listptr) {
			el.push_back(enemy);
		}
	}
}


bool SQLCONN::getEnemies(int location, std::string locType, std::vector<Enemy>& el) {
	if (isConnectionActive()) {
		SQLDisconnect(sqlConnection);
	}
	if (!connect()) {
		return false;
	}
	/*SQLINTEGER locationValue = location;
	SQLINTEGER dungeonNumValue = dungeonNum;*/

	SQLHSTMT hStmt;
	SQLAllocHandle(SQL_HANDLE_STMT, sqlConnection, &hStmt);

	SQLWCHAR* storedProcedure = (SQLWCHAR*)L"{CALL GetEnemiesByLocationAndDungeon(?)}";
	SQLRETURN ret = SQLPrepare(hStmt, storedProcedure, SQL_NTS);
	//const char* locTypeCStr = locType.c_str();
	SQLLEN locSize = locType.length();
	ret = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_CHAR, locSize, 0, (SQLPOINTER)locType.c_str(), 0, NULL);
	//ret = SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &dungeonNumValue, 0, NULL);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		SQLCHAR sqlState[6], message[SQL_MAX_MESSAGE_LENGTH];
		SQLINTEGER nativeError;
		SQLSMALLINT length;
		SQLGetDiagRecW(SQL_HANDLE_STMT, hStmt, 1, (SQLWCHAR*)sqlState, &nativeError, (SQLWCHAR*)message, SQL_MAX_MESSAGE_LENGTH, &length);
		std::wcerr << "SQLBindParameter failed with error: " << message << std::endl;
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		return false;
	}

	//ret = SQLPrepare(hStmt, storedProcedure, SQL_NTS);
	/*if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		SQLCHAR sqlState[6], message[SQL_RETURN_CODE_LEN];
		SQLINTEGER nativeError;
		SQLSMALLINT length;
		SQLGetDiagRecW(SQL_HANDLE_STMT, hStmt, 1, (SQLWCHAR*)sqlState, &nativeError, (SQLWCHAR*)message, SQL_RETURN_CODE_LEN, &length);
		std::wcerr << "SQLPrepare failed with error: " << message << std::endl;
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		return false;
	}*/

	ret = SQLExecute(hStmt);
	if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
		SQLCHAR sqlState[6], message[SQL_RETURN_CODE_LEN];
		SQLINTEGER nativeError;
		SQLSMALLINT length;
		SQLGetDiagRecW(SQL_HANDLE_STMT, hStmt, 1, (SQLWCHAR*)sqlState, &nativeError, (SQLWCHAR*)message, SQL_RETURN_CODE_LEN, &length);
		std::wcerr << "SQLExecute failed with error: " << message << std::endl;
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		return false;
	}
	std::cout << "fetching stats...." << std::endl;

	std::lock_guard<std::mutex>_lock(_mu);
	while (SQLFetch(hStmt) == SQL_SUCCESS) {

		//std::cout << enemy.location << " :";
		// Assuming columns in order of EnemyID, SpawnLocation, EnemyName, HP, MP, STR, DEF, SPD, DODGE, SKILL1, SKILL2, SKILL3
		SQLINTEGER enemyID, SpawnLocation, dungeonNum, HP, Str, Def, Spd, Intel, dex, endur, XPGiven, GoldGiven, enemyLvl;
		bool hasWings, hasLegs;
		SQLCHAR EnemyName[255], BodyType[255], LocType[255]; // Assuming max length of 50 for name

		SQLGetData(hStmt, 1, SQL_C_LONG, &enemyID, 0, NULL);
		SQLGetData(hStmt, 2, SQL_C_CHAR, BodyType, sizeof(BodyType), NULL);
		SQLGetData(hStmt, 3, SQL_C_CHAR, EnemyName, sizeof(EnemyName), NULL);
		SQLGetData(hStmt, 4, SQL_C_LONG, &HP, 0, NULL);
		SQLGetData(hStmt, 8, SQL_C_LONG, &Str, 0, NULL);
		SQLGetData(hStmt, 9, SQL_C_LONG, &Def, 0, NULL);
		SQLGetData(hStmt, 10, SQL_C_LONG, &Spd, 0, NULL);
		SQLGetData(hStmt, 14, SQL_C_BIT, &hasWings, 0, NULL);
		SQLGetData(hStmt, 15, SQL_C_BIT, &hasLegs, 0, NULL);
		/*SQLGetData(hStmt, 16, SQL_C_LONG, &SpawnLocation, 0, NULL);
		SQLGetData(hStmt, 17, SQL_C_LONG, &dungeonNum, 0, NULL);*/
		
		SQLGetData(hStmt, 16, SQL_C_LONG, &enemyLvl, 0, NULL);
		SQLGetData(hStmt, 17, SQL_C_LONG, &XPGiven, 0, NULL);
		SQLGetData(hStmt, 18, SQL_C_LONG, &GoldGiven, 0, NULL);
		SQLGetData(hStmt, 20, SQL_C_CHAR, &LocType, 0, NULL);

		std::string convertedEnemyName = reinterpret_cast<char*>(EnemyName);
		std::string convertedBodyType = reinterpret_cast<char*>(BodyType);
		// Populate Enemy object
		Enemy enemy; //how smart they are determines how many skills they can use
		enemy.setEnemyID(enemyID);
		/*enemy.setDunLoc(SpawnLocation);
		enemy.setDunLvl(dungeonNum);*/
		enemy.setName(reinterpret_cast<char*>(EnemyName));
		enemy.setHP(HP);
		enemy.setStr(Str);
		enemy.setDef(Def);
		enemy.setSpd(Spd);
		enemy.setLvl(enemyLvl);

		enemy.implementStats(location);

		if (hasLegs) {
			enemy.setHasLegs(1);
		}
		else {
			enemy.setHasLegs(0);
		}

		if (hasWings) {
			enemy.setHasWings(1);
		}
		else {
			enemy.setHasWings(0);
		}
		enemy.setBodyType(convertedBodyType);

		//std::string loctype = reinterpret_cast<char*>(LocType);
		if (locType == "Cave") {
			CaveEnemies.push_back(enemy);
		}
		else if (locType == "Forest") {
			ForestEnemies.push_back(enemy);
		}
		else {

		}
		//std::cout << enemy.getName() << std::endl;
		el.push_back(enemy);
		++_vectorSize;
	}


	SQLFreeHandle(SQL_HANDLE_STMT, sqlStatement);
	disconnect();
	_conVa.notify_one();
	std::cout << "fethcing completed" << std::endl;
	return true;
}
bool SQLCONN::getEnemySkills() {
	{
		std::unique_lock<std::mutex>uLock(_mu);
		_conVa.wait_for(uLock, std::chrono::seconds(3), [this] {return(_vectorSize > 0); });
	}
	
	if (isConnectionActive()) {
		SQLDisconnect(sqlConnection);
	}
	if (!connect()) {
		return false;
	}

	SQLINTEGER currentenemyID;
	int enemyVecPosition = 0;
	Enemy* updateEnemyPtr = nullptr;
	while (enemyVecPosition < _vectorSize) {
		currentenemyID = Game::getinstance().enemyList[enemyVecPosition].getID();
		if (currentenemyID <= 0) {
			//error break out of loop
			std::cerr << "Hit a snag..";
			break;
		}
		updateEnemyPtr = &Game::getinstance().enemyList[enemyVecPosition];
		//now we need to check to see if 
		SQLHSTMT hStmt;
		SQLAllocHandle(SQL_HANDLE_STMT, sqlConnection, &hStmt);

		SQLWCHAR* storedProcedure = (SQLWCHAR*)L"{CALL GetEnemiesMainSkills(?)}";
		SQLRETURN ret = SQLPrepare(hStmt, storedProcedure, SQL_NTS);
		ret = SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &currentenemyID, 0, NULL);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			SQLCHAR sqlState[6], message[SQL_MAX_MESSAGE_LENGTH];
			SQLINTEGER nativeError;
			SQLSMALLINT length;
			SQLGetDiagRecW(SQL_HANDLE_STMT, hStmt, 1, (SQLWCHAR*)sqlState, &nativeError, (SQLWCHAR*)message, SQL_MAX_MESSAGE_LENGTH, &length);
			std::wcerr << "SQLBindParameter failed with error: " << message << std::endl;
			SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
			return false;
		}

		ret = SQLPrepare(hStmt, storedProcedure, SQL_NTS);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			SQLCHAR sqlState[6], message[SQL_RETURN_CODE_LEN];
			SQLINTEGER nativeError;
			SQLSMALLINT length;
			SQLGetDiagRecW(SQL_HANDLE_STMT, hStmt, 1, (SQLWCHAR*)sqlState, &nativeError, (SQLWCHAR*)message, SQL_RETURN_CODE_LEN, &length);
			std::wcerr << "SQLPrepare failed with error: " << message << std::endl;
			SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
			return false;
		}

		ret = SQLExecute(hStmt);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
			SQLCHAR sqlState[6], message[SQL_RETURN_CODE_LEN];
			SQLINTEGER nativeError;
			SQLSMALLINT length;
			SQLGetDiagRecW(SQL_HANDLE_STMT, hStmt, 1, (SQLWCHAR*)sqlState, &nativeError, (SQLWCHAR*)message, SQL_RETURN_CODE_LEN, &length);
			std::wcerr << "SQLExecute failed with error: " << message << std::endl;
			SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
			return false;
		}
		std::cout << "fetching skills...." << std::endl;
		while (SQLFetch(hStmt) == SQL_SUCCESS) {

			
			SQLCHAR skillName[255], ReqTypes[255], skillTypes[255], skillEffect[255], appType[255], bodyReq[255], attackType[255];
			SQLINTEGER skillID, atkAmt, skillLevel;
			SQLFLOAT ReqPayment, EffectAmt;

			SQLGetData(hStmt, 1, SQL_C_LONG, &skillID, 0, NULL);
			SQLGetData(hStmt, 2, SQL_C_CHAR, skillName, sizeof(skillName), NULL);
			SQLGetData(hStmt, 3, SQL_C_CHAR, ReqTypes, sizeof(ReqTypes), NULL);
			SQLGetData(hStmt, 4, SQL_C_FLOAT, &ReqPayment, 0, NULL);
			SQLGetData(hStmt, 5, SQL_C_CHAR, skillTypes, sizeof(skillTypes),NULL);
			SQLGetData(hStmt, 6, SQL_C_CHAR, skillEffect, sizeof(skillEffect), NULL);
			SQLGetData(hStmt, 7, SQL_C_LONG, &EffectAmt, 0, NULL);
			SQLGetData(hStmt, 8, SQL_C_LONG, &atkAmt, 0, NULL);
			SQLGetData(hStmt, 9, SQL_C_CHAR, appType, sizeof(appType), NULL);
			SQLGetData(hStmt, 10, SQL_C_LONG, &skillLevel, 0, NULL);
			SQLGetData(hStmt, 11, SQL_C_CHAR, bodyReq, sizeof(bodyReq), NULL);
			SQLGetData(hStmt, 12, SQL_C_CHAR, attackType, sizeof(attackType), NULL);

			std::string convertedSkillName = reinterpret_cast<char*>(skillName);
			std::string convertedReqType = reinterpret_cast<char*>(ReqTypes);
			float convertedReqPayment = ReqPayment;
			std::string convertedskillTypes = reinterpret_cast<char*>(skillTypes);
			std::string conveertedskillEffect = reinterpret_cast<char*>(skillEffect);
			float convertedeffectAmt = EffectAmt;
			int convertedatkAmt = atkAmt;
			std::string convertedappType = reinterpret_cast<char*>(appType);
			int convertedSkillLevel = skillLevel;
			std::string convertedBodyReq = reinterpret_cast<char*>(bodyReq);
			std::string convertedAtkType = reinterpret_cast<char*>(attackType);

			Skills _skill(convertedSkillName, convertedReqType, convertedReqPayment, convertedskillTypes, conveertedskillEffect, convertedeffectAmt, convertedatkAmt, convertedappType, convertedSkillLevel, convertedBodyReq, convertedAtkType);

			updateEnemyPtr->loadSkills(std::move(_skill));
			

		}

		updateEnemyPtr = nullptr;
		++enemyVecPosition;
	}

	SQLFreeHandle(SQL_HANDLE_STMT, sqlStatement);
	disconnect();
	return true;
}