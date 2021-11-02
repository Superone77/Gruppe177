#include <stdlib.h>
#include <stdio.h>

typedef struct {
	char* classname;
	int base_strength;
	int base_wisdom;
} Class;

typedef struct {
	char* name;
	int age;
	Class cla;
} Character;

typedef struct {
	Character members[2];
} Party;

Class* initClasses(Class* allowed_classes) {
	allowed_classes = (Class*)malloc(3 * sizeof(Class));
	allowed_classes[0].classname = "Knight";
	allowed_classes[0].base_strength = 30;
	allowed_classes[0].base_wisdom = 10;

	allowed_classes[1].classname = "Sorceress";
	allowed_classes[1].base_strength = 10;
	allowed_classes[1].base_wisdom = 30;

	allowed_classes[2].classname = "Thief";
	allowed_classes[2].base_strength = 20;
	allowed_classes[2].base_wisdom = 20;

	return allowed_classes;
}

int main()
{
	Class* classes = NULL;
	classes = initClasses(classes);

	Character char1, char2;
	char1.name = "Harald";
	char1.age = 44;
	char1.cla = classes[0];

	char2.name = "Lydia";
	char2.age = 28;
	char2.cla = classes[2];

	Party myparty;
	myparty.members[0] = char1;
	myparty.members[0] = char2;

	printf("%s is a %s\n", char1.name, char1.cla.classname);
	printf("%s is a %s\n", char2.name, char2.cla.classname);

	free(classes);
	return 0;
}