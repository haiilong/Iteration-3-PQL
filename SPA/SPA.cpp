#include <string>
#include <fstream>
#include <sstream>

#include "SPA.h"

void SPA::parse(std::string filename) {
	std::ifstream sourceFile{ filename };
	std::stringstream sourceBuffer;
	sourceBuffer << sourceFile.rdbuf();
    try {
        sourceParser.parse(sourceBuffer.str());
    }
    catch(std::exception &e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        exit(EXIT_SUCCESS);
    }
}

void SPA::evaluate(std::string query, std::list<std::string>& results) {
	results = queryEngine.getResults(query);
}
