#include "BomberPersonConfig.hpp"

#include <tinyxml.h>

#include <sstream>

BomberPersonConfig::BomberPersonConfig(const std::string& cfg_path)
   : mConfigPath(cfg_path)
{
   Load();
}

BomberPersonConfig::~BomberPersonConfig()
{

}

std::string BomberPersonConfig::GetAppTitle() const
{
   return mAppTitle;
}

std::string BomberPersonConfig::GetResourceDir() const
{
   return mResDir;
}

Size BomberPersonConfig::GetResolution() const
{
   return mResolution;
}

int BomberPersonConfig::GetUpdatesPerSecond() const
{
   return mUpdatesPerSecond;
}

Size BomberPersonConfig::GetArenaSize() const
{
   return mArenaSize;
}

Point BomberPersonConfig::GetArenaPosition() const
{
   return mArenaPos;
}

Size BomberPersonConfig::GetArenaBorderSize() const
{
   return mArenaBorderSize;
}

int BomberPersonConfig::GetArenaCellsX() const
{
   return mArenaCellsX;
}

int BomberPersonConfig::GetArenaCellsY() const
{
   return mArenaCellsY;
}

std::vector<Point> BomberPersonConfig::GetSpawningCells() const
{
   return mSpawningCells;
}

Size BomberPersonConfig::GetScoreboardSize() const
{
   return mScoreboardSize;
}

Point BomberPersonConfig::GetScoreboardPosition() const
{
   return mScoreboardPos;
}

Size BomberPersonConfig::GetCellSize() const
{
   return mCellSize;
}

int BomberPersonConfig::GetBombLifetime() const
{
   return mBombLifetime;
}

int BomberPersonConfig::GetExplosionLifetime() const
{
   return mExplosionLifetime;
}

int BomberPersonConfig::GetPlayerSpawnLength() const
{
   return mPlayerSpawnLength;
}

int BomberPersonConfig::GetPlayerDeathLength() const
{
   return mPlayerDeathLength;
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
   TiXmlHandle root_hndl(doc_hndl.FirstChildElement());

   TiXmlHandle app_hndl(root_hndl.FirstChild("Application"));
   mAppTitle = app_hndl.FirstChild("Title").Element()->GetText();
   mResDir = app_hndl.FirstChild("ResourceDir").Element()->GetText();

   std::istringstream width_str(app_hndl.FirstChild("Resolution").FirstChild("Width").Element()->GetText());
   std::istringstream height_str(app_hndl.FirstChild("Resolution").FirstChild("Height").Element()->GetText());
   width_str >> mResolution.Width;
   height_str >> mResolution.Height;

   std::istringstream ups_str(app_hndl.FirstChild("UpdatesPerSecond").Element()->GetText());
   ups_str >> mUpdatesPerSecond;

   std::istringstream bomb_life_str(app_hndl.FirstChild("BombLifetime").Element()->GetText());
   bomb_life_str >> mBombLifetime;

   std::istringstream expl_life_str(app_hndl.FirstChild("ExplosionLifetime").Element()->GetText());
   expl_life_str >> mExplosionLifetime;

   std::istringstream player_spawn_str(app_hndl.FirstChild("PlayerSpawnLength").Element()->GetText());
   player_spawn_str >> mPlayerSpawnLength;

   std::istringstream player_death_str(app_hndl.FirstChild("PlayerDeathLength").Element()->GetText());
   player_death_str >> mPlayerDeathLength;

   TiXmlHandle arena_hndl(app_hndl.FirstChild("Arena"));
   std::istringstream arena_width_str(arena_hndl.FirstChild("Width").Element()->GetText());
   std::istringstream arena_height_str(arena_hndl.FirstChild("Height").Element()->GetText());
   arena_width_str >> mArenaSize.Width;
   arena_height_str >> mArenaSize.Height;

   std::istringstream arena_pos_x_str(arena_hndl.FirstChild("Position").FirstChild("X").Element()->GetText());
   std::istringstream arena_pos_Y_str(arena_hndl.FirstChild("Position").FirstChild("Y").Element()->GetText());
   arena_pos_x_str >> mArenaPos.X;
   arena_pos_Y_str >> mArenaPos.Y;

   std::istringstream arena_borders_width_str(arena_hndl.FirstChild("Borders").FirstChild("Width").Element()->GetText());
   std::istringstream arena_borders_height_str(arena_hndl.FirstChild("Borders").FirstChild("Height").Element()->GetText());
   arena_borders_width_str >> mArenaBorderSize.Width;
   arena_borders_height_str >> mArenaBorderSize.Height;

   std::istringstream arena_cells_x_str(arena_hndl.FirstChild("Cells").FirstChild("X").Element()->GetText());
   std::istringstream arena_cells_Y_str(arena_hndl.FirstChild("Cells").FirstChild("Y").Element()->GetText());
   arena_cells_x_str >> mArenaCellsX;
   arena_cells_Y_str >> mArenaCellsY;

   TiXmlHandle sb_hndl(app_hndl.FirstChild("Scoreboard"));
   std::istringstream sb_width_str(sb_hndl.FirstChild("Width").Element()->GetText());
   std::istringstream sb_height_str(sb_hndl.FirstChild("Height").Element()->GetText());
   sb_width_str >> mScoreboardSize.Width;
   sb_height_str >> mScoreboardSize.Height;

   std::istringstream sb_pos_x_str(sb_hndl.FirstChild("Position").FirstChild("X").Element()->GetText());
   std::istringstream sb_pos_Y_str(sb_hndl.FirstChild("Position").FirstChild("Y").Element()->GetText());
   sb_pos_x_str >> mScoreboardPos.X;
   sb_pos_Y_str >> mScoreboardPos.Y;

   mCellSize.Width = (mArenaSize.Width - mArenaBorderSize.Width) / mArenaCellsX;
   mCellSize.Height = (mArenaSize.Height - mArenaBorderSize.Height) / mArenaCellsY;

   for (auto arena_spawn_elem = arena_hndl.FirstChild("SpawningCell").Element();
        arena_spawn_elem;
        arena_spawn_elem = arena_spawn_elem->NextSiblingElement())
   {
      std::istringstream arena_spawn_x_str(arena_spawn_elem->FirstChildElement("X")->GetText());
      std::istringstream arena_spawn_y_str(arena_spawn_elem->FirstChildElement("Y")->GetText());
      int arena_spawn_x;
      int arena_spawn_y;
      arena_spawn_x_str >> arena_spawn_x;
      arena_spawn_y_str >> arena_spawn_y;
      mSpawningCells.push_back({ arena_spawn_x, arena_spawn_y });
   }

//   auto rend_elem = root_hndl.FirstChild("Render").Element();
//   for (; rend_elem; rend_elem = rend_elem->NextSiblingElement())
//   {
//      mRenderEngines.push_back(rend_elem->GetText());
//   }
}
