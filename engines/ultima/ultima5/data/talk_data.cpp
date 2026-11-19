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

#include "ultima/ultima5/data/talk_data.h"

namespace Ultima {
namespace Ultima5 {
namespace Data {

// Words that the conversations use a single byte for
const char *const TALK_WORDS[TALK_WORD_COUNT] = {
	"the", "thou", "of", "to", "and", "that",
	"for", "", "in", "is", "have", "with",
	"thee", "this", "not", "my", "it", "me",
	"but", "dost", "know", "be", "was", "Blackthorn",
	"from", "thy", "one", "", "are", "here",
	"many", "Lord", "am", "we", "they", "he",
	"would", "art", "on", "young", "what", "see",
	"like", "only", "by", "there", "Blackthorn's", "good",
	"been", "", "must", "his", "British", "fine",
	"an", "great", "thee,", "our", "who", "name",
	"heard", "as", "at", "has", "", "through",
	"", "once", "can", "", "him", "",
	"", "", "", "ye", "Shadowlords", "tell",
	"some", "believe", "all", "their", "upon", "even",
	"'tis", "find", "if", "about", "don't", "before",
	"these", "just", "make", "will", "when", "three",
	"Great", "might", "those", "old", "hast", "ask",
	"unto", "wish", "man", "so", "knows", "still",
	"Mantra", "out", "help", "well", "shall", "think",
	"where", "named", "talking", "more", "such", "very",
	"may", "lives", "canst", "which", "since", "need",
	"I've", "work"
};

// Words that anyone can be asked about, from the first of what
// the player types
const char *const TALK_KEYWORDS[TALK_KEYWORD_COUNT] = {
	"NAME", "JOB", "WORK", "BYE", "THANK",
	"FUCK", "SHIT", "DAMN", "DICK", "PRICK",
	"PUSSY", "CUNT", "ASS", "BUTT", "BOOGER",
	"PISS", "JACK OFF", "MASTURBATE", "SUCK", "FART",
	"TITS", "BOOB", "MELONS", "BLOW", "PENIS",
	"BREAST", "CLIT", "BALLS", "SCROTUM", "NUTS",
	"BULLSHIT", "CUM", "CROTCH", "MOTHERFUCKER", "ELECTRONIC ARTS"
};

} // namespace Data
} // namespace Ultima5
} // namespace Ultima
