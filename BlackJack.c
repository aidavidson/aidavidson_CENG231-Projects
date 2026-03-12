/*              Homework 4: Blackjack               */
/*                                                  */
/*              Author: Aiden Davidson              */
/*                                                  */
/*              09/24/2025       AMD                */
/*                                                  */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main (void)
{
        /* a value the user sets to one if they want another or 0 if they don't*/
        int userChoice = 2;

        /* ensures the user isn't dealed a 0 as that isn't possible*/
        int currentCard = 0;

        /* the cards the player has*/
        int playersValue = 0;

        /* the cards the dealer has*/
        int dealersValue = 0;

        /*setting a constant for conditionals improving readability*/
        /*the value of blackjack which is considered the highest possible value*/
        int blackjack = 21;

        /* whether or not switch case should be tested for who wins*/
        int finalDecision = 1;

        /* sets up random number generator*/
        srand(time(NULL));

        /*header*/
        printf("************************************\n");
        printf("** Welcome to Ceng 231 BlackJack! **\n");
        printf("************************************\n");

        /*PLAYERS HAND*/

        /* plays the initial card for the player*/
        currentCard = rand() % 13 + 1;

        /*tests for a face card to ensure that they are still valued at ten*/
        switch(currentCard)
        {
                case 11 :
                        {currentCard = 10;
                        break;}/* 11 esac*/
                case 12 :
                        {currentCard = 10;
                        break;}/* 12 esac*/
                case 13 :
                        {currentCard = 10;
                        break;}/* 13 esac*/
                default :
                        {/*any other card value that doesn't need changing*/}
        }/*hctiws*/


        /*prints out what the first card value is*/
        printf("Your first card is:%d", currentCard);

        /*increments player value by the card given to give an initial value before the repetition cycle*/
        playersValue += currentCard;

        /*plays the hand for the player while they want another card*/
        while(userChoice != 0)
        {
                /*prints out if user wants another card*/
                printf("\nDo you want another card?\n");

                /*presents user with the manual for receiving a new card*/
                printf("Type 1 for Yes, 0 for No:\n");

                /*scanse the input to determine if the user wants another card*/
                scanf("%d", &userChoice);

                /* if the user chose 0 they don't want more cards so the players hand loop is broken out of*/
                if(userChoice == 0)
                        {break;}/*fi*/

                /*deals the player a card*/
                currentCard = rand() % 13 + 1;

                /*checks whether the card is viable in value*/
                switch(currentCard)
                {

                        case 11 :

                                {currentCard = 10;
                                break;}/*11 esac*/

                        case 12 :

                                {currentCard = 10;
                                break;}/*12 esac*/

                        case 13 :

                                {currentCard = 10;
                                break;}/*13 esac*/

                        default :

                                {/* any other card value that doesn't need changing*/}/*tluafed*/

                }/*hctiws*/
        
                /* increments player value by the card they were dealt*/
                playersValue += currentCard;

                /* prints out the header for what the next card is along with the combined value*/
                printf("Your next card is: %d \nYour combined value is: %d", currentCard, playersValue);

                /*checks to see if the player loses before the final decision and if so it immediately breaks out of the loop so the user doesn't ask for more cards*/
                if(playersValue > blackjack){

                        /*lose text*/
                        printf("\nSorry. You lose!");

                        /* a variable to ensure final decision doesn't occur past this point because the player already lost*/
                        finalDecision = 0;

                        break;

                }/*fi*/

        }/*elihw*/

        /*DEALERS HAND*/

        /*ensures the dealer doesn't hit over 17 */
        while(dealersValue < blackjack && finalDecision == 1){

                /*deals the dealer a new card*/
                currentCard =  rand() % 13 + 1;

                /* checks if the card is a valid number if not then gets changed to one*/
                switch(currentCard)
                {

                        case 11 :

                                {currentCard = 10;
                                break;}/* 11 esac*/

                        case 12 :

                                {currentCard = 10;
                                break;}/* 12 esac*/

                        case 13 :

                                {currentCard = 10;
                                break;}/*13 esac*/

                        default :
                                {/*any other card value that doesn't need to be changed*/}/*tluafed*/

                }/*hctiws*/

                /*increments the dealers value by the card it was given*/
                dealersValue += currentCard;

                /* the text of what the dealer has and you have*/
                printf("\nDealer draws another card. \nDealers card is: %d\nDealers value is %d, you have %d", currentCard,dealersValue, playersValue);

        }/*elihw*/

        /*checks to see if you went over 21 already if so then you already lost so no final round is needed*/
        if(finalDecision == 1)

                /*checks to see if the players value/dealersValue is 0 or 1 based on truncation. */
                /* if it is 0 that means the player value is smaller therefore if the dealer is greater than blackjack you win*/
                /* however if the dealer isn't higher than black jack but their value is greater than yours the dealer wins*/
                /* if it is one there are a few possibilities the values are exactly the same which means that its a tie*/
                /* it could also be that your value is greater than the dealers which would mean since we've already checked before*/
                /* final decision whether the player is over 21 the player wins*/
                switch(playersValue/dealersValue)
                {

                        case 0 :

                                {if(dealersValue > blackjack)
                                        {printf("\nDealer loses. You win! ");}/*fi*/

                                else
                                    	{printf("\nYou lose. Dealer Wins! ");}/*esle*/

                                break;}/* 0 esac*/

                        case 1 :

                                {if(playersValue == dealersValue)
                                        {printf("You lose. Dealer Wins! ");}/*fi*/
                                else
                                    	{printf("Dealer loses. You win! ");}/*esle*/

                                break;}/* 1 esac*/

                        default :

                                {printf("\nTest failed");
                                break;}/* tluafed*/

                }/*hctiws*/

        }/*fi*/

        return 0;

}/*naim*/

