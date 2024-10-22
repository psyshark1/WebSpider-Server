#pragma once
#include"stringstructs.h"
#include"http_server.h"
#include"INI_parser.h"

void httpServer(tcp::acceptor& acceptor, tcp::socket& socket, std::string& dbTblDocsName, std::string& dbTblWordsName, std::string& dbTblIndexerName, pqxx::nontransaction& nt)
{
	acceptor.async_accept(socket,
		[&](beast::error_code ec)
		{
			if (!ec)
			{
				std::make_shared<http_server>(std::move(socket), std::move(&dbTblDocsName), std::move(&dbTblWordsName), std::move(&dbTblIndexerName), std::move(&nt))->start();
				httpServer(acceptor, socket, dbTblDocsName, dbTblWordsName, dbTblIndexerName, nt);
			}
		});
}

int main(int argc, char* argv[])
{	
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	std::system("chcp 1251");

	if (argc < 2)
	{
		try
		{
			ini_parser parser(inistr.INIFile);
			const unsigned short serverPort = parser.get_value<int>(inistr.ServerPort);
			if (!serverPort) { throw std::runtime_error("Server Port cannot be 0!"); }

			const std::string serverHost = parser.get_value<std::string>(inistr.ServerHost);
			const std::string postgresuser = parser.get_value<std::string>(inistr.user);
			const std::string postgreshost = parser.get_value<std::string>(inistr.host);
			const std::string postgrespass = parser.get_value<std::string>(inistr.password);
			const std::string postgresdbname = parser.get_value<std::string>(inistr.dbname);

			auto const address = net::ip::make_address(serverHost);

			net::io_context ioc{ 1 };

			const std::string connstr = dbstr.user + postgresuser + ' ' + dbstr.host + postgreshost + ' ' + dbstr.password + postgrespass + ' ' + dbstr.dbname + std::string(postgresdbname);
			tcp::acceptor acceptor{ ioc, { address, serverPort } };
			tcp::socket socket{ ioc };
			std::string dbTblDocsName{ dbstr.dbTblDocsName };
			std::string dbTblWordsName{ dbstr.dbTblWordsName };
			std::string dbTblIndexerName{ dbstr.dbTblIndexerName };
			pqxx::connection dbcon(connstr);
			pqxx::nontransaction nw(dbcon);
			httpServer(acceptor, socket, dbTblDocsName, dbTblWordsName, dbTblIndexerName, nw);

			std::cout << dbstr.httpServerexec << std::endl << "Connect: http://" << serverHost << ":" << serverPort << std::endl;

			ioc.run();

		}
		catch (WrongINI& w)
		{
			std::cerr << "INI ERROR: " << w.what() << std::endl << "Try running the program with command line parameters:" << std::endl <<
				"param1: Server address" << std::endl <<
				"param2: Server port" << std::endl <<
				"param3: Postgres username" << std::endl <<
				"param4: Postgres host" << std::endl <<
				"param5: Postgres password" << std::endl <<
				"param6: Postgres Database name" << std::endl <<
				"param7: Document table name" << std::endl <<
				"param8: Words table name" << std::endl <<
				"param9: Index table name" << std::endl;
			return -2;
		}
		catch (const std::runtime_error& re)
		{
			std::cerr << "SERVER ERROR: " << re.what() << std::endl;
			return -3;
		}
	}
	else if (argc == 10)
	{
		try
		{
			auto const address = net::ip::make_address(std::string(argv[1]));

			net::io_context ioc{ 1 };

			const unsigned short serverPort = std::atoi(argv[2]);
			const std::string connstr = "user=" + std::string(argv[3]) + " host=" + std::string(argv[4]) + " password=" + std::string(argv[5]) + " dbname=" + std::string(argv[6]);
			tcp::acceptor acceptor{ ioc, { address, serverPort } };
			tcp::socket socket{ ioc };
			std::string dbTblDocsName{ argv[7] };
			std::string dbTblWordsName{ argv[8] };
			std::string dbTblIndexerName{ argv[9] };
			pqxx::connection dbcon(connstr);
			pqxx::nontransaction nw(dbcon);
			httpServer(acceptor, socket, dbTblDocsName, dbTblWordsName, dbTblIndexerName, nw);

			std::cout << dbstr.httpServerexec << std::endl << "Connect: http://" << argv[1] << ":" << serverPort << std::endl;

			ioc.run();
		}
		catch (const std::runtime_error& re)
		{
			std::cerr << "SERVER ERROR: " << re.what() << std::endl;
			return -4;
		}
	}
	else if (argc > 1 && argc < 10)
	{
		std::cout << dbstr.httpServerexec << std::endl << "Not enough parameters!" << std::endl << "Necessary parameters:" << std::endl <<
			"param1: Server address" << std::endl <<
			"param2: Server port" << std::endl <<
			"param3: Postgres username" << std::endl <<
			"param4: Postgres host" << std::endl <<
			"param5: Postgres password" << std::endl <<
			"param6: Postgres Database name" << std::endl <<
			"param7: Document table name" << std::endl <<
			"param8: Words table name" << std::endl <<
			"param9: Index table name" << std::endl;
		return -5;
	}
	else
	{
		std::cout << dbstr.httpServerexec << std::endl << "Too much parameters!" << std::endl << "Necessary parameters:" << std::endl <<
			"param1: Server address" << std::endl <<
			"param2: Server port" << std::endl <<
			"param3: Postgres username" << std::endl <<
			"param4: Postgres host" << std::endl <<
			"param5: Postgres password" << std::endl <<
			"param6: Postgres Database name" << std::endl <<
			"param7: Document table name" << std::endl <<
			"param8: Words table name" << std::endl <<
			"param9: Index table name" << std::endl;
		return -6;
	}
	return 0;
}