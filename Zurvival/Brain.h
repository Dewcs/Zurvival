#pragma once

class Brain {
	private:
		unsigned input, output;
	public:
		Brain(int input, int output);
		Brain(const Brain &obj);
		~Brain();
		float* eval(float* input);
		void randomize();
		void tweak();
		
};

