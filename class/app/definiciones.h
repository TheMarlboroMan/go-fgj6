#pragma once

namespace App
{

struct env {

	static std::string  data_path;
	static std::string  usr_path;
};

struct version {

	static const int major{1},
					minor{1},
					patch{1};
};

}
