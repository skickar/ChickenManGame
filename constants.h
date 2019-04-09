#ifndef CONSTANTS_H
#define CONSTANTS_H

const int NUM_PASSWORDS = 15;

const char prefix0[] = "Chicken_Easy_";
const char prefix1[] = "Chicken_Medium_";
const char prefix2[] = "Chicken_Hard_";
  
const char* easyPass[] PROGMEM = {
	"password\0",
	"123456789\0",
	"12345678\0",
	"1q2w3e4r\0",
	"sunshine\0",
	"football\0",
	"1234567890\0",
	"computer\0",
	"superman\0",
	"internet\0",
	"iloveyou\0",
	"1qaz2wsx\0",
	"baseball\0",
	"whatever\0",
	"princess\0"
};

const char* mediumPass[] PROGMEM = {
	"sweetheart\0",
	"overlord\0",
	"michaela\0",
	"meredith\0",
	"buttercup\0",
	"abc12345\0",
	"aardvark\0",
	"Passw0rd\0",
	"12345678910\0",
	"universal\0",
	"trinidad\0",
	"thursday\0",
	"standard\0",
	"pearljam\0",
	"anonymous\0"
};

const char* hardPass[] PROGMEM = {
	"prairie chicken\0",
	"chicken-breasted\0",
	"chicken broth\0",
	"chicken cholera\0",
	"chickenpox\0",
	"chicken pox\0",
	"chicken roost\0",
	"Chicken scratch\0",
	"chicken septicemia\0",
	"CHICKEN SHACK\0",
	"chicken snake\0",
	"chickensoup\0",
	"chicken-spirited\0",
	"chickens-toes\0",
	"chicken thief\0"
};

const char* numberAdd[] PROGMEM = {
	"One\0",
	"Two\0",
	"Three\0",
	"Four\0",
	"Five\0",
	"Six\0",
	"Seven\0",
	"Eight\0",
	"Nine\0",
	"Ten\0",
	"Eleven\0",
	"Twelve\0",
	"Thirteen\0",
	"Fourteen\0",
	"Fifteen\0"
};

#endif

