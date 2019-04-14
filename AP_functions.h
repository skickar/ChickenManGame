#ifndef AP_FUNCS
#define AP_FUNCS

#include "constants.h"

// assuming each list has at least NUM_PASSWORDS
const char* getPassword(int diff, int randomBird){
  // invalid difficulty? assume easy
  diff = diff < 0 || diff > 3 ? 0 : diff;
  if(diff == 3) return SUPER_SECRET;
  const char** pass_list[] = {easyPass, mediumPass, hardPass};
  return pass_list[diff][randomBird];
}

const char* getSSIDPrefix(int diff){
  // invalid difficulty? assume easy
  diff = diff < 0 || diff > 3 ? 0 : diff;
  const char* diff_list[] = {prefix0, prefix1, prefix2, prefix3};
  return diff_list[diff];
}

const char* getSSIDNumber(int randomBird){
  // assume possible numbers is always same as number of passwords
  return numberAdd[randomBird];
}

#endif

