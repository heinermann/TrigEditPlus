/*
 * Copyright (c) 2014 trgk(phu54321@naver.com)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "../TriggerEditor.h"
#include "../StringUtils/StringCast.h"

struct AIScriptEntry{
	DWORD aidw;
	const char* str;
};

AIScriptEntry AIScriptList[] = {
	{'uCMT', "Terran Custom Level"},
	{'uCMZ', "Zerg Custom Level"},
	{'uCMP', "Protoss Custom Level"},
	{'xCMT', "Terran Expansion Custom Level"},
	{'xCMZ', "Zerg Expansion Custom Level"},
	{'xCMP', "Protoss Expansion Custom Level"},
	{'fOLT', "Terran Campaign Easy"},
	{'DEMT', "Terran Campaign Medium"},
	{'fIHT', "Terran Campaign Difficult"},
	{'PUST', "Terran Campaign Insane"},
	{'ERAT', "Terran Campaign Area Town"},
	{'fOLZ', "Zerg Campaign Easy"},
	{'DEMZ', "Zerg Campaign Medium"},
	{'fIHZ', "Zerg Campaign Difficult"},
	{'PUSZ', "Zerg Campaign Insane"},
	{'ERAZ', "Zerg Campaign Area Town"},
	{'fOLP', "Protoss Campaign Easy"},
	{'DEMP', "Protoss Campaign Medium"},
	{'fIHP', "Protoss Campaign Difficult"},
	{'PUSP', "Protoss Campaign Insane"},
	{'ERAP', "Protoss Campaign Area Town"},
	{'xOLT', "Expansion Terran Campaign Easy"},
	{'xEMT', "Expansion Terran Campaign Medium"},
	{'xIHT', "Expansion Terran Campaign Difficult"},
	{'xUST', "Expansion Terran Campaign Insane"},
	{'xRAT', "Expansion Terran Campaign Area Town"},
	{'xOLZ', "Expansion Zerg Campaign Easy"},
	{'xEMZ', "Expansion Zerg Campaign Medium"},
	{'xIHZ', "Expansion Zerg Campaign Difficult"},
	{'xUSZ', "Expansion Zerg Campaign Insane"},
	{'xRAZ', "Expansion Zerg Campaign Area Town"},
	{'xOLP', "Expansion Protoss Campaign Easy"},
	{'xEMP', "Expansion Protoss Campaign Medium"},
	{'xIHP', "Expansion Protoss Campaign Difficult"},
	{'xUSP', "Expansion Protoss Campaign Insane"},
	{'xRAP', "Expansion Protoss Campaign Area Town"},
	{'ciuS', "Send All Units on Strategic Suicide Missions"},
	{'RiuS', "Send All Units on Random Suicide Missions"},
	{'ucsR', "Switch Computer Player to Rescue Passive"},
	{'0iV+', "Turn ON Shared Vision for Player 1"},
	{'1iV+', "Turn ON Shared Vision for Player 2"},
	{'2iV+', "Turn ON Shared Vision for Player 3"},
	{'3iV+', "Turn ON Shared Vision for Player 4"},
	{'4iV+', "Turn ON Shared Vision for Player 5"},
	{'5iV+', "Turn ON Shared Vision for Player 6"},
	{'6iV+', "Turn ON Shared Vision for Player 7"},
	{'7iV+', "Turn ON Shared Vision for Player 8"},
	{'0iV-', "Turn OFF Shared Vision for Player 1"},
	{'1iV-', "Turn OFF Shared Vision for Player 2"},
	{'2iV-', "Turn OFF Shared Vision for Player 3"},
	{'3iV-', "Turn OFF Shared Vision for Player 4"},
	{'4iV-', "Turn OFF Shared Vision for Player 5"},
	{'5iV-', "Turn OFF Shared Vision for Player 6"},
	{'6iV-', "Turn OFF Shared Vision for Player 7"},
	{'7iV-', "Turn OFF Shared Vision for Player 8"},
	{'eTvM', "Move Dark Templars to Region"},
	{'CrlC', "Clear Previous Combat Data"},
	{'ymnE', "Set Player to Enemy"},
	{'Ally', "Set Player to Ally"},
	{'AulV', "Value This Area Higher"},
	{'kBnE', "Enter Closest Bunker"},
	{'gTtS', "Set Generic Command Target"},
	{'tPtS', "Make These Units Patrol"},
	{'rTnE', "Enter Transport"},
	{'rTxE', "Exit Transport"},
	{'eHuN', "AI Nuke Here"},
	{'eHaH', "AI Harass Here"},
	{'gDYJ', "Set Unit Order To: Junk Yard Dog"},
	{'eHWD', "Disruption Web Here"},
	{'eHeR', "Recall Here"},
	{'3reT', "Terran 3 - Zerg Town"},
	{'5reT', "Terran 5 - Terran Main Town"},
	{'H5eT', "Terran 5 - Terran Harvest Town"},
	{'6reT', "Terran 6 - Air Attack Zerg"},
	{'b6eT', "Terran 6 - Ground Attack Zerg"},
	{'c6eT', "Terran 6 - Zerg Support Town"},
	{'7reT', "Terran 7 - Bottom Zerg Town"},
	{'s7eT', "Terran 7 - Right Zerg Town"},
	{'m7eT', "Terran 7 - Middle Zerg Town"},
	{'8reT', "Terran 8 - Confederate Town"},
	{'L9rT', "Terran 9 - Light Attack"},
	{'H9rT', "Terran 9 - Heavy Attack"},
	{'01eT', "Terran 10 - Confederate Towns"},
	{'z11T', "Terran 11 - Zerg Town"},
	{'a11T', "Terran 11 - Lower Protoss Town"},
	{'b11T', "Terran 11 - Upper Protoss Town"},
	{'N21T', "Terran 12 - Nuke Town"},
	{'P21T', "Terran 12 - Phoenix Town"},
	{'T21T', "Terran 12 - Tank Town"},
	{'1DET', "Terran 1 - Electronic Distribution"},
	{'2DET', "Terran 2 - Electronic Distribution"},
	{'3DET', "Terran 3 - Electronic Distribution"},
	{'1WST', "Terran 1 - Shareware"},
	{'2WST', "Terran 2 - Shareware"},
	{'3WST', "Terran 3 - Shareware"},
	{'4WST', "Terran 4 - Shareware"},
	{'5WST', "Terran 5 - Shareware"},
	{'1reZ', "Zerg 1 - Terran Town"},
	{'2reZ', "Zerg 2 - Protoss Town"},
	{'3reZ', "Zerg 3 - Terran Town"},
	{'4reZ', "Zerg 4 - Right Terran Town"},
	{'S4eZ', "Zerg 4 - Lower Terran Town"},
	{'6reZ', "Zerg 6 - Protoss Town"},
	{'a7rZ', "Zerg 7 - Air Town"},
	{'g7rZ', "Zerg 7 - Ground Town"},
	{'s7rZ', "Zerg 7 - Support Town"},
	{'8reZ', "Zerg 8 - Scout Town"},
	{'T8eZ', "Zerg 8 - Templar Town"},
	{'9reZ', "Zerg 9 - Teal Protoss"},
	{'yl9Z', "Zerg 9 - Left Yellow Protoss"},
	{'yr9Z', "Zerg 9 - Right Yellow Protoss"},
	{'ol9Z', "Zerg 9 - Left Orange Protoss"},
	{'or9Z', "Zerg 9 - Right Orange Protoss"},
	{'a01Z', "Zerg 10 - Left Teal (Attack"},
	{'b01Z', "Zerg 10 - Right Teal (Support"},
	{'c01Z', "Zerg 10 - Left Yellow (Support"},
	{'d01Z', "Zerg 10 - Right Yellow (Attack"},
	{'e01Z', "Zerg 10 - Red Protoss"},
	{'1orP', "Protoss 1 - Zerg Town"},
	{'2orP', "Protoss 2 - Zerg Town"},
	{'R3rP', "Protoss 3 - Air Zerg Town"},
	{'G3rP', "Protoss 3 - Ground Zerg Town"},
	{'4orP', "Protoss 4 - Zerg Town"},
	{'I5rP', "Protoss 5 - Zerg Town Island"},
	{'B5rP', "Protoss 5 - Zerg Town Base"},
	{'7orP', "Protoss 7 - Left Protoss Town"},
	{'B7rP', "Protoss 7 - Right Protoss Town"},
	{'S7rP', "Protoss 7 - Shrine Protoss"},
	{'8orP', "Protoss 8 - Left Protoss Town"},
	{'B8rP', "Protoss 8 - Right Protoss Town"},
	{'D8rP', "Protoss 8 - Protoss Defenders"},
	{'9orP', "Protoss 9 - Ground Zerg"},
	{'W9rP', "Protoss 9 - Air Zerg"},
	{'Y9rP', "Protoss 9 - Spell Zerg"},
	{'01rP', "Protoss 10 - Mini-Towns"},
	{'C01P', "Protoss 10 - Mini-Town Master"},
	{'o01P', "Protoss 10 - Overmind Defenders"},
	{'A1BP', "Brood Wars Protoss 1 - Town A"},
	{'B1BP', "Brood Wars Protoss 1 - Town B"},
	{'C1BP', "Brood Wars Protoss 1 - Town C"},
	{'D1BP', "Brood Wars Protoss 1 - Town D"},
	{'E1BP', "Brood Wars Protoss 1 - Town E"},
	{'F1BP', "Brood Wars Protoss 1 - Town F"},
	{'A2BP', "Brood Wars Protoss 2 - Town A"},
	{'B2BP', "Brood Wars Protoss 2 - Town B"},
	{'C2BP', "Brood Wars Protoss 2 - Town C"},
	{'D2BP', "Brood Wars Protoss 2 - Town D"},
	{'E2BP', "Brood Wars Protoss 2 - Town E"},
	{'F2BP', "Brood Wars Protoss 2 - Town F"},
	{'A3BP', "Brood Wars Protoss 3 - Town A"},
	{'B3BP', "Brood Wars Protoss 3 - Town B"},
	{'C3BP', "Brood Wars Protoss 3 - Town C"},
	{'D3BP', "Brood Wars Protoss 3 - Town D"},
	{'E3BP', "Brood Wars Protoss 3 - Town E"},
	{'F3BP', "Brood Wars Protoss 3 - Town F"},
	{'A4BP', "Brood Wars Protoss 4 - Town A"},
	{'B4BP', "Brood Wars Protoss 4 - Town B"},
	{'C4BP', "Brood Wars Protoss 4 - Town C"},
	{'D4BP', "Brood Wars Protoss 4 - Town D"},
	{'E4BP', "Brood Wars Protoss 4 - Town E"},
	{'F4BP', "Brood Wars Protoss 4 - Town F"},
	{'A5BP', "Brood Wars Protoss 5 - Town A"},
	{'B5BP', "Brood Wars Protoss 5 - Town B"},
	{'C5BP', "Brood Wars Protoss 5 - Town C"},
	{'D5BP', "Brood Wars Protoss 5 - Town D"},
	{'E5BP', "Brood Wars Protoss 5 - Town E"},
	{'F5BP', "Brood Wars Protoss 5 - Town F"},
	{'A6BP', "Brood Wars Protoss 6 - Town A"},
	{'B6BP', "Brood Wars Protoss 6 - Town B"},
	{'C6BP', "Brood Wars Protoss 6 - Town C"},
	{'D6BP', "Brood Wars Protoss 6 - Town D"},
	{'E6BP', "Brood Wars Protoss 6 - Town E"},
	{'F6BP', "Brood Wars Protoss 6 - Town F"},
	{'A7BP', "Brood Wars Protoss 7 - Town A"},
	{'B7BP', "Brood Wars Protoss 7 - Town B"},
	{'C7BP', "Brood Wars Protoss 7 - Town C"},
	{'D7BP', "Brood Wars Protoss 7 - Town D"},
	{'E7BP', "Brood Wars Protoss 7 - Town E"},
	{'F7BP', "Brood Wars Protoss 7 - Town F"},
	{'A8BP', "Brood Wars Protoss 8 - Town A"},
	{'B8BP', "Brood Wars Protoss 8 - Town B"},
	{'C8BP', "Brood Wars Protoss 8 - Town C"},
	{'D8BP', "Brood Wars Protoss 8 - Town D"},
	{'E8BP', "Brood Wars Protoss 8 - Town E"},
	{'F8BP', "Brood Wars Protoss 8 - Town F"},
	{'A1BT', "Brood Wars Terran 1 - Town A"},
	{'B1BT', "Brood Wars Terran 1 - Town B"},
	{'C1BT', "Brood Wars Terran 1 - Town C"},
	{'D1BT', "Brood Wars Terran 1 - Town D"},
	{'E1BT', "Brood Wars Terran 1 - Town E"},
	{'F1BT', "Brood Wars Terran 1 - Town F"},
	{'A2BT', "Brood Wars Terran 2 - Town A"},
	{'B2BT', "Brood Wars Terran 2 - Town B"},
	{'C2BT', "Brood Wars Terran 2 - Town C"},
	{'D2BT', "Brood Wars Terran 2 - Town D"},
	{'E2BT', "Brood Wars Terran 2 - Town E"},
	{'F2BT', "Brood Wars Terran 2 - Town F"},
	{'A3BT', "Brood Wars Terran 3 - Town A"},
	{'B3BT', "Brood Wars Terran 3 - Town B"},
	{'C3BT', "Brood Wars Terran 3 - Town C"},
	{'D3BT', "Brood Wars Terran 3 - Town D"},
	{'E3BT', "Brood Wars Terran 3 - Town E"},
	{'F3BT', "Brood Wars Terran 3 - Town F"},
	{'A4BT', "Brood Wars Terran 4 - Town A"},
	{'B4BT', "Brood Wars Terran 4 - Town B"},
	{'C4BT', "Brood Wars Terran 4 - Town C"},
	{'D4BT', "Brood Wars Terran 4 - Town D"},
	{'E4BT', "Brood Wars Terran 4 - Town E"},
	{'F4BT', "Brood Wars Terran 4 - Town F"},
	{'A5BT', "Brood Wars Terran 5 - Town A"},
	{'B5BT', "Brood Wars Terran 5 - Town B"},
	{'C5BT', "Brood Wars Terran 5 - Town C"},
	{'D5BT', "Brood Wars Terran 5 - Town D"},
	{'E5BT', "Brood Wars Terran 5 - Town E"},
	{'F5BT', "Brood Wars Terran 5 - Town F"},
	{'A6BT', "Brood Wars Terran 6 - Town A"},
	{'B6BT', "Brood Wars Terran 6 - Town B"},
	{'C6BT', "Brood Wars Terran 6 - Town C"},
	{'D6BT', "Brood Wars Terran 6 - Town D"},
	{'E6BT', "Brood Wars Terran 6 - Town E"},
	{'F6BT', "Brood Wars Terran 6 - Town F"},
	{'A7BT', "Brood Wars Terran 7 - Town A"},
	{'B7BT', "Brood Wars Terran 7 - Town B"},
	{'C7BT', "Brood Wars Terran 7 - Town C"},
	{'D7BT', "Brood Wars Terran 7 - Town D"},
	{'E7BT', "Brood Wars Terran 7 - Town E"},
	{'F7BT', "Brood Wars Terran 7 - Town F"},
	{'A8BT', "Brood Wars Terran 8 - Town A"},
	{'B8BT', "Brood Wars Terran 8 - Town B"},
	{'C8BT', "Brood Wars Terran 8 - Town C"},
	{'D8BT', "Brood Wars Terran 8 - Town D"},
	{'E8BT', "Brood Wars Terran 8 - Town E"},
	{'F8BT', "Brood Wars Terran 8 - Town F"},
	{'A1BZ', "Brood Wars Zerg 1 - Town A"},
	{'B1BZ', "Brood Wars Zerg 1 - Town B"},
	{'C1BZ', "Brood Wars Zerg 1 - Town C"},
	{'D1BZ', "Brood Wars Zerg 1 - Town D"},
	{'E1BZ', "Brood Wars Zerg 1 - Town E"},
	{'F1BZ', "Brood Wars Zerg 1 - Town F"},
	{'A2BZ', "Brood Wars Zerg 2 - Town A"},
	{'B2BZ', "Brood Wars Zerg 2 - Town B"},
	{'C2BZ', "Brood Wars Zerg 2 - Town C"},
	{'D2BZ', "Brood Wars Zerg 2 - Town D"},
	{'E2BZ', "Brood Wars Zerg 2 - Town E"},
	{'F2BZ', "Brood Wars Zerg 2 - Town F"},
	{'A3BZ', "Brood Wars Zerg 3 - Town A"},
	{'B3BZ', "Brood Wars Zerg 3 - Town B"},
	{'C3BZ', "Brood Wars Zerg 3 - Town C"},
	{'D3BZ', "Brood Wars Zerg 3 - Town D"},
	{'E3BZ', "Brood Wars Zerg 3 - Town E"},
	{'F3BZ', "Brood Wars Zerg 3 - Town F"},
	{'A4BZ', "Brood Wars Zerg 4 - Town A"},
	{'B4BZ', "Brood Wars Zerg 4 - Town B"},
	{'C4BZ', "Brood Wars Zerg 4 - Town C"},
	{'D4BZ', "Brood Wars Zerg 4 - Town D"},
	{'E4BZ', "Brood Wars Zerg 4 - Town E"},
	{'F4BZ', "Brood Wars Zerg 4 - Town F"},
	{'A5BZ', "Brood Wars Zerg 5 - Town A"},
	{'B5BZ', "Brood Wars Zerg 5 - Town B"},
	{'C5BZ', "Brood Wars Zerg 5 - Town C"},
	{'D5BZ', "Brood Wars Zerg 5 - Town D"},
	{'E5BZ', "Brood Wars Zerg 5 - Town E"},
	{'F5BZ', "Brood Wars Zerg 5 - Town F"},
	{'A6BZ', "Brood Wars Zerg 6 - Town A"},
	{'B6BZ', "Brood Wars Zerg 6 - Town B"},
	{'C6BZ', "Brood Wars Zerg 6 - Town C"},
	{'D6BZ', "Brood Wars Zerg 6 - Town D"},
	{'E6BZ', "Brood Wars Zerg 6 - Town E"},
	{'F6BZ', "Brood Wars Zerg 6 - Town F"},
	{'A7BZ', "Brood Wars Zerg 7 - Town A"},
	{'B7BZ', "Brood Wars Zerg 7 - Town B"},
	{'C7BZ', "Brood Wars Zerg 7 - Town C"},
	{'D7BZ', "Brood Wars Zerg 7 - Town D"},
	{'E7BZ', "Brood Wars Zerg 7 - Town E"},
	{'F7BZ', "Brood Wars Zerg 7 - Town F"},
	{'A8BZ', "Brood Wars Zerg 8 - Town A"},
	{'B8BZ', "Brood Wars Zerg 8 - Town B"},
	{'C8BZ', "Brood Wars Zerg 8 - Town C"},
	{'D8BZ', "Brood Wars Zerg 8 - Town D"},
	{'E8BZ', "Brood Wars Zerg 8 - Town E"},
	{'F8BZ', "Brood Wars Zerg 8 - Town F"},
	{'A9BZ', "Brood Wars Zerg 9 - Town A"},
	{'B9BZ', "Brood Wars Zerg 9 - Town B"},
	{'C9BZ', "Brood Wars Zerg 9 - Town C"},
	{'D9BZ', "Brood Wars Zerg 9 - Town D"},
	{'E9BZ', "Brood Wars Zerg 9 - Town E"},
	{'F9BZ', "Brood Wars Zerg 9 - Town F"},
	{'A0BZ', "Brood Wars Zerg 10 - Town A"},
	{'B0BZ', "Brood Wars Zerg 10 - Town B"},
	{'C0BZ', "Brood Wars Zerg 10 - Town C"},
	{'D0BZ', "Brood Wars Zerg 10 - Town D"},
	{'E0BZ', "Brood Wars Zerg 10 - Town E"},
	{'F0BZ', "Brood Wars Zerg 10 - Town F"},
	{0, NULL}
};

std::string TriggerEditor::DecodeAIScript(int value) const {
	for(auto it : AIScriptList) {
		if(it.aidw == value) return Raw2CString(it.str);
	}

	return Int2String(value);
}
