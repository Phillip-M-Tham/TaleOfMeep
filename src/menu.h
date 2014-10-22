#ifndef _MENUS_
#define _MENUS_

#include "entity.h"
#include "window.h"

void MainMenu();
void UpdateMainMenu();
void PlayerMenu(Entity *self);
void LoadDashboard();
void CloseDashboard();
void DrawDashboard();
void UpdateDashboard();/*input handling for dashboard*/
int ReturnFlag();


#endif