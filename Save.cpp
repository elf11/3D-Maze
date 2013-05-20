#include "stdafx.h"
#include "Save.h"

std::string NumberToString (int number) {
     std::ostringstream ss;
     ss << number;
     return ss.str();
}

void update_users_file(std::string user) {
	std::fstream fs;
	fs.open ("users.txt", std::fstream::in | std::fstream::out | std::fstream::app);
	fs << user << std::endl;

	fs.close();
}

void save_user_data(game_data data, int userid) {
	std::string path = "Users\\";
	std::string file = NumberToString(userid);
	path.append(file.c_str());

	std::ofstream out(path.c_str()`);
	out << data.level << std::endl;
	out << data.life << std::endl;
	out << data.chosen_level << std::endl;
	out << data.name << std::endl;
	out << data.guns << std::endl;



	out.close();
}

void load_user_data(game_data *data, int userid) {
	// open file for reading
	std::string path = "Users\\";
	std::string line;
	std::string file = NumberToString(userid);
	path.append(file);
	std::ifstream infile(path.c_str());

	//get level
	std::getline(infile, line);
	data->level = atoi(line.c_str());

	//get life
	std::getline(infile, line);
	data->life = atoi(line.c_str());

	//get chosen_level
	std::getline(infile, line);
	data->chosen_level = atoi(line.c_str());

	//get name
	std::getline(infile, line);
	strcpy(data->name, line.c_str());
	
	//get guns
	std::getline(infile, line);
	data->guns = atoi(line.c_str());
	infile.close();
}


void load_user(game_data *data, std::string user, int* u_id) {
	int id;
	BOOL user_found = FALSE;
	int i = 0;
	std::string line;

	std::fstream infile("users.txt");
	std::ofstream outfile("out.txt");
	while (std::getline(infile, line)) {
		if (line.compare(user) == 0) {
			user_found = TRUE;
			id = *u_id;
			break;
		}
		i++;
	}
	*u_id = i;

	if (user_found == TRUE) {
		outfile << "user found \n";
		// read saved data from file
		load_user_data(data, id);
	}
	else {
		outfile << i << std::endl;
		// write te name in users.txt
		update_users_file(user);
		// write data in user's file
		std::string path = "Users\\";
		std::string userid = NumberToString(i);
		path.append(userid);
		outfile << path << std::endl;
		std::ofstream newfile(path.c_str());
		newfile << 0 << std::endl;
		newfile << 99 << std::endl;
		newfile << 0 << std::endl;
		newfile << 0 << std::endl;
		newfile << user << std::endl;
		data->level = 0;
		data->life = 99;
		data->guns = 0;
		data->chosen_level = 0;
		strcpy(data->name, (user.c_str()));
	}

	infile.close();
	outfile.close();
}