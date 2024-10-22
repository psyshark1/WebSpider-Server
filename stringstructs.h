#pragma once
inline static const struct dbStrings
{
    char user[6] = "user=";
    char password[11] = "password=";
    char dbname[8] = "dbname=";
    char host[6] = "host=";
    char dbTblDocsName[9] = "HTMLDocs";
    char dbTblWordsName[14] = "HTMLDocsWords";
    char dbTblIndexerName[12] = "HTMLIndexer";
    char httpServerexec[17] = "WebSpider_Server";
} dbstr;

inline static const struct INIStrings
{
    char user[14] = "Database.user";
    char password[18] = "Database.password";
    char dbname[16] = "Database.dbname";
    char host[14] = "Database.host";
    char ServerHost[21] = "WebSpiderServer.host";
    char ServerPort[21] = "WebSpiderServer.port";
    char INIFile[14] = "WebSpider.ini";
} inistr;