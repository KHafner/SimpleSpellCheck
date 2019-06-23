//
//  main.cpp
//  SpellCheck
//
//  Created by Kimberly Hafner on 6/18/19.
//  Copyright © 2019 Kimberly Hafner. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include "SpellCheck.h"

int main(int argc, const char * argv[]) {
    std::string input_file;
    std::string dictionary_file;
    
    if(argc < 2)
    {
        std::cout << "Enter the file to spell check." << std::endl;
        std::cin >> input_file;
    }
    else
        input_file = argv[1];
    
    if(read_dictionary("en_US.dic"))
    {
        std::cout << "Error reading dictionary file." << std::endl;
        return 1;
    }
    
    if(read_input(input_file))
    {
        std::cout << "Error reading input file." << std::endl;
        return 1;
    }
    
    if(spellcheck())
    {
        std::cout << "Error completing spell check." << std::endl;
        return 1;
    }
    else
        std::cout << "Check spellcheck_errors.txt for misspelled words." << std::endl;
    
    return 0;
}

int read_dictionary(std::string in)
{
    std::ifstream ifile (in, std::ios::in);
    
    if(ifile.is_open())
    {
        char line[LINEMAX];
        //ignore copyright notice in first 11 lines
        for(int i = 0; i < 11; i++)
            ifile.getline(line, LINEMAX - 1);
        while(ifile.good())
        {
            ifile.getline(line, LINEMAX - 1);
            //input should have one word per line, ignore specification after the forward slash
            char* pch = strtok(line, "/\n\r");
            if( pch != NULL)
            {
                std::string word = pch;
                std::transform(word.begin(), word.end(), word.begin(), ::tolower);
                dictionary.insert(word);
            }
        }
    }
    
    ifile.close();
    
    return 0;
}

int read_input(std::string in)
{
    std::ifstream ifile (in, std::ios::in);
    
    if(ifile.is_open())
    {
        char line[LINEMAX];
        int linecount = 0;
        while(ifile.good())
        {
            ifile.getline(line, LINEMAX - 1);
            linecount++;
            char* pch = strtok(line, "0123456789()_-—;:\",.!?#$%&*\n\r\t ");
            while(pch != NULL)
            {
                std::pair<std::map<std::string, std::string>::iterator, bool> it;
                std::string word = pch;
                std::transform(word.begin(), word.end(), word.begin(), ::tolower);
                it = minput.insert(std::pair<std::string, std::string>(word, std::to_string(linecount)));
                if(it.second == false)
                {
                    //this word has already been read from the input document, add the new line number
                    it.first->second.append(",");
                    it.first->second.append(std::to_string(linecount));
                    
                }
                pch = strtok(NULL, "0123456789()_-—;:\",.!?#$%&*\n\r\t ");
            }
        }
    }
    else
        return 1;
    
    ifile.close();
    
    return 0;
}

int spellcheck()
{
    std::ofstream outfile("spellcheck_errors.txt", std::ios::out | std::ios::trunc);
    if(outfile.is_open())
    {
        //iterate through words in input file and
        outfile << "Misspelled word: Found on lines" << std::endl;
        for (std::map<std::string ,std::string>::iterator it = minput.begin(); it != minput.end(); ++it)
        {
            if(dictionary.find(it->first) == dictionary.end())
            {
                //check first for prefixes and suffixes
                std::string word = it->first;
                if(word.length() > 1 && (word.back() == 's' || word.back() == 't'  || word.back() == 'd'))
                {
                    std::string firstcheck = word.substr(0, word.length() - 1);
                    if(dictionary.find(firstcheck) != dictionary.end())
                    {
                        continue;
                    }
                }
                
                std::string noprefix = remove_prefix(word);
                std::string nosuffix = remove_suffix(word);
                std::string both = remove_prefix(nosuffix);
                
                if(both != word && both != noprefix && both != nosuffix){
                    if(dictionary.find(both) != dictionary.end())
                    {
                        continue;
                    }
                }
                else if(noprefix != word)
                {
                    if(dictionary.find(noprefix) == dictionary.end())
                    {
                        if(noprefix.length() > 1 && (noprefix.back() == 's' || noprefix.back() == 't'  || noprefix.back() == 'd'))
                        {
                            std::string firstcheck = noprefix.substr(0, noprefix.length() - 1);
                            if(dictionary.find(firstcheck) == dictionary.end())
                            {
                                outfile << it->first << ": " << it->second << std::endl;
                            }
                        }
                    }
                }
                else if(nosuffix != word)
                {
                    if(dictionary.find(nosuffix) == dictionary.end())
                    {
                        outfile << it->first << ": " << it->second << std::endl;
                    }
                }
                else
                    outfile << it->first << ": " << it->second << std::endl;
            }
        }
    }
    else
    {
        std::cout << "Unable to open file 'spellcheck_errors.txt' for writing." << std::endl;
        return 1;
    }
    
    return 0;
}

std::string remove_suffix(std::string word)
{
    std::string ending1 = "";
    std::string ending2 = "";
    std::string ending3 = "";
    if(word.length() > 2)
        ending1 = word.substr(word.size() - 2);
    if (word.length() > 3)
        ending2 = word.substr(word.size() - 3);
    if(word.length() > 4)
        ending3 = word.substr(word.size() - 4);
    if (ending1 == "ed" || ending1 == "es" || ending1 == "er" || ending1 == "en" || ending1 == "ly")
    {
        word = word.substr(0, word.size() - 2);
    }
    else if(ending2 == "ing" || ending2 == "est")
    {
        word = word.substr(0, word.size() - 3);
    }
    else if(ending3 == "able" || ending3 == "ness" || ending3 == "less")
    {
        word = word.substr(0, word.size() - 4);
    }
    
    return word;
}

std::string remove_prefix(std::string word)
{
    if(word.length() > 2)
    {
        std::string beginning1 = word.substr(0,2);
        std::string beginning2 = "";
        if (word.length() > 3)
            beginning2 = word.substr(0,3);
        
        if( beginning1 == "un"  || beginning1 == "in"  || beginning1 == "re" )
        {
            word = word.substr(2);
        }
        else if( beginning2 == "dis" )
        {
            word = word.substr(3);
        }
    }
    return word;
}
