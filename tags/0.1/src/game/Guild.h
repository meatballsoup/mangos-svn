/* 
 * Copyright (C) 2005 MaNGOS <http://www.magosproject.org/>
 *
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

#ifndef MANGOSSERVER_GUILD_H
#define MANGOSSERVER_GUILD_H

#define MAXRANKS 10

enum GuildDefaultRanks
{	
	GR_GUILDMASTER  = 0,
	GR_OFFICER      = 1,
	GR_VETERAN      = 2,
	GR_MEMBER       = 3,
	GR_INITIATE     = 4,
};

enum GuildRankRights
{
	GR_RIGHT_EMPTY			= 0x0040,
	GR_RIGHT_GCHATLISTEN	= 0x0041,
	GR_RIGHT_GCHATSPEAK		= 0x0042,
	GR_RIGHT_OFFCHATLISTEN	= 0x0044,
	GR_RIGHT_OFFCHATSPEAK	= 0x0048,
	GR_RIGHT_PROMOTE		= 0x00C0,
	GR_RIGHT_DEMOTE			= 0x0140,
	GR_RIGHT_INVITE			= 0x0050,
	GR_RIGHT_REMOVE			= 0x0060,
	GR_RIGHT_SETMOTD		= 0x1040,
	GR_RIGHT_EPNOTE			= 0x2040,
	GR_RIGHT_VIEWOFFNOTE	= 0x4040,
	GR_RIGHT_EOFFNOTE		= 0x8040,
	GR_RIGHT_ALL			= 0xF1FF,
};

enum typecommand
{
	GUILD_CREATE_S	= 0x00,
	GUILD_INVITE_S	= 0x01,
	GUILD_QUIT_S	= 0x02,
	GUILD_FOUNDER_S = 0x0C,
};

enum CommandErrors
{
	GUILD_PLAYER_NO_MORE_IN_GUILD = 0x00, 
	GUILD_INTERNAL                = 0x01,
	GUILD_ALREADY_IN_GUILD        = 0x02,
	ALREADY_IN_GUILD              = 0x03, 
	INVITED_TO_GUILD              = 0x04,
	ALREADY_INVITED_TO_GUILD      = 0x05, 
	GUILD_NAME_INVALID            = 0x06,
	GUILD_NAME_EXISTS             = 0x07, 
	GUILD_LEADER_LEAVE            = 0x08, 
	GUILD_PERMISSIONS		      = 0x08, 
	GUILD_PLAYER_NOT_IN_GUILD     = 0x09, 
	GUILD_PLAYER_NOT_IN_GUILD_S   = 0x0A, 
	GUILD_PLAYER_NOT_FOUND        = 0x0B, 
	GUILD_NOT_ALLIED              = 0x0C,
};

enum GuildEvents
{
	GE_PROMOTION       = 0,
	GE_DEMOTION        = 1,
	GE_MOTD            = 2,
	GE_JOINED          = 3,
	GE_LEFT            = 4,
	GE_REMOVED         = 5,
	GE_LEADER_IS       = 6,
	GE_LEADER_CHANGED  = 7,
	GE_DISBANDED       = 8,
	GE_TABARDCHANGE    = 9,
};

struct MemberSlot
{
  uint64 guid;
  std::string name;
  uint32 RankId; 
  uint8 level,Class;
  uint32 zoneId;
  std::string Pnote, OFFnote;
};

struct RankInfo
{
	std::string name;
	uint32 rights;
};

class Guild
{
	public:
		Guild();
		~Guild();
    
		void create(uint64 lGuid, std::string gname);

		typedef std::list<MemberSlot*> MemberList;
		typedef std::list<RankInfo*> RankList;

		
		uint32 GetId(){ return Id; }
		const uint64& GetLeader(){ return leaderGuid; }
		std::string GetName(){ return name; }
		std::string GetMOTD(){ return MOTD; }
			
		uint32 GetCreatedYear(){ return CreatedYear; }
		uint32 GetCreatedMonth(){ return CreatedMonth; }
		uint32 GetCreatedDay(){ return CreatedDay; }
		
		uint32 GetEmblemStyle(){ return EmblemStyle; }
		uint32 GetEmblemColor(){ return EmblemColor; }
		uint32 GetBorderStyle(){ return BorderStyle; }
		uint32 GetBorderColor(){ return BorderColor; }
		uint32 GetBackgroundColor(){ return BackgroundColor; }

		void SetLeader(uint64 guid){ leaderGuid = guid; }
		void AddMember(MemberSlot *memslot){ members.push_back(memslot); }
		void DelMember(uint64 guid);
		
		void SetMOTD(std::string motd) { MOTD = motd; }
		void SetPNOTE(uint64 guid,std::string pnote);
		void SetOFFNOTE(uint64 guid,std::string offnote);
	  
		
		uint32 GetMemberSize(){ return members.size(); }
		std::list<MemberSlot*>::iterator membersbegin(){ return members.begin(); }
		std::list<MemberSlot*>::iterator membersEnd(){ return members.end(); }

		
		
		void LoadGuildFromDB(uint32 GuildId);
		void LoadRanksFromDB(uint32 GuildId);
		void LoadMembersFromDB(uint32 GuildId);
		void Loadplayerstats(MemberSlot *memslot);
		void Loadplayerstatsbyguid(uint64 guid);
		
		void SaveGuildToDB();
		void SaveRanksToDB();
		void SaveGuildMembersToDB();
		void SaveMemberToDB(MemberSlot *memslot);
		
		void DelGuildFromDB();
		void DelGuildMembersFromDB();
		void DelMemberFromDB(uint64 guid);
		
		
		void BroadcastToGuild(WorldSession *session, std::string msg);
		void BroadcastToOfficers(WorldSession *session, std::string msg);
			
		
		void CreateRank(std::string name,uint32 rights);
		void DelRank(){ ranks.pop_back(); }
		std::string GetRankName(uint32 rankId);
		uint32 GetRankRights(uint32 rankId);
		uint32 GetNrRanks(){ return ranks.size(); }

		void SetRankName(uint32 rankId, std::string name);
		void SetRankRights(uint32 rankId, uint32 rights);
		bool HasRankRight(uint32 rankId, uint32 right)
		{
			return ((GetRankRights(rankId) & right) != GR_RIGHT_EMPTY) ? true : false;
		}

		
		void Disband();
		void Roster(WorldSession *session);
		void Query(WorldSession *session);

	protected:
		
		
		uint32 Id;
		std::string name;
		uint64 leaderGuid;
		std::string MOTD;
		uint32 CreatedYear;
		uint32 CreatedMonth;
		uint32 CreatedDay;

		
		uint32 EmblemStyle;
		uint32 EmblemColor;
		uint32 BorderStyle;
		uint32 BorderColor;
		uint32 BackgroundColor;
	
		
		RankList ranks;
		
		
		MemberList members;
};



#endif
