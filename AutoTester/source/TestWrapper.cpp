#include "SPA.h"
#include "TestWrapper.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool TestWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
	// call your parser to do the parsing
	// ...rest of your code...
	spa.parse(filename);
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results){
	spa.evaluate(query, results);

	/* Check if AutoTester sent stop signal. */
	if (AbstractWrapper::GlobalStop) {
		return;
	}
}
