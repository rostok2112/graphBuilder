#include <stdio.h>
#include "../include/types.h"

#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841027019385211055596446229489549303819644288109756659334461284756482337867831652712019091

int main()
{
    char testStr [] = "2*(cos(x)+sin(x))/sqrt(abs(x-2))";

    TokenArr_t * tmp = rpnTranslator(_strtok(testStr));
    printf("Result = %.999g" , stackMachine(-PI, tmp));

    for(int i = 0; i < tmp->capacity; i++)
        free(tmp->tokens[i].token);
    free(tmp->tokens);
    free(tmp);

    return 0;
}
