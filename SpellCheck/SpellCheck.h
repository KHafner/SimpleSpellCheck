//
//  SpellCheck.h
//  SpellCheck
//
//  Created by Kimberly Hafner on 6/18/19.
//  Copyright © 2019 Kimberly Hafner. All rights reserved.
//

//Note the program assumes use of the input en_US.dic from SCOWL http://wordlist.aspell.net/ with the
//following copyright notice:
//Copyright 2000-2016 by Kevin Atkinson
//
//Permission to use, copy, modify, distribute and sell these word
//lists, the associated scripts, the output created from the scripts,
//and its documentation for any purpose is hereby granted without fee,
//provided that the above copyright notice appears in all copies and
//that both that copyright notice and this permission notice appear in
//supporting documentation. Kevin Atkinson makes no representations
//about the suitability of this array for any purpose. It is provided
//"as is" without express or implied warranty.

#include <set>
#include <map>

#ifndef SpellCheck_h
#define SpellCheck_h

//dictionary file
std::set<std::string> dictionary;
//stores words and line position from the input
std::map<std::string ,std::string> minput;

int LINEMAX = 1024;

//reads and tokenizes words in dictionary file to dictionary set
//returns 0 on success
int read_dictionary(std::string in);

//reads the words in the input file (.txt) and stores in the map minput
//returns 0 on success
int read_input(std::string in);

//peforms the spell check, writes mispelled words and their line number to output file
//returns 0 on success
int spellcheck();

std::string remove_prefix(std::string word);
std::string remove_suffix(std::string word);

#endif /* SpellCheck_h */
