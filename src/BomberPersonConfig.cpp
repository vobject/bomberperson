#include "BomberPersonConfig.hpp"

#include <tinyxml.h>

BomberPersonConfig::BomberPersonConfig(const std::string& cfg_path)
   : mConfigPath(cfg_path)
{
   Load();
}

BomberPersonConfig::~BomberPersonConfig()
{

}

std::string BomberPersonConfig::AppTitle() const
{
   return mAppTitle;
}

std::string BomberPersonConfig::ResourceDir() const
{
   return mResDir;
}

//std::vector BomberPersonConfig::RenderEngines() const
//{
//   return mRenderEngines;
//}

void BomberPersonConfig::Load()
{
   TiXmlDocument doc(mConfigPath);
   if (!doc.LoadFile()) {
      throw "Unable to load config XML file.";
   }

   TiXmlHandle doc_hndl(&doc);
   TiXmlHandle root_hndl(doc_hndl.FirstChild());

   TiXmlHandle app_hndl(root_hndl.FirstChild("Application"));
   mAppTitle = app_hndl.FirstChild("Title").Element()->GetText();
   mResDir = app_hndl.FirstChild("ResourceDir").Element()->GetText();

//   auto rend_elem = root_hndl.FirstChild("Render").Element();
//   for (; rend_elem; rend_elem = rend_elem->NextSiblingElement())
//   {
//      mRenderEngines.push_back(rend_elem->GetText());
//   }
}
