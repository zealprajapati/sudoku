//
//  CSP.cpp
//  Sudoku
//
//  Created by cpsfaculty on 02/10/18.
//  Copyright (c) 2018 ___Ju Shen___. All rights reserved.
//

#include <stdio.h>

#include "CSP.h"
#include<cmath>
/************************************** Below are the three functions you need to implement ***************************************/

/*Check whether current state satisfy the constraints*/

 
	

bool CSP::goalCheck(const State state)
{
	int r, c = 0;
	int x, y = 0;
	for (int i = 0; i < 81; i++)
	{
		r = i / 9;
		c = i % 9;
		if (r == 0 || c == 0)
		{
			for (int j = 0; j < 9; j++)
			{
				if (state.values[j][c] == state.values[r][c] && j != r)
				{
					return false;
				}
				if (state.values[r][j] == state.values[r][c] && j != c)
				{
					return false;
				}
				if (state.values[r][j] == state.values[r][j-1] && j != c)
				{
					return false;
				}
				if (state.values[r][j] == state.values[r-1][j] && j != c)
				{
					return false;
				}
			}
		}
		if ((r == 0 || r == 3 || r == 6 ) &&(c == 0 || c == 3 || c == 6))
		{
			x = r / 3 * 3;
			y = c / 3 * 3;
			for (int i = x; i < x + 3; i++)
			{
				for (int j = y; j < y + 3; j++)
				{
					if (state.values[i][j] == state.values[r][c] && i != r && j != c) {
						return false;
					}
				}
			}
		}
		return true;
	}
		
		
}




/*Update Domain for the forward checking*/
void CSP::updateDomain(const State state)
{
	for (int i = 0; i < 9; i++)

		for (int j = 0; j < 9; j++)
		{
			variables[i][j].domain.clear();
			for (int z = 1; z < 10; z++)
				variables[i][j].domain.push_back(z);
		}

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			//row
			for (int z = 0; z < 9; z++)
			{
				int p = variables[i][j].assignement;
				for (int k = 0; k < variables[i][z].domain.size(); k++)
				{
					if ((variables[i][z].domain[k] == p) && (variables[i][z].domain.size() > 0))
					{
						variables[i][z].domain.erase(variables[i][z].domain.begin() + k);
						break;
					}
				}
			}	
			//column
			for (int z = 0; z < 9; z++)
			{
					int p = variables[i][j].assignement;
					for (int k = 0; k < variables[z][j].domain.size(); k++)
					{
						if ((variables[z][j].domain[k] == p) && (variables[z][j].domain.size() > 0))
						{
							variables[z][j].domain.erase(variables[z][j].domain.begin() + k);
							break;
						}
					}
			}
			//3*3 box
			int i3 = (i / 3) * 3;
			int j3 = (j / 3) * 3;
			int p = variables[i][j].assignement;
			for (int x = i3; x < (i3 + 3); x++)
			{
				for (int y = j3; y < (j3 + 3); y++)
				{
					for(int k=0;k<variables[x][y].domain.size();k++)

						if ((variables[x][y].domain[k] == p) && (variables[x][y].domain.size()>0))
						{
							variables[x][y].domain.erase(variables[x][y].domain.begin() + k);
							break;
						}
				}
			}
			
		}
	}
}



/*Arc consistency use*/
void CSP::arcConsistency(const State state)
{
	updateDomain(state);
	
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (variables[i][j].assignement==0 && variables[i][j].domain.size() == 1)
			{
				int num = variables[i][j].domain[0];
		//row
				for (int z = 0; z < 9; z++)
				{
					if (j==z)
					{
						continue;
					}
					else 
					{

						for (int k = 0; k < variables[i][z].domain.size(); k++)
						{
							if (variables[i][z].domain[k] == num)
							{
								variables[i][z].domain.erase(variables[i][z].domain.begin() + k);
								break;
							}
						}
					}
				}
			//column
				for (int z = 0; z < 9; z++)
				{
					if (i==z)
					{
					 continue;
					}
					else
					{

						for (int k = 0; k < variables[z][j].domain.size(); k++)
						{
							if ((variables[z][j].domain[k] == num))
							{
							variables[z][j].domain.erase(variables[z][j].domain.begin() + k);
							break;
							}
						}
					}
				}
				
				////3*3 box
				int i3 = (i / 3) * 3;
				int j3 = (j / 3) * 3;

				for (int x = i3; x < (i3 + 3); x++)
				{
					for (int y = j3; y < (j3 + 3); y++)
					{
						if (x == i && y == j)
						{
							continue;
						}
						else
						{
							for (int k = 0; k < variables[x][y].domain.size(); k++)

								if ((variables[x][y].domain[k] == num))
								{
									variables[x][y].domain.erase(variables[x][y].domain.begin() + k);
									break;
								}
						}
					}
				}

			}
		}
	}
}
	

/************************************************	End of Assignment ***********************************************/




CSP::CSP()
{
	/*Initially assign the domain, assignment for each variable and initialize the current state*/
	for (int y = 0; y < 9; y++)
	{
		for (int x = 0; x < 9; x++)
		{
			variables[y][x].assignement = 0; //Initialize the assignment

			/*Initialize the domain*/
			for (int i = 1; i <= 9; i++)
			{
				variables[y][x].domain.push_back(i);
			}

			cur_state.values[y][x] = 0; //Initizlize the current state

		}
	}

	alg_opt = 1; //initially set it as back track

	srand(time(NULL));
	random = 0;
}


CSP::~CSP()
{

}




void CSP::setData(int *data)
{
	for (int y = 0; y < 9; y++)
	{
		for (int x = 0; x < 9; x++)
		{
			int idx = y * 9 + x;
			variables[y][x].assignement = data[idx]; //Initialize the assignment
			cur_state.values[y][x] = data[idx]; //Initizlize the current state

		}
	}
}

void CSP::clearData()
{
	/*Initially assign the domain, assignment for each variable and initialize the current state*/
	for (int y = 0; y < 9; y++)
	{
		for (int x = 0; x < 9; x++)
		{
			variables[y][x].assignement = 0; //Initialize the assignment

			/*Initialize the domain*/
			variables[y][x].domain.clear();
			for (int i = 1; i <= 9; i++)
			{
				variables[y][x].domain.push_back(i);
			}

			cur_state.values[y][x] = 0; //Initizlize the current state

		}
	}

	/*Check whether a random domain is use*/
	if (random == 1)
		reshuffleDomain();

	repeating_list.clear();
	while (!assigned_variables.empty())
	{
		assigned_variables.pop();
		repeating_list.clear();
	}

}


void CSP::reshuffleDomain()
{
	for (int i = 0; i < 81; i++)
	{
		int y = i / 9;
		int x = i % 9;

		std::random_shuffle(variables[y][x].domain.begin(), variables[y][x].domain.end());
	}
}

void CSP::sortDomain()
{
	for (int i = 0; i < 81; i++)
	{
		int y = i / 9;
		int x = i % 9;

		std::sort(variables[y][x].domain.begin(), variables[y][x].domain.end());
	}
}

/*Cancel last assignment*/
int CSP::goBack(int *chosen_cell)
{
	if (assigned_variables.size() > 0)
	{
		int cur_id = assigned_variables.top(); /*Remove last options*/
		assigned_variables.pop(); //pop out last option
		int y = cur_id / 9;
		int x = cur_id % 9;

		variables[y][x].assignement = 0; //assign the cell to zero
		cur_state.values[y][x] = 0; //update the assignment
		*chosen_cell = cur_id;

		// printf("(%d, %d)\n", y, x);
		if (alg_opt == 2)
		{
			updateDomain(cur_state);
		}
		else if (alg_opt == 3)
		{
			arcConsistency(cur_state);
		}

	}

	return goalCheck(cur_state);

}


bool CSP::arcCheckingOrder(int *chosen_cell)
{
	arcConsistency(cur_state);



	/*First go through all the variables and do backtrack if there is no empty domain */
	for (int i = 0; i < 81; i++)
	{
		int y = i / 9;
		int x = i % 9;


		if (cur_state.values[y][x] == 0 && variables[y][x].domain.size() == 0)
		{
			int available_assignemnt = 0; //an indicatior whether there are possible possible varaibles to be re-assigned
			while (available_assignemnt == 0) {
				int cur_id = assigned_variables.top();
				int y = cur_id / 9;
				int x = cur_id % 9;
				variables[y][x].assignement = 0;
				cur_state.values[y][x] = 0;
				arcConsistency(cur_state);


				for (int i = 0; i < variables[y][x].domain.size(); i++)
				{
					State temp_state;
					temp_state = cur_state;
					temp_state.values[y][x] = variables[y][x].domain[i];
					if (std::find(repeating_list.begin(), repeating_list.end(), temp_state) == repeating_list.end()) //if not in the repeating list
					{
						cur_state = temp_state;
						variables[y][x].assignement = variables[y][x].domain[i];
						repeating_list.push_back(temp_state);
						available_assignemnt = 1;
						*chosen_cell = cur_id;
						arcConsistency(cur_state);
						return false; //get out of the current varaible assignment
					}
				}

				if (available_assignemnt == 0) //if all the domain values have been tried for current variable
				{
					variables[y][x].assignement = 0;
					cur_state.values[y][x] = 0;
					assigned_variables.pop();

				}
			}

		}

	}

	/*If there is no variable that has empty domain, then assign variable here*/
	/*First go through all the variables and do backtrack if there is no empty domain */
	int count = 0;
	while (count < 81)
	{
		/*Find the index of minimum number of domain*/
		int min_idx = 0;
		int min_num = 10; //because the maximum number of domain is 10
		for (int i = 0; i < 81; i++)
		{
			int y = i / 9;
			int x = i % 9;
			if (cur_state.values[y][x] == 0 && variables[y][x].domain.size() > 0)
			{
				if (variables[y][x].domain.size() < min_num) {
					min_idx = i;
					min_num = variables[y][x].domain.size();
				}
			}
		}

		int y = min_idx / 9;
		int x = min_idx % 9;

		/*If there is any varable has not been assigned yet, assign it and return it*/
		if (cur_state.values[y][x] == 0 && variables[y][x].domain.size() > 0)
		{
			/*Find the smalles number in domain to assign it. Here no update domain for bracktrack*/
			int id_min = 0;
			cur_state.values[y][x] = variables[y][x].domain[id_min];
			variables[y][x].assignement = variables[y][x].domain[id_min];
			assigned_variables.push(min_idx); //push the variable into stack, which will be used for backtrack (or DFS)
			repeating_list.push_back(cur_state); //make this state into the repeat_list
			*chosen_cell = 9 * y + x;

			arcConsistency(cur_state); //Every time modify the assignment update the domain

			return false;
		}

		count++;

	}

	if (goalCheck(cur_state))
	{
		printf("find the goal\n");
		return true;
	}
	else
	{
		int available_assignemnt = 0; //an indicatior whether there are possible varaibles to be re-assigned
		while (available_assignemnt == 0) {
			int cur_id = assigned_variables.top();
			int y = cur_id / 9;
			int x = cur_id % 9;
			variables[y][x].assignement = 0;
			cur_state.values[y][x] = 0;
			arcConsistency(cur_state);
			for (int i = 0; i < variables[y][x].domain.size(); i++)
			{
				State temp_state;
				temp_state = cur_state;
				temp_state.values[y][x] = variables[y][x].domain[i];
				if (std::find(repeating_list.begin(), repeating_list.end(), temp_state) == repeating_list.end()) //if not in the repeating list
				{
					cur_state = temp_state;
					variables[y][x].assignement = variables[y][x].domain[i];
					repeating_list.push_back(cur_state);
					available_assignemnt = 1;
					*chosen_cell = cur_id;
					break; //get out of the current varaible assignment
				}
			}

			if (available_assignemnt == 0) //if all the domain values have been tried for current variable
			{

				assigned_variables.pop();

			}
		}

		return false;
	}
	return false;

}



/*arcChecking without ordering*/
bool CSP::arcChecking(int *chosen_cell)
{
	arcConsistency(cur_state);



	/*First go through all the variables and do backtrack if there is no empty domain */
	for (int i = 0; i < 81; i++)
	{
		int y = i / 9;
		int x = i % 9;

		if (cur_state.values[y][x] == 0 && variables[y][x].domain.size() == 0)
		{
			int available_assignemnt = 0; //an indicatior whether there are possible possible varaibles to be re-assigned
			while (available_assignemnt == 0) {
				int cur_id = assigned_variables.top();
				int y = cur_id / 9;
				int x = cur_id % 9;
				variables[y][x].assignement = 0;
				cur_state.values[y][x] = 0;
				arcConsistency(cur_state);


				for (int i = 0; i < variables[y][x].domain.size(); i++)
				{
					State temp_state;
					temp_state = cur_state;
					temp_state.values[y][x] = variables[y][x].domain[i];
					if (std::find(repeating_list.begin(), repeating_list.end(), temp_state) == repeating_list.end()) //if not in the repeating list
					{
						cur_state = temp_state;
						variables[y][x].assignement = variables[y][x].domain[i];
						repeating_list.push_back(temp_state);
						available_assignemnt = 1;
						*chosen_cell = cur_id;
						arcConsistency(cur_state);
						return false; //get out of the current varaible assignment
					}
				}

				if (available_assignemnt == 0) //if all the domain values have been tried for current variable
				{
					variables[y][x].assignement = 0;
					cur_state.values[y][x] = 0;
					assigned_variables.pop();

				}
			}

		}
	}

	/*If there is no variable that has empty domain, then assign variable here*/
	for (int i = 0; i < 81; i++)
	{
		int y = i / 9;
		int x = i % 9;

		/*If there is any varable has not been assigned yet, assign it and return it*/
		if (cur_state.values[y][x] == 0 && variables[y][x].domain.size() > 0)
		{
			/*Find the smalles number in domain to assign it. Here no update domain for bracktrack*/
			int id_min = 0;
			cur_state.values[y][x] = variables[y][x].domain[id_min];
			variables[y][x].assignement = variables[y][x].domain[id_min];
			assigned_variables.push(i); //push the variable into stack, which will be used for backtrack (or DFS)
			repeating_list.push_back(cur_state); //make this state into the repeat_list
			*chosen_cell = 9 * y + x;

			arcConsistency(cur_state); //Every time modify the assignment update the domain

			return false;
		}

	}

	if (goalCheck(cur_state))
	{
		printf("find the goal\n");
		return true;
	}
	else
	{
		int available_assignemnt = 0; //an indicatior whether there are possible varaibles to be re-assigned
		while (available_assignemnt == 0) {
			int cur_id = assigned_variables.top();
			int y = cur_id / 9;
			int x = cur_id % 9;
			variables[y][x].assignement = 0;
			cur_state.values[y][x] = 0;
			arcConsistency(cur_state);
			for (int i = 0; i < variables[y][x].domain.size(); i++)
			{
				State temp_state;
				temp_state = cur_state;
				temp_state.values[y][x] = variables[y][x].domain[i];
				if (std::find(repeating_list.begin(), repeating_list.end(), temp_state) == repeating_list.end()) //if not in the repeating list
				{
					cur_state = temp_state;
					variables[y][x].assignement = variables[y][x].domain[i];
					repeating_list.push_back(cur_state);
					available_assignemnt = 1;
					*chosen_cell = cur_id;
					break; //get out of the current varaible assignment
				}
			}

			if (available_assignemnt == 0) //if all the domain values have been tried for current variable
			{

				assigned_variables.pop();

			}
		}

		return false;
	}
	return false;

}



/*Forward Checking algorithm*/
bool CSP::forwardChecking(int *chosen_cell)
{
	updateDomain(cur_state); //the first step is based on current setting to update the domain



	/*First go through all the variables and do backtrack whether there is an empty domain */
	for (int i = 0; i < 81; i++)
	{
		int y = i / 9;
		int x = i % 9;

		if (cur_state.values[y][x] == 0 && variables[y][x].domain.size() == 0)
		{
			int available_assignemnt = 0; //an indicatior whether there are possible possible varaibles to be re-assigned
			while (available_assignemnt == 0) {
				int cur_id = assigned_variables.top();
				int y = cur_id / 9;
				int x = cur_id % 9;
				variables[y][x].assignement = 0;
				cur_state.values[y][x] = 0;
				updateDomain(cur_state);
				for (int i = 0; i < variables[y][x].domain.size(); i++)
				{
					State temp_state;
					temp_state = cur_state;
					temp_state.values[y][x] = variables[y][x].domain[i];
					if (std::find(repeating_list.begin(), repeating_list.end(), temp_state) == repeating_list.end()) //if not in the repeating list
					{
						cur_state = temp_state;
						variables[y][x].assignement = variables[y][x].domain[i];
						repeating_list.push_back(temp_state);
						available_assignemnt = 1;
						*chosen_cell = cur_id;
						updateDomain(cur_state);
						return false; //get out of the current varaible assignment
					}
				}

				if (available_assignemnt == 0) //if all the domain values have been tried for current variable
				{
					variables[y][x].assignement = 0;
					cur_state.values[y][x] = 0;
					assigned_variables.pop();

				}
			}

		}
	}

	/*If there is no variable that has empty domain, then assign variable here*/
	for (int i = 0; i < 81; i++)
	{
		int y = i / 9;
		int x = i % 9;

		/*If there is any varable has not been assigned yet, assign it and return it*/
		if (cur_state.values[y][x] == 0 && variables[y][x].domain.size() > 0)
		{
			/*Find the smalles number in domain to assign it. Here no update domain for bracktrack*/
			int id_min = 0;
			cur_state.values[y][x] = variables[y][x].domain[id_min];
			variables[y][x].assignement = variables[y][x].domain[id_min];
			assigned_variables.push(i); //push the variable into stack, which will be used for backtrack (or DFS)
			repeating_list.push_back(cur_state); //make this state into the repeat_list
			*chosen_cell = 9 * y + x;

			updateDomain(cur_state); //Every time modify the assignment update the domain

			return false;
		}

	}

	if (goalCheck(cur_state))
	{
		printf("find the goal\n");
		return true;
	}
	else
	{
		int available_assignemnt = 0; //an indicatior whether there are possible varaibles to be re-assigned
		while (available_assignemnt == 0) {
			int cur_id = assigned_variables.top();
			int y = cur_id / 9;
			int x = cur_id % 9;
			variables[y][x].assignement = 0;
			cur_state.values[y][x] = 0;
			updateDomain(cur_state);
			for (int i = 0; i < variables[y][x].domain.size(); i++)
			{
				State temp_state;
				temp_state = cur_state;
				temp_state.values[y][x] = variables[y][x].domain[i];
				if (std::find(repeating_list.begin(), repeating_list.end(), temp_state) == repeating_list.end()) //if not in the repeating list
				{
					cur_state = temp_state;
					variables[y][x].assignement = variables[y][x].domain[i];
					repeating_list.push_back(cur_state);
					available_assignemnt = 1;
					*chosen_cell = cur_id;
					break; //get out of the current varaible assignment
				}
			}

			if (available_assignemnt == 0) //if all the domain values have been tried for current variable
			{

				assigned_variables.pop();

			}
		}

		return false;
	}
	return false;

}


/*Forward Checking algorithm*/
bool CSP::forwardCheckingOrder(int *chosen_cell)
{

	updateDomain(cur_state); //the first step is based on current setting to update the domain



	/*First go through all the variables and do backtrack whether there is an empty domain */
	for (int i = 0; i < 81; i++)
	{
		int y = i / 9;
		int x = i % 9;

		if (cur_state.values[y][x] == 0 && variables[y][x].domain.size() == 0)
		{
			int available_assignemnt = 0; //an indicatior whether there are possible possible varaibles to be re-assigned
			while (available_assignemnt == 0) {
				int cur_id = assigned_variables.top();
				int y = cur_id / 9;
				int x = cur_id % 9;
				variables[y][x].assignement = 0;
				cur_state.values[y][x] = 0;
				updateDomain(cur_state);
				for (int i = 0; i < variables[y][x].domain.size(); i++)
				{
					State temp_state;
					temp_state = cur_state;
					temp_state.values[y][x] = variables[y][x].domain[i];
					if (std::find(repeating_list.begin(), repeating_list.end(), temp_state) == repeating_list.end()) //if not in the repeating list
					{
						cur_state = temp_state;
						variables[y][x].assignement = variables[y][x].domain[i];
						repeating_list.push_back(temp_state);
						available_assignemnt = 1;
						*chosen_cell = cur_id;
						updateDomain(cur_state);
						return false; //get out of the current varaible assignment
					}
				}

				if (available_assignemnt == 0) //if all the domain values have been tried for current variable
				{
					variables[y][x].assignement = 0;
					cur_state.values[y][x] = 0;
					assigned_variables.pop();

				}
			}

		}
	}


	int count = 0;
	while (count < 81)
	{
		/*Find the index of minimum number of domain*/
		int min_idx = 0;
		int min_num = 10; //because the maximum number of domain is 10
		for (int i = 0; i < 81; i++)
		{
			int y = i / 9;
			int x = i % 9;
			if (cur_state.values[y][x] == 0 && variables[y][x].domain.size() > 0)
			{
				if (variables[y][x].domain.size() < min_num) {
					min_idx = i;
					min_num = variables[y][x].domain.size();
				}
			}
		}

		int y = min_idx / 9;
		int x = min_idx % 9;

		/*If there is any varable has not been assigned yet, assign it and return it*/
		if (cur_state.values[y][x] == 0 && variables[y][x].domain.size() > 0)
		{
			/*Find the smalles number in domain to assign it. Here no update domain for bracktrack*/
			int id_min = 0;
			cur_state.values[y][x] = variables[y][x].domain[id_min];
			variables[y][x].assignement = variables[y][x].domain[id_min];
			assigned_variables.push(min_idx); //push the variable into stack, which will be used for backtrack (or DFS)
			repeating_list.push_back(cur_state); //make this state into the repeat_list
			*chosen_cell = 9 * y + x;
\
			updateDomain(cur_state); //Every time modify the assignment update the domain

			return false;
		}

		count++;
	}

	if (goalCheck(cur_state))
	{
		printf("find the goal\n");
		return true;
	}
	else
	{
		int available_assignemnt = 0; //an indicatior whether there are possible varaibles to be re-assigned
		while (available_assignemnt == 0) {
			int cur_id = assigned_variables.top();
			int y = cur_id / 9;
			int x = cur_id % 9;
			variables[y][x].assignement = 0;
			cur_state.values[y][x] = 0;
			updateDomain(cur_state);
			for (int i = 0; i < variables[y][x].domain.size(); i++)
			{
				State temp_state;
				temp_state = cur_state;
				temp_state.values[y][x] = variables[y][x].domain[i];
				if (std::find(repeating_list.begin(), repeating_list.end(), temp_state) == repeating_list.end()) //if not in the repeating list
				{
					cur_state = temp_state;
					variables[y][x].assignement = variables[y][x].domain[i];
					repeating_list.push_back(cur_state);
					available_assignemnt = 1;
					*chosen_cell = cur_id;
					break; //get out of the current varaible assignment
				}
			}

			if (available_assignemnt == 0) //if all the domain values have been tried for current variable
			{

				assigned_variables.pop();

			}
		}

		return false;
	}

	return false;

}



/*Back Track to solve the proble*/
bool CSP::backTrack(int *chosen_cell)
{



	for (int i = 0; i < 81; i++)
	{
		int y = i / 9;
		int x = i % 9;


		/*If there is any varable has not been assigned yet, assign it and break*/
		if (cur_state.values[y][x] == 0)
		{

			/*Find the smalles number in domain to assign it. Here no update domain for bracktrack*/
			int id_min = 0;
			cur_state.values[y][x] = variables[y][x].domain[id_min];
			variables[y][x].assignement = variables[y][x].domain[id_min];
			assigned_variables.push(i); //push the variable into stack, which will be used for backtrack (or DFS)
			repeating_list.push_back(cur_state); //make this state into the repeat_list
			*chosen_cell = 9 * y + x;
			return false;

		}
	}

	/*If all the the variable are assigned*/
	{
		if (assigned_variables.size() == 0)//reset all the variables if there are no any varaibles assigned yet
		{
			for (int i = 0; i < 81; i++)
			{
				assigned_variables.push(i);
			}
		}

		if (goalCheck(cur_state))
		{
			printf("find the goal\n");
			return true;
		}
		else
		{
			int available_assignemnt = 0; //an indicatior whether there are possible varaibles to be re-assigned
			while (available_assignemnt == 0) {
				int cur_id = assigned_variables.top();
				int y = cur_id / 9;
				int x = cur_id % 9;


				for (int i = 0; i < variables[y][x].domain.size(); i++)
				{
					State temp_state;
					temp_state = cur_state;
					temp_state.values[y][x] = variables[y][x].domain[i];
					if (std::find(repeating_list.begin(), repeating_list.end(), temp_state) == repeating_list.end()) //if not in the repeating list
					{
						cur_state = temp_state;
						variables[y][x].assignement = variables[y][x].domain[i];
						repeating_list.push_back(cur_state);
						available_assignemnt = 1;
						*chosen_cell = cur_id;
						break; //get out of the current varaible assignment
					}
				}

				if (available_assignemnt == 0) //if all the domain values have been tried for current variable
				{
					variables[y][x].assignement = 0;
					cur_state.values[y][x] = 0;
					assigned_variables.pop();

				}
			}

			return false;
		}
	}
} 