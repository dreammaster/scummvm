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

#include "ultima/ultima6/script/u6/u6_scripts.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {
namespace U6 {

// Strength, dexterity, intelligence, hit points, damage, alignment, flags read by statFlag(), then spells, weapons, armour, treasure and experience
const U6Scripts::ActorStats U6Scripts::ACTOR_STATS[] = {
	{ 364, 5, 5, 2, 10, 1, ACTOR_ALIGNMENT_CHAOTIC, { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 0 },  // Acid Slug
	{ 429, 20, 10, 3, 30, 10, ACTOR_ALIGNMENT_CHAOTIC, { 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 6 },  // Alligator
	{ 427, 16, 15, 2, 10, 8, ACTOR_ALIGNMENT_CHAOTIC, { 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 3 },  // Ant, Giant
	{ 410, 25, 25, 25, 30, 4, ACTOR_ALIGNMENT_GOOD, { 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 0 },  // Avatar
	{ 344, 5, 25, 4, 5, 6, ACTOR_ALIGNMENT_CHAOTIC, { 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 0 },  // Bat, Giant
	{ 384, 8, 8, 9, 6, 3, ACTOR_ALIGNMENT_CHAOTIC, { 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 38, 34, 0, 0, 0 }, { 185, 0, 0, 0 }, { 0, 0, 0 }, 0 },  // Peasant
	{ 356, 1, 28, 4, 3, 1, ACTOR_ALIGNMENT_NEUTRAL, { 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 0 },  // Bird
	{ 381, 8, 8, 9, 6, 2, ACTOR_ALIGNMENT_CHAOTIC, { 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 0 },  // Child
	{ 357, 17, 6, 3, 40, 15, ACTOR_ALIGNMENT_CHAOTIC, { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 0 },  // Corpser
	{ 428, 14, 7, 5, 10, 3, ACTOR_ALIGNMENT_NEUTRAL, { 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 129, 0, 0 }, 0 },  // Cow
	{ 424, 27, 8, 7, 40, 20, ACTOR_ALIGNMENT_EVIL, { 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 44, 33, 34, 0, 0 }, { 20, 3, 13, 0 }, { 98, 0, 0 }, 4 },  // Cyclops
	{ 367, 35, 26, 31, 100, 20, ACTOR_ALIGNMENT_EVIL, { 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1 }, { 17, 128, 34, 80, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 10 },  // Daemon
	{ 350, 11, 18, 5, 8, 2, ACTOR_ALIGNMENT_NEUTRAL, { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 129, 0, 0 }, 0 },  // Deer
	{ 349, 5, 16, 8, 6, 3, ACTOR_ALIGNMENT_NEUTRAL, { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 0 },  // Dog
	{ 411, 40, 35, 27, 150, 30, ACTOR_ALIGNMENT_EVIL, { 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0 }, { 34, 83, 68, 96, 119, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 98, 88, 88 }, 12 },  // Dragon
	{ 369, 22, 22, 13, 50, 10, ACTOR_ALIGNMENT_EVIL, { 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0 }, { 34, 68, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 88, 88, 0 }, 4 },  // Drake
	{ 387, 10, 14, 15, 8, 4, ACTOR_ALIGNMENT_NEUTRAL, { 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 38, 0, 0, 0, 0 }, { 185, 0, 0, 0 }, { 88, 0, 0 }, 0 },  // Woman
	{ 385, 15, 15, 10, 8, 4, ACTOR_ALIGNMENT_NEUTRAL, { 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 105, 101, 0, 0, 0 }, { 17, 0, 0, 0 }, { 88, 0, 0 }, 0 },  // Farmer
	{ 376, 20, 17, 11, 20, 6, ACTOR_ALIGNMENT_CHAOTIC, { 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 43, 42, 0, 0, 0 }, { 21, 10, 3, 22 }, { 88, 88, 0 }, 0 },  // Fighter
	{ 362, 15, 32, 33, 50, 6, ACTOR_ALIGNMENT_EVIL, { 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1 }, { 68, 65, 69, 0, 0, 0, 0, 0 }, { 46, 41, 47, 49, 0 }, { 18, 1, 0, 0 }, { 58, 0, 0 }, 5 },  // Gargoyle, Winged
	{ 363, 25, 19, 8, 30, 15, ACTOR_ALIGNMENT_EVIL, { 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 34, 49, 37, 0, 0 }, { 1, 9, 18, 20 }, { 0, 0, 0 }, 10 },  // Gargoyle, Worker
	{ 355, 2, 10, 21, 20, 1, ACTOR_ALIGNMENT_EVIL, { 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1 }, { 20, 68, 80, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 0 },  // Gazer
	{ 352, 1, 10, 9, 20, 12, ACTOR_ALIGNMENT_EVIL, { 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1 }, { 128, 99, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 0 },  // Ghost
	{ 353, 2, 26, 7, 5, 1, ACTOR_ALIGNMENT_CHAOTIC, { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 0 },  // Gremlin
	{ 382, 9, 30, 9, 100, 8, ACTOR_ALIGNMENT_EVIL, { 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 42, 47, 0, 0, 0 }, { 22, 10, 3, 0 }, { 88, 0, 0 }, 0 },  // Guard
	{ 370, 19, 13, 8, 20, 12, ACTOR_ALIGNMENT_EVIL, { 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 34, 36, 0, 0, 0 }, { 17, 9, 0, 0 }, { 88, 0, 0 }, 2 },  // Headless
	{ 430, 22, 27, 7, 15, 3, ACTOR_ALIGNMENT_NEUTRAL, { 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 0 },  // Horse
	{ 431, 22, 27, 7, 15, 3, ACTOR_ALIGNMENT_NEUTRAL, { 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 0 },  // Horse with rider
	{ 374, 26, 11, 2, 50, 15, ACTOR_ALIGNMENT_CHAOTIC, { 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1 }, { 69, 96, 98, 68, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 0 },  // Hydrae
	{ 343, 1, 40, 1, 1, 1, ACTOR_ALIGNMENT_CHAOTIC, { 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 0 },  // Insects
	{ 383, 14, 16, 12, 8, 4, ACTOR_ALIGNMENT_CHAOTIC, { 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 38, 33, 0, 0, 0 }, { 17, 0, 0, 0 }, { 88, 0, 0 }, 0 },  // Jester
	{ 409, 30, 30, 30, 255, 255, ACTOR_ALIGNMENT_GOOD, { 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 30 },  // Lord British
	{ 386, 12, 16, 14, 8, 4, ACTOR_ALIGNMENT_CHAOTIC, { 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 41, 33, 43, 0, 0 }, { 19, 1, 9, 0 }, { 88, 158, 0 }, 0 },  // Musician
	{ 378, 10, 14, 22, 30, 4, ACTOR_ALIGNMENT_EVIL, { 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }, { 17, 34, 20, 49, 68, 80, 69, 83 }, { 38, 0, 0, 0, 0 }, { 17, 1, 0, 0 }, { 58, 98, 88 }, 0 },  // Mage
	{ 380, 12, 12, 18, 8, 4, ACTOR_ALIGNMENT_EVIL, { 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 38, 43, 0, 0, 0 }, { 17, 1, 0, 0 }, { 88, 0, 0 }, 0 },  // Merchant
	{ 98, 22, 9, 8, 30, 15, ACTOR_ALIGNMENT_CHAOTIC, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1 }, { 18, 80, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 98, 98, 0 }, 5 },  // Mimic
	{ 372, 20, 27, 13, 30, 20, ACTOR_ALIGNMENT_EVIL, { 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 38, 43, 36, 37, 0 }, { 9, 17, 18, 0 }, { 98, 88, 0 }, 4 },  // Mongbat
	{ 354, 2, 25, 3, 5, 2, ACTOR_ALIGNMENT_NEUTRAL, { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 0 },  // Mouse
	{ 359, 3, 9, 3, 5, 2, ACTOR_ALIGNMENT_NEUTRAL, { 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 0 },  // Rabbit
	{ 342, 5, 13, 4, 8, 6, ACTOR_ALIGNMENT_CHAOTIC, { 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 0 },  // Rat, Giant
	{ 347, 21, 20, 16, 30, 20, ACTOR_ALIGNMENT_CHAOTIC, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1 }, { 17, 80, 66, 68, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 58, 88, 0 }, 4 },  // Reaper
	{ 360, 3, 17, 2, 5, 2, ACTOR_ALIGNMENT_CHAOTIC, { 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 0 },  // Rot Worm
	{ 426, 13, 13, 3, 20, 8, ACTOR_ALIGNMENT_CHAOTIC, { 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 3 },  // Scorpion, Giant
	{ 346, 17, 17, 9, 70, 30, ACTOR_ALIGNMENT_EVIL, { 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 34, 83, 51, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 2 },  // Sea Serpent
	{ 348, 5, 10, 4, 6, 2, ACTOR_ALIGNMENT_NEUTRAL, { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 129, 0, 0 }, 0 },  // Sheep
	{ 412, 30, 30, 30, 30, 30, ACTOR_ALIGNMENT_CHAOTIC, { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 30 },  // Ship
	{ 413, 60, 21, 8, 200, 60, ACTOR_ALIGNMENT_CHAOTIC, { 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 15 },  // Silver Serpent
	{ 368, 10, 14, 7, 20, 6, ACTOR_ALIGNMENT_EVIL, { 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 43, 36, 37, 41, 38 }, { 9, 1, 0, 0 }, { 88, 0, 0 }, 0 },  // Skeleton
	{ 375, 6, 6, 2, 15, 4, ACTOR_ALIGNMENT_CHAOTIC, { 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 0 },  // Slime
	{ 358, 3, 7, 6, 5, 2, ACTOR_ALIGNMENT_CHAOTIC, { 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 1 },  // Snake
	{ 361, 9, 11, 4, 10, 8, ACTOR_ALIGNMENT_CHAOTIC, { 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 }, { 89, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 129, 0, 0 }, 0 },  // Spider, Giant
	{ 345, 24, 20, 8, 50, 20, ACTOR_ALIGNMENT_EVIL, { 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1 }, { 17, 68, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 0 },  // Squid, Giant
	{ 377, 15, 18, 12, 15, 4, ACTOR_ALIGNMENT_CHAOTIC, { 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 43, 41, 33, 0, 0 }, { 19, 1, 9, 0 }, { 88, 88, 0 }, 0 },  // Swashbuckler
	{ 366, 20, 25, 1, 30, 1, ACTOR_ALIGNMENT_CHAOTIC, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 4 },  // Tangle Vine
	{ 371, 18, 14, 9, 15, 6, ACTOR_ALIGNMENT_EVIL, { 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 37, 36, 43, 41, 34 }, { 9, 18, 1, 19 }, { 98, 88, 88 }, 4 },  // Troll
	{ 379, 11, 12, 12, 8, 4, ACTOR_ALIGNMENT_CHAOTIC, { 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 43, 33, 0, 0, 0 }, { 17, 0, 0, 0 }, { 88, 0, 0 }, 0 },  // Villager
	{ 373, 1, 40, 20, 40, 1, ACTOR_ALIGNMENT_CHAOTIC, { 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1 }, { 20, 69, 129, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 0 },  // Wisp
	{ 351, 8, 16, 8, 8, 4, ACTOR_ALIGNMENT_CHAOTIC, { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 0 },  // Wolf
	{ 365, 20, 5, 1, 30, 1, ACTOR_ALIGNMENT_CHAOTIC, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1 }, { 20, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 4 },  // TV Flower
	{ 414, 20, 20, 20, 20, 20, ACTOR_ALIGNMENT_CHAOTIC, { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 20 },  // Skiff
	{ 415, 10, 10, 10, 10, 10, ACTOR_ALIGNMENT_CHAOTIC, { 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 10 },  // Raft
	{ 423, 10, 10, 10, 10, 10, ACTOR_ALIGNMENT_CHAOTIC, { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 10 },  // Balloon
	{ 388, 4, 18, 8, 6, 2, ACTOR_ALIGNMENT_NEUTRAL, { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 0 },  // Cat
	{ 263, 11, 12, 12, 8, 4, ACTOR_ALIGNMENT_CHAOTIC, { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0 }, 0 },  // Sinjen (Yew Stocks guy)
};
const uint U6Scripts::ACTOR_STATS_COUNT = ARRAYSIZE(U6Scripts::ACTOR_STATS);

const GameScripts::ObjValue U6Scripts::WEAPON_RANGES[] = {
	{ 40, 2 },  // morning star
	{ 47, 2 },  // halberd
	{ 38, 3 },  // dagger
	{ 83, 5 },  // flask of oil
	{ 33, 4 },  // sling
	{ 37, 3 },  // throwing axe
	{ 36, 4 },  // spear
	{ 41, 5 },  // bow
	{ 42, 7 },  // crossbow
	{ 57, 7 },  // spellbook
	{ 336, 5 },  // charge
	{ 49, 5 },  // boomerang
	{ 50, 7 },  // triple crossbow
	{ 412, 5 },  // ship
	{ 91, 4 },  // Zu Ylem
	{ 54, 7 },  // magic bow
	{ 79, 7 },  // lightning wand
	{ 80, 7 },  // fire wand
};
const uint U6Scripts::WEAPON_RANGES_COUNT = ARRAYSIZE(U6Scripts::WEAPON_RANGES);

const GameScripts::ObjValue U6Scripts::WEAPON_DAMAGE[] = {
	{ 4, 4 },  // spiked helm
	{ 33, 6 },  // sling
	{ 34, 8 },  // club
	{ 35, 8 },  // main gauche
	{ 36, 10 },  // spear
	{ 37, 10 },  // throwing axe
	{ 38, 6 },  // dagger
	{ 39, 15 },  // mace
	{ 40, 15 },  // morning star
	{ 41, 10 },  // bow
	{ 42, 12 },  // crossbow
	{ 43, 15 },  // sword
	{ 44, 20 },  // two-handed hammer
	{ 45, 20 },  // two-handed axe
	{ 46, 20 },  // two-handed sword
	{ 47, 30 },  // halberd
	{ 49, 8 },  // boomerang
	{ 50, 12 },  // triple crossbow
	{ 48, 255 },  // glass sword
	{ 13, 4 },  // spiked shield
	{ 221, 90 },  // cannon
	{ 83, 4 },  // flask of oil
	{ 54, 20 },  // magic bow
	{ 109, 2 },  // rolling pin
	{ 100, 6 },  // scythe
	{ 101, 4 },  // pitchfork
	{ 103, 4 },  // pick
	{ 104, 4 },  // shovel
	{ 105, 4 },  // hoe
	{ 113, 4 },  // cleaver
	{ 114, 4 },  // knife
	{ 141, 4 },  // decorative sword
	{ 412, 30 },  // ship
	{ 91, 0 },  // Zu Ylem
	{ 78, 4 },  // staff
	{ 79, 30 },  // lightning wand
	{ 80, 20 },  // fire wand
};
const uint U6Scripts::WEAPON_DAMAGE_COUNT = ARRAYSIZE(U6Scripts::WEAPON_DAMAGE);

const GameScripts::ObjValue U6Scripts::ARMOUR[] = {
	{ 1, 1 },  // leather helm
	{ 2, 2 },  // chain coif
	{ 3, 3 },  // iron helm
	{ 4, 3 },  // spiked helm
	{ 5, 2 },  // winged helm
	{ 6, 2 },  // brass helm
	{ 7, 3 },  // Gargoyle Helm
	{ 8, 5 },  // magic helm
	{ 9, 2 },  // wooden shield
	{ 10, 3 },  // curved heater
	{ 11, 3 },  // winged shield
	{ 12, 3 },  // kite shield
	{ 13, 2 },  // spiked shield
	{ 14, 2 },  // black shield
	{ 15, 4 },  // door shield
	{ 16, 5 },  // magic shield
	{ 17, 1 },  // cloth armour
	{ 18, 2 },  // leather armor
	{ 19, 3 },  // ring mail
	{ 20, 4 },  // scale mail
	{ 21, 5 },  // chain mail
	{ 22, 7 },  // plate mail
	{ 23, 10 },  // magic armour
	{ 35, 1 },  // main gauche
	{ 24, 2 },  // spiked collar
	{ 256, 5 },  // protection ring
};
const uint U6Scripts::ARMOUR_COUNT = ARRAYSIZE(U6Scripts::ARMOUR);

const U6Scripts::ProjectileInfo U6Scripts::PROJECTILES[] = {
	{ 33, 398, 0, 2, 0 },  // sling
	{ 36, 547, 45, 3, 0 },  // spear
	{ 37, 548, 0, 2, 10 },  // throwing axe
	{ 38, 549, 0, 2, 10 },  // dagger
	{ 41, 566, 90, 4, 0 },  // bow
	{ 42, 567, 90, 4, 0 },  // crossbow
	{ 49, 560, 0, 4, 10 },  // boomerang
	{ 83, 601, 0, 2, 0 },  // flask of oil
	{ 50, 567, 90, 4, 0 },  // triple crossbow
	{ 412, 399, 0, 2, 0 },  // ship
	{ 91, 612, 0, 2, 10 },  // Zu Ylem
	{ 54, 566, 90, 4, 0 },  // magic bow
	{ 79, 392, 0, 2, 0 },  // lightning wand
	{ 80, 393, 0, 2, 0 },  // fire wand
};
const uint U6Scripts::PROJECTILES_COUNT = ARRAYSIZE(U6Scripts::PROJECTILES);

// Direction offset added for each projectile of the triple crossbow and the wind spells, by position relative to the caster
const uint8 U6Scripts::PROJECTILE_OFFSETS[2][121] = {
	{
		4, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6,
		4, 4, 5, 5, 5, 6, 6, 6, 6, 6, 7,
		4, 4, 4, 5, 5, 6, 6, 6, 6, 7, 7,
		4, 4, 4, 4, 5, 6, 6, 6, 7, 7, 7,
		4, 4, 4, 4, 4, 6, 6, 7, 7, 7, 7,
		4, 4, 4, 4, 4, 0, 0, 0, 0, 0, 0,
		3, 3, 3, 3, 2, 2, 0, 0, 0, 0, 0,
		3, 3, 3, 2, 2, 2, 1, 0, 0, 0, 0,
		3, 3, 2, 2, 2, 2, 1, 1, 0, 0, 0,
		3, 2, 2, 2, 2, 2, 1, 1, 1, 0, 0,
		2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 0
	},
	{
		2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 4,
		1, 2, 2, 2, 2, 2, 3, 3, 3, 4, 4,
		1, 1, 2, 2, 2, 2, 3, 3, 4, 4, 4,
		1, 1, 1, 2, 2, 2, 3, 4, 4, 4, 4,
		1, 1, 1, 1, 2, 2, 4, 4, 4, 4, 4,
		0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4,
		0, 0, 0, 0, 0, 6, 6, 5, 5, 5, 5,
		0, 0, 0, 0, 7, 6, 6, 6, 5, 5, 5,
		0, 0, 0, 7, 7, 6, 6, 6, 6, 5, 5,
		0, 0, 7, 7, 7, 6, 6, 6, 6, 6, 5,
		0, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6
	}
};

} // End of namespace U6
} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima
