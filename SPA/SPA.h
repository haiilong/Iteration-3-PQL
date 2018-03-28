#pragma once

#include <list>
#include <string>

#include "IPKB.h"
#include "pkb.h"
#include "SourceParser.h"
#include "QueryEngine.h"

class SPA {
public:
	SPA() = default;
	void parse(std::string filename);
	void evaluate(std::string query, std::list<std::string>& results);
private:
	PKB &pkb = *PKB::getPKBInstance();
	SourceParser &sourceParser = *SourceParser::getInstance(pkb);
	QueryEngine queryEngine{ pkb };
};