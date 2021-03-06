#include "Story.h"
#include <ios>


Story* controller(int *d) {
	FILE * fp;
	fp = fopen("controll.txt", "r");
	if (fp == NULL)
	{
			
		throw ios_base::failure("File not found");
	}

	int n, index, left, right, ex;
	char text[4000];
	ex=fscanf(fp, "#%d", &n);
	if (!ex)
	{
		throw ios_base::failure("Couldn't scan the number of coming elements");
	}
	*d = n;
	Story * array = new Story[n];
	for (int i = 0; i < n; i++)
	{
		ex=fscanf(fp, "%d\t%d\t%d\t", &index, &left, &right);
		if (ex==0)
		{
			throw ios_base::failure("Couldn't scan the data");
		}
		if (fgets(text, 3999, fp)==NULL)
		{
			throw ios_base::failure("Couldn't scan the text");
		}
		array[index] = Story(index, left, right, text);
	}
	fclose(fp);
	return array;
}

Creature* createcreatures(int * d) {
	FILE * fp = fopen("creature.txt", "r");
	if (fp == NULL)
	{
		throw ios_base::failure("File not found");
	}
	int n;
	if (fscanf(fp, "#%d", &n)==0)
	{
		throw ios_base::failure("Couldn't scan the number of elements coming");
	}
	*d = n;
	double hp, basedmg;
	char name[30];
	Creature * array1 = new Creature[n];
	for (int i = 0; i < n; i++)
	{
		if (fscanf(fp, "%lf\t%s\t%lf", &hp, name, &basedmg)==0)
		{
			throw ios_base::failure("Couldn't scan data");
		}
		array1[i] = Creature(hp, name, basedmg, hp);
	}
	fclose(fp);
	return array1;
}

Pokemon* createpokemons(int *d)
{
	FILE *fp = fopen("pokemons.txt", "r");
	if (fp == NULL)
	{
		throw ios_base::failure("File not found");
	}
	int a;
	if (0==fscanf(fp, "#%d", &a))
	{
		throw ios_base::failure("Couldn't scan the number of elements coming.");
	}
	*d = a;
	double hp, basedmg, succfak, failfak;
	int own, weakness, lvl, growth, abcount;
	Type own1, weakness1;
	char name[20];
	Move v;
	Pokemon * array2 = new Pokemon[a];
	for (int i = 0; i < a; i++)
	{
		if (fscanf(fp, "%lf\t%s\t%lf\t%d\t%d\t%d\t%d\n", &hp, name, &basedmg, &own, &weakness, &lvl, &growth) == 0)
		{
			throw ios_base::failure("Couldn't scan data");
		}
		switch (own) {
		case 1: own1 = Water; break;
		case 2: own1 = Fire; break;
		case 3: own1 = Grass; break;
		default: break; //exception? 
		}
		switch (weakness) {
		case 1: weakness1 = Water; break;
		case 2: weakness1 = Fire; break;
		case 3: weakness1 = Grass; break;
		default: break; //exception?

		}
		array2[i] = Pokemon(hp, name, basedmg, hp, own1, weakness1, lvl, growth);
		if (fscanf(fp, "#%d", &abcount) == 0)
		{
			throw ios_base::failure("Couldn't scan the number of elements coming");
		}
		for (int k = 0; k < abcount; k++) {
			if (0 == fscanf(fp, "%d\t%s\t%lf\t%lf", &own, name, &succfak, &failfak))
			{
				throw ios_base::failure("Couldn't scan data");
			}
			switch (own) {
			case 1: own1 = Water; break;
			case 2: own1 = Fire; break;
			case 3: own1 = Grass; break;
			default: own1 = Neutral; break;
			}
			Move v(own1, name, succfak, failfak);
			array2[i].linkAbility(v);
		}
	}
	fclose(fp);
	return array2;
}

Trainer* createArena(Pokemon * array, int d) {
	FILE * fp = fopen("arena.txt", "r");
	if (fp == 0)
	{
		throw ios_base::failure("File not found");
	}
	int n;
	if (0 == fscanf(fp, "#%d", &n))
	{
		throw ios_base::failure("Couldn't scan the number of elements coming");
	}
	Trainer * op = new Trainer[n];
	char name[20];
	int g, balls, potions, elementNum, selectedIndex;
	int a = d - 3; // Ne legyen olyan pokemonja a t�bbi trainernek mint nek�nk
	for (int i = 0; i < n; i++)
	{
		if (0 == fscanf(fp, "%s\t%d\t%d\t%d\t%d", name, &balls, &potions, &elementNum, &selectedIndex))
		{
			throw ios_base::failure("Couldn't scan data");
		}
		op[i] = Trainer(name, balls, potions, elementNum, selectedIndex);
		g = rand() % a + 3;
		op[i].linktoArray(array[g]);
	}
	fclose(fp);
	return op;
}

int main() {
	srand(time(NULL));
	int crethossz;
	int storyhossz;
	int pokhossz;
	Story * control;
	Creature * creatures;
	Pokemon * pokemons;
	Trainer * arenaop;
	try { control = controller(&storyhossz); }
	catch (ios_base::failure& ex) { cout << ex.what(); exit(1); }
	try { creatures = createcreatures(&crethossz); }
	catch (ios_base::failure& ex) { cout << ex.what(); exit(1); }
	try { pokemons = createpokemons(&pokhossz); }
	catch (ios_base::failure& ex) { cout << ex.what(); exit(1); }
	try { arenaop = createArena(pokemons, pokhossz); }
	catch (ios_base::failure&ex) { cout << ex.what(); exit(1); }
	char choice;
	int dice, currentnode = 0, opponents = 0;
	string name;


	cout << "Welcome! Type your name: "; cin >> name; Trainer Player(name, 10, 2, 0, 0);
	cout << "Pokemon name\tHP\tStatus" << endl;
	Player.linktoArray(pokemons[0]);
	Player.linktoArray(pokemons[1]);
	Player.linktoArray(pokemons[2]);
	Player.list();

	control[currentnode].Print();

	do {
		cout << "Choices: (R)IGHT, (L)EFT, (H)EAL, E(X)IT" << endl;
		cin >> choice;
		switch (choice) {
		case 'R': {currentnode = control[currentnode].getright(); control[currentnode].Print(); 
			if (control[currentnode].getright() == control[currentnode].getleft() && control[currentnode].getright()== storyhossz-1){
				choice = 'X';
				break;
			}
			else if (control[currentnode].getid()>= control[currentnode].getarena()) {

				Player.battle(arenaop[opponents].getpokemons()[0]); break;
			}

			else if(control[currentnode].getid()> control[currentnode].getwild() && control[currentnode].getid()< control[currentnode].getarena())
			{
				dice = rand() % 3; if (dice == 2)
					Player.battle(creatures[rand() % (crethossz)-1]); break;
				
			}

		} WAIT break;
		case 'L': {currentnode = control[currentnode].getleft(); control[currentnode].Print();
			if (control[currentnode].getright() == control[currentnode].getleft()) {
				choice = 'X';
					break;
			}
			else if (control[currentnode].getid() > control[currentnode].getarena()) {

				Player.battle(arenaop[opponents].getpokemons()[0]); break;
			}

				
			else if ((control[currentnode].getid() >= control[currentnode].getwild()) && (control[currentnode].getid() <= control[currentnode].getarena()))
			{
				dice = rand() % 3; if (dice == 2)
					Player.battle(creatures[rand() % (crethossz)-1]); break;

			}

		} WAIT break;
		case 'H': Player.list(); cin >> dice; Player.usePotion(dice); WAIT break;
		case 'X': cout << "Are you sure, you want to quit? Y-Yes | N-No" << endl; cin >> choice; if (choice == 'Y') { choice = 'X'; WAIT break; }
				  else if (choice == 'N') { choice = 'Y'; WAIT break; }
		default:  cout << "Wrong Input" << endl; WAIT break;
		}
	} while (choice != 'X' || !(Player.alivePokemons()));
	
	cout << "And the END is here. Don't forget to come back!:)" << endl;
	cout << "Type something, to exit: ";
	cin >> choice;



	getchar();
	return 0;
}