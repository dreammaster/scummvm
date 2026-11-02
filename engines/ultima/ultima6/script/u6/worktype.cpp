/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "ultima/ultima6/core/game.h"
#include "ultima/ultima6/core/game_clock.h"
#include "ultima/ultima6/core/obj_manager.h"
#include "ultima/ultima6/core/party.h"
#include "ultima/ultima6/core/player.h"
#include "ultima/ultima6/script/common/script_random.h"
#include "ultima/ultima6/script/u6/u6_scripts.h"
#include "ultima/ultima6/gui/widgets/msg_scroll.h"
#include "ultima/ultima6/views/portrait_view.h"
#include "ultima/ultima6/views/view_manager.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {
namespace U6 {

namespace {

const uint16 TANGLE_VINE_FRAMES[16] = {
	1, 3, 1, 4,
	5, 0, 4, 0,
	1, 2, 1, 5,
	2, 0, 3, 0
};

int floorDiv(int value, int divisor) {
	int result = value / divisor;
	if ((value % divisor != 0) && ((value < 0) != (divisor < 0)))
		--result;
	return result;
}

NuvieDir reverseDirection(NuvieDir direction) {
	return (NuvieDir)((direction + 2) % 4);
}

} // End of anonymous namespace

bool U6Scripts::actorOkToAttack(Actor *actor, Actor *target) {
	if (!target->is_visible() && (!target->is_in_party() || !actor->is_in_party()))
		return false;

	if (target->get_z() != actor->get_z())
		return false;

	// Corpser underground
	if (target->get_obj_n() == 0x165 && target->get_frame_n() == 0)
		return false;

	if (target->get_corpser_flag())
		return false;

	return true;
}

Actor *U6Scripts::actorFindTarget(Actor *actor) {
	Actor *targetActor = nullptr;
	ActorAlignment align = actor->get_alignment();
	int actorX = actor->get_x();
	int actorY = actor->get_y();
	int bestDistance = 0x7fff;
	MapCoord playerLoc = Game::get_game()->get_player()->get_actor()->get_location();
	int wt = actor->get_worktype();

	for (int i = 0; i <= 0xff; ++i) {
		Actor *other = getActor(i);

		if (other->get_obj_n() == 0 || !other->is_alive() || other->get_actor_num() == actor->get_actor_num() ||
				!actorOkToAttack(actor, other))
			continue;

		ActorAlignment otherAlign = other->get_alignment();

		bool canTarget = wt == WT_FLEE || wt == WT_MOUSE || wt == WT_UNK_13 || wt == WT_RETREAT || wt == WT_BRAWLING ||
			((align != ACTOR_ALIGNMENT_NEUTRAL || (wt == WT_ATTACK_PARTY && otherAlign == ACTOR_ALIGNMENT_GOOD)) &&
			(align != ACTOR_ALIGNMENT_CHAOTIC || otherAlign != ACTOR_ALIGNMENT_CHAOTIC) &&
			otherAlign != ACTOR_ALIGNMENT_NEUTRAL &&
			(align != ACTOR_ALIGNMENT_GOOD || alignmentIsEvil(otherAlign)) &&
			(align != ACTOR_ALIGNMENT_EVIL || otherAlign == ACTOR_ALIGNMENT_GOOD || otherAlign == ACTOR_ALIGNMENT_CHAOTIC));

		if (!canTarget)
			continue;

		int targetX = other->get_x();
		int targetY = other->get_y();

		if (actorFindMaxXYDistance(actor, targetX, targetY) <= 8 &&
				(other->get_worktype() != WT_RETREAT ||
					(ABS(targetX - (int)playerLoc.x) <= 5 && ABS(targetY - (int)playerLoc.y) <= 5))) {
			int distance = (targetX - actorX) * (targetX - actorX) + (targetY - actorY) * (targetY - actorY);

			if (distance < bestDistance || (distance == bestDistance && randomRange(0, 1) != 0)) {
				bestDistance = distance;
				targetActor = other;
			}
		}
	}

	return targetActor;
}

bool U6Scripts::actorCatchUpToParty(Actor *actor) {
	Actor *targetActor = nullptr;
	int actorX = actor->get_x();
	int actorY = actor->get_y();
	int bestDistance = 0x32;

	Common::Array<Actor *> members;
	getPartyMembers(members);

	for (uint i = 0; i < members.size(); ++i) {
		Actor *member = members[i];

		if (member->is_alive() && actorOkToAttack(actor, member)) {
			int dx = (int)member->get_x() - actorX;
			int dy = (int)member->get_y() - actorY;
			int distance = dx * dx + dy * dy;

			if (distance < bestDistance || (distance == bestDistance && randomRange(0, 1) != 0)) {
				bestDistance = distance;
				targetActor = member;
			}
		}
	}

	if (!targetActor) {
		actorMoveTowardsLoc(actor, _partyAvgX, _partyAvgY);
		return false;
	}

	actorMoveTowardsLoc(actor, targetActor->get_x(), targetActor->get_y());

	return ABS((int)targetActor->get_x() - (int)actor->get_x()) <= 1 &&
		ABS((int)targetActor->get_y() - (int)actor->get_y()) <= 1;
}

void U6Scripts::caughtByGuard(Actor *actor) {
	Game *game = Game::get_game();
	Party *party = game->get_party();

	game->get_view_manager()->set_portrait_mode(actor, actor->get_name());

	print("\n\"Thou art under arrest!\"\n\n\"Wilt thou come quietly?\"\n\n:");

	Common::String answer = inputSelect("yn", false);

	actor->set_worktype(0x81);

	if (game->is_new_style())
		game->get_view_manager()->get_portrait_view()->Hide();
	else
		game->get_view_manager()->set_party_mode();

	if (answer == "Y") {
		print("es\n\nThe guard strikes thee unconscious!\n\nThou dost awaken to...\n");

		fadeOutEffect();

		if (party->is_in_combat_mode())
			party->set_in_combat_mode(false);

		game->get_player()->move(0xe7, 0xba, 0, true);

		GameClock *clock = game->get_clock();
		while (clock->get_hour() != 8)
			advanceTime(60);

		Common::Array<Actor *> members;
		getPartyMembers(members);

		for (uint i = 0; i < members.size(); ++i) {
			Common::Array<Obj *> inventory;
			getInventoryObjs(members[i], true, inventory);

			for (uint j = 0; j < inventory.size(); ++j) {
				Obj *obj = inventory[j];

				if (obj->obj_n == 0x3f) { // lockpick
					removeInventoryObj(members[i], obj);
				} else if (obj->obj_n == 0x40 && obj->quality == 9) { // key
					Obj *desk = mapGetObj(0xeb, 0xb7, 0, 0xb1);
					if (desk)
						game->get_obj_manager()->moveto_container(obj, desk);
				}
			}
		}

		Obj *door = mapGetObj(0xe7, 0xb8, 0, 0x12c); // steel door
		if (door)
			door->frame_n = 9; // Close and lock the door

		fadeInEffect();
	} else {
		print("o\n\n\"Then defend thyself, rogue!\"\n");
		activateCityGuards();
		actor->set_worktype(WT_ASSAULT);
		actor->set_alignment(ACTOR_ALIGNMENT_EVIL);

		for (int i = 1; i <= 0xff; ++i) {
			Actor *other = getActor(i);

			if (other->get_worktype() == WT_GUARD_ARREST_PLAYER) {
				other->set_worktype(WT_ASSAULT);
				other->set_alignment(ACTOR_ALIGNMENT_EVIL);
			}
		}
	}
}

bool U6Scripts::moveTanglevine(Actor *actor, NuvieDir newDirection) {
	int actorX = actor->get_x();
	int actorY = actor->get_y();
	NuvieDir oldDirection = actor->get_direction();

	if (newDirection != reverseDirection(oldDirection)) {
		if (actorMove(actor, newDirection, 1)) {
			Obj *vine = newObj(0x16e);

			vine->frame_n = (oldDirection < 4) ? TANGLE_VINE_FRAMES[oldDirection * 4 + newDirection] : 0;
			vine->status = 0x20;
			vine->quality = 0;

			if (!Game::get_game()->get_obj_manager()->moveto_map(vine, MapCoord(actorX, actorY, actor->get_z())))
				delete_obj(vine);

			actor->set_frame_n(TANGLE_VINE_FRAMES[newDirection * 4 + randomRange(0, 3)]);
			return true;
		}
	}

	return false;
}

void U6Scripts::wtFront(Actor *actor) {
	if (wtFrontPosition(actor) == 0)
		return;

	if (actor->is_in_party()) {
		wtAttack(actor);
	} else if (actorFindMaxXYDistance(actor, _partyAvgX, _partyAvgY) >= 8) {
		subtractMovementPts(actor, 5);
	} else if (actor != _frontTargetActor) {
		wtAttack(actor);
	} else if (randomRange(0, 7) != 0) {
		wtAttack(actor);
	} else {
		actorMoveTowardsLoc(actor, _partyAvgX, _partyAvgY);
	}
}

int U6Scripts::wtFrontPosition(Actor *actor) {
	// Returns 1 if the actor should carry on and attack, or 0 if it has moved
	if ((_numMonstersNear == 0 || !_frontTargetActor) && !actor->is_in_party()) {
		subtractMovementPts(actor, 5);
		return 1;
	}
	MapCoord playerLoc = Game::get_game()->get_player()->get_actor()->get_location();
	int actorX = actor->get_x();
	int actorY = actor->get_y();
	int centreX, centreY, diffX, diffY;

	if (!actor->is_in_party()) {
		centreX = _partyAvgX;
		centreY = _partyAvgY;
		diffX = (int)_frontTargetActor->get_x() - centreX;
		diffY = (int)_frontTargetActor->get_y() - centreY;
	} else {
		centreX = playerLoc.x;
		centreY = playerLoc.y;
		diffX = _combatAvgX - centreX;
		diffY = _combatAvgY - centreY;
	}

	int var1E = (actorX - centreX) * diffY - (actorY - centreY) * diffX;
	int var24 = (var1E <= 0) ? 0 : 1;

	if (var1E == 0)
		var24 = randomRange(0, 1);

	int var14 = (diffY <= 0) ? -1 : 1;
	int var12 = (diffX <= 0) ? 1 : -1;

	if (var24 == 0) {
		var14 = -var14;
		var12 = -var12;
	}

	Actor *tmpActor = actor->is_in_party() ? Game::get_game()->get_player()->get_actor() : _frontTargetActor;
	int var20 = ((int)tmpActor->get_x() - centreX) * diffX + ((int)tmpActor->get_y() - centreY) * diffY;

	if (actor->is_in_party())
		var20 += ABS(diffX) + ABS(diffY);

	int var1C = diffX * diffX + diffY * diffY;
	if (var1C == 0)
		var1C = 1;

	int targetX = floorDiv(var20 * diffX, var1C) + centreX;
	int targetY = floorDiv(var20 * diffY, var1C) + centreY;

	int chunkX = playerLoc.x - 16;
	chunkX -= chunkX % 8;
	int chunkY = playerLoc.y - 16;
	chunkY -= chunkY % 8;

	bool foundActor;
	do {
		if (targetX < chunkX || chunkX + 0x27 < targetX || targetY < chunkY || chunkY + 0x27 < targetY ||
				(actorX == targetX && actorY == targetY))
			return 1; // Too far away

		foundActor = mapGetActor(targetX, targetY, playerLoc.z) != nullptr;

		if (foundActor) {
			int var10 = (targetX + var14 - centreX) * diffX + (targetY - centreY) * diffY;
			int varE = (targetX - centreX) * diffX + (targetY + var12 - centreY) * diffY;

			if (ABS(var10 - var20) >= ABS(varE - var20))
				targetY += var12;
			else
				targetX += var14;
		}
	} while (foundActor);

	int movesLeft = actor->get_moves_left();
	if (actorMoveTowardsLoc(actor, targetX, targetY) != 0) {
		if (!actor->is_in_party() || (actor->get_x() == targetX && actor->get_y() == targetY))
			return 0;

		if (_numMonstersNear == 0)
			actor->set_moves_left(movesLeft);

		if (actorMoveTowardsLoc(actor, targetX, targetY) != 0)
			return 0;
	} else {
		actor->set_moves_left(movesLeft);
	}

	wtAttack(actor);
	return 0;
}

void U6Scripts::wtRear(Actor *actor) {
	MapCoord playerLoc = Game::get_game()->get_player()->get_actor()->get_location();
	Actor *var4, *var2;
	int avgX, avgY, avgXDiff, avgYDiff;

	if (!actor->is_in_party()) {
		if (_numMonstersNear == 0) {
			subtractMovementPts(actor, 5);
			return;
		}

		var4 = _rearMinMonster ? _rearMinMonster : actor;
		var2 = _rearMaxMonster ? _rearMaxMonster : actor;

		avgX = _combatAvgX;
		avgY = _combatAvgY;
		avgXDiff = _partyAvgX - _combatAvgX;
		avgYDiff = _partyAvgY - _combatAvgY;
	} else {
		if (_numMonstersNear == 0) {
			actorMoveTowardsLoc(actor, playerLoc.x, playerLoc.y);
			return;
		}

		var4 = _rearMaxParty ? _rearMaxParty : actor;
		var2 = _rearMinParty ? _rearMinParty : actor;
		avgX = _partyAvgX;
		avgY = _partyAvgY;
		avgXDiff = _combatAvgX - _partyAvgX;
		avgYDiff = _combatAvgY - _partyAvgY;
	}

	int var10 = 0x7fff;
	ActorAlignment align = actor->get_alignment();

	for (int i = 0; i <= 0xff; ++i) {
		Actor *other = getActor(i);

		if (other->is_alive() && other->get_worktype() == WT_FRONT && other->get_alignment() == align) {
			int var12 = ((int)other->get_x() - avgX) * avgXDiff + ((int)other->get_y() - avgY) * avgYDiff;
			if (var12 < var10)
				var10 = var12;
		}
	}

	int var12 = ((int)actor->get_x() - avgX) * avgXDiff + ((int)actor->get_y() - avgY) * avgYDiff;
	int movesLeft = actor->get_moves_left();
	int varC = 0;

	if (!actor->is_in_party() || actorFindMaxXYDistance(actor, playerLoc.x, playerLoc.y) <= 3) {
		if (var12 < var10) {
			var12 = ((int)actor->get_x() - avgX) * avgYDiff - ((int)actor->get_y() - avgY) * avgXDiff;

			int dx = ((int)var4->get_x() - avgX) * avgYDiff;
			if (dx - ((int)var4->get_y() - avgY) * avgXDiff >= var12) {
				dx = ((int)var2->get_x() - avgX) * avgYDiff;
				int ax = ((int)var2->get_y() - avgY) * avgXDiff;

				if (dx - ax <= var12) {
					varC = 1;
				} else {
					// The original script always ended up with 0 here, as it tested a number for truth
					actorMoveTowardsLoc(actor, actor->get_x() + avgYDiff, actor->get_y() - avgXDiff);
					varC = 0;
				}
			} else {
				actorMoveTowardsLoc(actor, actor->get_x() - avgYDiff, actor->get_y() + avgXDiff);
				varC = 0;
			}
		} else {
			actorMoveTowardsLoc(actor, actor->get_x() - avgXDiff, actor->get_y() - avgYDiff);
			varC = 0;
		}
	} else {
		actorMoveTowardsLoc(actor, playerLoc.x, playerLoc.y);
		varC = 0;
	}

	if (varC != 0) {
		actor->set_moves_left(movesLeft);
		wtAttack(actor);
	}
}

void U6Scripts::wtFlank(Actor *actor) {
	MapCoord playerLoc = Game::get_game()->get_player()->get_actor()->get_location();
	int playerX = playerLoc.x;
	int playerY = playerLoc.y;

	if (_numMonstersNear == 0 || (!actor->is_in_party() && actorFindMaxXYDistance(actor, playerX, playerY) > 7)) {
		if (actor->is_in_party() && actorFindMaxXYDistance(actor, playerX, playerY) > 2) {
			int movesLeft = actor->get_moves_left();
			actorMoveTowardsLoc(actor, playerX, playerY);
			actor->set_moves_left(movesLeft);
		}

		subtractMovementPts(actor, 5);
		return;
	}

	ActorAlignment actorAlign = actor->get_alignment();
	int actorX = actor->get_x();
	int actorY = actor->get_y();
	int tmpX = _combatAvgX - _partyAvgX;
	int tmpY = _combatAvgY - _partyAvgY;
	int var20 = (actorX - _partyAvgX) * tmpY - (actorY - _partyAvgY) * tmpX;
	int var1E = (var20 <= 0) ? 0 : 1;
	int var10, varE;

	if (var20 == 0)
		var1E = randomRange(0, 1);

	if (var1E == 0) {
		var10 = -tmpY;
		varE = tmpX;
		var20 = -var20;
	} else {
		var10 = tmpY;
		varE = -tmpX;
	}

	int var1A = -0x8000;
	Actor *targetActor = nullptr;

	for (int i = 1; i <= 0xff; ++i) {
		Actor *other = getActor(i);

		if (other->is_alive() && other->get_alignment() != actorAlign && actorOkToAttack(actor, other) &&
				(actorAlign != ACTOR_ALIGNMENT_GOOD || alignmentIsEvil(other->get_alignment())) &&
				(actorAlign != ACTOR_ALIGNMENT_EVIL || other->get_alignment() == ACTOR_ALIGNMENT_GOOD ||
					other->get_alignment() == ACTOR_ALIGNMENT_CHAOTIC)) {
			int targetX = other->get_x();
			int targetY = other->get_y();
			int otherWt = other->get_worktype();

			if ((otherWt != WT_RETREAT && otherWt != WT_FLEE && otherWt != WT_MOUSE) ||
					(playerX - 5 <= targetX && playerX + 5 >= targetX && playerY - 5 <= targetY && playerY + 5 >= targetY)) {
				if ((ABS(targetX - playerX) <= 7 && ABS(targetY - playerY) <= 7) ||
						(ABS(targetX - actorX) <= 5 && ABS(targetY - actorY) <= 5)) {
					int var1C = (targetX - _partyAvgX) * tmpY - (targetY - _partyAvgY) * tmpX;

					if (var1E == 0)
						var1C = -var1C;

					if (var1C > var1A) {
						var1A = var1C;
						targetActor = other;
					}
				}
			}
		}
	}

	if (!targetActor) {
		moveTowardsPlayer(actor);
		return;
	}

	tmpX = targetActor->get_x();
	tmpY = targetActor->get_y();

	bool shouldMoveActor = false;
	Target weapon = actorGetWeapon(actor, Target(targetActor));
	int weaponRange = getWeaponRange(weapon.objN());
	int attackRange = actorGetCombatRange(actor, tmpX, tmpY);

	if (attackRange < 9 && attackRange <= weaponRange) {
		if (!mapCanReachPoint(actorX, actorY, tmpX, tmpY, actor->get_z())) {
			if (randomRange(0, 1) == 0) {
				tmpX = targetActor->get_y() - actorY + actorX;
				tmpY = actorY - targetActor->get_x() - actorX;
			} else {
				tmpX = actorX - targetActor->get_y() - actorY;
				tmpY = targetActor->get_x() - actorX + actorY;
			}

			shouldMoveActor = true;
		} else {
			actorAttackTarget(actor, tmpX, tmpY, actor->get_z(), weapon, Target(targetActor));
			subtractMovementPts(actor, 10);
		}
	} else {
		if (var10 > 0)
			++tmpX;
		else if (var10 < 0)
			--tmpX;

		if (varE > 0)
			++tmpY;
		else if (varE < 0)
			--tmpY;

		shouldMoveActor = true;
	}

	if (shouldMoveActor) {
		int movesLeft = actor->get_moves_left();

		if (actorMoveTowardsLoc(actor, tmpX, tmpY) == 0) {
			actor->set_moves_left(movesLeft);
			wtAttack(actor);
		}
	}
}

void U6Scripts::wtBerserk(Actor *actor) {
	Actor *targetActor = nullptr;
	ActorAlignment alignment = actor->get_alignment();
	int actorX = actor->get_x();
	int actorY = actor->get_y();
	int maxStats = 0;

	for (int i = 1; i <= 0xff; ++i) {
		Actor *other = getActor(i);
		ActorAlignment otherAlign = other->get_alignment();

		if (other->is_alive() && other->get_actor_num() != actor->get_actor_num() &&
				actorFindMaxXYDistance(other, actorX, actorY) <= 8 && actorOkToAttack(actor, other) &&
				(alignment != ACTOR_ALIGNMENT_GOOD || otherAlign == ACTOR_ALIGNMENT_EVIL) &&
				(alignment != ACTOR_ALIGNMENT_EVIL || otherAlign == ACTOR_ALIGNMENT_GOOD) &&
				(alignment != ACTOR_ALIGNMENT_CHAOTIC ||
					(otherAlign != ACTOR_ALIGNMENT_CHAOTIC && otherAlign != ACTOR_ALIGNMENT_NEUTRAL))) {
			int combinedStats = other->get_strength() + other->get_dexterity() + other->get_intelligence();

			if (combinedStats > maxStats) {
				maxStats = combinedStats;
				targetActor = other;
			}
		}
	}

	if (!targetActor) {
		wtAttack(actor);
		subtractMovementPts(actor, 5);
		return;
	}

	int targetX = targetActor->get_x();
	int targetY = targetActor->get_y();
	Target weapon = actorGetWeapon(actor, Target(targetActor));
	int weaponRange = getWeaponRange(weapon.objN());
	int attackRange = actorGetCombatRange(actor, targetX, targetY);

	if (attackRange < 9 && attackRange <= weaponRange) {
		if (!mapCanReachPoint(actorX, actorY, targetX, targetY, actor->get_z())) {
			if (randomRange(0, 1) == 0) {
				targetX = targetActor->get_y() - actorY + actorX;
				targetY = actorY - targetActor->get_x() - actorX;
			} else {
				targetX = actorX - targetActor->get_y() - actorY;
				targetY = targetActor->get_x() - actorX + actorY;
			}
		} else {
			actorAttackTarget(actor, targetX, targetY, actor->get_z(), weapon, Target(targetActor));
			subtractMovementPts(actor, 15);
			return;
		}
	}

	int movesLeft = actor->get_moves_left();
	if (actorMoveTowardsLoc(actor, targetX, targetY) == 0) {
		actor->set_moves_left(movesLeft);
		wtAttack(actor);
	}

	subtractMovementPts(actor, 5);
}

void U6Scripts::wtGuardArrestPlayer(Actor *actor) {
	if (actorCatchUpToParty(actor))
		caughtByGuard(actor);
}

void U6Scripts::wtCombatTanglevine(Actor *actor) {
	Actor *target = actorFindTarget(actor);

	if (target) {
		int targetX = target->get_x();
		int targetY = target->get_y();
		int actorX = actor->get_x();
		int actorY = actor->get_y();

		if (ABS(targetX - actorX) < 2 && ABS(targetY - actorY) < 2 && randomRange(0, 1) != 0) {
			actorAttackTarget(actor, targetX, targetY, actor->get_z(), Target(actor), Target(target));
			subtractMovementPts(actor, 10);
			return;
		}

		if (ABS(targetX - actorX) < 5 && ABS(targetY - actorY) < 5 && randomRange(0, 3) == 0) {
			targetX -= actorX;
			targetY -= actorY;

			// The original also tried the other axis if this failed, but compared a boolean to 0 so never did
			if (ABS(targetX) <= ABS(targetY))
				moveTanglevine(actor, (targetY <= 0) ? NUVIE_DIR_N : NUVIE_DIR_S);
			else
				moveTanglevine(actor, (targetX <= 0) ? NUVIE_DIR_W : NUVIE_DIR_E);
		}
	}

	if (randomRange(0, 3) == 0) {
		NuvieDir direction = (NuvieDir)randomRange(0, 3);
		if (actor->get_direction() == direction)
			direction = reverseDirection(direction);

		moveTanglevine(actor, direction);
	} else {
		actor->set_moves_left(0);
	}
}

void U6Scripts::wtCombatStationary(Actor *actor) {
	ActorAlignment align = actor->get_alignment();

	if (align == ACTOR_ALIGNMENT_NEUTRAL) {
		subtractMovementPts(actor, 5);
		return;
	}

	for (int i = 0; i < 0x10; ++i) {
		int targetX = randomRange(0, 8) + actor->get_x() - 4;
		int targetY = randomRange(0, 8) + actor->get_y() - 4;
		Actor *targetActor = mapGetActor(targetX, targetY, actor->get_z());

		if (targetActor && actorOkToAttack(actor, targetActor) && targetActor->is_alive() &&
				targetActor->get_alignment() != align && targetActor->get_alignment() != ACTOR_ALIGNMENT_NEUTRAL) {
			actorAttackTarget(actor, targetX, targetY, actor->get_z(), actorGetWeapon(actor, Target(targetActor)),
				Target(targetActor));
			subtractMovementPts(actor, 10);
			return;
		}
	}

	subtractMovementPts(actor, 5);
}

void U6Scripts::wtWalkStraight(Actor *actor) {
	if (randomRange(0, 1) == 0) {
		subtractMovementPts(actor, 5);
		return;
	}

	int wt = actor->get_worktype();
	MapCoord sched = actor->get_location();
	actor->get_schedule_location(&sched);
	NuvieDir direction = actor->get_direction();

	if (wt >= WT_WALK_NORTH_SOUTH && actor->get_x() == sched.x && actor->get_y() == sched.y) {
		if (wt == WT_WALK_NORTH_SOUTH)
			direction = NUVIE_DIR_N;
		if (wt == WT_WALK_EAST_WEST)
			direction = NUVIE_DIR_E;
		if (wt == WT_WALK_SOUTH_NORTH)
			direction = NUVIE_DIR_S;
		if (wt == WT_WALK_WEST_EAST)
			direction = NUVIE_DIR_W;

		actor->set_direction(direction);
	}

	int movesLeft = actor->get_moves_left();

	if (!actorMove(actor, direction, 1)) {
		direction = reverseDirection(direction);

		actor->set_moves_left(movesLeft);
		actorMove(actor, direction, 1);
		actor->set_direction(direction);
	}
}

void U6Scripts::wtWanderAround(Actor *actor) {
	if (randomRange(0, 7) != 0) {
		subtractMovementPts(actor, 5);
		return;
	}

	struct Wander {
		static int range() {
			int i = 0;
			while (randomRange(0, 1) != 0)
				++i;
			return randomRange(0, 1) != 0 ? -i : i;
		}
	};

	MapCoord sched = actor->get_location();
	actor->get_schedule_location(&sched);
	int schedXOffset = (int)actor->get_x() - sched.x;
	int schedYOffset = (int)actor->get_y() - sched.y;
	NuvieDir direction;

	if (ABS(schedYOffset) - ABS(schedXOffset) >= Wander::range())
		direction = (Wander::range() <= schedYOffset) ? NUVIE_DIR_N : NUVIE_DIR_S;
	else
		direction = (Wander::range() <= schedXOffset) ? NUVIE_DIR_W : NUVIE_DIR_E;

	actor->set_direction(direction);
	actorMove(actor, direction, 1);
}

void U6Scripts::moveTowardsPlayer(Actor *actor) {
	if (actor->get_worktype() != WT_STATIONARY) {
		if (actor->is_in_party()) {
			MapCoord playerLoc = Game::get_game()->get_player()->get_actor()->get_location();

			if (actor->get_x() != playerLoc.x && actor->get_y() != playerLoc.y && randomRange(0, 3) == 0) {
				actorMoveTowardsLoc(actor, playerLoc.x, playerLoc.y);
				return;
			}
		}

		if (randomRange(0, 7) == 0) {
			actorMove(actor, (NuvieDir)randomRange(0, 3), 1);
			return;
		}
	}

	subtractMovementPts(actor, 5);
}

void U6Scripts::wtAttack(Actor *actor) {
	Actor *target = actorFindTarget(actor);
	Target weapon = actorGetWeapon(actor, Target(target));

	if (target) {
		int targetX = target->get_x();
		int targetY = target->get_y();
		int actorX = actor->get_x();
		int actorY = actor->get_y();
		int weaponRange = getWeaponRange(weapon.objN());

		if (ABS(targetX - actorX) < 8 && ABS(targetY - actorY) < 8 &&
				actorGetCombatRange(actor, targetX, targetY) <= weaponRange) {
			if (mapCanReachPoint(actorX, actorY, targetX, targetY, actor->get_z())) {
				actorAttackTarget(actor, target->get_x(), target->get_y(), target->get_z(), weapon, Target(target));
				subtractMovementPts(actor, 10);
				return;
			}

			if (randomRange(0, 1) == 0) {
				targetX = target->get_y() - actorY + actorX;
				targetY = actorY - target->get_x() - actorX;
			} else {
				targetX = actorX - target->get_y() - actorY;
				targetY = target->get_x() - actorX + actorY;
			}

			actorMoveTowardsLoc(actor, targetX, targetY);
			return;
		}

		if (actor->get_worktype() != 3) {
			actorMoveTowardsLoc(actor, targetX, targetY);
			return;
		}

		subtractMovementPts(actor, 5);
		return;
	}

	if (actor->get_worktype() != 3) {
		moveTowardsPlayer(actor);
		return;
	}

	subtractMovementPts(actor, 5);
}

void U6Scripts::wtTimid(Actor *actor) {
	MapCoord playerLoc = Game::get_game()->get_player()->get_actor()->get_location();

	if (actorFindMaxXYDistance(actor, playerLoc.x, playerLoc.y) > 7) {
		actor->set_moves_left(0);
		return;
	}

	Actor *target = actorFindTarget(actor);
	if (!target) {
		moveTowardsPlayer(actor);
		return;
	}

	int diffX = (int)target->get_x() - (int)actor->get_x();
	int diffY = (int)target->get_y() - (int)actor->get_y();
	int var4 = (actor->get_worktype() != WT_RETREAT) ? 3 : 8;

	if (ABS(diffX) < var4 && ABS(diffY) < var4) {
		int movesLeft = actor->get_moves_left();

		if (actorMoveTowardsLoc(actor, (int)actor->get_x() - diffX, (int)actor->get_y() - diffY) == 0) {
			if (actor->get_worktype() != WT_MOUSE && actor->get_worktype() != WT_FLEE) {
				actor->set_moves_left(movesLeft);
				wtAttack(actor);
			}
		}
	} else if (actor->get_alignment() == ACTOR_ALIGNMENT_GOOD && actor->get_worktype() != WT_FLEE &&
			actor->get_worktype() != WT_MOUSE) {
		actorMoveTowardsLoc(actor, _partyAvgX, _partyAvgY);
	} else {
		moveTowardsPlayer(actor);
	}

	if (actor->get_worktype() == WT_RETREAT) {
		if (randomRange(0, 3) == 0) {
			if (actor->get_level() > actor->get_hp())
				actor->set_hp(actor->get_hp() + 1);

			// A level of 0 divides to infinity in the original
			if (actor->get_level() == 0 || (actor->get_hp() * 4) / actor->get_level() > 0)
				actor->set_worktype(actor->get_combat_mode());
		}
	}
}

void U6Scripts::wtLike(Actor *actor) {
	int actorX = actor->get_x();
	int actorY = actor->get_y();

	Common::Array<Actor *> members;
	getPartyMembers(members);

	for (uint i = 0; i < members.size(); ++i) {
		if (ABS((int)members[i]->get_x() - actorX) < 3 && ABS((int)members[i]->get_y() - actorY) < 3) {
			if (randomRange(0, 1) == 0) {
				actorMoveTowardsLoc(actor, _partyAvgX, _partyAvgY);
				return;
			}

			break;
		}
	}

	wanderNearby(actor, actorX, actorY);
}

void U6Scripts::wtUnfriendly(Actor *actor) {
	int actorX = actor->get_x();
	int actorY = actor->get_y();

	Common::Array<Actor *> members;
	getPartyMembers(members);

	for (uint i = 0; i < members.size(); ++i) {
		if (ABS((int)members[i]->get_x() - actorX) < 3 && ABS((int)members[i]->get_y() - actorY) < 3) {
			if (randomRange(0, 7) == 0) {
				actor->set_worktype(WT_ASSAULT);
				wtAttack(actor);
				return;
			}

			break;
		}
	}

	wanderNearby(actor, actorX, actorY);
}

void U6Scripts::wanderNearby(Actor *actor, int actorX, int actorY) {
	if (randomRange(0, 1) == 0) {
		subtractMovementPts(actor, 5);
		return;
	}

	if (randomRange(0, 1) == 0)
		actorY += (randomRange(0, 1) == 0) ? -10 : 10;
	else
		actorX += (randomRange(0, 1) == 0) ? -10 : 10;

	actorMoveTowardsLoc(actor, actorX, actorY);
}

void U6Scripts::wtWalkToLocation(Actor *actor) {
	actor->walk_path();

	if (actor->is_at_scheduled_location()) {
		actor->set_worktype(actor->get_sched_worktype());

		if (actor->get_worktype() == WT_SLEEP)
			wtSleep(actor);
	}

	subtractMovementPts(actor, 5);
}

void U6Scripts::wtSleep(Actor *actor) {
	Obj *bed = mapGetObj(actor->get_x(), actor->get_y(), actor->get_z(), 0xa3);

	if (bed) {
		if (bed->frame_n == 1 || bed->frame_n == 5) { // horizontal bed
			actor->set_obj_n(0x92); // person sleeping
			actor->set_frame_n(0);
		} else if (bed->frame_n == 7 || bed->frame_n == 10) { // vertical bed
			actor->set_obj_n(0x92);
			actor->set_frame_n(1);
		}
	}
}

void U6Scripts::wtBeg(Actor *actor) {
	MapCoord playerLoc = Game::get_game()->get_player()->get_actor()->get_location();

	if ((actor->get_z() == playerLoc.z && ABS((int)actor->get_x() - (int)playerLoc.x) < 5) ||
			ABS((int)actor->get_y() - (int)playerLoc.y) < 5) {
		if (actorCatchUpToParty(actor)) {
			actor->set_worktype(WT_WORK);
			actorTalk(actor);
		}
	} else {
		wtWanderAround(actor);
	}
}

void U6Scripts::wtBrawling(Actor *actor) {
	if (randomRange(0, 3) == 0) {
		Actor *target = actorFindTarget(actor);
		MapCoord playerLoc = Game::get_game()->get_player()->get_actor()->get_location();

		if (target && actorFindMaxXYDistance(target, playerLoc.x, playerLoc.y) < 5) {
			if (actorFindMaxXYDistance(actor, target->get_x(), target->get_y()) < 2) {
				hitAnim(target->get_x(), target->get_y());
				actorMoveTowardsLoc(actor, playerLoc.x, playerLoc.y);
			} else {
				actorMoveTowardsLoc(actor, target->get_x(), target->get_y());
			}

			return;
		}
	}

	wtWanderAround(actor);
}

bool U6Scripts::hasWorktypeHandler(int worktype) {
	switch (worktype) {
	case WT_FRONT: case WT_REAR: case WT_FLANK: case WT_BERSERK: case WT_RETREAT: case WT_FLEE: case WT_UNK_13:
	case WT_MOUSE: case WT_ASSAULT: case WT_ATTACK_PARTY: case WT_LIKE: case WT_UNFRIENDLY: case WT_WANDER_NEAR_PLAYER:
	case WT_WANDER_AROUND: case WT_TANGLE: case WT_STATIONARY: case WT_GUARD_WALK_EAST_WEST:
	case WT_GUARD_WALK_NORTH_SOUTH: case WT_WALK_NORTH_SOUTH: case WT_WALK_EAST_WEST: case WT_WALK_SOUTH_NORTH:
	case WT_WALK_WEST_EAST: case WT_GUARD_ARREST_PLAYER: case WT_WALK_TO_LOCATION: case WT_WORK: case WT_UNK_94:
	case WT_BEG: case WT_BRAWLING:
		return true;
	default:
		return false;
	}
}

void U6Scripts::performWorktype(Actor *actor) {
	if (_timeStopped) {
		actor->set_moves_left(0);
		return;
	}

	if (!hasWorktypeHandler(actor->get_worktype())) {
		subtractMovementPts(actor, 5);
		return;
	}

	if (actor->get_moves_left() > 0) {
		switch (actor->get_worktype()) {
		case WT_FRONT:
			wtFront(actor);
			break;
		case WT_REAR:
			wtRear(actor);
			break;
		case WT_FLANK:
			wtFlank(actor);
			break;
		case WT_BERSERK:
			wtBerserk(actor);
			break;
		case WT_RETREAT:
		case WT_FLEE:
		case WT_UNK_13:
		case WT_MOUSE:
			wtTimid(actor);
			break;
		case WT_ASSAULT:
		case WT_ATTACK_PARTY:
			wtAttack(actor);
			break;
		case WT_LIKE:
			wtLike(actor);
			break;
		case WT_UNFRIENDLY:
			wtUnfriendly(actor);
			break;
		case WT_WANDER_NEAR_PLAYER:
		case WT_WANDER_AROUND:
			moveTowardsPlayer(actor);
			break;
		case WT_TANGLE:
			wtCombatTanglevine(actor);
			break;
		case WT_STATIONARY:
			wtCombatStationary(actor);
			break;
		case WT_GUARD_WALK_EAST_WEST:
		case WT_GUARD_WALK_NORTH_SOUTH:
		case WT_WALK_NORTH_SOUTH:
		case WT_WALK_EAST_WEST:
		case WT_WALK_SOUTH_NORTH:
		case WT_WALK_WEST_EAST:
			wtWalkStraight(actor);
			break;
		case WT_GUARD_ARREST_PLAYER:
			wtGuardArrestPlayer(actor);
			break;
		case WT_WALK_TO_LOCATION:
			wtWalkToLocation(actor);
			break;
		case WT_WORK:
		case WT_UNK_94:
			wtWanderAround(actor);
			break;
		case WT_BEG:
			wtBeg(actor);
			break;
		case WT_BRAWLING:
			wtBrawling(actor);
			break;
		default:
			break;
		}
	}

	if (actor->get_moves_left() == 0)
		subtractMovementPts(actor, 0xa);
}

} // End of namespace U6
} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima
