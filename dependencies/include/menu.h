//
//  MainMenu.hpp
//  GENT
//
//  Created by Vishakh Begari on 14.04.24.
//

#ifndef MainMenu_h
#define MainMenu_h

#include <stdio.h>
#include <map>

extern bool saveas;
extern bool openfile;
extern bool editpref;
extern std::map<std::string, std::string> appsettings;

void ShowMenu(bool* p_open);
#endif /* MainMenu_hpp */
