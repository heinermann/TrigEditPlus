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
#include "../Lua/lib/lua.hpp"
#include "../../resource.h"
#include "../Lua/LuaCommon.h"
#include <stdio.h>
#include <stdarg.h>

// User lua loader
void LuaAutoRequireLibs(lua_State* L);

TriggerEditor* LuaGetEditor(lua_State* L) {
	lua_getglobal(L, "__inst_global_TriggerEditor");
	TriggerEditor* e = (TriggerEditor*)lua_touserdata(L, -1);
	lua_pop(L, 1);
	return e;
}


int LuaErrorHandler(lua_State* L) {	
	lua_State *L1 = luaL_newstate();
	luaL_traceback(L1, L, "Error while running trigger script", 1);
	LuaGetEditor(L)->PrintErrorMessage(lua_tostring(L1, -1));
	lua_close(L1);
	
	return 1;
}



// String getter
int LuaEncodeUnit(lua_State* L);
int LuaEncodeLocation(lua_State* L);
int LuaEncodeSwitchName(lua_State* L);
int LuaEncodeString(lua_State* L);
int LuaEncodeProperty(lua_State* L);
void ClearPropertyMap();

// Trigger getter
int LuaEncodeTrigger(lua_State* L) {
	Trig t;
	memset(&t, 0, sizeof(Trig));

	if (!lua_istable(L, -1)) {
		return LuaErrorf(L, "table expected, got %s", lua_typename(L, lua_type(L, -1)));
	}

	LuaCheckTableEntry(L, { "players", "conditions", "actions", "flag", "starting_action", "callerLine" });

	// Players
	lua_pushstring(L, "players");
	lua_gettable(L, -2); // < t.players

	if (lua_istable(L, -1)) {
		int playern = lua_rawlen(L, -1);
		for (int i = 1; i <= playern; i++) {
			lua_getglobal(L, "ParsePlayer");
			lua_pushinteger(L, i);
			lua_gettable(L, -3); // < t.players[i]
			lua_call(L, 1, 1);

			int playerID = luaL_checkinteger(L, -1);
			lua_pop(L, 1); // > t.players[i]

			if (playerID < 0 || playerID > 27) {
				return LuaErrorf(L, "Trigger can't be executed by player #%d", playerID);
			}

			else if (t.effplayer[playerID] == 1) {
				return LuaErrorf(L, "Duplicate executing player #%d", playerID);
			}

			t.effplayer[playerID] = 1;
		}
	}
	lua_pop(L, 1); // > t.players



				   // Conditions.
	lua_pushstring(L, "conditions");
	lua_gettable(L, -2); // < t.players

	if (lua_istable(L, -1)) {
		int condn = lua_rawlen(L, -1);
		if (condn > 16) {
			return LuaErrorf(L, "trigger has too many conditions : got %d, max 16", condn);
		}

		for (int i = 1; i <= condn; i++) {
			lua_pushinteger(L, i);
			lua_gettable(L, -2); // < t.conditions[i]
			if (!lua_istable(L, -1)) {
				return LuaErrorf(L, "condition #%d is invalid. (not a table)", i);
			}

			if (lua_rawlen(L, -1) != 9) {
				return LuaErrorf(L, "condition #%d is invalid. (has %d elements, expected 9)", i, lua_rawlen(L, -1));
			}

			// pop all elements
			for (int j = 1; j <= 9; j++) {
				lua_pushinteger(L, j);
				lua_gettable(L, -1 - j); // < t.conditions[i][j]
			}

			t.cond[i - 1].locid = luaL_checkinteger(L, -9);
			t.cond[i - 1].player = luaL_checkinteger(L, -8);
			t.cond[i - 1].res = luaL_checkinteger(L, -7);
			t.cond[i - 1].uid = luaL_checkinteger(L, -6);
			t.cond[i - 1].setting = luaL_checkinteger(L, -5);
			t.cond[i - 1].condtype = luaL_checkinteger(L, -4);
			t.cond[i - 1].res_setting = luaL_checkinteger(L, -3);
			t.cond[i - 1].prop = luaL_checkinteger(L, -2);
			t.cond[i - 1].maskflag = luaL_checkinteger(L, -1);

			lua_pop(L, 9); // > t.conditions[i][1~9]

			lua_pushstring(L, "disabled"); // < t.conditions[i]["disabled"]
			lua_gettable(L, -2);
			if (lua_isboolean(L, -1)) {
				int a = lua_toboolean(L, -1);
				if (a) t.cond[i - 1].prop |= 2;
			}
			lua_pop(L, 2); // > t.conditions[i]["disabled"], > t.conditions[i]
		}
	}
	lua_pop(L, 1); // > t.conditions



				   // Actions
	lua_pushstring(L, "actions");
	lua_gettable(L, -2); // < t.players

	if (lua_istable(L, -1)) {
		int actn = lua_rawlen(L, -1);
		if (actn > 64) {
			return LuaErrorf(L, "trigger has too many actions : got %d, max 64", actn);
		}

		for (int i = 1; i <= actn; i++) {
			lua_pushinteger(L, i);
			lua_gettable(L, -2); // < t.actions[i]
			if (!lua_istable(L, -1)) {
				return LuaErrorf(L, "action #%d is invalid. (not a table)", i);
			}

			if (lua_rawlen(L, -1) != 11) {
				return LuaErrorf(L, "action #%d is invalid. (has %d elements, expected 11)", i, lua_rawlen(L, -1));
			}

			// pop all elements
			for (int j = 1; j <= 11; j++) {
				lua_pushinteger(L, j);
				lua_gettable(L, -1 - j); // < t.actions[i][j]
			}

			t.act[i - 1].locid = luaL_checkinteger(L, -11);
			t.act[i - 1].strid = luaL_checkinteger(L, -10);
			t.act[i - 1].wavid = luaL_checkinteger(L, -9);
			t.act[i - 1].time = luaL_checkinteger(L, -8);
			t.act[i - 1].player = luaL_checkinteger(L, -7);
			t.act[i - 1].target = luaL_checkinteger(L, -6);
			t.act[i - 1].setting = luaL_checkinteger(L, -5);
			t.act[i - 1].acttype = luaL_checkinteger(L, -4);
			t.act[i - 1].num = luaL_checkinteger(L, -3);
			t.act[i - 1].prop = luaL_checkinteger(L, -2);
			t.act[i - 1].maskflag = luaL_checkinteger(L, -1);

			lua_pop(L, 11); // > t.actions[i][1~11]

			lua_pushstring(L, "disabled"); // < t.actions[i]["disabled"]
			lua_gettable(L, -2);
			if (lua_isboolean(L, -1)) {
				int a = lua_toboolean(L, -1);
				if (a) t.act[i - 1].prop |= 2;
			}
			lua_pop(L, 2); // > t.actions[i]["disabled"], > t.actions[i]
		}
	}
	lua_pop(L, 1); // > t.actions



				   // Flag
	lua_pushstring(L, "flag");
	lua_gettable(L, -2); // < t.players

	if (lua_istable(L, -1)) {
		int flagn = lua_rawlen(L, -1);
		int flag = 0;

		lua_getglobal(L, "actexec");    // 0x1
		lua_getglobal(L, "preserved");  // 0x4
		lua_getglobal(L, "disabled");   // 0x8

		for (int i = 1; i <= flagn; i++) {
			lua_pushinteger(L, i);
			lua_gettable(L, -5); // < t.flag[i]

			/**/ if (lua_compare(L, -1, -4, LUA_OPEQ)) flag |= 0x1; //actexec
			else if (lua_compare(L, -1, -3, LUA_OPEQ)) flag |= 0x4; //preserved
			else if (lua_compare(L, -1, -2, LUA_OPEQ)) flag |= 0x8; //disabled
			else {
				return LuaErrorf(L, "Unknown flag of type \"%s\" given", lua_typename(L, lua_type(L, -1)));
			}

			lua_pop(L, 1); // > t.flag[i]
		}

		t.flag = flag;

		lua_pop(L, 3); // actexec, preserved, disabled
	}
	lua_pop(L, 1); // > t.flag



				   // starting_action
	lua_pushstring(L, "starting_action");
	lua_gettable(L, -2); // < t.starting_action
	if (lua_isnumber(L, -1)) {
		t.current_action = luaL_checkinteger(L, -1);
	}
	lua_pop(L, 1);

	// callee line number
	lua_pushstring(L, "callerLine");
	lua_gettable(L, -2);
	luaL_checkinteger(L, -1);
	int callerLine = luaL_checkinteger(L, -1);
	lua_pop(L, 1);

	// Done.
	TriggerEditor* e = LuaGetEditor(L);
	TrigBufferEntry tbe = { t, callerLine };
	e->_trigbuffer.push_back(tbe);

	return 0;
}



int LuaEncodeSpecialData(lua_State* L);

char* GetUPRPChunkData();

bool TriggerEditor::EncodeTriggerCode() {
	//Prepare for encode.
	_editordata->EngineData->MapStrings->BackupStrings();
	ClearErrors();

	// Initialize new lua state.
	lua_State *L;
	L = luaL_newstate();
	LuaAutoRequireLibs(L);
	
	// Add various compile-related functions.
	lua_pushlightuserdata(L, this);
	lua_setglobal(L, "__inst_global_TriggerEditor");
	lua_register(L, "__internal__AddTrigger", LuaEncodeTrigger);
	lua_register(L, "__internal__AddSpecialData", LuaEncodeSpecialData);
	lua_pushcfunction(L, LuaErrorHandler); // Error handler
	
	std::string editortext = GetEditorText();
	if(luaL_loadbufferx(L, editortext.data(), editortext.size(), "main", "t") != LUA_OK) {
		PrintErrorMessage(lua_tostring(L, -1));
		lua_close(L);
		_editordata->EngineData->MapStrings->RestoreBackup();
		PrintErrorMessage("Compile failed.");
		ClearPropertyMap();
		return false;
	}

	if(lua_pcall(L, 0, 0, -2) != LUA_OK) {
		PrintErrorMessage(lua_tostring(L, -1));
		lua_close(L);
		_editordata->EngineData->MapStrings->RestoreBackup();
		PrintErrorMessage("Compile failed.");
		_trigbuffer.clear();
		ClearPropertyMap();
		return false;
	}



	// Compile Done.

	_editordata->EngineData->MapStrings->ClearBackup();

	// Dereferencing should be after trigger compilication
	//
	// - Any string referenced by comment action or something won't change its ID.
	//
	
	// Deref strings.
	DerefStrings();


	// Replace trigger data
	scmd2_free(_editordata->Triggers->ChunkData);

	BYTE* newdata = (BYTE*)scmd2_malloc(_trigbuffer.size() * 2400);
	for(size_t i = 0 ; i < _trigbuffer.size() ; i++) {
		const Trig& trigData = _trigbuffer[i].trigData;
		memcpy(newdata + 2400 * i, &trigData, 2400);
	}


	_editordata->Triggers->ChunkData = newdata;
	_editordata->Triggers->ChunkSize = 2400 * _trigbuffer.size();

	memcpy(_editordata->UnitProperties->ChunkData, GetUPRPChunkData(), 1280);

	// Update trigger list
	UpdateTriggerList(_trigbuffer);

	// Cleanup
	_trigbuffer.clear();
	ClearPropertyMap();
	lua_close(L);
	PrintErrorMessage("Compile Complete!");
	return true;
}