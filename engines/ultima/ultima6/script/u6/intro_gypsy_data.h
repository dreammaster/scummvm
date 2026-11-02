// Generated from the Lua intro script

static const uint8 GYPSY_A_LOOKUP[28] = {
	0, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 2, 2, 2,
	2, 2, 3, 3, 3, 3, 4, 4,
	4, 5, 5, 6
};
static const uint8 GYPSY_B_LOOKUP[28] = {
	1, 2, 3, 4, 5, 6, 7, 2,
	3, 4, 5, 6, 7, 3, 4, 5,
	6, 7, 4, 5, 6, 7, 5, 6,
	7, 6, 7, 7
};
static const int16 GYPSY_QUESTIONS[64] = {
	-1, 105, 106, 107, 108, 109, 110, 111,
	105, -1, 112, 113, 114, 115, 116, 117,
	106, 112, -1, 118, 119, 120, 121, 122,
	107, 113, 118, -1, 123, 124, 125, 126,
	108, 114, 119, 123, -1, 127, 128, 129,
	109, 115, 120, 124, 127, -1, 130, 131,
	110, 116, 121, 125, 128, 130, -1, 132,
	111, 117, 122, 126, 129, 131, 132, -1
};
static const uint8 GYPSY_POUR_IMG[24] = {
	43, 67, 86, 107, 36, 116, 25, 44,
	44, 68, 87, 108, 35, 115, 24, 43,
	46, 69, 88, 109, 34, 114, 23, 42
};
static const uint8 GYPSY_POUR_Y[3] = {
	50, 55, 64
};
static const uint8 GYPSY_POUR_X[24] = {
	146, 146, 146, 146, 169, 169, 169, 169,
	145, 145, 145, 145, 169, 169, 169, 169,
	148, 148, 148, 148, 170, 170, 170, 170
};
static const uint8 GYPSY_VIAL_LIQUID[96] = {
	58, 79, 100, 124, 40, 126, 32, 52,
	54, 75, 96, 118, 38, 120, 28, 48,
	54, 75, 96, 118, 38, 120, 28, 48,
	60, 81, 102, 127, 20, 95, 34, 54,
	56, 77, 98, 121, 39, 123, 30, 50,
	56, 77, 98, 121, 39, 123, 30, 50,
	64, 83, 104, 129, 41, 131, 36, 56,
	64, 83, 104, 129, 41, 131, 36, 56,
	64, 83, 104, 129, 41, 131, 36, 56,
	0, 3, 6, 9, 22, 25, 28, 31,
	1, 4, 7, 10, 23, 26, 29, 32,
	2, 5, 8, 11, 24, 27, 30, 33
};
static const uint8 GYPSY_HAND_Y[12] = {
	27, 19, 19, 22, 14, 14, 14, 14,
	14, 11, 11, 11
};
static const uint8 GYPSY_HAND_X[96] = {
	4, 4, 4, 4, 20, 20, 20, 20,
	4, 4, 4, 4, 19, 19, 19, 19,
	4, 4, 4, 4, 19, 19, 19, 19,
	4, 4, 4, 4, 23, 23, 23, 23,
	3, 3, 3, 3, 23, 23, 23, 23,
	3, 3, 3, 3, 23, 23, 23, 23,
	3, 3, 3, 3, 27, 27, 27, 27,
	3, 3, 3, 3, 27, 27, 27, 27,
	3, 3, 3, 3, 27, 27, 27, 27,
	3, 3, 3, 3, 3, 3, 3, 3,
	3, 3, 3, 3, 3, 3, 3, 3,
	3, 3, 3, 3, 3, 3, 3, 3
};
static const char *const GYPSY_QUESTION_TEXT[28] = {
	"\"Entrusted to deliver an uncounted purse of gold, thou dost meet a poor beggar. Dost thou A) deliver the gold Honestly, knowing the trust in thee was well-placed; or B) show Compassion and give the beggar a coin, knowing it won't be missed?\177",
	"\"Thou has been prohibited by thy absent Lord from joining thy friends in a close-pitched battle. Dost thou A) refrain, so thou may Honestly claim obedience; or B) show Valor and aid thy comrades, knowing thou may deny it later?\177",
	"\"A merchant owes thy friend money, now long past due. Thou dost see the same merchant drop a purse of gold. Dost thou A) Honestly return the purse intact; or B) Justly give thy friend a portion of the gold first?\177",
	"\"Thee and thy friend are valiant but penniless warriors. Thou both set forth to slay a mighty dragon. Thy friend dost think he slew it, but the killing blow was thine. Dost thou A) Honestly claim the reward; or B) Sacrifice the gold for the sake of thy friendship?\177",
	"\"Thou art sworn to protect thy Lord at any cost, yet thou knowest he hath committed a crime. Authorities ask thee of the affair. Dost thou A) Break thine oath by Honestly speaking; or B) uphold Honor by silently keeping thine oath?\177",
	"\"Thy friend doth seek admittance to thy spiritual order. Thou art asked to vouch for his purity of spirit, of which thou art uncertain. Dost thou A) Honestly express thy doubt; or B) Vouch for him, hoping for his Spiritual improvement?\177",
	"\"Thy Lord doth mistakenly believe he slew a dragon. Thou hast proof that thy lance felled the beast. When asked, dost thou A) Honestly claim the kill; or B) Humbly permit thy Lord his belief?\177",
	"\"Thou dost manage to disarm thy mortal enemy in a duel. He is at thy mercy. Dost thou A) Show Compassion by permitting him to yield; or B) Slay him, as expected of a Valiant duelist?\177",
	"\"After twenty years thou hast found the slayer of thy best friends. The villain proves to be a man who provides the sole support for a young girl. Dost thou A) Spare him in Compassion for the child; or B) Slay him in the name of Justice?\177",
	"\"Thee and thy friends hath been routed and ordered to retreat. In defiance of thy orders, dost thou A) Stop in Compassion to aid a wounded comrade; or B) Sacrifice thyself to slow the pursuing enemy so that others might escape?\177",
	"\"Thou art sworn to uphold a Lord who doth participate in the forbidden torture of prisoners. Each night their cries of pain reach thee. Dost thou A) Show Compassion by reporting the deeds; or B) Honor thy oath and ignore the deeds?\177",
	"\"Thou hast been taught to preserve all life as sacred. A man lies fatally stung by a venomous serpent. He prays for a merciful death. Dost thou A) Show Compassion and end his pain; or B) heed thy Spiritual beliefs and turn away?\177",
	"\"The Captain of the King's Guard asks that one among thee visit a hospital to cheer the children with tales of valiant personal deeds. Dost thou A) Show thy Compassion and play the braggart; or B) Humbly let another go?\177",
	"\"Thou hast been sent to secure a needed treaty with a distant Lord. Thy host is agreeable to thy proposal but insults thy country at dinner. Dost thou A) Valiantly bear the slurs; or B) Justly rise and demand an apology?\177",
	"\"A burly knight accosts thee and demands thy food. Dost thou A) Valiantly refuse and engage the knight, or B) Sacrifice thy rations unto the hungry knight?\177",
	"\"During battle thou art ordered to guard thy commander's empty tent. The battle goes poorly and thou dost yearn to aid thy fellows. Dost thou A) Valiantly enter the battle; or B) Honor thy post as guard?\177",
	"\"A local bully pushes for a fight. Dost thou A) Valiantly trounce the rogue; or B) Decline, knowing in thy Spirit that no lasting good will come of it?\177",
	"\"Although a simple fisherman, thou art also a skillful swordsman. Thy Lord dost seek to assemble a peacetime guard. Dost thou A) Answer the call, so that all may witness thy Valor; or B) Humbly decline the offer to join thy Lord's largely ceremonial knighthood?\177",
	"\"During a pitched battle, thou dost see a fellow desert his post, endangering many. As he flees, he is set upon by several enemies. Dost thou A) Justly let him fight alone; or B) Risk the Sacrifice of thine own life to aid him?\177",
	"\"Thou hast sworn to do thy Lord's bidding in all. He covets a piece of land and orders its owner removed. Dost thou A) Serve Justice, refusing to act, thus being disgraced; or B) Honor thine oath and evict the landowner?\177",
	"\"Thou dost believe that virtue resides in all people. Thou dost see a rogue steal from thy Lord. Dost thou A) Call him to Justice; or B) Personally try to sway him back to the Spiritual path of good?\177",
	"\"Unwitnessed, thou hast slain a mighty dragon in defense of thy life. A poor warrior claims the offered reward. Dost thou A) Justly step forward to claim the bounty; or B) Humbly go about thy life, secure in thy self-esteem?\177",
	"\"Thou art a bounty hunter sworn to return an alleged murderer. After his capture, thou dost come to believe him innocent. Dost thou A) Sacrifice thy sizable bounty for thy belief; or B) Honor thine oath to return him as promised?\177",
	"\"Thou hast spent thy life in charitable and righteous work. Thine uncle the innkeeper lies ill and asks thee to take over his tavern. Dost thou A) Sacrifice thy life of purity to aid thy kin; or B) Decline and follow the call of Spirituality?\177",
	"\"Thou art an elderly, wealthy merchant. Thy end is near. Dost thou A) Sacrifice all thy wealth to feed hundreds of starving children, receiving public adulation; or B) Humbly live out thy life, willing thy fortune to thy heirs?\177",
	"\"In thy youth thou didst pledge to marry thy sweetheart. Now thou art on a sacred quest in distant lands. Thy sweetheart doth ask thee to keep thy vow. Dost thou A) Honor thy pledge to wed; or B) Follow thy Spiritual crusade?\177",
	"\"Though thou art but a peasant shepherd, thou art discovered to be the sole descendant of a noble family long thought extinct. Dost thou A) Honorably take up the arms of thy ancestors; or B) Humbly resume thy life of simplicity and peace?\177",
	"\"Thy parents wish thee to become an apprentice. Two positions are available. Dost thou A) Become an acolyte in a worthy Spiritual order; or B) Become an assistant to a humble village cobbler?\177"
};
