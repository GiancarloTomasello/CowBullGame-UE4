// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"


void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt"); //Gets path to content folder
    FFileHelper::LoadFileToStringArray(Words, *WordListPath); //Convert textfile to array

    Isograms = GetValidWords(Words);
    SetUpGame();

  //  PrintLine(TEXT("The hidden word is %s."), *HiddenWord); //Debug Line

}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
   

    if(GameOver){
        ClearScreen();
        SetUpGame();

    } else {
        ProcessGuess(Input);
    }

    
}

void UBullCowCartridge::SetUpGame(){

    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];
    Lives = 3;
    GameOver = false;

    PrintLine(TEXT("Welcome to Bull Cows!"));
    PrintLine(TEXT("Guess the %i letter word."), HiddenWord.Len()); //Magic number remvoe later
    PrintLine(TEXT("You have %i guesses to get it right"), Lives);
    PrintLine(TEXT("Type in your guess. \nPress enter to continue...."));

}

void UBullCowCartridge::EndGame(){
    GameOver = true;
    
    PrintLine(TEXT("\nPress enter to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess){
    if(Guess == HiddenWord){
        PrintLine(TEXT("You are Correct!"));
        EndGame();
        return;
    } 

    //Check if isogram
    if(!IsIsogram(Guess)){
        PrintLine("No Repeating letters, guess again.");
        return;
    }

    if(Guess.Len() != HiddenWord.Len()){
        PrintLine(TEXT("Sorry, that guess is incorrect!"));
        PrintLine(TEXT("The hidden word is %i character long!"), HiddenWord.Len());
        return; 
    } 

    --Lives;
    PrintLine("You lost a life!");

    if (Lives <= 0){
        ClearScreen();
        PrintLine(TEXT("You have no lives left!"));
        PrintLine(TEXT("The hidden word was %s."), *HiddenWord);
        EndGame();
        return;
    }

    FBullCowCount Score = GetBullCows(Guess);

    PrintLine(TEXT("You hace %i Bulls and %i Cows"), Score.Bulls, Score.Cows);
}

bool UBullCowCartridge::IsIsogram(const FString& Guess) const{
    for(int32 i = 0; i < Guess.Len()-1; i++){
        for(int32 j = i+1; j < Guess.Len(); j++){
            if(Guess[i] == Guess[j]){
                return false;
            }
        }
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const{
    TArray<FString> ValidWords;
    for(FString Word : WordList){
        if(Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word)){
            ValidWords.Emplace(Word);
        }
    }

    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const{
    FBullCowCount Count;

    for (int32 i = 0; i < Guess.Len(); i++)
    {
        if(Guess[i] == HiddenWord[i]){
            Count.Bulls++;
            continue;
        }
        
        for(int32 j = 0; j < HiddenWord.Len(); j++){
            if(Guess[i] == HiddenWord[j]){
                Count.Cows++;
                break;
            }
        }
    }

    return Count;
}