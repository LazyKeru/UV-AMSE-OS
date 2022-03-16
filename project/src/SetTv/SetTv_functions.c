/*============================================================================================*/
/*                          Set Tv for selected motor                                         */
/*--------------------------------------------------------------------------------------------*/
/*         Inspired by the example given by J.BOONAERT AMSE 2021-2022                         */
/*                           Killian ALLAIRE fise2023                                         */
/*============================================================================================*/

#include "../../header/SetTv.h"

/*&&&&&&&&&&&&&&&&&&&&&&*/
/* aide de ce programme */
/*&&&&&&&&&&&&&&&&&&&&&&*/
void usage( char *szPgmName)
{
    if( szPgmName == NULL)
    {
        exit( -1 );
    };
    printf("%s <consigne> <drive>\n", szPgmName);
    printf("   avec <drive> = L | R \n");
}