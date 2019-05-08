// Brian Malloy        Data-Driven Object oriented Game Construction
#include "engine.h"

ImageFactory*  ImageFactory::instance = NULL;
RenderContext* RenderContext::instance = NULL;

int main(int, char*[]) {
   try {
     int r = 1;
     while(r == 1){
      Engine engine;
      r = engine.play();
      //delete RenderContext::getInstance();
    };
    delete RenderContext::getInstance();
   }
   catch (const string& msg) { std::cout << msg << std::endl; }
   catch (...) {
      std::cout << "Oops, someone threw an exception!" << std::endl;
   }
   return 0;
}
