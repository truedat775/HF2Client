#include "Hack.h"
#include "Vars.h"

/*
BOOL GodmodeSet(){
	//Print(0, 4, "�c4Godmode:�c0 Pick up any item from your �c1inventory�c0 and a �c;Full Rejuvination�c0 potion from your �c1belt�c0.");
	Print(0, 4, "�c4Godmode:�c0 Pick up your hat from your �c1equipment�c0 and a �c;Full Rejuvination�c0 potion from your �c1belt�c0.");
	Print(0, 4, "�c4Godmode:�c0 Also click the �c1NPC�c0 you want to use for '�c4.god buy�c0'.");
	
	V_CSet = TRUE;

	return TRUE;
}

BOOL GodmodeStart(){
	if ((V_ICopied == TRUE) && (V_PCopied == TRUE))
	{
		Print(0, 4, "�c4Godmode:�c0 Godmode activated!");
		V_GMOn = TRUE;
	}
	else
	{
		Print(0, 4, "�c4Godmode:�c0 Please type '�c5.god set�c0' and pick up the full rejuv from your belt and item from your inventory first.");
	}

	return TRUE;
}

BOOL GodmodeStop(){
	Print(0, 4, "�c4Godmode:�c0 All flags disabled.");
	V_GMOn = FALSE;

	return TRUE;
}

BOOL GodmodeHeal(){
	if ((V_ICopied == TRUE) && (V_PCopied == TRUE))
	{
		INT Length = 9;
		LPBYTE Packet = new BYTE[Length];
		Packet[0] = 0x27;
		*(LPDWORD)&Packet[1] = V_Heal[1];
		*(LPDWORD)&Packet[5] = V_Heal[5];

		for (INT i = 0; i < Length; i++)
			TestPacket(Packet[i]);

		D2NET_SendPacket(Length, 1, Packet);

		delete [] Packet;

		Print(0, 4, "�c4Godmode:�c0 Healed.");
	}
	else
	{
		Print(0, 4, "�c4Godmode:�c0 Please type '�c5.god set�c0' and pick up the full rejuv from your belt and item from your inventory first.");
	}

	return TRUE;
}

BOOL GodmodeBuy(){
	if (V_NPCCopied == TRUE)
	{
		Print(0, 4, "�c4Godmode:�c0 Health purchased.");

		INT Length = 5;
		LPBYTE Packet = new BYTE[Length];
		Packet[0] = 0x39;
		*(LPDWORD)&Packet[1] = V_BuyHeal[1];

		for (INT i = 0; i < Length; i++)
			TestPacket(Packet[i]);

		D2NET_SendPacket(Length, 1, Packet);

		delete [] Packet;
	}

	return TRUE;
}
*/