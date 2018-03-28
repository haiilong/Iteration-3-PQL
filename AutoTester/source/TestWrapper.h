#ifndef TESTWRAPPER_H
#define TESTWRAPPER_H

#include <string>
#include <iostream>
#include <list>

// include your other headers here
#include "AbstractWrapper.h"
//#include "SPA.h"

class TestWrapper : public AbstractWrapper {
public:
  // default constructor
  TestWrapper();
  
  // destructor
  ~TestWrapper() = default;
  
  // method for parsing the SIMPLE source
  virtual void parse(std::string filename);
  
  // method for evaluating a query
  virtual void evaluate(std::string query, std::list<std::string>& results);

private:
  // entry point for query is QueryEngine
  // TODO: in future the QueryEngine instance should be replaced
  //   by a SPA instance containing QueryEngine which will forward
  //   evaluate() calls to the QueryEngine.
  SPA spa;
};

#endif
