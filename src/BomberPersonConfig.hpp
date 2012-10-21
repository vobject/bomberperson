#ifndef BOMBERPERSON_CONFIG_HPP
#define BOMBERPERSON_CONFIG_HPP

#include "utils/Utils.hpp"

#include <string>
//#include <vector>

class BomberPersonConfig
{
public:
   BomberPersonConfig(const std::string& cfg_path);
   ~BomberPersonConfig();

   BomberPersonConfig(const BomberPersonConfig&) = delete;
   BomberPersonConfig& operator=(const BomberPersonConfig&) = delete;

   std::string AppTitle() const;
   std::string ResourceDir() const;
//   std::vector<std::string> RenderEngines() const;

private:
   void Load();

   const std::string mConfigPath;
   std::string mAppTitle;
   std::string mResDir;

//   std::vector<std::string> mRenderEngines;
};

#endif // BOMBERPERSON_CONFIG_HPP
