#include <stdlib.h>
#include <stdio.h> // 1. Error

typedef struct {
	char* classname; // 2. Error
	int base_strength;
	int base_wisdom;
} Class;

typedef struct {
	char* name;
	int age;
	Class cla; // 3. Error
} Character;

typedef struct {
	Character members[2];
} Party;

Class* initClasses(Class* allowed_classes) {
	allowed_classes = (Class*)malloc(3 * sizeof(Class)); // 4. Error
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

int main() // 5. Error
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
	myparty.members[0] = char1;// 6. Error
	myparty.members[1] = char2; // 7. Error

	printf("%s is a %s\n", myparty.members[0].name, myparty.members[0].cla.classname);// 8.,9.,10. Error
	printf("%s is a %s\n", myparty.members[1].name, myparty.members[1].cla.classname); // 11.,12. Error

    free((char*)classes); //13. Error
	return 0;
}