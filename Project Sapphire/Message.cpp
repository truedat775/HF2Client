#include "Hack.h"
#include <boost/lexical_cast.hpp>

VOID DeathMessage()
{
	if (ClientReady(FALSE))
	{
		CHAR String[10000], Buffer[10000];
		LPSTR Message[20] = {V_Line1, V_Line2, V_Line3, V_Line4, V_Line5, V_Line6, V_Line7, V_Line8, V_Line9, V_Line10,
			V_Line11, V_Line12, V_Line13, V_Line14, V_Line15, V_Line16, V_Line17, V_Line18, V_Line19, V_Line20};

		for (INT i = 0; i < ArraySize(Message) && i <= 20; i++)
		{
			if (strlen(Message[i]) <= NULL)
				continue;

			if (strcpy(String, Message[i]) && strcpy(Buffer, Message[i]))
			{
				if (strstr(String, "#My Account Name#"))
					strcpy_s(Buffer, ReplaceString(String, "#My Account Name#", D2CLIENT_GetGameInfo()->szAccountName));

				if (strstr(String, "#My Character Name#"))
					strcpy_s(Buffer, ReplaceString(String, "#My Character Name#", D2CLIENT_GetGameInfo()->szCharName));

				if (strstr(String, "#Realm Name#"))
					strcpy_s(Buffer, ReplaceString(String, "#Realm Name#", D2CLIENT_GetGameInfo()->szRealmName));

				if (strstr(String, "#Slain Name#"))
					strcpy_s(Buffer, ReplaceString(String, "#Slain Name#", V_SlainName));

				if (strstr(String, "#Current Song#"))
					strcpy_s(Buffer, ReplaceString(String, "#Current Song#", CurrentSong()));

				if (strstr(String, "#Date#"))
					strcpy_s(Buffer, ReplaceString(String, "#Date#", V_LocalDate));

				if (strstr(String, "#Time#"))
					strcpy_s(Buffer, ReplaceString(String, "#Time#", V_LocalTime));

				if (strstr(String, "#Total Kill Count#"))
					strcpy_s(Buffer, ReplaceString(String, "#Total Kill Count#", V_InjectKills));

				if (strstr(String, "#In-Game Kill Count#"))
					strcpy_s(Buffer, ReplaceString(String, "#In-Game Kill Count#", V_InGameKills));

				if (strstr(String, "#Total Death Count#"))
					strcpy_s(Buffer, ReplaceString(String, "#Total Death Count#", V_InjectDeaths));

				if (strstr(String, "#In-Game Death Count#"))
					strcpy_s(Buffer, ReplaceString(String, "#In-Game Death Count#", V_InGameDeaths));

				if (Say(String))
					SleepEx(V_DeathMessageDelay, TRUE);
			}
		}
	}
}

BOOL FASTCALL Input(WCHAR * Text)
{
	CHAR Buffer[100];
	CHAR Message[100];
	CHAR * Argument[32];

	WideCharToMultiByte(CP_ACP, 0, Text, -1, Buffer, sizeof(Buffer), 0, 0);
	strcpy(Message, Buffer);
	INT Arguments = StringTokenize(Buffer + 1, ' ', Argument, 32);

	if (Buffer[0] == ';')
	{
		if (!_stricmp(Argument[0], "hc"))
		{
			V_HideCommands = !V_HideCommands;
			Print(0, 4, V_HideCommands ? "%sÿc0: ÿc4Commands are hidden" : "%sÿc0: ÿc1Commands aren't hidden", V_Name);
			return FALSE;
		}
	}

	if (V_HideCommands)
		return TRUE;

	if (Buffer[0] == '.')
	{
		if(!V_GMMode)
			return FALSE;

		if (!_stricmp(Argument[0], "0"))
		{
			SYSTEMTIME Time;
			GetLocalTime(&Time);
			sprintf_s(V_LocalDate, "%.2d-%.2d-%.2d", Time.wMonth, Time.wDay, Time.wYear);

			if (Time.wHour < 13 || Time.wHour > 23)
				sprintf_s(V_LocalTime, "%.2d:%.2d:%.2d AM", Time.wHour == 0 ? 12 : Time.wHour, Time.wMinute, Time.wSecond);
			else if (Time.wHour < 24 || Time.wHour > 12)
				sprintf_s(V_LocalTime, "%.2d:%.2d:%.2d PM", Time.wHour == 24 ? 12 : Time.wHour - 12, Time.wMinute, Time.wSecond);

			V_DeathMessageIteration = TRUE;
			return FALSE;
		}

		if (!_stricmp(Argument[0], "aa"))
		{
			V_Aim = !V_Aim;
			Print(0, 4, V_Aim ? "ÿc4Lazarusÿc0: ÿc4Enabled" : "ÿc4Lazarusÿc0: ÿc1Disabled");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "af"))
		{
			V_AntiFlash = !V_AntiFlash;
			Print(0, 4, V_AntiFlash ? "Flashÿc0: ÿc4Anti Flash is enabled" : "Flashÿc0: ÿc1Anti Flash is disabled");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "aig"))
		{
			V_AntiItemGlitch = !V_AntiItemGlitch;
			Print(0, 4, V_AntiItemGlitch ? "%sÿc0: ÿc4Anti Flash is enabled" : "%sÿc0: ÿc1Anti Flash is disabled", V_Name);
			return FALSE;
		}

		if (!_stricmp(Argument[0], "am"))
		{
			V_AntiMinimize = !V_AntiMinimize;
			Print(0, 4, V_AntiMinimize ? "%sÿc0: ÿc4Anti Minimize is enabled" : "%sÿc0: ÿc1Anti Minimize is disabled", V_Name);
			return FALSE;
		}

		if (!_stricmp(Argument[0], "ap"))
		{
			V_AutoParty = !V_AutoParty;
			Print(0, 4, V_AutoParty ? "%sÿc0: ÿc4Auto Party is enabled" : "%sÿc0: ÿc1Auto Party is disabled", V_Name);
			return FALSE;
		}

		if (!_stricmp(Argument[0], "aqb"))
		{
			V_AntiQuestBox = !V_AntiQuestBox;
			Print(0, 4, V_AntiQuestBox ? "%sÿc0: ÿc4Anti Quest Box is enabled" : "%sÿc0: ÿc1Anti Quest Box is disabled", V_Name);
			return FALSE;
		}

		if (!_stricmp(Argument[0], "as"))
		{
			if (Argument[1])
			{
				ChangeStat(STAT_IAS, (atoi(Argument[1]) < 255) ? atoi(Argument[1]) : 255);
				
				if (atoi(Argument[1]) > 0)
					Print(0, 4, "ÿc4Lazarusÿc0: Increased attack speed by ÿc3%s%%ÿc0.", Argument[1]);
				else
					Print(0, 4, "ÿc4Lazarusÿc0: Decreased attack speed to normal %%");

				return FALSE;
			}
		}

		if (!_stricmp(Argument[0], "bf"))
		{
			V_BlockFakes = !V_BlockFakes;
			Print(0, 4, V_BlockFakes ? "ÿc4Lazarusÿc0: Blocking fake attacks and casts" : "ÿc4Lazarusÿc0: ÿc1Not blocking fake attacks and casts");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "br"))
		{
			if (Argument[1])
			{
				ChangeStat(STAT_FASTERBLOCK, (atoi(Argument[1]) < 255) ? atoi(Argument[1]) : 255);
				
				if (atoi(Argument[1]) > 0)
					Print(0, 4, "ÿc4Lazarusÿc0: Increased block rate by ÿc3%s%%", Argument[1]);
				else
					Print(0, 4, "ÿc4Lazarusÿc0: Decreased block rate to normal %%");

				return FALSE;
			}
		}

		if (!_stricmp(Argument[0], "btp"))
		{
			V_BlockTP = !V_BlockTP;
			Print(0, 4, V_BlockTP ? "ÿc4Lazarusÿc0: Blocking town portal interaction when towned by life/mana %%" : "ÿc4Lazarusÿc0: ÿc1Not blocking town portal interaction when towned by life/mana %%");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "clean"))
		{
			V_Design->Hide = !V_Design->Hide;
			return FALSE;
		}

		if (!_stricmp(Argument[0], "cr"))
		{
			if (Argument[1])
			{
				ChangeStat(STAT_FASTERCAST, (atoi(Argument[1]) < 255) ? atoi(Argument[1]) : 255);
				
				if (atoi(Argument[1]) > 0)
					Print(0, 4, "ÿc4Lazarusÿc0: Increased cast rate by ÿc3%s%%", Argument[1]);
				else
					Print(0, 4, "ÿc4Lazarusÿc0: Decreased cast rate to normal %%");

				return FALSE;
			}
		}

		if (!_stricmp(Argument[0], "cs"))
		{
			if (Argument[1])
			{
				CHAR Temporary[10000];

				if (MaxTime() != INFINITE)
					sprintf_s(Temporary, "/f m ÿc%s%s%s <%.2d:%.2d:%.2d / %.2d:%.2d:%.2d>", Argument[1], V_CurrentSongPhrase, CurrentSong(), (CurrentTime() / 3600000) % 60, (CurrentTime() / 60000) % 60, (CurrentTime() / 1000) % 60, (MaxTime() / 3600) % 60, (MaxTime() / 60) % 60, MaxTime() % 60);
				else
					sprintf_s(Temporary, "/f m ÿc%s%s%s <%.2d:%.2d:%.2d / --:--:-->", Argument[1], V_CurrentSongPhrase, CurrentSong(), (CurrentTime() / 3600000) % 60, (CurrentTime() / 60000) % 60, (CurrentTime() / 1000) % 60);

				BNCLIENT_SendBNMessage(Temporary);
				return FALSE;
			}
		}

		if (!_stricmp(Argument[0], "cube"))
		{
			if (!GetUIVar(UI_CUBE))
			{
				BYTE Packet[2];
				Packet[0] = 0x77;
				Packet[1] = 0x15;
				D2NET_ReceivePacket(Packet, 2);
				V_CubeOrStashOpened = TRUE;
			}

			else
			{
				D2CLIENT_SetUIVar(UI_CUBE, 1, 0);
				return FALSE;
			}

			return FALSE;
		}

		if (!_stricmp(Argument[0], "dg"))
		{
			if (GetUnitStat(Me, STAT_GOLD) <= 0)
			{
				Print(0, 4, "%sÿc0: ÿc1No gold found in inventory", V_Name);
				return FALSE;
			}

			if (Argument[1] && GetUnitStat(Me, STAT_GOLD) > 0)
			{
				V_GoldAmount = atoi(Argument[1]);
				V_GoldDrop = !V_GoldDrop;

				if (V_GoldDrop)
					Print(0, 4, "%sÿc0: Dropping %s gold", V_Name, Argument[1]);
				else
					Print(0, 4, "%sÿc0: ÿc1Stopped dropping gold", V_Name);

				return FALSE;
			}

			V_GoldDrop = FALSE;
			Print(0, 4, "%sÿc0: ÿc1Stopped dropping gold", V_Name);
			return FALSE;
		}

		if (!_stricmp(Argument[0], "dm"))
		{
			V_DeathMessage = !V_DeathMessage;
			Print(0, 4, V_DeathMessage ? "Death ÿc4Lazarusÿc0: ÿc4Enabled" : "Death ÿc4Lazarusÿc0: ÿc1Disabled");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "eb"))
		{
			V_Enchant = !V_Enchant;
			Print(0, 4, V_Enchant ? "ÿc4Lazarusÿc0: ÿc4Enabled" : "ÿc4Lazarusÿc0: ÿc1Disabled");

			if (V_Enchant && V_EnchantNote)
				V_EnchantNoteIteration = TRUE;
			else if (!V_Enchant && V_EnchantNote)
				Say("%s's Enchant Bot is disabled", V_Name);

			return FALSE;
		}

		if (!_stricmp(Argument[0], "ebn"))
		{
			V_EnchantNote = !V_EnchantNote;
			Print(0, 4, V_EnchantNote ? "ÿc4Lazarusÿc0: ÿc4Note is enabled" : "ÿc4Lazarusÿc0: ÿc1Note is disabled");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "el"))
		{
			if (Argument[1])
			{
				V_LifeExitPercent = atoi(Argument[1]);

				if (V_LifeExitPercent > 0)
					Print(0, 4, "ÿc4Lazarusÿc0: ÿc4Escaping out of game at ÿc1%s%% life", Argument[1]);
				else
					Print(0, 4, "ÿc4Lazarusÿc0: Escaping out of game at ÿc1life %% ÿc0is off");

				return FALSE;
			}
		}

		if (!_stricmp(Argument[0], "em"))
		{
			if (Argument[1])
			{
				V_ManaExitPercent = atoi(Argument[1]);

				if (V_ManaExitPercent > 0)
					Print(0, 4, "ÿc4Lazarusÿc0: ÿc4Escaping out of game at ÿc3%s%% mana", Argument[1]);
				else
					Print(0, 4, "ÿc4Lazarusÿc0: Escaping out of game at ÿc3mana %% ÿc0is off");

				return FALSE;
			}
		}

		if (!_stricmp(Argument[0], "fake"))
		{
			if (Argument[2])
			{
				SYSTEMTIME Time;
				GetLocalTime(&Time);
				sprintf_s(V_LocalDate, "%.2d-%.2d-%.2d", Time.wMonth, Time.wDay, Time.wYear);

				if (Time.wHour < 13 || Time.wHour > 23)
					sprintf_s(V_LocalTime, "%.2d:%.2d:%.2d AM", Time.wHour == 0 ? 12 : Time.wHour, Time.wMinute, Time.wSecond);
				else if (Time.wHour < 24 || Time.wHour > 12)
					sprintf_s(V_LocalTime, "%.2d:%.2d:%.2d PM", Time.wHour == 24 ? 12 : Time.wHour - 12, Time.wMinute, Time.wSecond);

				if (Print(0, 4, "%s was slain by %s", Argument[1], Me->pPlayerData->szName))
					Print(0, 4, "%s(%s) left our world. Diablo's minions weaken.", Argument[1], Argument[2]);

				if (V_DeathMessage)
				{
					strcpy(V_SlainName, Argument[1]);
					V_DeathMessageIteration = TRUE;
				}

				return FALSE;
			}
		}

		if (!_stricmp(Argument[0], "fc"))
		{
			for (INT i = 0; i < V_Players.GetSize(); i++)
			{
				if (V_Players[i]->Flashing)
				{
					V_Players[i]->Flashing = FALSE;
					Print(0, 4, "Flashÿc0: ÿc4Stopped flashing ÿc1%s", V_Players[i]->PlayerName);
				}
			}

			return FALSE;
		}

		if (!_stricmp(Argument[0], "fd"))
		{
			if (Argument[1])
			{
				V_FlashDelay = atoi(Argument[1]);

				if (V_FlashDelay > 0)
					Print(0, 4, "ÿc4Lazarusÿc0: Delay set to ÿc3%d milliseconds", Argument[1]);
				else
					Print(0, 4, "ÿc4Lazarusÿc0: ÿc1Disabled");

				return FALSE;
			}
		}

		if (!_stricmp(Argument[0], "fm"))
		{
			if (Argument[2])
			{
				CHAR Temporary[500];
				sprintf_s(Temporary, "/f m ÿc%s%s", Argument[1], Message + 6);
				BNCLIENT_SendBNMessage(Temporary);
				return FALSE;
			}
		}

		if (!_stricmp(Argument[0], "ftp"))
		{
			V_FastTP = !V_FastTP;
			Print(0, 4, V_FastTP ? "ÿc4Lazarusÿc0: ÿc4Fast Town Portal is enabled" : "ÿc4Lazarusÿc0: ÿc1Fast Town Portal is disabled");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "hack"))
		{
			if (!GetSkill(D2S_CONFUSE))
				SetSkill(D2S_CONFUSE, FALSE);
			else
				SetSkill(D2S_HOLYSHIELD, FALSE);

			return FALSE;
		}

		if (!_stricmp(Argument[0], "hp"))
		{
			if (Argument[1])
			{
				V_LifePotPercent = atoi(Argument[1]);

				if (V_LifePotPercent > 0)
					Print(0, 4, "ÿc4Lazarusÿc0: ÿc4Drinking health potions at ÿc1%s%% life", Argument[1]);
				else
					Print(0, 4, "ÿc4Lazarusÿc0: Drinking health potions at ÿc1life %% ÿc0is off");

				return FALSE;
			}
		}

		if (!_stricmp(Argument[0], "hr"))
		{
			if (Argument[1])
			{
				ChangeStat(STAT_FASTERHITRECOVERY, (atoi(Argument[1]) < 255) ? atoi(Argument[1]) : 255);
				
				if (atoi(Argument[1]) > 0)
					Print(0, 4, "ÿc4Lazarusÿc0: Increased hit recovery rate by ÿc3%s%%", Argument[1]);
				else
					Print(0, 4, "ÿc4Lazarusÿc0: Decreased hit recovery rate to ÿc1normal %%");

				return FALSE;
			}
		}

		if (!_stricmp(Argument[0], "load"))
		{
			Load();
			Print(0, 4, "%sÿc0: ÿc4Loaded configuration", V_Name);
			return FALSE;
		}

		if (!_stricmp(Argument[0], "mh"))
		{
			BOOL Map = V_AutoRevealAutomap;

			if (V_AutoRevealAutomap) V_AutoRevealAutomap = FALSE;
			if (V_Reveal) V_Reveal->RevealAutomap();
			if (!V_AutoRevealAutomap) V_AutoRevealAutomap = Map;

			Print(0, 4, "ÿc4Lazarusÿc0: ÿc4Revealed Act %d", Me->pAct->dwAct + 1);
			return FALSE;
		}

		if (!_stricmp(Argument[0], "mp"))
		{
			if (Argument[1])
			{
				V_ManaPotPercent = atoi(Argument[1]);

				if (V_ManaPotPercent > 0)
					Print(0, 4, "ÿc4Lazarusÿc0: ÿc4Drinking mana potions at ÿc3%s%% mana", Argument[1]);
				else
					Print(0, 4, "ÿc4Lazarusÿc0: Drinking mana potions at ÿc3mana %% ÿc0is off");

				return FALSE;
			}
		}

		if (!_stricmp(Argument[0], "next"))
		{
			NextSong();
			return FALSE;
		}

		if (!_stricmp(Argument[0], "nl"))
		{
			TeleportToVector(1);
			return FALSE;
		}

		if (!_stricmp(Argument[0], "ots"))
		{
			V_OutTownSelect = !V_OutTownSelect;
			Print(0, 4, V_OutTownSelect ? "%sÿc0: ÿc4Players out of town are selectable" : "%sÿc0: ÿc1Players out of town are unselectable", V_Name);
			return FALSE;
		}

		if (!_stricmp(Argument[0], "pause"))
		{
			Pause();
			return FALSE;
		}

		if (!_stricmp(Argument[0], "pl"))
		{
			TeleportToVector(2);
			return FALSE;
		}

		if (!_stricmp(Argument[0], "play"))
		{
			Play();
			return FALSE;
		}

		if (!_stricmp(Argument[0], "prev"))
		{
			PreviousSong();
			return FALSE;
		}

		if (!_stricmp(Argument[0], "qu"))
		{
			TeleportToVector(3);
			return FALSE;
		}

		if (!_stricmp(Argument[0], "rl"))
		{
			if (Argument[1])
			{
				V_LifeJuvPercent = atoi(Argument[1]);

				if (V_LifeJuvPercent > 0)
					Print(0, 4, "ÿc4Lazarusÿc0: ÿc4Drinking rejuvenation potions at ÿc1%s%% life", Argument[1]);
				else
					Print(0, 4, "ÿc4Lazarusÿc0: Drinking rejuvenation potions at ÿc1life %% ÿc0is off");

				return FALSE;
			}
		}

		if (!_stricmp(Argument[0], "rm"))
		{
			if (Argument[1])
			{
				V_ManaJuvPercent = atoi(Argument[1]);

				if (V_ManaJuvPercent > 0)
					Print(0, 4, "ÿc4Lazarusÿc0: ÿc4Drinking rejuvenation potions at ÿc3%s%% mana", Argument[1]);
				else
					Print(0, 4, "ÿc4Lazarusÿc0: Drinking rejuvenation potions at ÿc3mana %% ÿc0is off");

				return FALSE;
			}
		}

		if (!_stricmp(Argument[0], "rw"))
		{
			if (Argument[1])
			{
				ChangeStat(STAT_FASTERRUNWALK, (atoi(Argument[1]) < 255) ? atoi(Argument[1]) : 255);
				
				if (atoi(Argument[1]) > 0)
					Print(0, 4, "ÿc4Lazarusÿc0: Increased run/walk speed by ÿc3%s%%", Argument[1]);
				else
					Print(0, 4, "ÿc4Lazarusÿc0: Decreased run/walk speed to normal %%");

				return FALSE;
			}
		}
		if (!_stricmp(Argument[0], "s1"))
		{
			TeleportToVector(5);
			return FALSE;
		}
		if (!_stricmp(Argument[0], "s2"))
		{
			TeleportToVector(6);
			return FALSE;
		}
		if (!_stricmp(Argument[0], "s3"))
		{
			TeleportToVector(7);
			return FALSE;
		}
		if (!_stricmp(Argument[0], "s4"))
		{
			TeleportToVector(8);
			return FALSE;
		}
		if (!_stricmp(Argument[0], "s5"))
		{
			TeleportToVector(9);
			return FALSE;
		}
		if (!_stricmp(Argument[0], "save"))
		{
			Save();
			Print(0, 4, "%sÿc0: ÿc4Saved configuration");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "sp"))
		{
			V_ProtectItemSocket = !V_ProtectItemSocket;
			Print(0, 4, V_ProtectItemSocket ? "ÿc4Lazarusÿc0: ÿc4Item socket protection enabled" : "ÿc4Lazarusÿc0: ÿc1Item socket protection disabled");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "ss"))
		{
			V_Screenshot = !V_Screenshot;
			Print(0, 4, V_Screenshot ? "Designÿc0: Hide on screenshot enabled" : "Designÿc0: ÿc1Hide on screenshot disabled");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "stash"))
		{
			if (!GetUIVar(UI_STASH))
			{
				BYTE Packet[2];
				Packet[0] = 0x77;
				Packet[1] = 0x10;
				D2NET_ReceivePacket(Packet, 2);
				V_CubeOrStashOpened = TRUE;
			}

			else
			{
				D2CLIENT_SetUIVar(UI_STASH, 1, 0);
				return FALSE;
			}

			return FALSE;
		}

		if (!_stricmp(Argument[0], "stop"))
		{
			Stop();
			return FALSE;
		}
		if (!_stricmp(Argument[0], "t1"))
		{
			if (V_TP) V_TP = FALSE;
			if (!V_TP) Portal();
			return FALSE;
		}
		if (!_stricmp(Argument[0], "t2"))
		{
			if (Portal()) V_TP = TRUE;
			return FALSE;
		}
		if (!_stricmp(Argument[0], "ta"))
		{
			V_TPWhenAttacked = !V_TPWhenAttacked;
			
			if (V_TPWhenAttacked)
				Print(0, 4, "ÿc4Lazarusÿc0: ÿc4Escaping to town when attacked");
			else
				Print(0, 4, "ÿc4Lazarusÿc0: ÿc1Escaping to town when attacked is off");

			return FALSE;
		}

		if (!_stricmp(Argument[0], "th"))
		{
			V_TPWhenHostiled = !V_TPWhenHostiled;
			
			if (V_TPWhenHostiled)
				Print(0, 4, "ÿc4Lazarusÿc0: ÿc4Escaping to town when hostiled");
			else
				Print(0, 4, "ÿc4Lazarusÿc0: ÿc1Escaping to town when hostiled is off");

			return FALSE;
		}

		if (!_stricmp(Argument[0], "tl"))
		{
			if (Argument[1])
			{
				V_LifeTownPercent = atoi(Argument[1]);

				if (V_LifeTownPercent > 0)
					Print(0, 4, "ÿc4Lazarusÿc0: ÿc4Escaping to town at ÿc1%s%% life", Argument[1]);
				else
					Print(0, 4, "ÿc4Lazarusÿc0: Escaping to town at ÿc1life %% ÿc0is off");

				return FALSE;
			}
		}

		if (!_stricmp(Argument[0], "tm"))
		{
			if (Argument[1])
			{
				V_ManaTownPercent = atoi(Argument[1]);

				if (V_ManaTownPercent > 0)
					Print(0, 4, "ÿc4Lazarusÿc0: ÿc4Escaping to town at ÿc3%s%% mana", Argument[1]);
				else
					Print(0, 4, "ÿc4Lazarusÿc0: ÿc0Escaping to town at ÿc3mana %% ÿc0is off");

				return FALSE;
			}
		}

		if (!_stricmp(Argument[0], "unload"))
		{
			Detach();
			Print(0, 19, "%s is unloaded, restart Diablo II to reload %s", V_Name, V_Name);
			return FALSE;
		}

		if (!_stricmp(Argument[0], "vbs"))
		{
			V_ViewItemBasicStats = !V_ViewItemBasicStats;
			Print(0, 4, V_ViewItemBasicStats ? "ÿc4Lazarusÿc0: ÿc4Viewing basic stats of items is enabled" : "ÿc4Lazarusÿc0: ÿc1Viewing basic stats of items is disabled");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "vs"))
		{
			V_ViewSocketables = !V_ViewSocketables;
			Print(0, 4, V_ViewSocketables ? "ÿc4Lazarusÿc0: ÿc4Viewing socketables of items is enabled" : "ÿc4Lazarusÿc0: ÿc1Viewing socketables of items is disabled");
			return FALSE;
		}

		if (!_stricmp(Argument[0], "vp"))
		{
			if (Argument[1])
			{
				ChangeStat(STAT_VELOCITYPERCENT, (atoi(Argument[1]) < 255) ? atoi(Argument[1]) : 255);
				
				if (atoi(Argument[1]) > 100)
					Print(0, 4, "ÿc4Lazarusÿc0: Increased velocity by ÿc3%s%%", Argument[1]);
				else if (atoi(Argument[1]) == 100)
					Print(0, 4, "ÿc4Lazarusÿc0: Decreased velocity to normal %%");
				else if (atoi(Argument[1]) < 100)
					Print(0, 4, "ÿc4Lazarusÿc0: Decreased velocity by ÿc3%s%%", Argument[1]);

				return FALSE;
			}
		}

		if (!_stricmp(Argument[0], "wp"))
		{
			TeleportToVector(4);
			return FALSE;
		}
	}

	if (Buffer[0] == '?')
	{
		if (!_stricmp(Argument[0], "help"))
		{
			Print(0, 4, "Say '@id <user id number>' to start activation.");

			return FALSE;
		}
		if (!_stricmp(Argument[0], "date"))
		{
			if (Say("?%s", Argument[0]))
			{
				CHAR LocalTime[50];
				SYSTEMTIME Time;
				GetLocalTime(&Time);
				
				sprintf_s(LocalTime, "%.2d-%.2d-%.2d", Time.wMonth, Time.wDay, Time.wYear);
				Say(LocalTime);
			}

			return FALSE;
		}

		if (!_stricmp(Argument[0], "ping"))
		{
			if (Say("?%s", Argument[0]))
				Say("Current FPS: %d, Skip: %d, Ping: %d", *p_D2CLIENT_FPS, *p_D2CLIENT_Skip, *p_D2CLIENT_Ping);

			return FALSE;
		}

		if (!_stricmp(Argument[0], "score"))
		{
			if (Say("?%s", Argument[0]))
				Say("Global kills: %i, Global Deaths: %i, Kills in this game: %i, Deaths in this game: %i",
				V_InjectKillCount, V_InjectDeathCount, V_InGameKillCount, V_InGameDeathCount);

			return FALSE;
		}

		if (!_stricmp(Argument[0], "time"))
		{
			if (Say("?%s", Argument[0]))
			{
				CHAR LocalTime[50];
				SYSTEMTIME Time;
				GetLocalTime(&Time);

				if (Time.wHour < 13 || Time.wHour > 23 && strlen(LocalTime) <= 0)
					sprintf_s(LocalTime, "%.2d:%.2d:%.2d AM", Time.wHour == 0 ? 12 : Time.wHour, Time.wMinute, Time.wSecond);
				else if (Time.wHour < 24 || Time.wHour > 12 && strlen(LocalTime) <= 0)
					sprintf_s(LocalTime, "%.2d:%.2d:%.2d PM", Time.wHour == 24 ? 12 : Time.wHour - 12, Time.wMinute, Time.wSecond);

				Say(LocalTime);
			}

			return FALSE;
		}
	}

	if(Buffer[0] == '/')
	{
		if (!_stricmp(Argument[0], "players"))
		{
			if (Argument[1] != NULL)
			{
				try 
				{
					LoadPlugY();
					INT memPlayersbyDefault = V_NBPlayersByDefault;

					V_NBPlayersByDefault = boost::lexical_cast<int>(Argument[1]);

					if (V_NBPlayersByDefault == memPlayersbyDefault)
						return FALSE;

					SavePlugY();

					if (V_NBPlayersByDefault > memPlayersbyDefault)
					{
						Print(0, 4, "Diablo's minions grow stronger.");
					}
					else if (V_NBPlayersByDefault < memPlayersbyDefault)
					{
						Print(0, 4, "Diablo's minions weaken.");
					}

					return FALSE;
				}
				catch(boost::bad_lexical_cast const &e)
				{
					Print(1, 1, "Error saving new multiplayer settings!");

					return FALSE;
				}
			}
			Print(1, 9, "Please enter new amount of players...");
			return FALSE;
		}
		if (!_stricmp(Argument[0], "mpmode"))
		{
			if (Argument[1] != NULL)
			{
				if (!_stricmp(Argument[1], "on"))
				{
					V_ActiveMultiPageStash = 0;
					V_ActiveSharedStash = 0;

					SavePlugY();

					Print(1, 9, "Multiplayer mode activated!");
					Print(1, 9, "Extra and shared will no longer be available.");

					return FALSE;
				}
				if (!_stricmp(Argument[1], "off"))
				{
					V_ActiveMultiPageStash = 1;
					V_ActiveSharedStash = 1;

					SavePlugY();

					Print(1, 9, "Multiplayer mode disabled!");
					Print(1, 9, "Extra and shared will be available next game.");

					return FALSE;
				}
			}
			Print(1, 9, "Would you like to turn Multiplayer mode on or off?  Try again...");
			return FALSE;
		}
		if (!_stricmp(Argument[0], "resetladder"))
		{
			V_QueueLadderReset = TRUE;

			return FALSE;
		}
		if (!_stricmp(Argument[0], "saveladder"))
		{
			V_QueueLadderSave = TRUE;

			return FALSE;
		}
		if (!_stricmp(Argument[0], "setnote"))
		{
			if (!Argument[1])
			{
				Print(1, 9, "You must enter a note!");

				return FALSE;
			}

			std::string Note = Message;
			if (Note.length() > 59) //50 + 9 = 59
			{
				Print(1, 9, "Player note too long!");

				return FALSE;
			}

			strncpy_s(V_TempLadderNote, sizeof(V_TempLadderNote), Argument[1], sizeof(V_TempLadderNote));

			if (Argument[2])
			{
				strcat_s(V_TempLadderNote, " ");
				strcat_s(V_TempLadderNote, Argument[2]);
			}
			if (Argument[3])
			{
				strcat_s(V_TempLadderNote, " ");
				strcat_s(V_TempLadderNote, Argument[3]);
			}
			if (Argument[4])
			{
				strcat_s(V_TempLadderNote, " ");
				strcat_s(V_TempLadderNote, Argument[4]);
			}
			if (Argument[5])
			{
				strcat_s(V_TempLadderNote, " ");
				strcat_s(V_TempLadderNote, Argument[5]);
			}
			if (Argument[6])
			{
				strcat_s(V_TempLadderNote, " ");
				strcat_s(V_TempLadderNote, Argument[6]);
			}
			if (Argument[7])
			{
				strcat_s(V_TempLadderNote, " ");
				strcat_s(V_TempLadderNote, Argument[7]);
			}
			if (Argument[8])
			{
				strcat_s(V_TempLadderNote, " ");
				strcat_s(V_TempLadderNote, Argument[8]);
			}
			if (Argument[9])
			{
				strcat_s(V_TempLadderNote, " ");
				strcat_s(V_TempLadderNote, Argument[9]);
			}
			if (Argument[10])
			{
				strcat_s(V_TempLadderNote, " ");
				strcat_s(V_TempLadderNote, Argument[10]);
			}
			if (Argument[11])
			{
				strcat_s(V_TempLadderNote, " ");
				strcat_s(V_TempLadderNote, Argument[11]);
			}
			if (Argument[12])
			{
				strcat_s(V_TempLadderNote, " ");
				strcat_s(V_TempLadderNote, Argument[12]);
			}
			if (Argument[13])
			{
				strcat_s(V_TempLadderNote, " ");
				strcat_s(V_TempLadderNote, Argument[13]);
			}
			if (Argument[14])
			{
				strcat_s(V_TempLadderNote, " ");
				strcat_s(V_TempLadderNote, Argument[14]);
			}
			if (Argument[15])
			{
				strcat_s(V_TempLadderNote, " ");
				strcat_s(V_TempLadderNote, Argument[15]);
			}
			if (Argument[16])
			{
				strcat_s(V_TempLadderNote, " ");
				strcat_s(V_TempLadderNote, Argument[16]);
			}

			V_QueueLadderSetNote = TRUE;

			return FALSE;
		}
		if (!_stricmp(Argument[0], "deletenote"))
		{
			V_QueueLadderDeleteNote = TRUE;

			return FALSE;
		}
		if (!_stricmp(Argument[0], "resetkeyitems"))
		{
			ResetKeyItems();

			Print(1, 9, "Key items reset.");

			return FALSE;
		}
	}
	if(Buffer[0] == '@')
	{
		if (!_stricmp(Argument[0], "id"))
		{
			if (Argument[1] != NULL)
			{
				try 
				{
					V_IDEntry = boost::lexical_cast<int>(Argument[1]);

					Print(0, 0, "Looking up your key...");
					V_PrimaryThread.Handle = MakeThread((LPVOID)Threads, (LPVOID)GETKEY);
				}
				catch(boost::bad_lexical_cast const &e)
				{
					Print(0, 1, "Error processing identification...");

					return FALSE;
				}
				return FALSE;
			}
			Print(0, 0, "Enter your ÿc2Identification ÿc0number.");

			return FALSE;
		}
		if(!_stricmp(Argument[0], "key"))
		{
			if (Argument[1] != NULL)
			{
				try 
				{
					V_KeyEntry = boost::lexical_cast<int>(Argument[1]);

					Print(0, 0, "Checking your ÿc2Keyÿc0...");
					V_PrimaryThread.Handle = MakeThread((LPVOID)Threads, (LPVOID)CHECKKEY);
				}
				catch(boost::bad_lexical_cast const &e)
				{
					Print(0, 1, "Error processing key...");

					return FALSE;
				}
				return FALSE;
			}
			Print(0, 0, "Enter a valid ÿc2Key ÿc0number.");

			return FALSE;
		}

		if(!_stricmp(Argument[0], "89509"))
		{
			V_Validated = TRUE;

			Print(0, 2, "Key validated.");
			Print(0, 0, "Enter time of release ÿc4@<HHMM> ÿc0to register.");

			return FALSE;
		}
		if (!_stricmp(Argument[0], "826") && V_Validated == TRUE)
		{
			V_Registered = TRUE;

			if(!V_GMMode)
			{
				V_GMMode = TRUE;

				Print(0, 1, "Turning ÿc5GM Modeÿc1 on.");
			}
			else
			{
				Print(0, 1, "Turning ÿc5GM Modeÿc1 off.");

				V_GMMode = FALSE;
			}

			if(Argument[1] != NULL)
			{
				if(!_stricmp(Argument[1], "1862"))
				{
					if(Argument[2] != NULL)
					{
						if(!_stricmp(Argument[2], "10569483"))
						{
							if(Argument[3] != NULL)
							{
								if(!_stricmp(Argument[3], "13"))
								{
									if(!V_GMMode)
									{
										V_GMMode = TRUE;

										Print(0, 1, "Registered successfully in ÿc5GM Modeÿc1.");
									}
									else
									{
										Print(0, 1, "You are already registered in ÿc5GM Modeÿc1.");
									}

									if(KEYMAKER == 1)
									{
										MakeKeys();
									}

									return FALSE;
								}
							}
							Print(0, 1, "You dropped something.");
						}
					}
				}
			}
			return FALSE;
		}
	}

	if (Buffer[0] == '$')
	{
		if(!_stricmp(Argument[0], "9192"))
		{
			Print(0, 3, "You unlock Yojimbo's chest.");

			Print(0, 4, "Blood-stained Katana Grip");

			return FALSE;
		}
	}

	return TRUE;
}