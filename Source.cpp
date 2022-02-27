#include<list>
#include<map>
#include<string>
#include<fstream>
#include<set>
#include<climits>
#include<iostream>
#include<vector>
#include<mutex>
using namespace std;

struct Project
{
	string name;
	unsigned int Di;//an integer Di(1 ≤Di ≤ 105)
		//– the number of days it takes to complete the project,
		//an integer Si
	unsigned int Si;//(1 ≤ Si ≤ 105)
		//– the score awarded for project’s completion,
	unsigned int Bi;//an integer Bi
		//(1 ≤ Bi ≤ 105)
		//– the “best before” day for the project,
	unsigned int Ri;//an integer Ri
        //(1 ≤ Ri ≤ 100) – the number of roles in the project
	map<string, list<unsigned int>> skillsetRequired;
	list<string> skillsName;
};
int main()
{
	map<string, map<string, unsigned int>> employees;
	map < string, map<string, unsigned int>> skills;
	map < string, pair<string, Project>> projects;
	map < string, Project> projectInterm;
	unsigned int noProjects;
	unsigned int noContributors;
	std::ifstream myfile;
	myfile.open("f_find_great_mentors.in.txt");

	//TO DO

	std::string myText;
	std::getline(myfile, myText);
	noContributors = atoi(myText.substr(0, myText.find(' ')).c_str());
	myText = myText.substr(myText.find(' ') + 1);
	noProjects = atoi(myText.substr(0, myText.find(' ')).c_str());

	for (unsigned int i = 0; i < noContributors; i++)
	{
		std::getline(myfile, myText);
		string name = myText.substr(0, myText.find(' '));
		myText = myText.substr(myText.find(' ') + 1);
		unsigned int noSkils = atoi(myText.substr(0, myText.find(' ')).c_str());
		map<string, unsigned int> skillset;
		for (unsigned int j = 0; j < noSkils; j++)
		{
			std::getline(myfile, myText);
			string nameOFSkill = myText.substr(0, myText.find(' '));
			myText = myText.substr(myText.find(' ') + 1);
			unsigned int level = atoi(myText.substr(0, myText.find(' ')).c_str());
			skillset[nameOFSkill] = level;
			if (skills.find(nameOFSkill) == skills.end())
			{
				map<string, unsigned int> listOFPersons;
				listOFPersons[name] = level;
				skills[nameOFSkill] = listOFPersons;
			}
			else
			{
				skills[nameOFSkill][name] = level;
			}
		}
		employees[name] = skillset;
	}

	for (unsigned int i = 0; i < noProjects; i++)
	{
		Project p;
		std::getline(myfile, myText);
		string name = myText.substr(0, myText.find(' '));
		myText = myText.substr(myText.find(' ') + 1);
		p.Di = atoi(myText.substr(0, myText.find(' ')).c_str());
		myText = myText.substr(myText.find(' ') + 1);
		p.Si = atoi(myText.substr(0, myText.find(' ')).c_str());
		myText = myText.substr(myText.find(' ') + 1);
		p.Bi = atoi(myText.substr(0, myText.find(' ')).c_str());
		myText = myText.substr(myText.find(' ') + 1);
		p.Ri = atoi(myText.substr(0, myText.find(' ')).c_str());

		map<string, list<unsigned int>> skillset;
		for (unsigned int j = 0; j < p.Ri; j++)
		{
			std::getline(myfile, myText);
			string nameOFSkill = myText.substr(0, myText.find(' '));
			myText = myText.substr(myText.find(' ') + 1);
			unsigned int level = atoi(myText.substr(0, myText.find(' ')).c_str());
			skillset[nameOFSkill].push_back(level);
			p.skillsName.push_front(nameOFSkill);
		}
		p.skillsetRequired = skillset;
		projectInterm[name] = p;
	}

	myfile.close();

	int cnt = 1;
	for(auto i:projectInterm)
	{

		double score = (double)i.second.Di/ (double)i.second.Si;
		if (projects.find(to_string(score)) != projects.end())
		{
			score = score + 0.00001*(double)cnt;
			cnt++;
		}
		projects[to_string(score)] = pair<string, Project>(i.first, i.second);
	}

	vector<pair < string, list<string>>> assignments;// project with list of employees
    map<string, bool> employeeBusy;
	map<string, unsigned int> busyFor;

	unsigned int counter = 0;
	for (auto i: employees)
	{
		busyFor[i.first] = 0;
	}
	while (counter < 106 && !projects.empty())
	{
		for (auto i : busyFor)
		{
			if (i.second)
			{
				i.second--;
				if (!i.second)
				{
					employeeBusy[i.first] = false;
				}
			}
		}
		list<string> projectsToErase;
		for (auto i : projects)
		{
			bool isDoable = true;
			for (auto j : i.second.second.skillsetRequired)
			{
				for (auto z : j.second)
				{
					bool breaked = false;
					for (auto emp : skills[j.first])
					{
						if (emp.second >= z && !employeeBusy[emp.first])
						{
							breaked = true;
							break;
						}
					}
					if (!breaked)
					{
						isDoable = false;
						break;
					}
				}
				if (!isDoable)
				{
					break;
				}

			}
			if (isDoable)
			{
				list<string> listAsign;
				list<string> toBusy;
				map<string,list<string>> increaseSkill;
				for (auto nameSkill : i.second.second.skillsName)
				{
					auto j = i.second.second.skillsetRequired[nameSkill];
					for (auto z : j)
					{
						bool breaked = false;
						for (auto &emp : skills[nameSkill])
						{
							if (emp.second >= z && !employeeBusy[emp.first])
							{
								toBusy.push_back(emp.first);
								busyFor[emp.first] = i.second.second.Di;
								employeeBusy[emp.first] = true;
								listAsign.push_front(emp.first);
								breaked = true;
								if (z == emp.second)
								{
									increaseSkill[emp.first].push_back(nameSkill);
								}
								break;
							}
						}
						if (!breaked)
						{
							isDoable = false;
							break;
						}
					}

					if (!isDoable)
					{
						break;
					}
				}
				if (!isDoable)
				{
					for (auto g : toBusy)
					{
						employeeBusy[g] = false;
						busyFor[g] = 0;
					}
				}
				else
				{
					for (auto g : increaseSkill)
					{
						for (auto h : g.second)
						{
							employees[g.first][h]++;
						}
					}
					projectsToErase.push_back(i.first);

					assignments.push_back(pair<string, list<string>>(i.second.first, listAsign));
				}

			}
		}
		for (auto i : projectsToErase)
		{
			projects.erase(i);
		}
		counter++;
	}

	std::ofstream myfile1;
	myfile1.open("output_F1.txt");
	myfile1 << assignments.size() << endl;
	for (const auto &i : assignments)
	{
		myfile1 << i.first << endl;
		for (const auto &j : i.second)
		{
			myfile1 << j << " ";
		}
		myfile1 << endl;
	}

	myfile1.close();
}