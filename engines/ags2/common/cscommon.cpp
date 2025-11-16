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

#include "ags2/common/cscomp.h"

namespace AGS2 {

int ccCompOptions = SCOPT_LEFTTORIGHT;
int currentline = 0;
int ccError = 0;
int ccErrorLine = 0;
char ccErrorString[400];
char ccErrorCallStack[400];
bool ccErrorIsUserError = false;
const char *ccCurScriptName = "";

void cc_error(char *descr, ...) {
	ccErrorCallStack[0] = 0;
	ccErrorIsUserError = false;
	if (descr[0] == '!')
	{
		ccErrorIsUserError = true;
		descr++;
	}

	va_list ap;

	va_start(ap, descr);
	Common::String displbuf = Common::String::vformat(descr, ap);
	va_end(ap);

	if (currentline > 0) {

		if (ccGetCurrentInstance() == NULL) {
			Common::sprintf_s(ccErrorString, "Error (line %d): %s", currentline, displbuf.c_str());
		} else {
			Common::sprintf_s(ccErrorString, "Error: %s\n", displbuf.c_str());
			ccGetCallStack(ccGetCurrentInstance(), ccErrorCallStack, 5);
		}
	} else {
		Common::sprintf_s(ccErrorString, "Runtime error: %s", displbuf.c_str());
	}

	ccError = 1;
	ccErrorLine = currentline;
}

void ccSetOption(int optbit, int onoroff) {
	if (onoroff)
		ccCompOptions |= optbit;
	else
		ccCompOptions &= ~optbit;
}

int ccGetOption(int optbit) {
	if (ccCompOptions & optbit)
		return 1;

	return 0;
}

const char *ccGetSectionNameAtOffs(ccScript *scri, long offs) {

	int i;
	for (i = 0; i < scri->numSections; i++) {
		if (scri->sectionOffsets[i] < offs)
			continue;
		break;
	}

	// if no sections in script, return unknown
	if (i == 0)
		return "(unknown section)";

	return scri->sectionNames[i - 1];
}

void ccGetCallStack(ccInstance *inst, char *buffer, int maxLines) {

	if (inst == NULL) {
		// not in a script, no call stack
		buffer[0] = 0;
		return;
	}

	Common::sprintf_s(buffer, 256, "in \"%s\", line %d\n", ccGetSectionNameAtOffs(inst->runningInst->instanceof, inst->pc), inst->line_number);

	char lineBuffer[300];
	int linesDone = 0;
	for (int j = inst->callStackSize - 1; (j >= 0) && (linesDone < maxLines); j--, linesDone++) {
		Common::sprintf_s(lineBuffer, "from \"%s\", line %d\n", ccGetSectionNameAtOffs(inst->callStackCodeInst[j]->instanceof, inst->callStackAddr[j]), inst->callStackLineNumber[j]);
		Common::strcat_s(buffer, 256, lineBuffer);
		if (linesDone == maxLines - 1)
			Common::strcat_s(buffer, 256, "(and more...)\n");
	}

}

void ccFreeScript(ccScript *ccs) {
	if (ccs->globaldata != NULL)
		free(ccs->globaldata);

	if (ccs->code != NULL)
		free(ccs->code);

	if (ccs->strings != NULL)
		free(ccs->strings);

	if (ccs->fixups != NULL && ccs->numfixups > 0)
		free(ccs->fixups);

	if (ccs->fixuptypes != NULL && ccs->numfixups > 0)
		free(ccs->fixuptypes);

	ccs->globaldata = NULL;
	ccs->code = NULL;
	ccs->strings = NULL;
	ccs->fixups = NULL;
	ccs->fixuptypes = NULL;

	int aa;
	for (aa = 0; aa < ccs->numimports; aa++) {
		if (ccs->imports[aa] != NULL)
			free(ccs->imports[aa]);
	}

	for (aa = 0; aa < ccs->numexports; aa++)
		free(ccs->exports[aa]);

	for (aa = 0; aa < ccs->numSections; aa++)
		free(ccs->sectionNames[aa]);

	if (ccs->sectionNames != NULL)
	{
		free(ccs->sectionNames);
		free(ccs->sectionOffsets);
		ccs->sectionNames = NULL;
		ccs->sectionOffsets = NULL;
	}


	if (ccs->imports != NULL)
	{
		free(ccs->imports);
		free(ccs->exports);
		free(ccs->export_addr);
		ccs->imports = NULL;
		ccs->exports = NULL;
		ccs->export_addr = NULL;
	}
	ccs->numimports = 0;
	ccs->numexports = 0;
	ccs->numSections = 0;
}

void fputstring(const char *sss, Common::WriteStream *ddd) {
	int b = 0;
	while (sss[b] != 0) {
		ddd->writeByte(sss[b]);
		b++;
	}
	ddd->writeByte(0);
}

void fgetstring_limit(char *sss, Common::ReadStream *ddd, int bufsize) {
	int b = -1;
	do {
		if (b < bufsize - 1)
			b++;
		sss[b] = ddd->readByte();
		if (ddd->eos())
			return;
	} while (sss[b] != 0);
}

void fgetstring(char *sss, Common::ReadStream *ddd) {
	fgetstring_limit(sss, ddd, 50000000);
}

// *** TREEMAP CODE **** //

ICompareStrings ccCompareStringsNormal;

ccTreeMap::ccTreeMap() {
	left = NULL;
	right = NULL;
	text = NULL;
	value = -1;
}

ccTreeMap *ccTreeMap::findNode(const char *key, ICompareStrings *comparer) {
	if (text == NULL) {/*
	  // if we are removing items, this entry might have been
	  // removed, but we still need to check left and right
	  ccTreeMap *tnode;
	  if (left != NULL)
		tnode = left->findNode(key, comparer);
	  if ((right != NULL) && (tnode == NULL))
		tnode = right->findNode(key, comparer);
	  return tnode;*/
		return NULL;
	}

	int cmpv = comparer->compare(key, text);
	if (cmpv == 0)
		return this;

	if (cmpv < 0) {
		if (left == NULL)
			return NULL;
		return left->findNode(key, comparer);
	} else {
		if (right == NULL)
			return NULL;
		return right->findNode(key, comparer);
	}
}

int ccTreeMap::findValue(const char *key, ICompareStrings *comparer) {
	ccTreeMap *result = findNode(key, comparer);
	if (result == NULL)
		return -1;
	return result->value;
}

int ccTreeMap::findValue(const char *key) {
	return findValue(key, &ccCompareStringsNormal);
}

void ccTreeMap::Clone(ccTreeMap *node) {
	this->text = node->text;
	this->left = node->left;
	this->right = node->right;
	this->value = node->value;
}

void ccTreeMap::removeNode() {

	// clean up any empty nodes
	if ((left != NULL) && (left->text == NULL)) {
		delete left;
		left = NULL;
	}
	if ((right != NULL) && (right->text == NULL)) {
		delete right;
		right = NULL;
	}

	// delete this node
	if ((left == NULL) && (right == NULL)) {
		// leaf node -- remove it
		text = NULL;
		value = -1;
		return;
	}

	if (left == NULL) {
		// has a right child only -- just move the child up into it
		ccTreeMap *oldNode = right;
		Clone(oldNode);
		oldNode->destroyNonRecursive();
		return;
	}

	if (right == NULL) {
		// has a left child only -- just move the child up into it
		ccTreeMap *oldNode = left;
		Clone(oldNode);
		oldNode->destroyNonRecursive();
		return;
	}

	// at this point, the node to be deleted has both a left
	// and right child
	// locate the rightmost descendant of the left child of the node
	ccTreeMap *searching = left;
	while ((searching->right != NULL) && (searching->right->text != NULL))
		searching = searching->right;

	// pull up the node we found into the deleted one's position
	text = searching->text;
	value = searching->value;

	// remove it (dealing with any left child tree appropriately)
	searching->removeNode();
}

void ccTreeMap::removeEntry(const char *key) {

	ccTreeMap *node = findNode(key, &ccCompareStringsNormal);
	if (node == NULL)
		return;

	node->removeNode();
}

void ccTreeMap::addEntry(const char *ntx, int p_value) {
	if ((ntx == NULL) || (ntx[0] == 0))
		// don't add if it's an empty string or if it's already here
		return;

	if (text == NULL) {
		text = ntx;
		value = p_value;
		return;
	}

	int cmpval = ccCompareStringsNormal.compare(ntx, text);
	if (cmpval == 0) {
		value = p_value;
	} else if (cmpval < 0) {
		// Earlier in alphabet, add to left
		if (left == NULL)
			left = new ccTreeMap();

		left->addEntry(ntx, p_value);
	} else if (cmpval > 0) {
		// Later in alphabet, add to right
		if (right == NULL)
			right = new ccTreeMap();

		right->addEntry(ntx, p_value);
	}
}

void ccTreeMap::destroyNonRecursive() {
	left = NULL;
	right = NULL;
	text = NULL;
	delete this;
}

void ccTreeMap::clear() {
	if (left) {
		left->clear();
		delete left;
	}
	if (right) {
		right->clear();
		delete right;
	}
	left = NULL;
	right = NULL;
	text = NULL;
	value = -1;
}

ccTreeMap::~ccTreeMap() {
	clear();
}

} // namespace AGS2
