/*
    TODO:
    - Finish configure sensors function
    - Finish drive function
    - Finish draw function
    - Create 'hard' mode intelligence for robot
    - Clean up main function, logic for gameOver and ensure robot works as expected
*/
const int SQUARESIZE = 25;

void PlayerMove(int gameBoard[3][3], int letter)
{
        int currentX = 1;
        int currentY = 1;
    //waiting for enter to confirm selection
    while(!getButtonPress(buttonEnter))
    {
        //draw empty board with x to chose location (x may need to change if player is o for clarity)
        char choice[3][3] = {' ',' ',' ',' ','X',' ',' ',' ',' '};
        displayString(3, "%s|%s|%s",choice[0][0],choice[0][1],choice[0][2]);
        displayString(4, "-----");
        displayString(5, "%s|%s|%s",choice[1][0],choice[1][1],choice[1][2]);
        displayString(6, "-----");
        displayString(7, "%s|%s|%s",choice[1][0],choice[1][1],choice[1][2]);
        //check for button press and which button is pressed, if enter is pressed it will skip all and recheck the outter while loop and end
        while(!getButtonPress(buttonAny))
        {}
        if(getButtonPress(buttonUp))
        {
            choice[currentX][currentY] = 0;
            currentY++;
            choice[currentX][currentY] = 1;
        }
        if(getButtonPress(buttonDown))
        {
            choice[currentX][currentY] = 0;
            currentY--;
            choice[currentX][currentY] = 1;
        }
        if(getButtonPress(buttonRight))
        {
            choice[currentX][currentY] = 0;
            currentX++;
            choice[currentX][currentY] = 1;
        }
        if(getButtonPress(buttonLeft))
        {
            choice[currentX][currentY] = 0;
            currentX--;
            choice[currentX][currentY] = 1;
        }
    }
    gameBoard[currentX][currentY] = letter;
}
int GameOver(int gameBoard[3][3])
{
    //check row victory
    int sum = 0;
    int count1 =0;
    while(abs(sum) < 3 && count1 < 3)
    {
        sum = 0;
        for(int count2 = 0; count2 < 3; count2++)
        {
            sum += gameBoard[count1][count2];
        }
        count1++;
    }
    //check col victory
    count1 = 0;
    if(abs(sum) != 3)
    {
        while(abs(sum) < 3 && count1 < 3)
        {
            sum = 0;
            for(int count2 = 0; count2 < 3; count2++)
            {
                sum += gameBoard[count2][count1];
            }
            count1++;
        }
    }
    //check for left diagoal victory
    if(abs(sum) != 3)
    {
        sum = 0;
        for(int count = 0; count < 3; count++)
            sum += gameBoard[count][count];
    }
    //check for right diagoal victory
    if(abs(sum) != 3)
    {
        sum = 0;
        for(int count3 = 2; count3 >= 0; count3--)
        {
            int count2 = 2-count3;
            sum += gameBoard[count3][count2];
        }
    }
    //return values
    if(sum == 3)
        return 1;
    else if(sum == -3)
        return -1;
    else
        return 0;
}
void configureSensors()
{
    SensorType[S1] = sensorEV3_Touch;
    SensorType[S2] = sensorEV3_Ultrasonic;
    SensorType[S3] = sensorEV3_Color;
    SensorType[S4] = sensorEV3_Gyro;
    wait1Msec(50);
    Sensormode = modeEV3Gyro_Calibration;
    wait1Msec(50);
    Sensormode = modeEV3Gyro_RateAndAngle;
}

void penDown()
{
    nMotorEncoder[motorB] = 0;
    motor[motorB]=10;
    while (nMotorEncoder[motorB] < 120)
    {}
    motor[motorB] = 0;
}
void penUp()
{
    nMotorEncoder[motorB] = 0;
    motor[motorB]= -10;
    while (abs(nMotorEncoder[motorB]) < 120)
    {}
    motor[motorB] = 0;
}

void driveTo(int horizontal, int vertical)
{
    const int SPEED = 20;
    int enc_horizontal = (SQUARESIZE * horizontal) * 360 / (2PI2.75);
    int enc_vertical = (SQUARESIZE * vertical) * 360 / (2PI2.75);

    nMotorEncoder[motorA] = 0;
    motor[motorA] = motor[motorD] = SPEED;
    while (abs(nMotorEncoder[motorA]) < enc_horizontal)
    {}
    motor[motorA] = motor[motorD] = 0;
    wait1Msec(1000);
    rotateAngle(-90,10);
    nMotorEncoder[motorA] = 0;
    motor[motorA] = motor[motorD] = SPEED;
    while (abs(nMotorEncoder[motorA]) < enc_vertical)
    {}
    motor[motorA] = motor[motorD] = 0;
    wait1Msec(1000);
}
void driveBack(int horizontal, int vertical)
{
    const int SPEED = 20;
    int enc_horizontal = (SQUARESIZE * horizontal) * 360 / (2PI2.75);
    int enc_vertical = (SQUARESIZE * vertical) * 360 / (2PI2.75);

    nMotorEncoder[motorA] = 0;
    motor[motorA] = motor[motorD] = -SPEED;
    while (abs(nMotorEncoder[motorA]) < enc_vertical)
    {}
    motor[motorA] = motor[motorD] = 0;
    wait1Msec(1000);
    rotateAngle(90,10);
    nMotorEncoder[motorA] = 0;
    motor[motorA] = motor[motorD] = -SPEED;
    while (abs(nMotorEncoder[motorA]) < enc_horizontal)
    {}
    motor[motorA] = motor[motorD] = 0;
    wait1Msec(1000);
}
void drawFunction(int xORo)
{
    //1 for X, 0 for O
}
void rotateAngle(int angle, int motorPower)
{
    resetGyro(S4);
    if (angle > 0)
    {
        motor[motorA] = -motorPower;
        motor[motorD] = motorPower;
    }
    else
    {
        motor[motorA] = motorPower;
        motor[motorD] = -motorPower;
    }
    angle = abs(angle);
    while(abs(getGyroDegrees(S4)) < angle)
    {}
    motor[motorA] = motor[motorD] = 0;
}
void easyMode(int gameBoard[3][3])
{
    int randomChoice = random(1) //random number between 0 and 1
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            if (gameBoard[row][col] == -1)
            {
                //Not entirely complete
                drive(currentSquare, futureSquare);
                drawFunction(randomChoice);
            }
        }
    }
}
task main()
{
    //1 for X, -1 for O, 0 for empty. We assume robot starts at [0][0] for convinience (upper left corner)
    int gameboard[3][3] = {{-1}};
    
    //Starting Game
    displayString(1, "Press touch sensor to start game!");
    while(sensorValue[S1] == 0)
    {}
    
    //Deciding who goes first (this can be changed later)
    displayString(1, "Press UP to go first, DOWN for robot goes first")
    
    int whoseTurn = -1; //-1 for user term, 1 for robot turn
        while(!getButtonPress(buttonAny))
        {}
        if (getButtonPress(buttonDown))
        {
            whoseTurn = 1;
        }
    
    //Main game loop
    bool gameOver = false;
    while (gameOver == false)
    {
        if (whoseTurn == -1)
        {
            int upCounter = 0;
            int currentSquare = 0;
            //The following squence to get button pressess may be flawed
            while(!getButtonPress(buttonAny))
            {}
            while(!getButtonPress(buttonEnter) && getButtonPress(buttonUp))
            {
                upCounter++;
            }
            //Pseudocode-ish. Will fix later
            drive(currentSquare, upCounter); //drive to desired square
            
            displayString(1, "Press UP for X, DOWN for O!";)
            
            //Choosing between X and O
            while(!getButtonPress(buttonAny))
            {}
            if(getButtonPress(buttonUp))
            {
                drawFunction(1);
                gameBoard[X][X] = 1; //NOT CORRECT SYNTAX WILL FIX
            }
            else
            {
                drawFunction(0);
                gameboard[X][X] = 0; //FIX
            }
            whoseTurn *= -1;
        }
    if (whoseTurn == 1)
    {
        easyMode(gameboard); //or HARD MODE
        whoseTurn *= -1;
    }
    //Need to add condition for game end
    }
}
