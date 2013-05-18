#ifndef _save_h_
#define _save_h_

#include <sstream>
#include <string>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include "global.h"

void save_user_data(game_data data, int userid);
void load_user_data(game_data *data, int userid);
void load_user(game_data *data, std::string user, int* user_id);

#endif