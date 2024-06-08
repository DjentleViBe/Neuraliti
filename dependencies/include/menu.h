#ifndef menu_h
#define menu_h

#include <stdio.h>
#include <map>

extern bool saveas;
extern bool openfile;
extern bool editpref;
extern bool audiopref;
extern std::string filename;
extern std::map<std::string, std::string> appsettings;

void ShowMenu(bool* p_open);
#endif /* menu.h */
