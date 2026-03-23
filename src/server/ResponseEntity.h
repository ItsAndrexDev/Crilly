#pragma once
#include <iostream>
#include <vector>
enum ActionType {
	UPLOAD_FILE,
	DOWNLOAD_FILE,
	EXECUTE_COMMAND,
	GET_SCREENSHOT,
	OPEN_WEBSITE,
	NEW_CLIENT,
	UNKNOWN
};

struct ClientIdentifiers {
	char hwid[128];
	char ip[16];
	char username[32];
	char os[64];
	char antivirus[64];
	char country[64];
	bool admin;
};

struct PacketHeader {
	size_t length;
};

struct DataPacket {
	std::vector<char> data;
};