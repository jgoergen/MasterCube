byte selectedDirection = 99;
byte gameState = 0;
byte targetColors[TURNS];
byte turnColors[ROUNDS][TURNS];
byte roundCorrect[ROUNDS];
byte turn = 0;
byte currentRound = 0;
uint32_t lastTurn = 0;
int lastGameUpdate = 0;
int introFrames = 0;
int gameWaitFrames = 0;

void Game_Init() {
  currentRound = 0;
  turn = 0;
  lastTurn = 0;
  Game_GenerateGameColors();
  gameState = 0;
  introFrames = 50;
  Display_SetMotion(1);
  Serial.println(F("Game Setup"));
}

void Game_Update() {
  
  if (millis() - lastGameUpdate >= GAME_UPDATE_RATE)
  {
    lastGameUpdate = millis();

    if (gameWaitFrames > 0)
    {
      gameWaitFrames --;

      if (gameWaitFrames > 0)
      {
        Serial.print(".");
      }
      else
      {
        Serial.println(".");
      }
      return;
    }

    switch(gameState) {
      case 0:
        Game_RunIntro();
        break;
        
      case 1:
          Game_Draw_Turn_Left();
          GAME_Start_Turn();
          break;
  
      case 2:
          GAME_Entry_Update();
          break;
  
      case 3:
          GAME_End_Turn();
          break;
    }
  
    return;
  }
}

void Game_GenerateGameColors() {
  byte colors[TURNS] = {TOP, LEFT, RIGHT, FRONT, BACK, BOTTOM};

  for (int i = 0; i < TURNS; i++)
  {
      targetColors[i] = 99;
      roundCorrect[i] = 99;

      for (int o = 0; o < ROUNDS; o++)
      {
          turnColors[o][i] = 99;
      }
  }

  for (byte i = 0; i < TURNS; i++)
  {
      byte newIndex = floor(random(0, TURNS));

      // as long as this side hasn't been used AND if its the first time, the top and bottom can't be it
      if (colors[newIndex] != 99 && !(i == 0 && newIndex == TOP) && !(i == 0 && newIndex == BOTTOM))
      {
          Serial.println(colors[newIndex]);
          targetColors[i] = colors[newIndex];
          colors[newIndex] = 99;
      }
      else
      {
          i--;
      }
  }
}

void Game_RunIntro() {
  //Display_Clear();
  int dir = int((cos(millis()) + 1) * 3.5);
  Display_SetColor(
    dir, 
    floor(random(255)), 
    floor(random(255)), 
    floor(random(255)));

  introFrames --;
  if (introFrames < 1) 
  {
    Serial.println("");
    Display_Clear();
    Game_Enforce_Top_Position();
    gameState = 1;
  }
}

void GAME_Start_Turn()
{
    Serial.print("Round ");
    Serial.print(currentRound);
    Serial.print(", Turn");
    Serial.print(turn);
    Serial.println(" Start");
    gameState = 2;
}

void GAME_Entry_Update()
{
  bool knocked = MPU_GetKnock();
  byte newDirection = MPU_GetDirection();

  // is this a direction at all, and is it different then the last one?
  if (newDirection != 9 && newDirection != selectedDirection) // knocked && 
  {
      // confirm this direction hasn't been chosen already
      if (wasAlreadyChosen(newDirection))
      {
        return;
      }

      selectedDirection = newDirection;        
      Display_Set_Direction(selectedDirection);
      turnColors[currentRound][turn] = selectedDirection;
      Serial.println(selectedDirection);
      gameWaitFrames = 20;
      Display_SetAll(255, 255, 255);
      gameState = 3;
  }
}

void GAME_End_Turn()
{
    Display_SetAll(0, 0, 0);
    gameState = 1;
    turn++;

    if (turn >= TURNS)
    {
        GAME_End_Round();
    }
}

bool wasAlreadyChosen(byte dir)
{
  for (byte i = 0; i < TURNS; i++)
  {
    if (turnColors[currentRound][i] == dir) 
    {
      return true;
    }
  }

  return false;
}

void GAME_End_Round()
{
    lastTurn = 0;
    Serial.println("round over");
    int correct = 0;

    for (byte i = 0; i < TURNS; i++)
    {
        if (turnColors[currentRound][i] == targetColors[i])
        {
            correct++;
            Display_SetAll(255, 255, 255);
            Display_Force_Update();
            delay(500);
            Display_SetAll(0, 0, 0);
            Display_Force_Update();
            delay(500);
        }
    }

    Serial.print("Correct ");
    Serial.println(correct);
    roundCorrect[currentRound] = correct;

    if (correct == TURNS)
    {
        Display_Flash(0, 255, 0, 5, 1000);
        GAME_Draw_Answer();
        Game_Init();
        Serial.println("You won!");
    }
    else
    {
        if (correct == 0) {
          Display_Flash(255, 0, 0, 2, 1000);
        }
      
        Game_Enforce_Top_Position();
        turn = 0;
        currentRound++;
    }

    if (currentRound == ROUNDS)
    {
        Display_Flash(255, 0, 0, 5, 1000);
        GAME_Draw_Answer();
        Game_Init();
        Serial.println("You lost!");
    }
}

void Game_Draw_Turn_Left() {
  Display_Set_Direction(TOP);
  Display_Set_Direction(LEFT);
  Display_Set_Direction(RIGHT);
  Display_Set_Direction(FRONT);
  Display_Set_Direction(BACK);
  Display_Set_Direction(BOTTOM);
  
  for (byte i = 0; i < TURNS; i++)
  {
    if (turnColors[currentRound][i] != 99) {
      Display_SetColor(turnColors[currentRound][i], 0, 0, 0);
    }
  }
}

void GAME_Draw_Answer() {  
  for (byte o = 0; o < 5; o++) {
    Display_SetAll(100, 155, 255);    
    Display_Force_Update();
    delay(1000);
    for (byte i = 0; i < TURNS; i++)
    {
      Display_Clear();
      Display_Set_Direction(targetColors[i]);
      Display_Force_Update();
      delay(500);
    }
    delay(500);
  }
  
  delay(1000);
  Display_Flash(255, 155, 100, 2, 100);
}

void Game_Enforce_Top_Position() {
  while(MPU_GetDirection() != TOP) {
    Display_Flash(255, 255, 0, 1, 100);
    MPU_Update();
  }
  selectedDirection = TOP;
}
