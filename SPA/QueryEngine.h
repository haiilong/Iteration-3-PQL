#pragma once

#include <string>
#include <list>

#include "QueryObject.h"
#include "QueryPreProcessor.h"
#include "QueryEvaluator.h"
#include "ResultFetcher.h"

class QueryEngine {
public:
	QueryEngine(ResultFetcher &resultFetcher) : evaluator(resultFetcher) {};
	std::list<std::string> getResults(std::string query);
private:
	QueryPreProcessor preprocessor;
	QueryEvaluator evaluator;
};
