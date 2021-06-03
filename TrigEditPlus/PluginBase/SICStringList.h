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

#ifndef _STRING_H
#define _STRING_H

#include <windows.h>	//	for types


struct SI_VirtSCStringList {
  virtual void blah_00() = 0;
  virtual void blah_04() = 0;
  virtual void blah_08() = 0;
  virtual int FindString_RawIndex(const char*) = 0;
  
  // Retrives stringtable[stringID].
  // stringID == 0 : return blank string
  // No valid string at given stringID : return NULL
  // Else : return SCMD2String
  //
  // Returned memory is managed by SCMDraft2, and shouldn't be freed manually.
  virtual const char* GetString(int strid) = 0;
  virtual void blah_14() = 0;
  virtual int AddSCMD2String(const char*, int, char) = 0;

  // Delete 1 reference from given section to string. String with no reference are deleted automatically.
  // a3 is unknown or unused. Set it to 0
  virtual int Dereference(__int16 stringindex, int SectionName, int unkown_arg) = 0;

  // Dereference & Add in one function.
  virtual int DerefAndAddString(const char* Text, int oldStringIndex, int SectionName) = 0;
  virtual char SetSCMD2Text(const char*, int) = 0;
  virtual void blah_28() = 0;

  // Get reserved space for string table.
  // Returned size = (used stringtable slot) + (unused stringtable slot num)
  // Stringtable slot 0(empty string) is assumed as being used.
  virtual int GetTotalStringNum() = 0;
  virtual void blah_30() = 0;
  virtual void blah_34() = 0;
  virtual void blah_38() = 0;

  // Backup string table temporarilly. Mainly used for trigger editor.
  // If anything goes wrong, you can call RestoreBackup to revert changes made to string table
  // If all goes right, call ClearBackup.
  virtual char BackupStrings() = 0;

  // Counterpart of StringTable_BackupStrings 
  virtual char RestoreBackup() = 0;

  // Counterpart of StringTable_BackupStrings
  virtual char ClearBackup() = 0;
};


// Find string number for given string.
// If there isn't exact match, return -1;
int  __stdcall StringTable_FindString(SI_VirtSCStringList *strtb, const char *tofind);

// Add string to stringtable. If string table is full. this function increases string table's capacity for new string.
// If AlwaysCreate is False, then StringTable_AddString recycles existing strings if possible.
// This function never fails. No 65536byte restriction, no 1024 string count restriction.
// return : corresponding string number.
// Blank string -> return 0. (Blank string)
int  __stdcall StringTable_AddString(SI_VirtSCStringList *strtb, const char *text, int SectionName, char AlwaysCreate);

// strcpy(stringtable[stringID].content, string); 0 if failed.
char  __stdcall StringTable_SetText(SI_VirtSCStringList *strtb, const char *string, int stringID);


#endif