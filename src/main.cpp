#include "BomberPersonApp.hpp"

#include <iostream>

int main()
{
   try
   {
      BomberPersonApp app;
      app.Start();
   }
   catch (const char* e)
   {
      std::cout << "ERROR: " << e << std::endl;
   }
   catch (...)
   {
      std::cout << "Unknown exception thrown!" << std::endl;
   }
}
