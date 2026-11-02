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

#include "ultima/ultima6/script/md/md_scripts.h"

namespace Ultima {
namespace Ultima6 {
namespace Scripts {
namespace MD {

// Alignments: 1 neutral, 2 evil, 3 good, 4 chaotic. Values after the alignment are the unknown, damage and flag bytes
const MDScripts::ActorStats MDScripts::ACTOR_STATS[] = {
	{ 343, { 25, 25, 25, 90, 3, 0, 6, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },  // OBJ_YOURSELF5
	{ 344, { 25, 25, 25, 90, 3, 0, 6, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },  // OBJ_YOURSELF6
	{ 342, { 22, 22, 22, 60, 2, 0, 6, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },  // OBJ_POOR_MONK
	{ 345, { 15, 15, 25, 30, 3, 0, 6, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },  // OBJ_SCIENTIST
	{ 346, { 18, 18, 22, 60, 3, 0, 6, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },  // OBJ_YOUNG_WOMAN
	{ 347, { 20, 25, 18, 60, 3, 0, 6, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },  // OBJ_COWBOY
	{ 348, { 25, 25, 25, 90, 3, 0, 6, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },  // OBJ_GENTLEMAN
	{ 349, { 25, 25, 25, 90, 3, 0, 6, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },  // OBJ_UNIFORMED_MAN
	{ 350, { 25, 25, 25, 90, 3, 0, 6, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },  // OBJ_ADVENTURER
	{ 351, { 25, 25, 25, 90, 3, 0, 6, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },  // OBJ_COMMON_FELLOW
	{ 352, { 25, 25, 25, 90, 3, 0, 6, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },  // OBJ_MAN
	{ 353, { 25, 25, 25, 90, 3, 0, 6, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },  // OBJ_MAN_IN_WHITE
	{ 354, { 25, 25, 25, 90, 3, 0, 6, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },  // OBJ_LADY
	{ 355, { 25, 25, 25, 90, 3, 0, 6, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },  // OBJ_WOMAN
	{ 356, { 25, 25, 25, 90, 3, 0, 6, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },  // OBJ_ADVENTURESS
	{ 357, { 25, 25, 25, 90, 3, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },  // OBJ_METAL_WOMAN1
	{ 358, { 25, 15, 10, 40, 3, 4, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },  // OBJ_MECHANICAL_MAN
	{ 369, { 15, 15, 20, 30, 1, 0, 6, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 } },  // OBJ_MARTIAN
	{ 359, { 20, 20, 10, 45, 4, 4, 35, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 } },  // OBJ_SEXTELLEGER
	{ 360, { 25, 12, 11, 35, 1, 8, 20, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0 } },  // OBJ_BUSHALO
	{ 361, { 15, 22, 9, 20, 1, 1, 12, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 } },  // OBJ_PLANTELLOPE
	{ 372, { 20, 15, 10, 40, 4, 8, 25, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 } },  // OBJ_ROCKWORM2
	{ 377, { 20, 20, 20, 80, 1, 3, 30, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },  // OBJ_WISP
	{ 378, { 10, 20, 15, 10, 2, 0, 6, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 } },  // OBJ_AIRSQUID
	{ 380, { 15, 15, 8, 30, 2, 4, 20, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 } },  // OBJ_AIRSQUID1
	{ 373, { 30, 10, 9, 100, 4, 3, 30, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 } },  // OBJ_GIANT_MAW
	{ 362, { 20, 20, 9, 10, 4, 2, 20, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0 } },  // OBJ_TENTACLE1
	{ 383, { 20, 12, 3, 30, 4, 4, 25, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 } },  // OBJ_CREEPING_CACTUS
	{ 363, { 15, 15, 3, 20, 4, 10, 12, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 } },  // OBJ_AMMONOID
	{ 364, { 20, 19, 10, 20, 2, 2, 10, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0 } },  // OBJ_PROTO_MARTIAN
	{ 145, { 20, 19, 10, 20, 2, 2, 12, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0 } },  // OBJ_MONSTER_FOOTPRINTS
	{ 384, { 15, 18, 3, 25, 4, 4, 12, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 } },  // OBJ_POD_DEVIL
	{ 385, { 10, 22, 10, 10, 4, 1, 4, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 } },  // OBJ_JUMPING_BEAN
	{ 365, { 17, 12, 8, 20, 4, 3, 20, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 } },  // OBJ_HEDGEHOG
	{ 374, { 15, 15, 3, 20, 4, 1, 16, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 } },  // OBJ_CREEPER
	{ 376, { 29, 18, 10, 30, 4, 10, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 } },  // OBJ_AGROBOT
	{ 379, { 20, 20, 15, 30, 4, 5, 25, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0 } },  // OBJ_CANAL_WORM
	{ 381, { 30, 30, 30, 255, 1, 99, 60, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } },  // OBJ_DUST_DEVIL
	{ 386, { 25, 25, 10, 40, 4, 4, 18, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0 } },  // OBJ_SAND_TRAPPER
	{ 375, { 10, 25, 12, 20, 4, 1, 10, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 } },  // OBJ_OXY_LEECH
	{ 366, { 15, 15, 15, 30, 2, 2, 15, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 } },  // OBJ_CAVE_WORM
	{ 367, { 20, 15, 10, 30, 4, 10, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 } },  // OBJ_JANITOR
	{ 387, { 10, 25, 10, 20, 2, 2, 12, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 } },  // OBJ_BUSHRAT
	{ 368, { 15, 22, 12, 20, 4, 4, 20, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 } },  // OBJ_PLANTHER
	{ 398, { 20, 18, 8, 90, 1, 2, 15, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 } },  // OBJ_MINOTAUR
	{ 391, { 12, 20, 15, 255, 4, 9, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 } },  // OBJ_YOUR_MOTHER
	{ 408, { 25, 18, 5, 90, 1, 3, 12, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 } },  // OBJ_TREE
	{ 424, { 25, 18, 5, 90, 1, 3, 12, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 } },  // OBJ_SOMETHING_YOU_SHOULDNT2
	{ 403, { 20, 20, 10, 255, 2, 99, 12, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }  // OBJ_HUGE_RED_SPOT
};

const uint MDScripts::ACTOR_STATS_COUNT = ARRAYSIZE(MDScripts::ACTOR_STATS);

// Keyed by tile number rather than object number
const GameScripts::ObjValue MDScripts::READIABLE_LOCATIONS[] = {
	{ 512, 0 },  // cap
	{ 513, 0 },  // cowboy hat
	{ 514, 0 },  // pith hat
	{ 515, 0 },  // military hat
	{ 516, 0 },  // derby
	{ 517, 1 },  // kerchief
	{ 518, 1 },  // silk scarf
	{ 519, 1 },  // muffler
	{ 1048, 1 },  // martian jewelry
	{ 1049, 1 },  // martian jewelry
	{ 1050, 1 },  // martian jewelry
	{ 1051, 1 },  // martian jewelry
	{ 1052, 1 },  // martian jewelry
	{ 1053, 1 },  // martian jewelry
	{ 1054, 1 },  // martian jewelry
	{ 1055, 1 },  // martian jewelry
	{ 527, 3 },  // bloody saber
	{ 552, 3 },  // derringer
	{ 553, 3 },  // revolver
	{ 560, 3 },  // hatchet
	{ 561, 3 },  // axe
	{ 562, 3 },  // ball-peen hammer
	{ 565, 3 },  // knife
	{ 566, 3 },  // machete
	{ 567, 3 },  // saber
	{ 622, 3 },  // pry bar
	{ 650, 3 },  // weed sprayer
	{ 667, 3 },  // tongs
	{ 1068, 3 },  // martian ritual pod knife
	{ 528, 2 },  // cloth jacket
	{ 529, 2 },  // wool sweater
	{ 530, 2 },  // cape
	{ 531, 2 },  // duster
	{ 532, 2 },  // wool overcoat
	{ 533, 2 },  // sheepskin jacket
	{ 534, 2 },  // arctic parka
	{ 608, 2 },  // electric belt
	{ 630, 3 },  // torch
	{ 631, 3 },  // lit torch
	{ 632, 3 },  // candlestick
	{ 633, 3 },  // lit candlestick
	{ 634, 3 },  // candelabra
	{ 635, 3 },  // lit candelabra
	{ 636, 3 },  // oil lamp
	{ 637, 3 },  // lit oil lamp
	{ 638, 3 },  // lantern
	{ 639, 3 },  // lit lantern
	{ 536, 6 },  // cotton pants
	{ 537, 6 },  // cotton dress
	{ 538, 6 },  // denim jeans
	{ 539, 6 },  // wool pants
	{ 540, 6 },  // wool dress
	{ 541, 6 },  // chaps and jeans
	{ 520, 7 },  // man's shoes
	{ 521, 7 },  // woman's shoes
	{ 522, 7 },  // riding boots
	{ 523, 7 },  // ruby slippers
	{ 524, 7 },  // thigh boots
	{ 525, 7 },  // hip boots
	{ 526, 7 },  // winged shoes
	{ 542, 7 },  // man's shoes
	{ 551, 4 },  // Cupid's bow and arrows
	{ 554, 4 },  // shotgun
	{ 555, 4 },  // rifle
	{ 556, 4 },  // Belgian combine
	{ 557, 4 },  // elephant gun
	{ 558, 4 },  // sling
	{ 559, 4 },  // bow
	{ 563, 4 },  // sledge hammer
	{ 576, 4 },  // pick
	{ 577, 4 },  // shovel
	{ 578, 4 },  // hoe
	{ 579, 4 },  // rake
	{ 580, 4 },  // pitchfork
	{ 581, 4 },  // cultivator
	{ 582, 4 },  // scythe
	{ 583, 4 },  // saw
	{ 717, 4 },  // throw rug
	{ 718, 4 },  // red cape
	{ 1066, 4 },  // heat ray gun
	{ 1067, 4 },  // freeze ray gun
	{ 1093, 4 },  // spray gun
	{ 1095, 4 },  // martian hoe (couldn't be equipped in original)
	{ 1096, 4 },  // martian scythe (couldn't be equipped in original)
	{ 1097, 4 },  // martian pitchfork (couldn't be equipped in original)
	{ 1098, 4 },  // martian rake (couldn't be equipped in original)
	{ 1099, 4 },  // martian shovel (couldn't be equipped in original)
	{ 1188, 4 },  // M60 machine gun
	{ 1206, 4 },  // martian pick (couldn't be equipped in original)
	{ 1897, 4 },  // pool cue
	{ 544, 5 },  // lady's silk gloves
	{ 545, 5 },  // driving gloves
	{ 546, 5 },  // cotton work gloves
	{ 547, 5 },  // work gloves
	{ 548, 5 },  // wool mittens
	{ 549, 5 },  // rubber gloves
	{ 550, 5 }  // welding gloves
};

const uint MDScripts::READIABLE_LOCATIONS_COUNT = ARRAYSIZE(MDScripts::READIABLE_LOCATIONS);

const GameScripts::ObjValue MDScripts::WEAPON_RANGES[] = {
	{ 41, 4 },
	{ 42, 5 },
	{ 43, 4 },
	{ 44, 9 },
	{ 45, 9 },
	{ 46, 9 },
	{ 47, 4 },
	{ 48, 5 },
	{ 240, 6 },
	{ 241, 6 },
	{ 129, 2 },
	{ 261, 2 },
	{ 313, 9 },
	{ 40, 6 },
	{ 366, 3 },
	{ 386, 4 },
	{ 364, 3 },
	{ 384, 2 }
};

const uint MDScripts::WEAPON_RANGES_COUNT = ARRAYSIZE(MDScripts::WEAPON_RANGES);

const GameScripts::ObjValue MDScripts::WEAPON_DAMAGE[] = {
	{ 16, 30 },  // bloody saber
	{ 40, 1 },  // Cupid's bow and arrows (charms)
	{ 41, 15 },  // derringer
	{ 42, 18 },  // revolver
	{ 43, 20 },  // shotgun
	{ 44, 30 },  // rifle
	{ 45, 30 },  // Belgian combine
	{ 46, 45 },  // elephant gun
	{ 47, 8 },  // sling
	{ 48, 12 },  // bow
	{ 49, 15 },  // hatchet
	{ 50, 20 },  // axe
	{ 51, 10 },  // ball-peen hammer
	{ 52, 25 },  // sledge hammer
	{ 54, 10 },  // knife
	{ 55, 20 },  // machete
	{ 56, 25 },  // saber
	{ 65, 15 },  // pick
	{ 66, 8 },  // shovel
	{ 67, 10 },  // hoe
	{ 68, 10 },  // rake
	{ 69, 15 },  // pitchfork
	{ 70, 12 },  // cultivator
	{ 71, 20 },  // scythe
	{ 72, 10 },  // saw
	{ 102, 12 },  // pry bar
	{ 129, 60 },  // weed sprayer -- FIXME: no damage normally. Only effects plants?
	{ 241, 20 },  // heat ray gun
	{ 242, 10 },  // freeze ray gun
	{ 261, 60 },  // spray gun -- FIXME: no damage normally. Only effects plants?
	{ 263, 10 },  // martian hoe (couldn't be equipped in original)
	{ 264, 20 },  // martian scythe (couldn't be equipped in original)
	{ 265, 15 },  // martian pitchfork (couldn't be equipped in original)
	{ 266, 10 },  // martian rake (couldn't be equipped in original)
	{ 267, 8 },  // martian shovel (couldn't be equipped in original)
	{ 313, 254 },  // M60 machine gun (scripted to only attack and kill the big bad)
	{ 327, 15 },  // martian pick (couldn't be equipped in original)
	{ 401, 12 }  // pool cue
};

const uint MDScripts::WEAPON_DAMAGE_COUNT = ARRAYSIZE(MDScripts::WEAPON_DAMAGE);

const GameScripts::ObjValue MDScripts::ARMOUR[] = {
	{ 3, 1 },  // pith helmet
	{ 4, 2 },  // military helmet
	{ 11, 1 },  // riding boots
	{ 13, 2 },  // thigh boots
	{ 14, 3 },  // hip boots
	{ 21, 1 },  // wool overcoat
	{ 22, 2 },  // sheepskin jacket
	{ 23, 2 },  // arctic parka
	{ 27, 1 },  // denim jeans
	{ 30, 3 },  // chaps and jeans
	{ 34, 1 },  // driving gloves
	{ 36, 2 },  // work gloves
	{ 38, 1 },  // rubber gloves
	{ 39, 3 }  // welding gloves
};

const uint MDScripts::ARMOUR_COUNT = ARRAYSIZE(MDScripts::ARMOUR);

const GameScripts::ObjValue MDScripts::CLOTHING_WARMTH[] = {
	{ 1, 1 },  // OBJ_CAP
	{ 2, 2 },  // OBJ_COWBOY_HAT
	{ 3, 3 },  // OBJ_PITH_HELMET
	{ 4, 3 },  // OBJ_MILITARY_HELMET
	{ 5, 2 },  // OBJ_DERBY
	{ 6, 1 },  // OBJ_KERCHIEF
	{ 7, 1 },  // OBJ_SILK_SCARF
	{ 8, 2 },  // OBJ_MUFFLER
	{ 9, 1 },  // OBJ_MANS_SHOES
	{ 10, 1 },  // OBJ_WOMANS_SHOES
	{ 11, 2 },  // OBJ_RIDING_BOOTS
	{ 14, 4 },  // OBJ_HIP_BOOTS
	{ 13, 3 },  // OBJ_THIGH_BOOTS
	{ 17, 2 },  // OBJ_CLOTH_JACKET
	{ 18, 3 },  // OBJ_WOOL_SWEATER
	{ 19, 3 },  // OBJ_CAPE
	{ 20, 4 },  // OBJ_DUSTER
	{ 21, 4 },  // OBJ_WOOL_OVERCOAT
	{ 22, 3 },  // OBJ_SHEEPSKIN_JACKET
	{ 23, 5 },  // OBJ_ARCTIC_PARKA
	{ 90, 12 },  // OBJ_ELECTRIC_BELT
	{ 25, 2 },  // OBJ_COTTON_PANTS
	{ 26, 2 },  // OBJ_COTTON_DRESS
	{ 27, 2 },  // OBJ_DENIM_JEANS
	{ 28, 3 },  // OBJ_WOOL_PANTS
	{ 29, 3 },  // OBJ_WOOL_DRESS
	{ 30, 3 },  // OBJ_CHAPS_AND_JEANS
	{ 33, 1 },  // OBJ_LADYS_SILK_GLOVES
	{ 34, 1 },  // OBJ_DRIVING_GLOVES
	{ 35, 1 },  // OBJ_COTTON_WORK_GLOVES
	{ 36, 2 },  // OBJ_WORK_GLOVES
	{ 37, 3 },  // OBJ_WOOL_MITTENS
	{ 38, 1 },  // OBJ_RUBBER_GLOVES
	{ 39, 3 }  // OBJ_WELDING_GLOVES
};

const uint MDScripts::CLOTHING_WARMTH_COUNT = ARRAYSIZE(MDScripts::CLOTHING_WARMTH);

} // End of namespace MD
} // End of namespace Scripts
} // End of namespace Ultima6
} // End of namespace Ultima
