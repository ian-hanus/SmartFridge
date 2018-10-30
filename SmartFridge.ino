

#define pinOut A1

#define tempPin1 A0
#define tempPin2 A2



//Set freezer off at beginning
void setup() {
  pinMode(pinOut, OUTPUT);
  pinMode(tempPin1, INPUT);
  pinMode(tempPin2, INPUT);
  digitalWrite(pinOut, LOW);
  Serial.begin(9600);
}

void loop() {
  changeTemp(10, 100);
  keepTemp(20);
  changeTemp(20, 100);
  keepTemp(30);
  changeTemp(11, 212);
  
  delay(10000);
  while(1){
    keepTemp(1000);
    delay(10000);
  }
}


//Method for checking temperature
double checkTempPrint() {
  double readingSum = 0;
  int numRead = 20;
  for (int i = 0; i < numRead; i++) { //get temperature and average over 10 samples w/ 0.01 delay between samples
    double reading1 = analogRead(tempPin1);
    double reading2 = analogRead(tempPin2);
    readingSum = readingSum + reading1 + reading2;
    delay(10);
  }
  double readingAve = readingSum / numRead / 2;
  double inTempC = readingAve / 4.4 + 3.3;

  Serial.print("Current Temperatures: ");
  Serial.print(inTempC );
  Serial.println(" degrees C");

 
  return inTempC;
}

double checkTemp() {
  double readingSum = 0;
  int numRead = 20;
  for (int i = 0; i < numRead; i++) { //get temperature and average over 10 samples w/ 0.01 delay between samples
    double reading1 = analogRead(tempPin1);
    double reading2 = analogRead(tempPin2);
    readingSum = readingSum + reading1 + reading2;
    delay(10);
  }
  double readingAve = readingSum / numRead;
  double inTempC = readingAve / 4.4 + 3.3;
  
  return inTempC;
}

//Method for keeping temp constant
void keepTemp(int duration){
  int startTime = millis();
  int currentTime = millis();
  int stayTemp = checkTemp();
  int currentTemp = checkTemp();
  while(duration * 1000 * 60 < currentTime - startTime){
    if(currentTemp > stayTemp){
      digitalWrite(pinOut, HIGH);
    }
    else{
      digitalWrite(pinOut, LOW);
    }
    delay(5000);
    currentTemp = checkTempPrint();
    currentTime = millis();
  }
}

//Method for changing temperature
//setTemp is desired temperature in degrees Celsius
//setDuration is change in time over which temperature is changed in minutes
void changeTemp(int setTemp, float setDuration){
  float myDuration = setDuration * 1000. * 60.;
  Serial.println(myDuration);
  int startTime = millis();
  float timeStep = 6000;
  int numSteps = myDuration / timeStep;

    
  //Find initial temp and desired temp change
  double startTemp = checkTemp();          
  double tempCelsius = startTemp;
  double totalChange = setTemp - startTemp;
  
  //Calculate the desired points at given times
  double timeCutoffs[numSteps];
  for(int k = 1; k < numSteps + 1; k++){
    timeCutoffs[k-1] = timeStep * k;
  }

  
  double tempGoal[numSteps];
  double stepSize = totalChange / numSteps;
  for(int k =0; k <= numSteps; k++){
    tempGoal[k] = startTemp + k * stepSize; 
  }
 
  //For each step cooling down
  for(int k = 0; k < numSteps; k++){        //for each step
    while(startTime - millis() < timeCutoffs[k]){    //while in the designate time for each step
      if(tempCelsius >  tempGoal[k]){
        digitalWrite(pinOut, HIGH);           // turn on freezer if too high temp
      }
      else{
        digitalWrite(pinOut, LOW);            // turn off freezer if too low temp
      }
      delay(5000);                            //delay to stop freezer from constantly turnign on/off
      tempCelsius = checkTempPrint();
    }
  }
}



void changeTemp(double setTemp, float setDuration){
  //Establish desired temperature change
  float startTemp = checkTemp();
  float currentTemp = checkTemp();
  float totalChange = setTemp - startTemp;  

  //Establish number of steps and how long each step lasts
  setDuration = setDuration * 1000 * 60;    
  float timeStep = 30 * 1000; 
  float numSteps = setDuration / timeStep; 
  int fullSteps = floor(numSteps);    
  float partialStep = numSteps - fullSteps;
  float startTime = millis();

  //Calculate time each step should end at
  float timeCutoffs[numSteps];
  for(int k = 0; k < fullSteps; k++){
    timeCutoffs[k] = 6000 * (k + 1);
  }
  timeCutoffs[numSteps] = setDuration;

  //Calculate temp each step should end at
  float tempStepSize = totalChange / fullSteps;
  float tempGoals[numSteps];
  for(int k = 0; k < fullSteps; k++){
    tempGoals[k] = tempStepSize * (k + 1);
  }
  tempGoals[numSteps] = tempGoals[numSteps - 1];
  
  for(int k = 0; k < numSteps; k++){
    while(millis() < startTime + setDuration){
      
    }
  }

  
  
  
}
