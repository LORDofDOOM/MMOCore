/* PryDevServer: Ultra Core custom ScriptDev2 Scripts
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: item_custom_stone
SD%Complete: 100
SDComment: Item that gives an extra talent point
Author: Zendik
Conversion: Pryd from PDS-4TC and PDS-UC
SDCategory: Items
EndScriptData */

#include "ScriptPCH.h"

class ItemUse_item_custom_stone : public ItemScript
{
    public:

        ItemUse_item_custom_stone()
            : ItemScript("item_custom_stone")
        {
        }

       




	bool OnUse(Player* pPlayer, Item* pItem, const SpellCastTargets &pTargets)
  	{
    	if ((pPlayer->isInCombat()) || (pPlayer->isInFlight()) || (pPlayer->isDead()))
    	{
      	pPlayer->SendEquipError(EQUIP_ERR_NOT_IN_COMBAT, pItem, NULL);
        return false;
    	}
    	if (pPlayer->IsMounted())
    	{
      	pPlayer->SendEquipError(EQUIP_ERR_CANT_DO_RIGHT_NOW, pItem, NULL);
        return true;
    	}
    	{
      pPlayer->SetFreeTalentPoints(1);
      pPlayer->SendTalentsInfoData(false);
      pPlayer->DestroyItemCount(pItem->GetEntry(),1,true);
      return true;
  		}

	}
};

// bool OnUse(Player* /*player*/, Item* /*item*/, SpellCastTargets const& /*targets*/)
       // {
         //   sScriptMgr.LoadDatabase();
           // return true;
      //  }



void AddSC_item_custom_stone()
{
    new ItemUse_item_custom_stone();
}