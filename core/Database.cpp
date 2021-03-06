// ----------------------------------------------------------------------------
//   ___  ___  ___  ___       ___  ____  ___  _  _
//  /__/ /__/ /  / /__  /__/ /__    /   /_   / |/ /
// /    / \  /__/ ___/ ___/ ___/   /   /__  /    /  emulator
//
// ----------------------------------------------------------------------------
// Copyright 2005 Greg Stanton
// 
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
// ----------------------------------------------------------------------------
// Database.cpp
// ----------------------------------------------------------------------------
#include "Database.h"
#include "Cartridge.h"
#include "Common.h"
#define DATABASE_SOURCE "Database.cpp"

bool database_enabled = true;
std::string database_filename;

static std::string database_GetValue(std::string entry)
{
   int index = entry.rfind('=');
   return entry.substr(index + 1);
}

// ----------------------------------------------------------------------------
// Initialize
// ----------------------------------------------------------------------------
void database_Initialize(void)
{
   database_filename = common_defaultPath + "ProSystem.dat";
}

// ----------------------------------------------------------------------------
// Load
// ----------------------------------------------------------------------------
bool database_Load(const char *digest)
{
   if(database_enabled)
   {
      FILE* file = fopen(database_filename.c_str( ), "r");
      if(file == NULL) {
         return false;  
      }

      char buffer[256];
      while(fgets(buffer, 256, file) != NULL)
      {
         std::string line = buffer;
         if(line.compare(1, 32, digest) == 0)
         {
            int index;
            std::string entry[7];

            for(index = 0; index < 7; index++)
            {
               fgets(buffer, 256, file);
               entry[index] = common_Remove(buffer, '\n');  
               entry[index] = common_Remove(entry[index], '\r');
            }

            cartridge_type = common_ParseByte(database_GetValue(entry[1]).c_str());
            cartridge_pokey = common_ParseBool(database_GetValue(entry[2]));
            cartridge_controller[0] = common_ParseByte(database_GetValue(entry[3]).c_str());
            cartridge_controller[1] = common_ParseByte(database_GetValue(entry[4]).c_str());
            cartridge_region = common_ParseByte(database_GetValue(entry[5]).c_str());
            cartridge_flags = common_ParseUint(database_GetValue(entry[6]).c_str());
            break;
         }
      }    

      fclose(file);  
   }

   return true;
}
