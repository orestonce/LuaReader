bool	= true
int		= 1
double	= 1.1
string	= "Hello, this is a string1"

table = {
	bool	= false,
	int		= 2,
	double	= 2.1,
	string	= "Hello, this is a string2",
	
	[1]		= true,
	[2]		= 3,
	[3]		= 3.1,
	[4]		= "Hello, this is a string3",
	
	table2 = {
		[1]		= false,
		[2]		= 4,
		[3]		= 4.1,
		[4]		= "Hello, this is a string4",
		
		bool	= true,
		int		= 5,
		double	= 5.1,
		string	= "Hello, this is a string5",
	},
}

table3 = {
	[1] = {
		[1]		= false,
		[2]		= 6,
		[3]		= 6.1,
		[4]		= "Hello, this is a string6",
		
		bool	= true,
		int		= 7,
		double	= 7.1,
		string	= "Hello, this is a string7",
	},
	
	[2] = {
		[1]		= false,
		[2]		= 8,
		[3]		= 8.1,
		[4]		= "Hello, this is a string8",
		
		bool	= true,
		int		= 9,
		double	= 9.1,
		string	= "Hello, this is a string9",
	}
}