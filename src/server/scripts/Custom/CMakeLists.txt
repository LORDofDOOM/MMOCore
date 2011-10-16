# Copyright (C) 2008-2011 TrinityCore <http://www.trinitycore.org/>
#
# This file is free software; as a special exception the author gives
# unlimited permission to copy and/or distribute it, with or without
# modifications, as long as this notice is preserved.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY, to the extent permitted by law; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  Custom/hunterpet_vendor.cpp
  Custom/npc_customize.cpp      
  Custom/npc_mount.cpp 
  Custom/argent_tournament.cpp
  Custom/npc_item_summon.cpp      
  Custom/resetduel.cpp  
  #Custom/killstreak_pvp_system.cpp  
  Custom/item_custom_stone.cpp  
  #Custom/item_requester.cpp
  Custom/allinone.cpp
  Custom/professionnpc.cpp
  #Custom/npc_lottery.cpp
  Custom/npcs_mini_boss.cpp  
  Custom/guildhouse_npc.cpp    
  Custom/cs_fake.cpp  
  Custom/level_rewards.cpp  
  Custom/npc_legion_master.cpp
)

message("  -> Prepared: Custom")
