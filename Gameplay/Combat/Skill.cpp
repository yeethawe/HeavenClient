/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright � 2015 Daniel Allendorf                                        //
//                                                                          //
// This program is free software: you can redistribute it and/or modify     //
// it under the terms of the GNU Affero General Public License as           //
// published by the Free Software Foundation, either version 3 of the       //
// License, or (at your option) any later version.                          //
//                                                                          //
// This program is distributed in the hope that it will be useful,          //
// but WITHOUT ANY WARRANTY; without even the implied warranty of           //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
// GNU Affero General Public License for more details.                      //
//                                                                          //
// You should have received a copy of the GNU Affero General Public License //
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    //
//////////////////////////////////////////////////////////////////////////////
#include "Skill.h"

#include "Data\DataFactory.h"
#include "Util\Misc.h"

#include "nlnx\node.hpp"
#include "nlnx\nx.hpp"

namespace Gameplay
{
	Skill::Skill(int32_t id)
	{
		skillid = id;

		string strid;
		if (skillid < 10000000)
		{
			strid = Format::extendid(skillid, 7);
		}
		else
		{
			strid = std::to_string(skillid);
		}
		node src = nl::nx::skill[strid.substr(0, 3) + ".img"]["skill"][strid];

		int32_t reqw = src["weapon"];
		reqweapon = Weapon::typebyvalue(100 + reqw);

		/*
		affected = src["affected"];

		preparestance = src["prepare"]["action"];
		preparetime = src["prepare"]["time"];*/

		projectile = true;
		overregular = false;
		offensive = false;
		passive = (skillid % 10000) / 1000 == 0;

		for (node sub : src["level"])
		{
			Level level;

			bool hasdamage = sub["damage"].data_type() == node::type::integer;
			bool hasmad = sub["mad"].data_type() == node::type::integer;
			bool hasfixdamage = sub["fixdamage"].data_type() == node::type::integer;

			level.damage = sub["damage"];
			level.damage /= 100;
			level.matk = sub["mad"];

			offensive = !passive && (hasdamage || hasmad || hasfixdamage);

			if (offensive)
			{
				level.fixdamage = sub["fixdamage"];
				level.mastery = sub["mastery"];
				level.attackcount = sub["attackCount"];
				if (level.attackcount == 0)
					level.attackcount = 1;
				level.bulletcount = sub["bulletCount"];
				if (level.bulletcount == 0)
					level.bulletcount = 1;
				level.bulletcost = sub["bulletConsume"];
				if (level.bulletcost == 0)
					level.bulletcost = level.bulletcount;
				level.mobcount = sub["mobCount"];
				if (level.mobcount == 0)
					level.mobcount = 1;
			}
			level.chance = sub["prop"];
			if (level.chance <= 0.0f)
				level.chance = 100.0f;
			level.chance /= 100;
			level.hpcost = sub["hpCon"];
			level.mpcost = sub["mpCon"];
			level.hrange = sub["range"];
			if (level.hrange <= 0.0f)
				level.hrange = 100.0f;
			level.hrange /= 100;
			level.range = sub;

			int32_t lvlid = StringConversion<int32_t>(sub.name()).ordefault(-1);
			levels[lvlid] = level;
		}

		sound = unique_ptr<SkillSound>(new SingleSkillSound(strid));

		bool byleveleffect = src["CharLevel"]["10"]["effect"].size() > 0;
		bool multieffect = src["effect0"].size() > 0;
		if (byleveleffect)
		{
			useeffect = unique_ptr<SkillUseEffect>(new ByLevelUseEffect(src));
		}
		else if (multieffect)
		{
			useeffect = unique_ptr<SkillUseEffect>(new MultiUseEffect(src));
		}
		else
		{
			bool isanimation = src["effect"]["0"].data_type() == node::type::bitmap;
			bool haseffect1 = src["effect"]["1"].size() > 0;
			if (isanimation)
			{
				useeffect = unique_ptr<SkillUseEffect>(new SingleUseEffect(src));
			}
			else if (haseffect1)
			{
				useeffect = unique_ptr<SkillUseEffect>(new TwoHUseEffect(src));
			}
			else
			{
				useeffect = unique_ptr<SkillUseEffect>(new NoUseEffect());
			}
		}

		bool bylevelhit = src["CharLevel"]["10"]["hit"].size() > 0;
		bool byskilllevelhit = src["level"]["1"]["hit"].size() > 0;
		bool hashit0 = src["hit"]["0"].size() > 0;
		bool hashit1 = src["hit"]["1"].size() > 0;
		if (bylevelhit)
		{
			if (hashit0 && hashit1)
			{
				hiteffect = unique_ptr<SkillHitEffect>(new ByLevelTwoHHitEffect(src));
			}
			else
			{
				hiteffect = unique_ptr<SkillHitEffect>(new ByLevelHitEffect(src));
			}
		}
		else if (byskilllevelhit)
		{
			hiteffect = unique_ptr<SkillHitEffect>(new BySkillLevelHitEffect(src));
		}
		else if (hashit0 && hashit1)
		{
			hiteffect = unique_ptr<SkillHitEffect>(new TwoHHitEffect(src));
		}
		else if (hashit0)
		{
			hiteffect = unique_ptr<SkillHitEffect>(new SingleHitEffect(src));
		}
		else
		{
			hiteffect = unique_ptr<SkillHitEffect>(new NoHitEffect());
		}

		bool hasaction0 = src["action"]["0"].data_type() == node::type::string;
		bool hasaction1 = src["action"]["1"].data_type() == node::type::string;
		if (hasaction0 && hasaction1)
		{
			action = unique_ptr<SkillAction>(new TwoHAction(src));
		}
		else if (hasaction0)
		{
			action = unique_ptr<SkillAction>(new SingleAction(src));
		}
		else if (offensive)
		{
			bool bylevel = src["level"]["1"]["action"].data_type() == node::type::string;
			if (bylevel)
			{
				action = unique_ptr<SkillAction>(new ByLevelAction(src, skillid));
			}
			else
			{
				action = unique_ptr<SkillAction>(new RegularAction());
				overregular = true;
			}
		}
		else
		{
			action = unique_ptr<SkillAction>(new NoAction());
		}

		bool hasball = src["ball"].size() > 0;
		bool bylevelball = src["level"]["1"]["ball"].size() > 0;
		if (bylevelball)
		{
			bullet = unique_ptr<SkillBullet>(new BySkillLevelBullet(src, skillid));
		}
		else if (hasball)
		{
			bullet = unique_ptr<SkillBullet>(new SingleBullet(src));
		}
		else
		{
			bullet = unique_ptr<SkillBullet>(new RegularBullet());
			projectile = false;
		}
	}

	void Skill::applystats(const Char& user, Attack& attack) const
	{
		attack.skill = skillid;

		int32_t lv = user.getskilllevel(skillid);
		auto level = Optional<Level>::from(levels, lv);
		if (level)
		{
			if (level->fixdamage)
			{
				attack.fixdamage = level->fixdamage;
				attack.damagetype = Attack::DMG_FIXED;
			}
			else if (level->matk)
			{
				attack.matk += level->matk;
				attack.damagetype = Attack::DMG_MAGIC;
			}
			else
			{
				attack.mindamage *= level->damage;
				attack.maxdamage *= level->damage;
				attack.damagetype = Attack::DMG_WEAPON;
			}
			attack.critical += level->critical;
			attack.ignoredef += level->ignoredef;
			attack.mobcount = level->mobcount;
			attack.hrange = level->hrange;

			switch (attack.type)
			{
			case Attack::RANGED:
				attack.hitcount = level->bulletcount;
				break;
			default:
				attack.hitcount = level->attackcount;
			}

			if (!level->range.empty())
				attack.range = level->range;
		}

		if (projectile && !attack.bullet)
		{
			switch (skillid)
			{
			case THREE_SNAILS:
				switch (lv)
				{
				case 1:
					attack.bullet = 4000019;
					break;
				case 2:
					attack.bullet = 4000000;
					break;
				case 3:
					attack.bullet = 4000016;
					break;
				}
				break;
			default:
				attack.bullet = skillid;
			}
		}

		if (overregular)
		{
			using Character::CharLook;
			CharLook::AttackLook attacklook = user.getlook().getattacklook();
			attack.stance = attacklook.stance;
			if (attack.type == Attack::CLOSE && !projectile)
			{
				attack.range = attacklook.range;
			}
		}
	}

	bool Skill::isoffensive() const
	{
		return offensive;
	}

	int32_t Skill::getid() const
	{
		return skillid;
	}

	SpecialMove::ForbidReason Skill::canuse(int32_t lv, Weapon::Type weapon, uint16_t job, uint16_t hp, uint16_t mp, uint16_t bullets) const
	{
		auto level = Optional<Level>::from(levels, lv);
		if (level)
		{
			auto reqjob = static_cast<uint16_t>(skillid / 10000);
			using Character::CharJob;
			if (!CharJob(job).issubjob(reqjob))
				return FBR_OTHER;

			if (hp <= level->hpcost)
				return FBR_HPCOST;

			if (mp < level->mpcost)
				return FBR_MPCOST;

			if (weapon != reqweapon && reqweapon != Weapon::NONE)
				return FBR_WEAPONTYPE;

			switch (weapon)
			{
			case Weapon::BOW:
			case Weapon::CROSSBOW:
			case Weapon::CLAW:
			case Weapon::GUN:
				return (bullets >= level->bulletcost) ? FBR_NONE : FBR_BULLETCOST;
			default:
				return FBR_NONE;
			}
		}
		else
		{
			return FBR_OTHER;
		}
	}
}