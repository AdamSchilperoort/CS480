#ifndef STR_UTILS
#define STR_UTILS

//ALL STRING-RELATED UTILITIES

//RETURNS STRING LENGTH
int strLen(const char* string);

//COPIES ONE STRING INTO ANOTHER
void strCopy(char *src, char *dest);

//READS THE LAST 4 OF A STRING TO DETERMINE THE EXTENSION
char* getFileExt(char* src);

//COMPARES TWO STRINGS
int strCmp(const char* src, char* target);

//CHECKS TO SEE IF STRING HAS SUBSTRING
int strContains(char* src, char* substr);

//RETURNS END SYMBOL POSITION OF SUBSTRING IN STRING
int substrPos(char *src, char *target);

//REVERSES A STRING IN PLACE
void rev_str(char *src);

//CREATES A SUBSTRING FROM INDEX START TO END AND STORES IT IN substr
void substr(char *src, int start, int end, char *substr);

//REMOVES ALL NEWLINE CHARACTERS IN A STRING
void removeNewline(char *src);

//REMOVES ALL NON_SYMBOL CHARACTERS
void removeNonSymbols(char *src);

//CONVERTS A STRING TO A FLOAT VALUE
float s2f(char *src);

//converts a string into an int value
int s2i(char *src);

//converts a character into an int value
int c2i(char src);

//CHECKS PROVIDED STRING AGAINST COMPATIBLE SCHEDULER TYPES
int checkCpuSched(char *src);

//CHECKS STRING AGAINST COMPATIBLE LOG TO TYPES
int checkLogTo(char *src);

//SETTS ALL VALUES IN A STRING TO NULL
void strClear(char *src);

//REMOVES ALL NON-NUMBERS FROM A STRING
void strRmNonNumbers(char *src);

//CHECKS IF A CHARACTER IS A NUMBER
int charIsNum(char src);

//CHECKS IF A CHARACTER IS UPPERCASE
int charIsUpper(char src);

#endif

