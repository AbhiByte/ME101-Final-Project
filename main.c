int gameBoard[3][3] = {0,0,0,0,0,0,0,0,0};
const int SQUARESIZE = 35;
const int negativeRightAngle = -90;
const int halfRight = 45;
const int distance = 5 * 360/(2*PI*2.75);
const int doubleDist = 10 * 360/(2*PI*2.75);
const int motorPower = 15;

void configureSensors()
{
	SensorType[S1] = sensorEV3_Touch;
	SensorType[S2] = sensorEV3_Ultrasonic;
	SensorType[S3] = sensorEV3_Color;
	SensorType[S4] = sensorEV3_Gyro;
	wait1Msec(50);
	SensorMode[S4] = modeEV3Gyro_Calibration;
	wait1Msec(50);
	SensorMode[S4] = modeEV3Gyro_RateAndAngle;
	SensorMode[S3] = modeEV3Color_Color;
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

void penUp()
{
	nMotorEncoder[motorB] = 0;
	motor[motorB]= -10;
	while (abs(nMotorEncoder[motorB]) < 120)
	{}
	motor[motorB] = 0;
}

void penDown()
{
	nMotorEncoder[motorB] = 0;
	motor[motorB]=10;
	while (nMotorEncoder[motorB] < 120)
	{}
	motor[motorB] = 0;
}

void driveTo(int horizontal, int vertical)
{
	const int SPEED = 60;
	int enc_horizontal = (SQUARESIZE * (horizontal+1)) * 360 / (2*PI*2.75);
	int enc_vertical = (SQUARESIZE * (vertical+1)) * 360 / (2*PI*2.75);

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

void driveBack()
{
	const int SPEED = 20;
	const int DISTWALL = 90;

	motor[motorA] = motor[motorD] = -SPEED;
	while (SensorValue(S3) != (int)colorBlue)
	{}
	motor[motorA] = motor[motorD] = 0;
	//wait1Msec(1000);
	rotateAngle(90,10);
	motor[motorA] = motor[motorD] = -SPEED;

	while (SensorValue(S3) != (int)colorRed)
	{}
	nMotorEncoder[motorA] = 0;
	while (abs(nMotorEncoder[motorA]) < DISTWALL)
	{}
	motor[motorA] = motor[motorD] = 0;
}

void movement(int distance, int motorPower)
{
	nMotorEncoder[motorA] = 0;
	//motor[motorA] = motor[motorD] = 0;
	motor[motorA] = motor[motorD] = motorPower;
	while (fabs(nMotorEncoder[motorA]) < distance) //drive forward
	{}
	motor[motorA] = motor[motorD] = 0;
}

void drawX(int motorPower, int distance, int angleSmall, int angleLarge)
{
	motor[motorA] = motor[motorD] = 0;
	rotateAngle(angleSmall, motorPower);
	movement(distance, motorPower);
	penDown();
	movement(doubleDist, -motorPower);
	penUp();
	movement(distance, motorPower);
	rotateAngle(angleLarge, motorPower);
	movement(distance, motorPower);
	penDown();
	movement(doubleDist, -motorPower);
	penUp();
	movement(distance, motorPower);
	rotateAngle(angleSmall, motorPower);
}

void drawStick(int motorPower, int distance)
{
	motor[motorA] = motor[motorD] = 0;
	movement(distance, motorPower);
	penDown();
	movement(doubleDist, -motorPower);
	penUp();
	movement(distance, motorPower);
}

void PlayerMove(int letter)
{
	int currentX = 1;
	int currentY = 1;
	char choice[3][3] = {' ',' ',' ',' ','X',' ',' ',' ',' '};
	//waiting for enter to confirm selection
	do{
		if (gameBoard[currentY][currentX] != 0)
		{
			eraseDisplay();
			displayString(10, "INVALID SQUARE, TRY AGAIN");
			wait1Msec(3000);
			eraseDisplay();
		}
		while(!getButtonPress(buttonEnter))
		{
			//draw empty board with x to chose location (x may need to change if player is o for clarity)

			displayString(3, "%c|%c|%c",choice[0][0],choice[0][1],choice[0][2]);
			displayString(4, "-----");
			displayString(5, "%c|%c|%c",choice[1][0],choice[1][1],choice[1][2]);
			displayString(6, "-----");
			displayString(7, "%c|%c|%c",choice[2][0],choice[2][1],choice[2][2]);
			//check for button press and which button is pressed, if enter is pressed it will skip all and recheck the outter while loop and end
			while(getButtonPress(buttonAny))
			{}
			wait1Msec(50);
			//while(!getButtonPress(buttonAny))
			//{}
			if(getButtonPress(buttonUp) && currentY != 0)
			{
				choice[currentY][currentX] = ' ';
				currentY--;
				choice[currentY][currentX] = 'X';
				displayString(10, "went up");
			}
			if(getButtonPress(buttonDown) && currentY != 2)
			{
				choice[currentY][currentX] = ' ';
				currentY++;
				choice[currentY][currentX] = 'X';
				displayString(10, "went down");
			}
			if(getButtonPress(buttonRight)&& currentX != 2)
			{
				choice[currentY][currentX] = ' ';
				currentX++;
				choice[currentY][currentX] = 'X';
				displayString(10, "went right");
			}
			if(getButtonPress(buttonLeft)&& currentX != 0)
			{
				choice[currentY][currentX] = ' ';
				currentX--;
				choice[currentY][currentX] = 'X';
				displayString(10, "went left");
			}

			displayString(9, "%d|%d",currentX,currentY);
		}
	}while(gameBoard[currentY][currentX] != 0);

	gameBoard[currentY][currentX] = letter;
	driveTo(currentY,currentX);
	if (letter == 1)
	{
		drawX(motorPower, distance, halfRight, negativeRightAngle);
	}
	else
	{
		drawStick(motorPower,distance);
	}
	driveBack();
}

void easyMode(int letter)
{

	int row = 0, col = 0;
	while (row<3)
	{
		col = 0;
		while (col < 3)
		{

			displayString(15, "%d", gameBoard[row][col]);
			if (gameBoard[row][col] == 0)
			{
				//Not entirely complete
				displayString(15, "row: %d col: %d", row, col);
				gameBoard[row][col] = letter;
				driveTo(row, col);
				row = 3;
				col = 3;
			}
			col++;


		}
		row++;

	}
	if (letter == 1){
		drawX(motorPower, distance, halfRight, negativeRightAngle);
	}
	else
		drawStick(motorPower,distance);
	driveBack();
}

void hardMode(int letter)
{
	//check if it can win
	int sum = 0;
	int count1 = 0;
	int row = 0;
	int col = 0;

	//check rows
	while((sum != 2*letter) && count1 < 3)
	{
		sum = 0;
		for(int count2 = 0; count2 < 3; count2++)
		{
			sum += gameBoard[count1][count2];
		}
		//chosing location
		if (sum == 2*letter)
		{
			row = count1;
			for(int count2 = 0; count2 < 3; count2++)
				if(gameBoard[count1][count2] == 0)
				col = count2;
		}
		else
			count1++;
	}
	//check cols
	count1 = 0;
	if(sum != 2*letter)
	{
		while(sum != 2*letter && count1 < 3)
		{
			sum = 0;
			for(int count2 = 0; count2 < 3; count2++)
			{
				sum += gameBoard[count2][count1];
			}
			//chosing location
			if (sum == 2*letter)
			{
				col = count1;
				for(int count2 = 0; count2 < 3; count2++)
					if(gameBoard[count2][count1] ==0)
					row = count2;
			}
			else
				count1++;
		}
	}
	//check for left diagoal victory
	if(sum != 2*letter)
	{
		sum = 0;
		for(int count = 0; count < 3; count++)
			sum += gameBoard[count][count];
		//chosing location
		if (sum == 2*letter)
			for(int count = 0; count < 3; count++)
		{
			if(gameBoard[count][count] == 0)
			{
				row = count;
				col = count;
			}
		}
	}
	//check for right diagoal victory
	if(sum != 2*letter)
	{
		sum = 0;
		for(int count3 = 2; count3 >= 0; count3--)
		{
			int count2 = 2-count3;
			sum += gameBoard[count3][count2];
		}
		//chosing location
		if (sum == 2*letter)
			for(int count3 = 2; count3 >= 0; count3--)
		{
			int count2 = 2-count3;
			if(gameBoard[count3][count2] == 0)
			{
				row = count3;
				col = count2;
			}
		}
	}
	if (sum == 2*letter)
	{
		gameBoard[row][col] = letter;
		driveTo(row,col);
		if (letter == 1)
		{
			drawX(motorPower, distance, halfRight, negativeRightAngle);
		}
		else
		{
			drawStick(motorPower,distance);
		}
		driveBack();
	}
	else
	{
		//check if player can win
		sum = 0;
		count1 = 0;
		row = 0;
		col = 0;

		//check rows
		while((sum != 2*-letter) && count1 < 3)
		{
			sum = 0;
			for(int count2 = 0; count2 < 3; count2++)
			{
				sum += gameBoard[count1][count2];
			}
			//chosing location
			if (sum == 2*-letter)
			{
				row = count1;
				for(int count2 = 0; count2 < 3; count2++)
					if(gameBoard[count1][count2] == 0)
					col = count2;
			}
			else
				count1++;
		}
		//check cols
		count1 = 0;
		if(sum != 2*-letter)
		{
			while(sum != 2*-letter && count1 < 3)
			{
				sum = 0;
				for(int count2 = 0; count2 < 3; count2++)
				{
					sum += gameBoard[count2][count1];
				}
				//chosing location
				if (sum == 2*-letter)
				{
					col = count1;
					for(int count2 = 0; count2 < 3; count2++)
						if(gameBoard[count2][count1] ==0)
						row = count2;
				}
				else
					count1++;
			}
		}
		//check for left diagoal victory
		if(sum != 2*-letter)
		{
			sum = 0;
			for(int count = 0; count < 3; count++)
				sum += gameBoard[count][count];
			//chosing location
			if (sum == 2*-letter)
				for(int count = 0; count < 3; count++)
			{
				if(gameBoard[count][count] == 0)
				{
					row = count;
					col = count;
				}
			}
		}
		//check for right diagoal victory
		if(sum != 2*-letter)
		{
			sum = 0;
			for(int count3 = 2; count3 >= 0; count3--)
			{
				int count2 = 2-count3;
				sum += gameBoard[count3][count2];
			}
			//chosing location
			if (sum == 2*-letter)
				for(int count3 = 2; count3 >= 0; count3--)
			{
				int count2 = 2-count3;
				if(gameBoard[count3][count2] == 0)
				{
					row = count3;
					col = count2;
				}
			}
		}
		if (sum == 2*-letter){
			//temp for testing
			gameBoard[row][col] = letter;
			driveTo(row,col);
			if (letter == 1)
			{
				drawX(motorPower, distance, halfRight, negativeRightAngle);
			}
			else
			{
				drawStick(motorPower,distance);
			}
			driveBack();
		}
		else
		{
			//check if it can make 2 in a row in a free space





			int avaliable = 0;
			for(int count2 = 0; count2 < 3; count2++)
			{
				for(int count3 = 0; count3 < 3; count3++)
				{
					if(gameBoard[count2][count3] == 0)
					{
						avaliable++;
					}
				}
			}
			int place = random(avaliable-1);
			int sum = 0;
			for(int count2 = 0; count2 < 3; count2++)
			{
				for(int count3 = 0; count3 < 3; count3++)
				{
					if(gameBoard[count2][count3] == 0)
					{
						sum++;
						if(sum == place)
						{
							row = count2;
							col = count3;
						}
					}
				}
			}

			//temp for testing
			gameBoard[row][col] = letter;
			driveTo(row,col);
			if (letter == 1)
			{
				drawX(motorPower, distance, halfRight, negativeRightAngle);
			}
			else
			{
				drawStick(motorPower,distance);
			}
			driveBack();
		}
	}
}

int GameOver()
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

task main()
{
	int gameDone = 0;
	int numTurns = 0;
	//Starting Game
	displayString(1, "Press touch sensor to start game!");
	while(SensorValue[S1] == 0)
	{}
	eraseDisplay();
	//Decide if hard or easy mode
	displayString(1, "Press up for easy mode,");
	displayString(2, "Down for hard mode");
	bool hard = true;
	while(!getButtonPress(buttonUp) && !getButtonPress(buttonDown))
	{}
	if (getButtonPress(buttonUp))
	{
		hard = false;
	}
	eraseDisplay();
	wait1Msec(250);

	//Deciding who goes first (this can be changed later)
	displayString(1, "Press UP to go first,");
	displayString(2, "DOWN for robot goes first");

	int whoseTurn = -1;//-1 for user term 1 for robot turn
	while(!getButtonPress(buttonAny))
	{}
	if (getButtonPress(buttonDown))
	{
		whoseTurn = 1;
	}
	eraseDisplay();

	wait1Msec(250);

	//Decide if player is x or |
	displayString(1, "Press up for |,");
	displayString(2, "Down for X");
	int letter = 1;
	while(!getButtonPress(buttonAny))
	{}
	if (getButtonPress(buttonUp))
	{
		letter = -1;
	}
	wait1Msec(250);

	//Main game loop
	//eraseDisplay();

	configureSensors();

	while (gameDone == 0 && numTurns < 9)
	{

		if (whoseTurn == -1)
		{

			eraseDisplay();
			displayString(13, "PlayerTurn");
			PlayerMove(letter);

		}

		else if (whoseTurn == 1 && hard == false)
		{
			eraseDisplay();
			displayString(13, "EasyMode");
			easyMode(letter*-1);

		}
		else
		{
			eraseDisplay();
			displayString(13, "HardMode");
			hardMode(letter*-1);
		}
		whoseTurn*= -1;
		//displayString(13, "%d", whoseTurn);
		numTurns++;
		gameDone = GameOver();
	}

	if (gameDone == letter)
	{	eraseDisplay();
		displayString(5, "You win!");
		wait1Msec(5000);
	}
	else if (gameDone == -letter)
	{
		eraseDisplay();
		displayString(5, "Robot wins!");
		wait1Msec(5000);
	}
	else
	{
		eraseDisplay();
		displayString(5, "Draw Game!");
		wait1Msec(5000);
	}

}
