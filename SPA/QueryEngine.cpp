#include "QueryEngine.h"
#include "IPKB.h"

// validate a query, create a QueryObject, evaluate and return result list
std::list<std::string> QueryEngine::getResults(std::string query) {
	std::list<std::string> results; 

	if (preprocessor.validate(query)) {  
		auto object = preprocessor.getQueryObject();
		auto resultsVector = evaluator.evaluate(object);
		results.assign(resultsVector.cbegin(), resultsVector.cend());
	}
	return results;
}
