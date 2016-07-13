// Genetic Insects.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//#include "Utils.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "Utils.h"

using std::vector;





void SetBit(int &dna, int idx)
{
	dna |= (1 << idx);
}

bool CheckBit(int &dna, int idx)
{
	return dna & (1 << idx);
}

void ResetBit(int &dna, int idx)
{
	dna &= ~(1 << idx);
}


namespace SIMPLE_GA {

	class InsectChromosome1
	{
	public:

		int dna = 0;

		float fitness	= 0.0f;

		int target		= 0;
		int numTraits	= 8;
		int numGenes	= 25;

		int ANTENNAE_MASK	= 0;
		int HEAD_MASK		= 0;
		int WINGS_MASK		= 0;
		int BODY_MASK		= 0;
		int FEET_MASK		= 0;
		int BODY_COLOR_MASK = 0;
		int SIZE_MASK		= 0;
		int HEAD_COLOR_MASK = 0;

		int antennae_start		= 0;
		int head_start			= 4;
		int wing_start			= 6;
		int body_start			= 10;
		int feet_start			= 16;
		int body_color_start	= 17;
		int size_start			= 20;
		int head_color_start	= 22;
		int head_color_end		= 25;

		InsectChromosome1()
		{
			// compute the masks
			for (int i = antennae_start; i < head_start; i++)		SetBit(ANTENNAE_MASK, i);
			for (int i = head_start; i < wing_start; i++)			SetBit(HEAD_MASK, i);
			for (int i = wing_start; i < body_start; i++)			SetBit(WINGS_MASK, i);
			for (int i = body_start; i < feet_start; i++)			SetBit(BODY_MASK, i);
			for (int i = feet_start; i < body_color_start; i++)		SetBit(FEET_MASK, i);
			for (int i = body_color_start; i < size_start; i++)		SetBit(BODY_COLOR_MASK, i);
			for (int i = size_start; i < head_color_start; i++)		SetBit(SIZE_MASK, i);
			for (int i = head_color_start; i < head_color_end; i++) SetBit(HEAD_COLOR_MASK, i);
		}

		void SetAntennae(int choice)
		{
			dna |= (ANTENNAE_MASK & (choice));
		}
		void SetHead(int choice)
		{
			dna |= (HEAD_MASK & (choice << head_start));
		}
		void SetWing(int choice)
		{
			dna |= (WINGS_MASK & (choice << wing_start));
		}
		void SetBody(int choice)
		{
			dna |= (BODY_MASK & (choice << body_start));
		}
		void SetFeet(int choice)
		{
			dna |= (FEET_MASK & (choice << feet_start));
		}
		void SetBodyColor(int choice)
		{
			dna |= (BODY_COLOR_MASK & (choice << body_color_start));
		}
		void SetSize(int choice)
		{
			dna |= (SIZE_MASK & (choice << size_start));
		}
		void SetHeadColor(int choice)
		{
			dna |= (HEAD_COLOR_MASK & (choice << head_color_start));
		}
		int GetAntennae()
		{
			return dna & (ANTENNAE_MASK);
		}
		int GetHead()
		{
			return ((dna & HEAD_MASK) >> head_start);
		}
		int GetWing()
		{
			return ((dna & WINGS_MASK) >> wing_start);
		}
		int GetBody()
		{
			return ((dna & BODY_MASK) >> body_start);
		}
		int GetFeet()
		{
			return ((dna & FEET_MASK) >> feet_start);
		}
		int GetBodyColor()
		{
			return ((dna & BODY_COLOR_MASK) >> body_color_start);
		}
		int GetSize()
		{
			return ((dna & SIZE_MASK) >> size_start);
		}
		int GetHeadColor()
		{
			return ((dna & HEAD_COLOR_MASK) >> head_color_start);
		}



		void Fitness() {
			int score = 0;
			for (int i = 0; i < this->numGenes; i++) {
				//Is the character correct ?
				if (CheckBit(dna, i))
				{
					if (CheckBit(target, i))
						score++;
				}
				else
				{
					if (!CheckBit(target, i))
						score++;
				}

			}
			//Fitness is the percentage correct.
			fitness = float(score) / (float)numGenes;
			//fitness = fitness * fitness;
		}

		void mutate(float mutationRate) {
			
			if (RandomFloat(0.0, 1.0) < mutationRate) {
				int gene_to_mutate = RandomInt(0, 25);
				
				if (dna & (1 << gene_to_mutate))
				{
					dna |= (0 << gene_to_mutate);
				}
				else dna |= (1 << gene_to_mutate);
			}
		}

		int crossoverSinglePoint(int dnaB)
		{
			int cross_point = RandomInt(0, 25);

			int output = 0;

			for (int i = 0; i < cross_point; i++)
			{
				if (CheckBit( dna, i ))
					SetBit(output, i);
			}
			for (int i = cross_point; i < 32; i++)
			{
				if (CheckBit(dnaB, i))
					SetBit(output, i);
			}

			return output;
		}

		int crossoverTwoPoint(int dnaB)
		{
			int cross_point_1 = RandomInt(0, 25);
			int cross_point_2 = 0;

			if (cross_point_1 > 12)
			{
				cross_point_2 = RandomInt(0, 12);
				// swap them
				int temp = cross_point_1;
				cross_point_1 = cross_point_2;
				cross_point_2 = temp;
			}
			else
			{
				cross_point_2 = RandomInt(12, 25);
			}

			int output = 0;

			for (int i = 0; i < cross_point_1; i++)
			{
				if (CheckBit(dna, i))
					SetBit(output, i);
			}

			for (int i = cross_point_1; i < cross_point_2; i++)
			{
				if (CheckBit(dnaB, i))
					SetBit(output, i);
			}
			for (int i = cross_point_2; i < 32; i++)
			{
				if (CheckBit(dna, i))
					SetBit(output, i);
			}

			return output;

		}

		int uniformCrossover(int dnaB)
		{
			int mask = 0;
			for (int i = 0; i < 25; i++)
			{
				SetBit( mask, RandomInt(0, 2) );
			}

			int output = 0;

			output = (dna & mask) | (dnaB & ~mask);
			
			return output;
		}

		int heuristicCrossover(int dnaB, float alpha)
		{
			int output = 0;

			output = (int) ((float)dna * alpha) + (1 - alpha) * (float)dnaB;

			return output;
		}

		void SetTarget(int t){ target = t; }

	};



	class Population
	{
	public:

		vector<InsectChromosome1*> population;

		unsigned int population_size	= 200;
		float mutationRate				= 0.03;
		float sumFitness				= 0.0f;
		float best_fitness				= 0.0f;
		int best_index					= 0;
		bool match						= false;
		int target						= 0;

		InsectChromosome1 temp_mating_insect_storage;
		
		std::vector<int> mating_pool;
		std::vector<int> last_phase_mating_pool;

		std::vector<int> *mating_pool_ptrA = 0;
		std::vector<int> *mating_pool_ptrB = 0;
		std::vector<int> *temp_mating_pool_ptr = 0;

		std::ofstream myfile;

		Population()
		{
			InsectChromosome1 c1;

			// setup the target insect with predetermined choices

			c1.SetHead(2);
			c1.SetAntennae(4);
			c1.SetBody(34);
			c1.SetWing(5);
			c1.SetFeet(1);
			c1.SetBodyColor(3);
			c1.SetSize(2);
			c1.SetHeadColor(1);

			target = c1.dna;
		}

		Population(int size)
		{
			population_size = size;

			// setup the target insect with predetermined choices
			InsectChromosome1 c1;

			c1.SetHead(2);
			c1.SetAntennae(4);
			c1.SetBody(34);
			c1.SetWing(5);
			c1.SetFeet(1);
			c1.SetBodyColor(3);
			c1.SetSize(2);
			c1.SetHeadColor(1);

			target = c1.dna;
		}

		~Population()
		{
			if (population.size() > 0)
			{
					for (int i = 0; i < population.size(); i++)
					{
						// Safe Delete
						if ( population[i] )delete population[i];
						population[i] = 0;
					}

					population.clear();
				
			}

			myfile.close();
		}

		int GetBest()
		{ 
			return population[best_index]->dna; 
		}

		void Initialize()
		{
			myfile.open("example_stats6.csv");

			for (int i = 0; i < population_size; i++) {
				
				//Initializing each member of the population
				InsectChromosome1* dna = new InsectChromosome1();
				
				dna->SetTarget(target);
				population.push_back(dna);
				mating_pool.push_back(dna->dna);
				last_phase_mating_pool.push_back(dna->dna);

				myfile << i << ",";
			}
			myfile <<"mean," << std::endl;

			mating_pool_ptrA = &mating_pool;
			mating_pool_ptrB = &last_phase_mating_pool;
		}

		//
		// Worst case. this is O(n^2) because we have the inner loop in the second phase
		//
		void Update()
		{
			int best = 0;
			best_index = 0;
			best_fitness = 0.0f;
			sumFitness = 0.0f;

			float sum_last_fitness = sumFitness;

			//evaluate fitness
			for (int i = 0; i < population.size(); i++) {
				
				population[i]->Fitness();

				sumFitness += population[i]->fitness;

				if (population[i]->fitness > best_fitness)
				{
					best_fitness = population[i]->fitness;
					best_index = i;
				}


				mating_pool[i] = population[i]->dna;
					//mating_pool.push_back(population[i]->dna);
			}

			int best_value = this->GetBest();

			float mean_fitness = sumFitness / population.size();
			static int epoch = 0;
			//myfile << "EPOCH " << epoch << ", ";
		//	myfile << "SUM FITNESS: " << sumFitness << ", SUM FITNESS*100: " << sumFitness*100 << std::endl;


			for (int i = 0; i < population.size(); i++) {

				//std::string s = string_dna(population[i]->dna);
				myfile /*<< s << "," */ << population[i]->fitness << ",";// << std::endl;

				if (i == this->best_index)
				{
					
					continue;
				}

				int a = this->ChooseParent(-1);
				int b = this->ChooseParent(a);

				if (a == b) if (a <= population.size() - 2)b = a + 1; else b = 0;

				int partnerA = mating_pool[a]; 
				int partnerB = mating_pool[b];

				temp_mating_insect_storage.dna = partnerA;

				//Step 3a: Crossover  (Note some of the other crossover operations have not been implemented)
				population[i]->dna = temp_mating_insect_storage.crossoverSinglePoint(partnerB);
				
				//Step 3b: Mutation
				population[i]->mutate(mutationRate);




				
			}
			myfile <<mean_fitness << "," << std::endl;

			epoch++;
			//mating_pool.clear();
		}


		// 
		int ChooseParent(int parent_to_skip)
		{
			int randSelector = (int)RandomFloat(0, sumFitness) * 100;

			int memberID = 0;
			int partialSum = 0;

			for (int i = 0; i < population.size(); i++) {
				
				int n = (int)(population[i]->fitness * 100);
				if (partialSum < randSelector && partialSum + n >= randSelector)
				{
					if (i == parent_to_skip)
					{
						if (i + 1 == population.size()) return best_index; // more breeding with population best
						else return i + 1;
					}
					else
					{
						return i;
					}
				}
				partialSum += n;
			}

			return this->best_index;
		}


		void Delete()
		{
			for (int i = 0; i < population.size(); i++)
			{
				delete population[i];
				population[i] = 0;
			}

			population.clear();
		}

		void print_dna(int g)
		{
			for (int i = 0; i < 32; i++)
			{
				if (CheckBit(g, i)) std::cout << "1";
				else std::cout << "0";
			}
		}

		std::string string_dna(int g)
		{
			std::string ret = "";
			for (int i = 0; i < 32; i++)
			{
				if (CheckBit(g, i)) ret += "1";
				else ret += "0";
			}
			return ret;
		}
	};

	
};

using namespace SIMPLE_GA;

//
//  Object: SimpleExample
//  abstract base class
//
class SimpleExample
{
protected:
	bool m_bDeleted = false;
public:

	SimpleExample(){}
	~SimpleExample(){
		if (m_bDeleted == false)
			this->Delete();
	}

	virtual void Initialize(){}
	virtual void Update(){};
	virtual void Draw(){};
	virtual void Delete(){};
	virtual void keyboard(unsigned char key, int x, int y){};
	virtual void keyboardup(unsigned char key, int x, int y){};
	virtual void Special(int key, int x, int y){};

	virtual void mouse(int button, int state, int x, int y){}
};

//
//  Object: SimpleExample
//  Simply runs the code 
//
class SimpleGA : public SimpleExample
{
public:

	int population_size;


	float mutationRate = 0.01;

	bool solved = false;


	Population population3;



	void SetPopulationSize(int n)
	{ 
		this->population_size = n; 
	}

	void Initialize()
	{
		// seed random number generator ...
		int randSelector = (int)RandomFloat(0, 23);
		 randSelector = (int)RandomFloat(0, 23);
		 randSelector = (int)RandomFloat(0, 23);

		population3.Initialize();
	}

	void Draw() {}

	void Update() {

		if (solved == false)
		{
			population3.Update();

			static int generation_count = 0;
			std::cout << "generation" << generation_count++ << std::endl;//
			
			population3.print_dna(population3.GetBest());
			std::cout << std::endl;
			population3.print_dna(population3.target);

			
			std::cout << std::endl;
			
			if (population3.best_fitness > 0.97f)
			{
				solved = true; // print statistics
				std::cout << "****************************************************" << std::endl;
				std::cout << std::endl << "Insect Chosen Characteristics" << std::endl;
				int best = population3.best_index;
				if (best < population3.population.size())
				{
					std::cout << "Antennae Type: " << population3.population[best]->GetAntennae() << std::endl;
					std::cout << "Head Type: " << population3.population[best]->GetHead() << std::endl;
					std::cout << "Wing Type: " << population3.population[best]->GetWing() << std::endl;
					std::cout << "Body Type: " << population3.population[best]->GetBody() << std::endl;
					std::cout << "Size Type: " << population3.population[best]->GetSize() << std::endl;
					std::cout << "Body Color: " << population3.population[best]->GetBodyColor() << std::endl;
					std::cout << "Feet: " << population3.population[best]->GetFeet() << std::endl;
					std::cout << "Head Color: " << population3.population[best]->GetHeadColor() << std::endl ;
					std::cout << std::endl << "****************************************************" << std::endl;
				}
			}
		}
	}

	void keyboard(unsigned char key, int x, int y){}
	void keyboardup(unsigned char key, int x, int y){}
	void Special(int key, int x, int y){};

	void mouse(int button, int state, int x, int y){}

	void Delete(){
		
		population3.Delete();
		//mating_pool.clear();
		this->m_bDeleted = true;

	}
};




/*
ANTENNAE_MASK 00001111111111111111111111111111
HEAD_MASK	  11110001111111111111111111111111
WINGS_MASK	  11111110000111111111111111111111
BODY_MASK	  11111111111000000111111111111111
FEET_MASK	  11111111111111111011111111111111
BCOLOR_MASK	  11111111111111111100011111111111
SIZE_MASK	  11111111111111111111100111111111
HCOLOR_MASK	  11111111111111111111111000111111
*/



int _tmain(int argc, _TCHAR* argv[])
{


	SimpleGA *example = new  SimpleGA();
	example->Initialize();
	while (example->solved == false)
		example->Update();
	example->Delete();
	
	return 0;
}

