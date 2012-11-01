#ifndef BOMBERPERSON_CONFIG_HPP
#define BOMBERPERSON_CONFIG_HPP

#include "utils/Utils.hpp"

#include <string>
#include <vector>

class BomberPersonConfig
{
public:
   BomberPersonConfig(const std::string& cfg_path);
   ~BomberPersonConfig();

   BomberPersonConfig(const BomberPersonConfig&) = delete;
   BomberPersonConfig& operator=(const BomberPersonConfig&) = delete;

   std::string GetAppTitle() const;
   std::string GetResourceDir() const;
   Size GetResolution() const;
   int GetUpdatesPerSecond() const;
//   std::vector<std::string> RenderEngines() const;

   Size GetArenaSize() const;
   Point GetArenaPosition() const;
   Size GetArenaBorderSize() const;
   int GetArenaCellsX() const;
   int GetArenaCellsY() const;
   std::vector<Point> GetSpawningCells() const;

   Size GetScoreboardSize() const;
   Point GetScoreboardPosition() const;

   Size GetCellSize() const;
   int GetBombLifetime() const;
   int GetExplosionLifetime() const;
   int GetPlayerSpawnLength() const;
   int GetPlayerDeathLength() const;

private:
   void Load();

   const std::string mConfigPath;
   std::string mAppTitle;
   std::string mResDir;
   Size mResolution = { 0, 0 };
   int mUpdatesPerSecond = 0;

   Size mArenaSize = { 0, 0 };
   Point mArenaPos = { 0, 0 };
   Size mArenaBorderSize = { 0, 0 };
   int mArenaCellsX = 0;
   int mArenaCellsY = 0;
   std::vector<Point> mSpawningCells;

   Size mScoreboardSize = { 0, 0 };
   Point mScoreboardPos = { 0, 0 };

   Size mCellSize = { 0, 0 };
   int mBombLifetime = 0;
   int mExplosionLifetime = 0;
   int mPlayerSpawnLength = 0;
   int mPlayerDeathLength = 0;

//   std::vector<std::string> mRenderEngines;
};

#endif // BOMBERPERSON_CONFIG_HPP
