#pragma once
#include"http_server.h"

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

	if (argc < 10)
	{
		std::cout << "WebSpider Server" << std::endl << "Not enough parameters!" << std::endl <<
			"param1: Server address" << std::endl <<
			"param2: Server port" << std::endl << 
			"param3: Postgres username" << std::endl <<
			"param4: Postgres host" << std::endl <<
			"param5: Postgres password" << std::endl <<
			"param6: Postgres Database name" << std::endl <<
			"param7: Document table name" << std::endl <<
			"param8: Words table name" << std::endl <<
			"param9: Index table name" << std::endl;
		return -1;
	}
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

		std::cout << "WebSpider Server" << std::endl << "Connect: http://" << argv[4] << ":" << serverPort << std::endl;

		ioc.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << "SERVER ERROR: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}