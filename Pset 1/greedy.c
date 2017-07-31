#include <cs50.h>
#include <stdio.h>
#include <math.h>

float change = 0;

int main(void)
{
    // ask user to input amount of change required
    do
    {
        printf("O hai! How much change is owed?\n");
        change = get_float();
    }
    while (change <= 0);
    
    // convert change to cents
    int cents = (int)round(change * 100);
    
    // calculate number of quarters
    int quarters = cents / 25;
    int remaining = cents % 25;
    
    // calculate number of dimes
    int dimes = remaining / 10;
    remaining = remaining % 10;
    
    // calculate number of nickels
    int nickels = remaining / 5;
    remaining = remaining % 5;
    
    // remaining at this point is number of pennies
    
    // calculate number of coins required to make change
    int coin_count = quarters + dimes + nickels + remaining;
    
    printf("%i\n", coin_count);
}