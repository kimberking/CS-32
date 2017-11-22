class Animal { //the parent class

public:
	Animal(string name);
	virtual ~Animal() {}
	virtual string name() const { return(mName); }
	virtual void speak() const = 0; //make it pure because animal cant speak
	virtual string moveAction() const { return("walk"); }

private:
	string mName;
};

Animal::Animal(string name) :mName(name) {}

class Cat : public Animal { //cat is a subclass of animal

public:
	Cat(string name) :Animal(name) {};
	virtual	~Cat();
	virtual void speak() const;
};

void Cat::speak() const
{
	cout << "Meow";
}

Cat::~Cat()
{
	cout << "Destroying " << Animal::name() << " the cat" << endl;
}

class Pig : public Animal //pig is a subclass of animal
{
public:
	Pig(string name, int weight) :Animal(name), m_weight(weight) {};
	virtual ~Pig();
	virtual void speak() const;
private:
	int m_weight;
};

void Pig::speak() const
{
	if (m_weight < 170) //the weight specified 
		cout << "Oink";
	else
		cout << "Grunt";
}

Pig::~Pig() {
	cout << "Destroying " << Animal::name() << " the pig" << endl;
}

class Duck : public Animal { //duck is a subclass

public:
	Duck(string name) :Animal(name) {};
	virtual ~Duck();
	virtual void speak() const;
	virtual string moveAction() const;
};

void Duck::speak() const
{
	cout << "Quack";
}

string Duck::moveAction() const
{
	return("swim"); //swim for ducks
}

Duck::~Duck()
{
	cout << "Destroying " << Animal::name() << " the duck" << endl;
}


