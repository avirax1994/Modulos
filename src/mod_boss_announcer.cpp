//by SymbolixDEV
//Fixed Error SymbolixDEV
//Editado Avirax
#include "ScriptMgr.h"
#include "Config.h"
#include "Map.h"
#include "Group.h"
#include "Guild.h"
#include <Player.h>

class Boss_Announcer : public PlayerScript
{
public:
	Boss_Announcer() : PlayerScript("Boss_Announcer") {}
	
    void OnLogin(Player *player)
    {
        if (sConfigMgr->GetBoolDefault("Boss.Announcer.Enable", true))
        {
            if (sConfigMgr->GetBoolDefault("Boss.Announcer.Announce", true))
            {
                ChatHandler(player->GetSession()).SendSysMessage("Este servidor está ejecutando el |cff4CFF00BossAnnouncer |rmodule.");
            }
        }
    }

	void OnCreatureKill(Player* player, Creature* boss)
	{
		if (sConfigMgr->GetBoolDefault("Boss.Announcer.Enable", true))
		{
			if (boss->isWorldBoss())
			{
			int gmcount = 0;
			char gn[50];
			if (!player->GetGroup())
				gmcount = 1;
			else
				gmcount = player->GetGroup()->GetMembersCount();
			if (!player->GetGuildId())
				strcpy(gn, "Ninguna");
			else
				strcpy(gn, player->GetGuild()->GetName().c_str());
			char msg[600];
			snprintf(msg, 600, "|CFF7BBEF7[Anuncio de Raid]:|r %s %d%s la Hermandad: |cffff0000 %s|r ha asesinado a |cffff0000 %s|r. Progreso:%d/%d, Jugadores:%d/%d", player->GetMap()->GetMapName(), player->GetMap()->Is25ManRaid() ? 25 : 10, player->GetMap()->IsHeroic() ? "H" : "N", gn, boss->GetName().c_str(),player->GetInstanceScript()->GetCompletedEncounterMask() - 1, player->GetInstanceScript()->GetEncounterCount() - 1, gmcount, player->GetMap()->Is25ManRaid() ? 25 : 10);
			sWorld->SendServerMessage(SERVER_MSG_STRING, msg);
		}
		}
	};
};

class Boss_Announcer_World : public WorldScript
{
public:
	Boss_Announcer_World() : WorldScript("Boss_Announcer_World") { }

	void OnBeforeConfigLoad(bool reload) override
	{
		if (!reload) {
			std::string conf_path = _CONF_DIR;
			std::string cfg_file = conf_path + "/mod_boss_announcer.conf";
#ifdef WIN32
			cfg_file = "mod_boss_announcer.conf";
#endif
			std::string cfg_def_file = cfg_file + ".dist";
			sConfigMgr->LoadMore(cfg_def_file.c_str());

			sConfigMgr->LoadMore(cfg_file.c_str());
		}
	}
};

void AddBoss_AnnouncerScripts()
{
	new Boss_Announcer_World;
	new Boss_Announcer;
}