#include "../TriggerEditor.h"
#include "../TriggerEncDec.h"
#include "../StringUtils/StringCast.h"

// Decode part

std::string TriggerEditor::DecodeTriggers(CChunkData* Triggers) const {
	// Some notice
	StringBuffer decode_out;
	decode_out << "-- This text is generated by TrigEditPlus\r\n\r\n";

	// Decode triggers
	std::vector<Trig> trigv;
	{
		size_t trign = Triggers->ChunkSize / 2400;
		Trig* trigdata = (Trig*)Triggers->ChunkData;
		for(size_t i = 0 ; i < trign ; i++) {
			DecodeTrigger(decode_out, *trigdata++);
		}
	}

	std::string ret = decode_out.str();
	decode_out.clear();
	return ret;
}


void TriggerEditor::DecodeTrigger(StringBuffer& buf, const Trig& content) const {
	size_t i;

	// If trigger executed by no players, then special-decode that trigger.
	for(i = 0 ; i < 27 ; i++) {
		if(content.effplayer[i]) break;
	}
	if(i == 27) {
		// TODO: Put special handler here.
		return;
	}



	// 1. Write trigger header
	
	// print comment & header of trigger
	bool comment_singleline = false;
	for(i = 0 ; i < 64 ; i++) {
		if(content.act[i].acttype == 0) { // no comment
			buf << "Trigger {\r\n";
			break;
		}

		else if(content.act[i].acttype == COMMENT) {
			int strid = content.act[i].strid;
			const char* rawcomment0 = StringTable_GetString(_editordata->EngineData->MapStrings, strid);
			if(rawcomment0 == NULL) break;
			if(strchr(rawcomment0, '\n') == NULL) comment_singleline = true;
			else comment_singleline = false;

			// Decode string to lua comments
			std::string rawcomment = rawcomment0;

			char *comment = (char*)alloca(rawcomment.size() * 5 + 4);
			strcpy(comment, "-- ");
			char *p = comment + 3;

			for(char ch : rawcomment) {
				/**/ if(ch == '\t') *p++ = '\t';
				else if(ch == '\r');
				else if(ch == '\n') {
					*p++ = '\r';
					*p++ = '\n';
					*p++ = '-';
					*p++ = '-';
					*p++ = ' ';
				}

				else if(1 <= ch && ch <= 31) continue;
				else *p++ = ch;
			}

			*p = '\0';
			
			if(comment_singleline) {
				buf << "Trigger { " << comment << "\r\n";
			}
			else {
				buf << comment << "\n\nTrigger {\r\n";
			}
			break;
		}
	}
	if(i == 64) buf << "Trigger {\r\n"; // no comment, 64 actions all used.


	// 2. Write player fields
	buf << "\tplayers = {";

	bool firstplayer = true;
	for(int i = 0 ; i < 27 ; i++) {
		if(content.effplayer[i]) {
			if(!firstplayer) buf << ", ";
			else firstplayer = false;
			buf << DecodePlayer(i);
		}
	}
	buf << "},\n";

	// 2. Write conditions.
	if(content.cond[0].condtype != 0) { // There is at least one condition.
		buf << "\tconditions = {\r\n";

		for(i = 0 ; i < 16 ; i++) {
			if(content.cond[i].condtype == 0) break;
			DecodeCondition(buf, content.cond[i]);
		}

		buf << "\t},\r\n";
	}


	// 3. Write actions.
	if(content.act[0].acttype != 0) { // There is at least one action.
		buf << "\tactions = {\r\n";

		for(i = 0 ; i < 64 ; i++) {
			if(content.act[i].acttype == 0) break;
			DecodeAction(buf, content.act[i]);
		}
		
		buf << "\t},\r\n";
	}

	// 4. Print auxilarry informations
	std::vector<const char*> auxinfo;
	if(content.flag & 1) auxinfo.push_back("actexec");
	if(content.flag & 4) auxinfo.push_back("preserved");
	if(content.flag & 8) auxinfo.push_back("disabled");

	if(!auxinfo.empty()) {
		buf << "\tflag = {" << auxinfo[0];
		for(i = 1 ; i < auxinfo.size() ; i++) {
			buf << ", " << auxinfo[i];
		}
		buf << "},\r\n";
	}

	// 5. Print starting condition
	if(content.current_action != 0) {
		buf << "\tstarting_action = " << content.current_action << ",\r\n";
	}

	buf << "}\r\n\r\n\r\n";
}