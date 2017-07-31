#!/usr/bin/env python3

import cs50

def main():
    
    # ask user to input amount of change required
    while True:
        print("O hai! How much change is owed? ", end="")
        change = cs50.get_float()
        if change > 0:
            break
        
    # convert change to cents
    cents = round(change * 100)
    
    # calculate number of quarters
    quarters = cents // 25
    remaining = cents % 25
    
    # calculate number of dimes
    dimes = remaining // 10
    remaining = remaining % 10
    
    # calculate number of nickels
    nickels = remaining // 5
    remaining = remaining % 5
    
    # calculate number of coins required to make change
    coin_count = quarters + dimes + nickels + remaining
    
    print(coin_count)
    
if __name__ == "__main__":
    main()