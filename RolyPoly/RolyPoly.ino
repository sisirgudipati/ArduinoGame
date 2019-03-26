//Assignment 8
//Partner 1: Sisir Gudipati
//Student ID: 451212
//Partner 2: Ramone Agard
//Student ID: 

#include <Wire.h>
#include "SparkFun_MMA8452Q.h"

MMA8452Q accel; 

int row[] = {2,3,4,5,6,7,8}; 
int colms[] = {9,10,11,12,13}; 
int board[][5] = {{0,0,0,0,0}, {0,0,0,0,0}, {0,0,0,0,0}, {0,0,0,0,0}, {0,0,0,0,0}, {0,0,0,0,0}, {0,0,0,0,0}}; 
float aX; 
float aY; 
float aZ; 
int colPrev; 
int rowPrev; 
int predCol1; 
int predCol2; 
int predRow1; 
int predRow2; 
const unsigned long deltaPrey = 10; 
unsigned long preyTime = 0; 
unsigned long predatorTime = 0; 

unsigned long turnTime = 0; 
unsigned long deltaTurn = 250; 
boolean on = true; 

boolean noWinner = true; 
unsigned long gameTime = 0; 
unsigned long gameDuration = 15000;

int predatorWinCount = 0; 
int preyWinCount = 0; 
boolean predatorWinCheck = false; 
boolean preyWinCheck = false; 

unsigned long tempMillis; 

void setup(){
  Serial.begin(9600); 
  Wire.begin(); 
  if (accel.begin() == false){
    Serial.println("Not connected"); 
    while (1); 
  }
  pinMode(2, OUTPUT); 
  pinMode(3, OUTPUT); 
  pinMode(4, OUTPUT); 
  pinMode(5, OUTPUT); 
  pinMode(6, OUTPUT); 
  pinMode(7, OUTPUT); 
  pinMode(8, OUTPUT); 
  pinMode(9, OUTPUT); 
  pinMode(10, OUTPUT); 
  pinMode(11, OUTPUT); 
  pinMode(12, OUTPUT); 
  pinMode(13, OUTPUT); 
  digitalWrite(colms[0],HIGH);
  digitalWrite(colms[1],HIGH);
  digitalWrite(colms[2],HIGH);
  digitalWrite(colms[3],HIGH);
  digitalWrite(colms[4],HIGH);
  randomSeed(analogRead(0));
  positionSet(); 
}

void loop(){
  while (noWinner){
    printBoard(); 
    unsigned long t = millis(); 
    turn(); 
    //if the predator lights are on, check to see if game has been won 
    if (on){
      if (!(checkTouching())){
      debugBoard(); 
      Serial.print("Predator wins"); 
      tempMillis = millis(); 
      predatorWinCount++;
      predatorWinCheck = true; 
      noWinner = false; 
    }
   }
  //implement rounds
  if (t > gameTime + 15000){
    Serial.print("Prey wins"); 
    gameTime += gameDuration; 
    tempMillis = millis(); 
    preyWinCount++;
    preyWinCheck = true; 
    noWinner = false; 
  }
  
  //implement prey
  if (t > preyTime){
    aX = accel.getCalculatedX(); 
    aY = accel.getCalculatedY(); 
    if (aX < -0.1){
      Serial.println("tilt left"); 
       updatePrey(1); 
   }
    else if (aX > 0.1) {
      Serial.println("tilt right"); 
      updatePrey(2); 
    }
    else if (aY < -0.1) {
      Serial.println("tilt back"); 
       updatePrey(3); 
    }
    else if (aY > 0.1){
      Serial.println("tilt forward"); 
       updatePrey(4); 
    }
  preyTime += deltaPrey; 
 }
  
  //implement predator 
  if (t > predatorTime){
    //check if a,w,s,or d was received 
    char c = (char)Serial.read(); 
    Serial.print(c); 
    if (c == 'a'){
      //left
      Serial.print("pred go left"); 
      updatePredator(1);
    }
    else if (c == 'd'){
      //right
      Serial.print("pred go right"); 
      updatePredator(2); 
    }
    
    else if (c == 's'){
      //backwards
      Serial.print("pred go back"); 
      updatePredator(3); 
    }
    
    else if (c == 'w'){
      //forward
      Serial.print("pred go forwards"); 
      updatePredator(4); 
    }
  predatorTime += (2*deltaPrey); 
  }
 } 
 
 //four second pause + display winner 
 if ((millis()-tempMillis)>=4000){
    resetRound(); 
 }
 else {
    //call function to display winner 
    displayWinner();   
  }
}

void resetRound(){
  //reset all global variables required 
  positionSet(); 
  gameTime = millis(); 
  preyTime = millis(); 
  predatorTime = millis(); 
  predatorWinCheck = false; 
  preyWinCheck = false; 
  noWinner = true; 
}

void displayWinner(){
  clearBoard(); 
  int difference; 
  if (preyWinCheck){
    board[3][2] = 1; 
    difference = preyWinCount- predatorWinCount; 
    if (difference == 0){
      board[0][2] = 1;
    }
    else if ((difference > 0) && (difference <= 2)){
      //winning by less than 2 
      board[0][2] = 1; 
      board[0][1] = 1; 
    }
    else if ((difference < 0) && (difference >= -2)){
      //losing by less than 2
      board[0][2]=1;
      board[0][3]=1;
    }
    else if (difference > 2){
      //winning by more than 2
      board[0][2] = 1; 
      board[0][1] = 1; 
      board[0][0] = 1;
    }
    else if (difference < -2){
      //losing by more than 2
      board[0][2]=1;
      board[0][3]=1;
      board[0][4]=1;
    }
  }
  else if (predatorWinCheck){
      board[3][2] = 1; 
      board[2][2] = 1; 
      board[3][1] = 1; 
      board[2][1] = 1; 
      difference = predatorWinCount - preyWinCount; 
      if (difference == 0){
      board[0][2] = 1;
    }
    else if ((difference > 0) && (difference <= 2)){
      //winning by less than 2 
      board[0][2] = 1; 
      board[0][1] = 1; 
    }
    else if ((difference < 0) && (difference >= -2)){
      //losing by less than 2
      board[0][2]=1;
      board[0][3]=1;
    }
    else if (difference > 2){
      //winning by more than 2
      board[0][2] = 1; 
      board[0][1] = 1; 
      board[0][0] = 1;
    }
    else if (difference < -2){
      //losing by more than 2
      board[0][2]=1;
      board[0][3]=1;
      board[0][4]=1;
    }
  }
  printBoard();
}

void positionSet(){
  boolean checker = false; 
  while (!checker){
    clearBoard();
    //lower right hand column
    int colNumber = random(4); 
    //lower right hand row
    int rowNumber = random(6);
    Serial.print("here"); 
    Serial.print(colNumber);
    Serial.println(rowNumber);
    board[rowNumber][colNumber] = 1;
    board[rowNumber][colNumber+1] = 1; 
    board[rowNumber+1][colNumber] = 1; 
    board[rowNumber+1][colNumber+1] = 1; 
    predCol1 = colNumber+1; 
    predCol2=colNumber;
    predRow1=rowNumber+1; 
    predRow2=rowNumber;  
    int randPreyY = random(7);
    int randPreyX = random(5);
    Serial.print(randPreyY); 
    Serial.print(randPreyX);   
    board[randPreyY][randPreyX] = 1;
    rowPrev = randPreyY; 
    colPrev = randPreyX; 
    checker = checkTouching();     
  }
}

boolean checkTouching(){
    int count = 0; 
    for (int i = 0; i < 5; i++){
      for (int j = 0; j<7; j++){
        if (board[j][i] == 1){
          count++; 
        }
      }
    }
    if (count == 5){
      //check boundary 
      return true; 
    }
    else {
      Serial.println("This is the count variable: "); 
      Serial.println(count); 
      debugBoard();  
      return false; 
    }
}

void clearBoard(){
  for (int i = 0; i < 5; i++){
    for (int j=0; j<7; j++){
      board[j][i] = 0;  
    }
  }
}

void updatePredator(int x){
  if (x == 1){
    //left
    if (predCol1 == 4){
        Serial.print("maxPredCol"); 
    }
    else {
      board[predRow1][predCol2] = 0; 
      board[predRow2][predCol2] = 0; 
      board[predRow1][predCol1] = 0; 
      board[predRow2][predCol1] = 0; 
      predCol2++; 
      predCol1++; 
      board[predRow1][predCol2] = 1; 
      board[predRow2][predCol2] = 1; 
      board[predRow1][predCol1] = 1; 
      board[predRow2][predCol1] = 1; 
    }
  }
  else if (x ==2){
    //right
    if (predCol2 == 0){
      Serial.print("maxPredCol"); 
    }
    else {
      board[predRow1][predCol2] = 0; 
      board[predRow2][predCol2] = 0; 
      board[predRow1][predCol1] = 0; 
      board[predRow2][predCol1] = 0; 
      predCol2--; 
      predCol1--; 
      board[predRow1][predCol2] = 1; 
      board[predRow2][predCol2] = 1; 
      board[predRow1][predCol1] = 1; 
      board[predRow2][predCol1] = 1; 
    }
  }
  else if (x==3){
    //backwards
    if (predRow2 == 0){
      Serial.print("maxPredRow"); 
    }
    else {
      board[predRow1][predCol2] = 0; 
      board[predRow2][predCol2] = 0; 
      board[predRow1][predCol1] = 0; 
      board[predRow2][predCol1] = 0; 
      predRow2--; 
      predRow1--; 
      board[predRow1][predCol2] = 1; 
      board[predRow2][predCol2] = 1; 
      board[predRow1][predCol1] = 1; 
      board[predRow2][predCol1] = 1; 
    }
  }
  else if(x==4){
    //forwards
    if (predRow1 == 6){
      Serial.print("maxPredRow"); 
    }
    else {
      board[predRow1][predCol2] = 0; 
      board[predRow2][predCol2] = 0; 
      board[predRow1][predCol1] = 0; 
      board[predRow2][predCol1] = 0; 
      predRow2++; 
      predRow1++; 
      board[predRow1][predCol2] = 1; 
      board[predRow2][predCol2] = 1; 
      board[predRow1][predCol1] = 1; 
      board[predRow2][predCol1] = 1; 
    }
  }
}

void updatePrey(int x){
  if (x == 1){
    //left
    if (colPrev == 4){
    Serial.print("maxCol"); 
    }
    else {
        board[rowPrev][colPrev] = 0; 
        colPrev++; 
        board[rowPrev][colPrev] = 1; 
    }
  }
  else if (x == 2){
    //right
    if (colPrev == 0){
      Serial.print("maxCol"); 
    }
    else {
      board[rowPrev][colPrev] = 0; 
      colPrev--; 
      board[rowPrev][colPrev] = 1; 
    }
  }
  else if (x == 3){
    //back
    if (rowPrev==0){
        Serial.println("maxRow");
    }
    else {
      board[rowPrev][colPrev] = 0; 
      rowPrev--;
      board[rowPrev][colPrev] = 1; 
    }
  }
  else if (x == 4){
    //forward
     if (rowPrev==6){
        Serial.println("maxRow"); 
      }
      else {
        board[rowPrev][colPrev] = 0; 
        rowPrev++; 
        board[rowPrev][colPrev] = 1; 
      }
  }
  else {
     Serial.print("There was an error with your update"); 
   }  
}

void turn(){
  unsigned long timer = millis(); 
  //to blink predator at 4 Hz
  if (timer > turnTime){
    if (on){
      board[predRow1][predCol2] = 0; 
      board[predRow2][predCol2] = 0; 
      board[predRow1][predCol1] = 0; 
      board[predRow2][predCol1] = 0; 
    }
    else {
      board[predRow1][predCol2] = 1; 
      board[predRow2][predCol2] = 1; 
      board[predRow1][predCol1] = 1; 
      board[predRow2][predCol1] = 1; 
    }
    on = !on; 
    turnTime += deltaTurn; 
  }
}

void debugBoard(){
  for (int i = 0; i <7; i++){
    for (int j = 0; j < 5; j++){
      Serial.print(board[i][j]);
    }
    Serial.println(); 
  }
}

void printBoard(){
  int prev = 4; 
  for (int i = 0; i < 5; i++){
    digitalWrite(colms[i], LOW);
    digitalWrite(colms[prev], HIGH);
    prev =i; 
    int temp; 
    for (int j = 0; j < 7; j++){
      temp = board[j][i]; 
      if (temp == 1){
        digitalWrite(row[j], HIGH);  
      }
      else {
        digitalWrite(row[j], LOW);  
      } 
    }
    delay(2); 
  }
}
